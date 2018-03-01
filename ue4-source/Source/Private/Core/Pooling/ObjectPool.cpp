// Copyright 2015-2018 Joy Machine, LLC. All rights reserved.

#include "steelhunters.h"

#include "ObjectPool.h"

#include "DateTime.h"

#include "IPooledObject.h"
#include "IObjectPooling.h"

// Console variable definition.
static TAutoConsoleVariable< int > CVarDebugObjectPooling( TEXT( "joy.DebugObjectPooling" ), 
	0,
	TEXT( "Verbose output from the object pooling system for debugging." ),
	ECVF_Default );

// Static class definitions.
const int64 UObjectPool::kDefaultPruneStaleSeconds = 60;

uint32 UObjectPool::IDCounter = 0;
uint32 UObjectPool::PoolCount = 0;

//----------------------------------------------------------------------------------------------------
UObjectPool::UObjectPool( const class FObjectInitializer& ObjectInitializer )
: Name( *FString::Printf( TEXT( "Pool-%d" ), PoolCount ) )
, PoolID( PoolCount )
, PoolOwner( nullptr )
, PoolSizeOptimal( 256 )
, PoolSizeMaximum( 2048 )
, PruneStale( false )
, PruneLastTimestamp( FDateTime::Now( ).ToUnixTimestamp( ) )
, PruneStale_Seconds( kDefaultPruneStaleSeconds )
{
	Pool.Empty( );

	// Set this pool ID.
	PoolID = ++PoolCount;

	{
		const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );
		if( debug )
		{
			// Just toss out some debug information.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Creating new pool (Pool Count: %d)." ), PoolCount );
		}
	}
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::SetPoolOwner( UObject* PoolOwnerIn )
{
	// Update the pool owner.
	if( PoolOwner.IsValid( ) )
	{
		// Reset the existing pointer.
		PoolOwner.Reset( );
		PoolOwner = nullptr;
	}

	if( !IsValid( PoolOwnerIn ) )
	{
		// The call was just removing the prior owner.
		return;
	}

#if WITH_EDITORONLY_DATA
	{
		// Ensure that this is a valid owner.
		IObjectPooling* pObjectPooling = dynamic_cast< IObjectPooling* >( PoolOwnerIn );
		ensure( pObjectPooling != nullptr );
	}
#endif	// WITH_EDITORONLY_DATA.

	PoolOwner = TWeakObjectPtr< UObject >( PoolOwnerIn );
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::OnObjectReturn( IPooledObject* PooledObject, bool Destroying )
{
	check( PooledObject != nullptr );
	if( Destroying )
	{
		uint32 objectID = PooledObject->ID;
		TWeakObjectPtr< UObject >* pDeathRowObject = Pool.FindByPredicate( [&]( TWeakObjectPtr< UObject > pObject ) {
			return( dynamic_cast< IPooledObject* >( pObject.Get( ) )->ID == objectID );
		} );

		// Now remove the object from the pool and destroy it.
		Pool.Remove( *pDeathRowObject );

		DestroyObject( dynamic_cast< IPooledObject* >( ( *pDeathRowObject ).Get( ) ) );
		( *pDeathRowObject ).Reset( );

		return;
	}

	// Reset the object (pure virtual method, so the logic is reliant on the child class).
	PooledObject->IsActive = false;
	PooledObject->LastActiveTimestamp = FDateTime::Now( ).ToUnixTimestamp( );
	PooledObject->ResetState( );
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::Prune( )
{
	const int64 currentTime = FDateTime::Now( ).ToUnixTimestamp( );
	int32 prunedObjectCount = Pool.RemoveAll( [&]( TWeakObjectPtr< UObject > pObject ) {
		if( !pObject.IsStale( ) && pObject.IsValid( ) )
		{
			// The object pointer is either stale or no longer valid.
			return false;
		}

		IPooledObject* pPooledObject = dynamic_cast< IPooledObject* >( pObject.Get( ) );
		if( !pPooledObject->GetIsActive( ) && ( PruneStale && ( ( currentTime - pPooledObject->LastActiveTimestamp ) > PruneStale_Seconds ) ) )
		{
			// Destroy the object and remove it from the pool.
			DestroyObject( pPooledObject );
			pPooledObject = nullptr;

			// Reset the weak pointer.
			pObject.Reset( );
			return true;
		}

		return false;
	} );

	{
		const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );
		if( debug )
		{
			// Log the number of pruned objects.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool::Prune] Pruning completed (Pool: %s, Object Count: %d, Pruned Count: %d." ), *Name.ToString( ), Pool.Num( ), prunedObjectCount );
		}
	}

	PruneLastTimestamp = currentTime;
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::DestroyObject( IPooledObject* PooledObject )
{
	// This shouldn't be destroyed yet.
	check( PooledObject != nullptr );

	// Just in case any logic in ::Deactivate is necessary, execute that, and then the ::Destroy method.
	PooledObject->ReturnToPool.Unbind( );

	PooledObject->ResetState( );
	PooledObject->DestroyInstance( );
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::SetName( const FName& PoolName )
{
	// Just set the pool name; doesn't have to be unique.
	Name = GenerateName( PoolName );
}

//----------------------------------------------------------------------------------------------------
FName UObjectPool::GenerateName( const FName& BaseName )
{
	return( *FString::Printf( TEXT( "%s-%d" ), *BaseName.ToString( ), PoolCount ) );
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::Empty( bool SeparateActiveInstances )
{
	const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );

	// Remove all objects from the pool and destroy them (unless SeparateActiveInstances is true, in which case leave them alone for now).
	int32 totalObjectCount = Pool.Num( );
	int32 destroyedObjectCount = Pool.RemoveAll( [&]( TWeakObjectPtr< UObject > pObject ) {
		IPooledObject* pPooledObject = dynamic_cast< IPooledObject* >( pObject.Get( ) );
		check( pPooledObject != nullptr );
		if( pPooledObject != nullptr )
		{
			// Bad pool entry.
			return true;
		}

		const bool isActive = pPooledObject->GetIsActive( );
		if( isActive && SeparateActiveInstances )
		{
			// This instance is still active and it's okay to leave it alive, but Unbind the return to pool delegate as it's now on its own.
			pPooledObject->ReturnToPool.Unbind( );

			pObject.Reset( );
			return true;
		}

		// Unbind the delegate, reset the object state, and destroy the object (active or not).
		DestroyObject( pPooledObject );
		return true;
	} );

	if( !SeparateActiveInstances )
	{
		if( debug )
		{
			// Log the object destroyed count and the destruction of the pool.
		}

		return;
	}

	if( debug )
	{
		if( !SeparateActiveInstances || ( totalObjectCount == destroyedObjectCount ) )
		{
			// Log the object destroyed count and the destruction of the pool.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool::Empty] Pool emptied and all instances destroyed (Pool: %s, Destroyed Object Count: %d)." ), *Name.ToString( ), destroyedObjectCount );
		}
		else if( SeparateActiveInstances )
		{
			// Log the separated object count, destroyed object count, and the destruction of the pool.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool::Empty] Poole emptied and all instances destroyed (Pool: %s, Separated Object Count: %d, Destroyed Object Count: %d)." ), *Name.ToString( ), ( totalObjectCount - destroyedObjectCount ), destroyedObjectCount );
		}
		else if( totalObjectCount != destroyedObjectCount )
		{
			// There are remaining objects despite SeparateActiveInstances not being specified.
			checkf( false, TEXT( "[UObjectPool::Empty] Pool emptied but destroy count doesn't match pool object count. (Pool: %s, Destroyed Object Count: %d, Remainder: %d)." ), *Name.ToString( ), destroyedObjectCount, ( totalObjectCount - destroyedObjectCount ) );
		}
	}

#if !UE_BUILD_SHIPPING
	ensure( Pool.Num( ) == 0 );
#endif	// !UE_BUILD_SHIPPING.
}

//----------------------------------------------------------------------------------------------------
int32 UObjectPool::Add( IPooledObject* ObjectIn, bool Active )
{
	check( ObjectIn != nullptr );

	// Check to ensure that IPooledObject is a derivative of UObject.
	UObject* pUObject = dynamic_cast< UObject* >( ObjectIn );

#if !UE_BUILD_SHIPPING
	ensure( IsValid( pUObject ) );
#endif	// !UE_BUILD_SHIPPING.

	// Predicate to check if this object is already in the pool.
	const uint32 objectID = ObjectIn->ID;
	if( Pool.ContainsByPredicate( [objectID]( TWeakObjectPtr< UObject > pObject ) {
		IPooledObject* pPooledObject = dynamic_cast< IPooledObject* >( pObject.Get( ) );
		return( objectID == pPooledObject->ID );
	} ) )
	{
		// Object is already in the pool.
		return false;
	}

	// Add this object to the pool.
	int32 objectIdx = Pool.Add( pUObject );

	// Set whether or not the object is active (if being added, it usually is), and give it an ID.
	ObjectIn->IsActive = Active;
	ObjectIn->ID = IDCounter++;
	ObjectIn->LastActiveTimestamp = FDateTime::Now( ).ToUnixTimestamp( );

	// Setup the object's pool return delegate (executed when the object is deactivated).
	ObjectIn->ReturnToPool.BindUObject( this, &UObjectPool::OnObjectReturn );

	{
		const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );
		if( debug )
		{
			// Debug logging.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Adding object to pool (Pool: %s, Object Count: %d)." ), *Name.ToString( ), Pool.Num( ) );
		}
	}

	return objectIdx;
}

//----------------------------------------------------------------------------------------------------
template< typename T >
T* UObjectPool::GetPooledObject( bool& SpawnedNewInstanceOut )
{
	IPooledObject* pPooledObject = GetPooledObject( SpawnedNewInstanceOut );
	if( pPooledObject == nullptr )
	{
		// No valid pooled objects.
		return nullptr;
	}

	T* pTemplateObject = Cast< T >( pPooledObject );
	check( IsValid( pTemplateObject ) );

	return pTemplateObject;
}

//----------------------------------------------------------------------------------------------------
template< typename T >
T* UObjectPool::GetPooledObject( )
{
	bool spawnedNewInstance;

	IPooledObject* pPooledObject = GetPooledObject( spawnedNewInstance );
	if( pPooledObject == nullptr )
	{
		// No valid pooled objects.
		return nullptr;
	}

	T* pTemplateObject = Cast< T >( pPooledObject );
	check( IsValid( pTemplateObject ) );

	return pTemplateObject;
}

//----------------------------------------------------------------------------------------------------
IPooledObject* UObjectPool::GetPooledObject( bool& SpawnedNewInstanceOut )
{
	const int64 currentTime = FDateTime::Now( ).ToUnixTimestamp( );
	bool executePrune = PruneStale && ( ( currentTime - PruneLastTimestamp ) > PruneStale_Seconds );		// If this isn't true, then it may be time to prune anyway if the search finds an invalid object.

	// Ensure the spawn value is initially false.
	SpawnedNewInstanceOut = false;

	// Find the first inactive and valid object.
	int32 idx = Pool.IndexOfByPredicate( [&]( TWeakObjectPtr< UObject > pObject ) {
		IPooledObject* pPooledObject = dynamic_cast< IPooledObject* >( pObject.Get( ) );
		return( !pPooledObject->GetIsActive( ) );
	} );

	{
		const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );
		if( !Pool.IsValidIndex( idx ) )
		{
			if( debug && !PoolOwner.IsValid( ) )
			{
				// Debug logging.
				UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Unable to get a free object from the pool using ::GetPooledObject (Pool: %s, Object Count: %d)." ), *Name.ToString( ), Pool.Num( ) );
				return nullptr;
			}
			else if( !PoolOwner.IsValid( ) )
			{
				// No assigned pool owner to instantiate a new pooled object, so it's up to the owner of this pool to either set itself as the owner (... yeah, see? makes sense) or instantiate and add a new instance on its own.
				return nullptr;
			}

			// Instantiate a new pooled object using the parent's spawn method (and check it after, as it shouldn't fail at this point).
			IPooledObject* pPooledObject = nullptr;
			
			{
				// Get the IObjectPooling interface to execute the object instantiation method.
				IObjectPooling* pObjectPooling = dynamic_cast< IObjectPooling* >( PoolOwner.Get( ) );
				check( pObjectPooling != nullptr );

				pPooledObject = dynamic_cast< IPooledObject* >( pObjectPooling->InstantiatePooledObject( ) );
				SpawnedNewInstanceOut = true;
			}

			check( pPooledObject != nullptr );

			// Add the new instance to the pool.
			idx = Add( pPooledObject, true );
			if( !Pool.IsValidIndex( idx ) )
			{
				if( debug )
				{
					// Debug logging.
					UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Unable to get a free object from the pool using ::GetPooledObject -- and an attempt to instantiate and add a new pooled object failed (Pool: %s, Object Count: %d)." ), *Name.ToString( ), Pool.Num( ) );
					return nullptr;
				}

				return nullptr;
			}
		}
		else if( debug )
		{
			// Debug logging.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Got a valid pooled object in ::GetPooledObject (Pool: %s, Index: %d, Object Count: %d)." ), *Name.ToString( ), idx, Pool.Num( ) );
		}
	}

	// Get the found valid object, remove its place in the array, and add it to the back for a more efficient check later.
	//	NOTE (trent, 12/10/17): Ensure that this is actually an optimization.
	UObject* pUObject = Pool[idx].Get( );
	IPooledObject* pValidObject = dynamic_cast< IPooledObject* >( pUObject );
	Pool.RemoveAtSwap( idx, 1, false );
	Pool.Add( pUObject );

	// Activate the object.
	pValidObject->IsActive = true;
	pValidObject->LastActiveTimestamp = FDateTime::Now( ).ToUnixTimestamp( );

	if( executePrune )
	{
		// Prune the pool if need be (or if it's just time).
		//	NOTE: Since the object that was just found has already been reactivated and given an updated timestamp, it won't be pruned.
		Prune( );
	}

	return pValidObject;
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::SetPruneStale( bool PruneStaleIn )
{
	PruneStale = PruneStaleIn;
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::SetPruneInactiveTime( int64 PruneInactivitySeconds )
{
	PruneStale_Seconds = PruneInactivitySeconds;
}