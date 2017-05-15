// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

// Enumeration for basic collider types.
UENUM( BlueprintType )
enum class ECollisionShapeType : uint8
{
	CST_Box				UMETA( DisplayName = "Box" ),
	CST_Sphere			UMETA( DisplayName = "Sphere" ),
	CST_Capsule			UMETA( DisplayName = "Capsule" ),

	CST_Max				UMETA( Hidden )
}; 
