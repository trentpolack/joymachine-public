// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#include "steelhunters.h"

#include "ObjectPool.h"

#include "DateTime.h"

#include "IPooledObject.h"

// Console variable definition.
static TAutoConsoleVariable< bool > CVarDebugObjectPooling( TEXT( "joy.DebugObjectPooling" ), 
	false,
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
void UObjectPool::OnObjectReturn( IPooledObject* pObject )
{
	check( pObject != nullptr );

	// Reset the object (pure virtual method, so the logic is reliant on the child class).
	pObject->IsActive = false;
	pObject->Reset( );
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
		if( !pPooledObject->GetIsActive( ) && !pPooledObject->Check( ) || ( PruneStale && ( ( currentTime - pPooledObject->LastActiveTimestamp ) > PruneStale_Seconds ) ) )
		{
			// This object is invalid or stale. Remove it.
			if( pPooledObject != nullptr )
			{
				// Just in case any logic in ::Deactivate is necessary, execute that, and then the ::Destroy method.
				pPooledObject->ReturnToPool.Unbind( );

				pPooledObject->DestroyInstance( );
				pPooledObject = nullptr;
			}

			return true;
		}

		return false;
	} );

	{
		const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );
		if( debug )
		{
			// Log the number of pruned objects.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Pool pruning logic completed (Pool: %s, Object Count: %d, Pruned Count: %d." ), *Name.ToString( ), Pool.Num( ), prunedObjectCount );
		}
	}

	PruneLastTimestamp = currentTime;
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
	int32 destroyedObjectCount = Pool.RemoveAll( [&]( TWeakObjectPtr< UObject > pObject ) {
		IPooledObject* pPooledObject = dynamic_cast< IPooledObject* >( pObject.Get( ) );
		if( !( SeparateActiveInstances && ( pPooledObject != nullptr ) && pPooledObject->GetIsActive( ) ) )
		{
			// Just in case any logic in ::Deactivate is necessary, execute that, and then the ::Destroy method.
			pPooledObject->ReturnToPool.Unbind( );

			pPooledObject->DestroyInstance( );
			pPooledObject = nullptr;
			return true;
		}

		return false;
	} );

	if( !SeparateActiveInstances )
	{
		if( debug )
		{
			// Log the object destroyed count and the destruction of the pool.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Pool destroy completed (Pool: %s, Destroyed Object Count: %d)." ), *Name.ToString( ), destroyedObjectCount );
		}

		return;
	}

	// Now handle the separation of objects.
	int32 separatedObjectCount = Pool.RemoveAll( [&]( TWeakObjectPtr< UObject > pObject ) {
		IPooledObject* pPooledObject = dynamic_cast< IPooledObject* >( pObject.Get( ) );
		pPooledObject->ReturnToPool.Unbind( );

		pPooledObject->OnPoolRemovalWhileActive( );
		return true;
	} );

	if( debug )
	{
		// Log the object destroyed count and the destruction of the pool.
		UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Pool destroy completed with object separation (Pool: %s, Separated Object Count: %d, Destroyed Object Count: %d)." ), *Name.ToString( ), separatedObjectCount, destroyedObjectCount );
	}

#if !UE_BUILD_SHIPPING
	ensure( Pool.Num( ) == 0 );
#endif	// !UE_BUILD_SHIPPING.
}

//----------------------------------------------------------------------------------------------------
bool UObjectPool::Add( IPooledObject* ObjectIn, bool Active )
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
	Pool.Add( pUObject );

	// Set whether or not the object is active (if being added, it usually is), and give it an ID.
	ObjectIn->IsActive = Active;
	ObjectIn->ID = IDCounter++;

	// Setup the object's pool return delegate (executed when the object is deactivated).
	ObjectIn->ReturnToPool.BindSP( this, &UObjectPool::OnObjectReturn );

	{
		const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );
		if( debug )
		{
			// Debug logging.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Adding object to pool (Pool: %s, Object Count: %d)." ), *Name.ToString( ), Pool.Num( ) );
		}
	}
}

//----------------------------------------------------------------------------------------------------
template< typename T >
T* UObjectPool::GetPooledObject( )
{
	IPooledObject* pPooledObject = GetPooledObject( );
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
IPooledObject* UObjectPool::GetPooledObject( )
{
	const int64 currentTime = FDateTime::Now( ).ToUnixTimestamp( );
	bool executePrune = PruneStale && ( ( currentTime - PruneLastTimestamp ) > PruneStale_Seconds );		// If this isn't true, then it may be time to prune anyway if the search finds an invalid object.

	// Find the first inactive and valid object.
	int32 idx = Pool.IndexOfByPredicate( [&]( TWeakObjectPtr< UObject > pObject ) {
		IPooledObject* pPooledObject = dynamic_cast< IPooledObject* >( pObject.Get( ) );
		if( !pPooledObject->GetIsActive( ) )
		{
			if( !pPooledObject->Check( ) )
			{
				// This object is invalid. Prune the pool after this search.
				executePrune = true;
				return false;
			}

			return true;
		}

		return false;
	} );

	if( idx == INDEX_NONE )
	{
		const bool debug = CVarDebugObjectPooling.GetValueOnGameThread( );
		if( debug )
		{
			// Debug logging.
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Unable to get a free object from the pool using ::GetPooledObject (Pool: %s, Object Count: %d)." ), *Name.ToString( ), Pool.Num( ) );
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