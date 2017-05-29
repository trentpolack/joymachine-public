// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#include "steelhunters.h"

#include "UObject/ConstructorHelpers.h"

#include "CollisionQueryParams.h"
#include "WorldCollision.h"

#include "EngineGlobals.h"
#include "Engine/World.h"

#include "GameFramework/Pawn.h"

#include "DrawDebugHelpers.h"

#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "JoyCameraBoomComponent.h"

//----------------------------------------------------------------------------------------------------
UJoyCameraBoomComponent::UJoyCameraBoomComponent( const FObjectInitializer& ObjectInitializer )
: Super( ObjectInitializer )
{

	IgnoredActors.Empty( );
}

//----------------------------------------------------------------------------------------------------
void UJoyCameraBoomComponent::UpdateDesiredArmLocation( bool DoTrace, bool DoLocationLag, bool DoRotationLag, float DeltaTime )
{
	FRotator rotationDesired = GetComponentRotation( );
	if( !bAbsoluteRotation )
	{
		// If inheriting rotation, check options for which components to inherit
		if( !bInheritPitch )
		{
			rotationDesired.Pitch = RelativeRotation.Pitch;
		}

		if( !bInheritYaw )
		{
			rotationDesired.Yaw = RelativeRotation.Yaw;
		}

		if( !bInheritRoll )
		{
			rotationDesired.Roll = RelativeRotation.Roll;
		}
	}

	const float inverseCameraLagMaxTimeStep = ( 1.0f/CameraLagMaxTimeStep );
	if( DoRotationLag )
	{
		// Apply 'lag' to rotation.
		if( bUseCameraLagSubstepping && ( DeltaTime > CameraLagMaxTimeStep ) && ( CameraRotationLagSpeed > 0.0f ) )
		{
			const FRotator rotationArmStep = ( rotationDesired - PreviousDesiredRot ).GetNormalized( )*( CameraLagMaxTimeStep/DeltaTime );
			FRotator lerpTarget = PreviousDesiredRot;
			float timeRemaining = DeltaTime;

			while( timeRemaining > KINDA_SMALL_NUMBER )
			{
				const float lerpAmount = FMath::Min( CameraLagMaxTimeStep, timeRemaining );
				lerpTarget+= rotationArmStep*( lerpAmount*inverseCameraLagMaxTimeStep );
				timeRemaining-= lerpAmount;

				rotationDesired = FMath::RInterpTo( PreviousDesiredRot, lerpTarget, lerpAmount, CameraRotationLagSpeed );
				PreviousDesiredRot = rotationDesired;
			}
		}
		else
		{
			rotationDesired = FMath::RInterpTo(PreviousDesiredRot, rotationDesired, DeltaTime, CameraRotationLagSpeed);
		}
	}

	PreviousDesiredRot = rotationDesired;

	// Get the spring arm 'origin', the target we want to look at.
	FVector boomOrigin = GetComponentLocation( ) + TargetOffset;
	FVector locationDesired = boomOrigin;

	if( DoLocationLag )
	{
		// Lag the target, not the actual camera position, so rotating the camera around does not have lag.
		if( bUseCameraLagSubstepping && ( DeltaTime > CameraLagMaxTimeStep ) && ( CameraLagSpeed > 0.0f ) )
		{
			const FVector boomMovementStep = (boomOrigin - PreviousArmOrigin) * (CameraLagMaxTimeStep / DeltaTime);			
			FVector lerpTarget = PreviousArmOrigin;
			float timeRemaining = DeltaTime;

			while( timeRemaining > KINDA_SMALL_NUMBER )
			{
				const float lerpAmount = FMath::Min( CameraLagMaxTimeStep, timeRemaining );
				lerpTarget += boomMovementStep*( lerpAmount*inverseCameraLagMaxTimeStep );
				timeRemaining -= lerpAmount;

				locationDesired = FMath::VInterpTo( PreviousDesiredLoc, lerpTarget, lerpAmount, CameraLagSpeed );
				PreviousDesiredLoc = locationDesired;
			}
		}
		else
		{
			locationDesired = FMath::VInterpTo(PreviousDesiredLoc, locationDesired, DeltaTime, CameraLagSpeed);
		}

		// Clamp distance if requested.
		bool clampDistance = false;
		if( CameraLagMaxDistance > 0.0f )
		{
			const FVector fromOrigin = locationDesired - boomOrigin;
			if( fromOrigin.SizeSquared( ) > FMath::Square( CameraLagMaxDistance ) )
			{
				locationDesired = boomOrigin + fromOrigin.GetClampedToMaxSize( CameraLagMaxDistance );
				clampDistance = true;
			}
		}		

#if !( UE_BUILD_SHIPPING || UE_BUILD_TEST )
		if( bDrawDebugLagMarkers )
		{
			UWorld* pWorld = GetWorld( );
			DrawDebugSphere( pWorld, boomOrigin, 5.0f, 8, FColor::Green );
			DrawDebugSphere( pWorld, locationDesired, 5.0f, 8, FColor::Yellow );

			const FVector toOrigin = boomOrigin - locationDesired;
			DrawDebugDirectionalArrow( pWorld, locationDesired, locationDesired + ( toOrigin*0.5f ), 7.5f, clampDistance ? FColor::Red : FColor::Green );
			DrawDebugDirectionalArrow( pWorld, locationDesired + toOrigin*0.5f, boomOrigin, 7.5f, clampDistance ? FColor::Red : FColor::Green );
		}
#endif
	}

	PreviousArmOrigin = boomOrigin;
	PreviousDesiredLoc = locationDesired;

	// Now offset camera position back along the boom's rotation.
	locationDesired-= rotationDesired.Vector( )*TargetArmLength;

	// Add socket offset in local space.
	locationDesired+= FRotationMatrix( rotationDesired ).TransformVector( SocketOffset );

	// Do a sweep to ensure we are not penetrating the world.
	FVector locationResult;
	if( DoTrace && ( TargetArmLength != 0.0f ) )
	{
		static FName traceTagName( TEXT( "SpringArm" ) );
		FCollisionQueryParams queryParameters( traceTagName, false );
		queryParameters.AddIgnoredActors( IgnoredActors );

		FHitResult hitResult;
		GetWorld( )->SweepSingleByChannel( hitResult, boomOrigin, locationDesired, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere( ProbeSize ), queryParameters );

		locationResult = BlendLocations( locationDesired, hitResult.Location, hitResult.bBlockingHit, DeltaTime );
	}
	else
	{
		locationResult = locationDesired;
	}

	// Convert to relative to component.
	FTransform relativeCameraTransform = FTransform( rotationDesired, locationResult ).GetRelativeTransform( ComponentToWorld );

	// Update socket location/rotation.
	RelativeSocketLocation = relativeCameraTransform.GetLocation( );
	RelativeSocketRotation = relativeCameraTransform.GetRotation( );

	UpdateChildTransforms( );
}

//----------------------------------------------------------------------------------------------------
void UJoyCameraBoomComponent::AddIgnoredActor( AActor* IgnoredActor )
{
	IgnoredActors.AddUnique( IgnoredActor );
}

//----------------------------------------------------------------------------------------------------
void UJoyCameraBoomComponent::AddIgnoredActors( const TArray< AActor* >& IgnoredActorsIn )
{
	for( AActor* pActor : IgnoredActorsIn )
	{
		AddIgnoredActor( pActor );
	}
}
