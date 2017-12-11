// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Object.h"
#include "UObject/ObjectMacros.h"

#include "ObjectPool.generated.h"

// Declarations.
class IObjectPooled;

// Delegate for pooled objects to execute when returning to the pool.
DECLARE_DELEGATE_OneParam( FObjectReturnToPool, IObjectPooled* );

// UObjectPool Class Definition.
//	TODO (trent, 12/10/17): Need to add logic for:
//		- Method to reserve a set amount of created object instances (likely through a Create pure virtual method in IObjectPooled).
//		- Logic to create an object instance in ::GetPooledObject if one isn't found (via a ::Create method like above).
UCLASS( )
class STEELHUNTERS_API UObjectPool : public UObject
{
	GENERATED_BODY( )

protected:
	UObjectPool( const class FObjectInitializer& ObjectInitializer );

private:
	// Default timespan for pruning stale objects.
	static const int64 kDefaultPruneStaleSeconds;

private:
	// Poor man's unique ID. This is local to any object pool and only resets when the pool is totally destroyed. So if it ever goes over 4294967295 then, uh, something is bad.
	static uint32 IDCounter;

	// Total count of existing pools.
	static uint32 PoolCount;

protected:
	UPROPERTY( )
	FName Name;
	UPROPERTY( )
	uint32 PoolID;

	UPROPERTY( )
	TArray< TWeakObjectPtr< IObjectPooled > > Pool;

	UPROPERTY( )
	uint32 PoolSizeOptimal;
	UPROPERTY( )
	uint32 PoolSizeMaximum;

	UPROPERTY( )
	bool PruneStale;
	UPROPERTY( )
	int64 PruneLastTimestamp;
	UPROPERTY( )
	int64 PruneStale_Seconds;

private:
	UFUNCTION( )
	void OnObjectReturn( IObjectPooled* pObject );

	// Prune pooled objects if they are no longer valid (IPooledObject::Check) and, if PruneStale is true, prune stale objects.
	UFUNCTION( )
	void Prune( );

public:
	// Set the pool name; not necessary, just useful.
	UFUNCTION( )
	void SetName( const FName& PoolName );

	// Add an instance of a pooled object to this pool.
	//	NOTE: will return false if this object is in this pool already.
	UFUNCTION( )
	bool Add( IObjectPooled* ObjectIn, bool Active = true );

	// Get an object from the pool.
	//	NOTE: returns a nullptr if an object isn't found.
	template< class T >
	T* GetPooledObject( );

	UFUNCTION( )
	IObjectPooled* GetPooledObject( );

	// Set whether or not to prune the pool for inactive objects (default is false).
	UFUNCTION( )
	void SetPruneStale( bool PruneStaleIn = true );
	// If pruning based on inactivity, set the inactivity duration before pruning (default is sixty seconds).
	UFUNCTION( )
	void SetPruneInactiveTime( int64 PruneInactivitySeconds );
};