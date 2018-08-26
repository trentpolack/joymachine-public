// Copyright 2015-2018 Joy Machine, LLC. All rights reserved.

#pragma once

#include "SteelHuntersPCH.h"

#include "Object.h"
#include "ObjectMacros.h"

#include "WorldCollision.h"

#include "DrawDebugHelpers.h"

#include "GameDebugStatics.generated.h"

// Declarations.
class AActor;

// UGameDebugStatics [Static] Class Definition.
UCLASS( Transient )
class STEELHUNTERS_API UGameDebugStatics : public UObject
{
	GENERATED_BODY( )

protected:
	// Helper method to consolidate the code for a single line trace and a multi line trace.
	static void DebugDrawLineTraceHelper( const UWorld* World, const FVector& Start, const FVector& End, const TArray< FHitResult >& Hits, const FColor& ColorQuery, const FColor& ColorBlock, const FColor& ColorOverlap, const FColor& ColorInvalid, float Lifetime, float Thickness );

public:
	/*
	 *	Standard Parameters for the debug drawing methods (for consistent rendering/coloring).
	*/

	static const float kDefaultWorldTextSize;									// 1.0f.
	static const FVector2D kDefaultScreenTextSize;								// FVector2D::UnitVector.

	static const float kDefaultArrowSize;										// 25.0f.

	static const float kDefaultCrosshairSize;									// 1.0f.

	static const float kDefaultCoordinateSize;									// 1.0f.
	static const float kDefaultCameraSize;										// 1.0f.

	static const FVector2D kDefaultHistogramSize;								// { 64.0f, 32.0f }.

	static const FColor kDefaultColor;											// Joy Machine Blue (#5381B0) to differentiate from default coloring of UE4-side debug draws.

	static const FColor kDefaultTextColor;										// White.
	static const FColor kDefaultTextColor_Verbose;								// Dark gray.
	static const FColor kDefaultTextColor_Log;									// White.
	static const FColor kDefaultTextColor_Warning;								// Yellow.
	static const FColor kDefaultTextColor_Error;								// Red.

	static const bool kDefaultTextShadowEnabled;								// false.

	static const float kDefaultLifetime;										// -1.0f.
	static const float kDefaultThickness;										// 0.0f.

	// Specifying some standard collision test-related color defaults (which should almost always be used); these colors are intended to be similar in meaning to the simulation collision test's color palette (query/overlap are just orange/yellow enough to not be white).
	static const FColor kDefaultCollisionQueryColor;							// Light orange (#ffddbc).

	static const FColor kDefaultCollisionBlockColor;							// Green.
	static const FColor kDefaultCollisionOverlapColor;							// Light yellow (#fffcaa).
	static const FColor kDefaultCollisionInvalidColor;							// Red.
	static const FColor kDefaultCollisionInvalidPenetrationColor;				// Magenta.

	// Size of the impact point drawn for collision queries.
	static const float kDefaultCollisionImpactPointSize;						// 4.0f.
	static const float kDefaultCollisionInvalidPenetrationPointSizeModifier;	// 2.0f.
	// Length and size of the arrow for the directional arrow representing teh impact normal of a collision query.
	static const float kDefaultCollisionImpactNormalArrowLength;				// 75.0f.
	static const float kDefaultCollisionImpactNormalArrowSize;					// 24.0f.

public:

	/*
	 *	Debug Text Drawing Methods.
	 *		NOTE (trent, 8/24/18): When doing a build for testing/shipping (ENABLE_DRAW_DEBUG) the functions invoked by these methods will be empty.
	*/

	// Draw text in world space (optionally attached to the passed-in AttachmentActor).
	static void DebugDrawText( const UWorld* World, const FVector& Location, const FString& Text, AActor* AttachmentActor = nullptr, float Size = kDefaultWorldTextSize, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, bool ShadowEnabled = kDefaultTextShadowEnabled );

	// Print a message to the screen (text appears above the last string printed).
	static void DebugDrawScreenText( const FString& Text, FVector2D Size = kDefaultScreenTextSize, const FColor& Color = kDefaultTextColor, float Lifetime = kDefaultLifetime );

	//	Various screen prints to mirror the logging color/naming scheme.
	static void DebugDrawScreenText_Verbose( const FString& Text, FVector2D Size = kDefaultScreenTextSize, float Lifetime = kDefaultLifetime );
	static void DebugDrawScreenText_Log( const FString& Text, FVector2D Size = kDefaultScreenTextSize, float Lifetime = kDefaultLifetime );
	static void DebugDrawScreenText_Warning( const FString& Text, FVector2D Size = kDefaultScreenTextSize, float Lifetime = kDefaultLifetime );
	static void DebugDrawScreenText_Error( const FString& Text, FVector2D Size = kDefaultScreenTextSize, float Lifetime = kDefaultLifetime );

	/*
	 *	Common Use-Case Debug Drawing Methods.
	*/

	static void DebugDrawPoint( const UWorld* World, const FVector& Position, float Size, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime );
	static void DebugDrawLine( const UWorld* World, const FVector& Start, const FVector& End, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawBox( const UWorld* World, const FVector& Center, const FVector& Extent, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawBox( const UWorld* World, const FVector& Center, const FQuat& Rotation, const FVector& Extent, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawSphere( const UWorld* World, const FVector& Center, float Radius, int32 Segments, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawCylinder( const UWorld* World, const FVector& Start, const FVector& End, float Radius, int32 Segments, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawCapsule( const UWorld* World, const FVector& Center, float HalfHeight, float Radius, const FQuat& Rotation, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawCone( const UWorld* World, const FVector& Origin, const FVector& Direction, float Length, float AngleWidth, float AngleHeight, int32 Segments, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );

	static void DebugDrawDirectionalArrow( const UWorld* World, const FVector& Start, const FVector& End, float ArrowSize, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );

	static void DebugDrawShape( const UWorld* World, const FVector& Location, const FCollisionShape& Shape, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawShape( const UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );

	/*
	 *	Somewhat... More Elaborate Debug Drawing Methods.
	*/

	static void DebugDrawCrosshairs( const UWorld* World, const FVector& AxialLocation, const FRotator& AxialRotation, float Size = kDefaultCrosshairSize, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime );

	static void DebugDrawCoordinateSystem( const UWorld* World, const FVector& AxialLocation, const FRotator& AxialRotation, float Size = kDefaultCoordinateSize, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawFrustum( const UWorld* World, const FMatrix& FrustumToWorldMatrix, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	static void DebugDrawCamera( const UWorld* World, const FVector& Location, const FRotator& Rotation, float FOVDegrees, float Size = kDefaultCameraSize, const FColor& Color = kDefaultColor, float Lifetime = kDefaultLifetime );

	static void DebugDrawHistogram( const UWorld* World, const FDebugFloatHistory& HistoryFloatData, const FVector& Location, const FVector2D& Size = kDefaultHistogramSize, const FColor& Color = kDefaultColor, const float Lifetime = kDefaultLifetime );
	static void DebugDrawHistogram( const UWorld* World, const FDebugFloatHistory& HistoryFloatData, const FTransform& Transform, const FVector2D& Size = kDefaultHistogramSize, const FColor& Color = kDefaultColor, const float Lifetime = kDefaultLifetime );

	/*
	 *	Collision (Overlay/Sweep) Debug Drawing Methods.
	*/

	// Alternate version of the UE4's line trace (single) rendering function (I didn't like how it handled the trace results).
	static void DebugDrawLineTraceResult( const UWorld* World, const FVector& Start, const FVector& End, const FHitResult& Hit, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	// Alternate version of the UE4's line trace (single) rendering function (I didn't like how it handled the trace results). With color overrides.
	static void DebugDrawLineTraceResult( const UWorld* World, const FVector& Start, const FVector& End, const FHitResult& Hit, const FColor& ColorQueryOverride, const FColor& ColorBlockOverride, const FColor& ColorOverlapOverride, const FColor& ColorInvalidOverride, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	// Alternate version of the UE4's line trace (multi) rendering function (I didn't like how it handled the trace results).
	static void DebugDrawLineTraceResults( const UWorld* World, const FVector& Start, const FVector& End, const TArray< FHitResult >& Hits, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );
	// Alternate version of the UE4's line trace (multi) rendering function (I didn't like how it handled the trace results). With color overrides.
	static void DebugDrawLineTraceResults( const UWorld* World, const FVector& Start, const FVector& End, const TArray< FHitResult >& Hits, const FColor& ColorQueryOverride, const FColor& ColorBlockOverride, const FColor& ColorOverlapOverride, const FColor& ColorInvalidOverride, float Lifetime = kDefaultLifetime, float Thickness = kDefaultThickness );

	static void DebugDrawSweepResult( const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const FHitResult& Hit, float Lifetime = kDefaultLifetime );
	static void DebugDrawSweepResult( const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape& Shape, const FHitResult& Hit, float Lifetime = kDefaultLifetime );
	static void DebugDrawSweepResults( const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const TArray< FHitResult >& Hits, float Lifetime = kDefaultLifetime );
	static void DebugDrawSweepResults( const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape& Shape, const TArray< FHitResult >& Hits, float Lifetime = kDefaultLifetime );
};