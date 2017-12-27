// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "ObjectPool.h"

#include "IPooledObject.generated.h"

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
	FObjectReturnToPool ReturnToPool;

	uint32 ID;
	bool IsActive;

	int64 LastActiveTimestamp;

private:
	UFUNCTION( )
	virtual bool GetIsActive( ) const
	{
		return IsActive;
	}

protected:
	UFUNCTION( )
	virtual bool CheckState( ) = 0;

	UFUNCTION( )
	virtual void ResetState( ) = 0;
	UFUNCTION( )
	virtual void ActivateInstance( ) = 0;

	UFUNCTION( )
	virtual void DestroyInstance( ) = 0;

	UFUNCTION( )
	virtual void OnPoolRemovalWhileActive( ) = 0;

public:
	UFUNCTION( )
	virtual void Deactivate( )
	{
		ReturnToPool.ExecuteIfBound( this );
	}
};