// Copyright 2015-2018 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "ObjectPool.h"

#include "IPooledObject.generated.h"

// TODO (trent, 3/1/18): PLEASE MAKE THIS NOT AS AWFUL.

// UObjectPooledInterface Object Interface Definition.
UINTERFACE( BlueprintType, MinimalAPI, meta = ( CannotImplementInterfaceInBlueprint ) )
class UPooledObject : public UInterface
{
	GENERATED_BODY( )
};

// IObjectPooled Interface Definition.
class STEELHUNTERS_API IPooledObject
{
	GENERATED_BODY( )
	friend class UObjectPool;

private:
	/*
	 *	Data for the owning object pool and the interface.
	*/

	// Delegate back to the owning pool.
	//	NOTE (trent, 3/1/18): If this is unbound, that means this instance has been removed from the pool (while it was active).
	FObjectReturnToPool ReturnToPool;

	// Object ID and whether it's active or not.
	uint32 ID;
	bool IsActive;

	// The timestamp for when this object instance was last activated (specifically when the object is allocated or, if ever activated, deactivated).
	int64 LastActiveTimestamp;

private:
	// Just an accessor that allows the owning pool to query this object's state.
	UFUNCTION( )
	virtual bool GetIsActive( ) const
	{
		return IsActive;
	}

protected:
	// Methods executed upon activation/deactivation of this object instance; neither are -- in a majority of cases -- called by the object itself.
	UFUNCTION( )
	virtual void ActivateInstance( ) = 0;
	UFUNCTION( )
	virtual void ResetState( ) = 0;

	// Method to execute when this object has to be destroyed; can be executed by the object pool or the object. (It's just a wrapper over what deactivate does, really).
	UFUNCTION( )
	virtual void DestroyInstance( ) = 0;

public:
	// The primary method for a child class to reset its state and return to the object pool for further use.
	// If Destroying is true, execution of this method will trigger ::DestroyInstance (either after being removed from the pool or, if it already was, instantly).
	//	NOTE (trent, 2/28/18): Please don't override this.
	UFUNCTION( )
	virtual void Deactivate( bool Destroying = false )
	{
		if( !ReturnToPool.IsBound( ) )
		{
			// This object was removed from the pool while still active (meaning the pool no longer manages it, for whatever reason), destroy it (resetting first, just to ensure there arent loose ends).
			ResetState( );

			DestroyInstance( );
			return;
		}

		// Mark this object as inactive and return it to the pool. The object pool will reset its state.
		IsActive = false;
		ReturnToPool.ExecuteIfBound( this, Destroying );
	}
};