// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptInterface.h"
#include "Components/SceneComponent.h"
#include "Engine/Scene.h"

#include "GameFramework/SpringArmComponent.h"

#include "JoyCameraBoomComponent.generated.h"

// Declarations.
class AActor;

UCLASS( ClassGroup = Camera, HideCategories = ( "Mobility" ), meta = ( BlueprintSpawnableComponent ) )
class STEELHUNTERS_API UJoyCameraBoomComponent : public USpringArmComponent
{
	GENERATED_BODY( )

public:
	UJoyCameraBoomComponent( const class FObjectInitializer& ObjectInitializer );

protected:
	TArray< AActor* > IgnoredActors;

protected:
	// Update the desired location of the camera boom; this is a complete override, but is largely the same functionality (though with smarter collision ignorance).
	virtual void UpdateDesiredArmLocation( bool DoTrace, bool DoLocationLag, bool DoRotationLag, float DeltaTime ) override;

public:
	virtual void AddIgnoredActor( AActor* IgnoredActor );
	virtual void AddIgnoredActors( const TArray< AActor* >& IgnoredActorsIn );
};
