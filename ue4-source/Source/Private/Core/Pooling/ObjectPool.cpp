// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#include "steelhunters.h"

#include "ObjectPool.h"

#include "DateTime.h"

#include "IObjectPooled.h"

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
void UObjectPool::OnObjectReturn( IObjectPooled* pObject )
{
	check( IsValid( pObject ) );

	// Reset the object (pure virtual method, so the logic is reliant on the child class).
	pObject->IsActive = false;
	pObject->Deactivate( );
	pObject->Reset( );
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::Prune( )
{
	const int64 currentTime = FDateTime::Now( ).ToUnixTimestamp( );
	int32 prunedObjectCount = Pool.RemoveAll( [&]( IObjectPooled* pObject ) {
		if( !pObject->GetIsActive( ) && !pObject->Check( ) || ( PruneStale && ( ( currentTime - pObject->LastActiveTimestamp ) > PruneStale_Seconds ) ) )
		{
			// This object is invalid or stale. Remove it.
			if( IsValid( pObject ) )
			{
				// Just in case any logic in ::Deactivate is necessary, execute that, and then the ::Destroy method.
				pObject->Deactivate( );
				pObject->Destroy( );
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
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Pool pruning logic completed (Pool: %s, Object Count: %d, Pruned Count: %d." ), Name, Pool.Num( ), prunedObjectCount );
		}
	}

	PruneLastTimestamp = currentTime;
}

//----------------------------------------------------------------------------------------------------
void UObjectPool::SetName( const FName& PoolName )
{
	// Just set the pool name; doesn't have to be unique.
	Name = *FString::Printf( TEXT( "%s-%d" ), PoolName, PoolCount );
}

//----------------------------------------------------------------------------------------------------
bool UObjectPool::Add( IObjectPooled* ObjectIn, bool Active )
{
	check( ObjectIn != nullptr );

	// Predicate to check if this object is already in the pool.
	const uint32 objectID = ObjectIn->ID;
	if( Pool.ContainsByPredicate( [objectID]( const IObjectPooled* pObject ) {
		return( objectID == pObject->ID );
	} ) )
	{
		// Object is already in the pool.
		return false;
	}

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
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Adding object to pool (Pool: %s, Object Count: %d)." ), Name, Pool.Num( ) );
		}
	}
}

//----------------------------------------------------------------------------------------------------
template< typename T >
T* UObjectPool::GetPooledObject( )
{
	T* pPooledObject = GetPooledObject( );
	if( !IsValid( pPooledObject ) )
	{
		// No valid pooled objects.
		return nullptr;
	}

	// Not a UObject-derived class; use standard checks.
	T* pCastPooledObject = Cast< T >( pPooledObject );
	check( IsValid( pCastPooledObject ) );
	return pCastPooledObject;
}

//----------------------------------------------------------------------------------------------------
IObjectPooled* UObjectPool::GetPooledObject( )
{
	const int64 currentTime = FDateTime::Now( ).ToUnixTimestamp( );
	bool executePrune = PruneStale && ( ( currentTime - PruneLastTimestamp ) > PruneStale_Seconds );		// If this isn't true, then it may be time to prune anyway if the search finds an invalid object.

	// Find the first inactive and valid object.
	int32 idx = Pool.IndexOfByPredicate( [&]( IObjectPooled* pObject ) {
		if( !pObject->GetIsActive( ) )
		{
			if( !pObject->Check( ) )
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
			UE_LOG( SteelHuntersLog, Log, TEXT( "[UObjectPool] Unable to get a free object from the pool using ::GetPooledObject (Pool: %s, Object Count: %d)." ), Name, Pool.Num( ) );
		}
	}

	// Get the found valid object, remove its place in the array, and add it to the back for a more efficient check later.
	//	NOTE (trent, 12/10/17): Ensure that this is actually an optimization.
	IObjectPooled* pValidObject = Pool[idx].Get( );
	Pool.RemoveAtSwap( idx, 1, false );
	Pool.Add( pValidObject );

	// Activate the object.
	pValidObject->IsActive = true;
	pValidObject->LastActiveTimestamp = FDateTime::Now( ).ToUnixTimestamp( );
	pValidObject->Activate( );

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