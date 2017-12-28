// Copyright 2015-2018 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Object.h"

#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"

#include "IObjectPooling.generated.h"

// This class does not need to be modified.
UINTERFACE( BlueprintType, MinimalAPI, meta = ( CannotImplementInterfaceInBlueprint ) )
class UObjectPooling : public UInterface
{
	GENERATED_BODY( )
};

/*
 * Interface that any class using object pools can derive from for pool instance instantiation (as UE4 objects/actors tend to be somewhat involved).
*/
class STEELHUNTERS_API IObjectPooling
{
	GENERATED_BODY( )

public:
	UFUNCTION( )
	virtual UObject* InstantiatePooledObject( ) = 0;
};