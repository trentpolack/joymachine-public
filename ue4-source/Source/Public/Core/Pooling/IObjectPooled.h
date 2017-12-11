// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "ObjectPool.h"

#include "IObjectPooled.generated.h"

// UObjectPooledInterface Object Interface Definition.
UINTERFACE( BlueprintType, MinimalAPI, meta = ( CannotImplementInterfaceInBlueprint ) )
class UObjectPooled : public UInterface
{
	GENERATED_BODY( )
};

// IObjectPooled Interface Definition.
class STEELHUNTERS_API IObjectPooled
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
	virtual bool GetIsActive( ) const;

protected:
	UFUNCTION( )
	virtual bool Check( ) = 0;

	UFUNCTION( )
	virtual void Reset( ) = 0;
	UFUNCTION( )
	virtual void Activate( ) = 0;

	UFUNCTION( )
	virtual void Destroy( ) = 0;

public:
	UFUNCTION( )
	virtual void Deactivate( );
};