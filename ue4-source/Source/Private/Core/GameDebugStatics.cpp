// Copyright 2015-2018 Joy Machine, LLC. All rights reserved.

#include "GameDebugStatics.h"

#include "Engine/Engine.h"

#include "CollisionDebugDrawingPublic.h"

#include "JoySerializationManager.h"

#include "PhysicalMaterialDataObject.h"

// Constant [Static] Nember Definitions.
const float UGameDebugStatics::kDefaultWorldTextSize = 1.0f;
const FVector2D UGameDebugStatics::kDefaultScreenTextSize = FVector2D::UnitVector;

const float UGameDebugStatics::kDefaultArrowSize = 25.0f;

const float UGameDebugStatics::kDefaultCrosshairSize = 1.0f;

const float UGameDebugStatics::kDefaultCoordinateSize = 1.0f;
const float UGameDebugStatics::kDefaultCameraSize = 1.0f;

const FVector2D UGameDebugStatics::kDefaultHistogramSize = { 64.0f, 32.0f };

const FColor UGameDebugStatics::kDefaultColor = FColor( 83, 129, 176 );

const FColor UGameDebugStatics::kDefaultTextColor = FColor::White;
const FColor UGameDebugStatics::kDefaultTextColor_Verbose = FColor( 160, 160, 160 );
const FColor UGameDebugStatics::kDefaultTextColor_Log = FColor::White;
const FColor UGameDebugStatics::kDefaultTextColor_Warning = FColor::Yellow;
const FColor UGameDebugStatics::kDefaultTextColor_Error = FColor::Red;

const bool UGameDebugStatics::kDefaultTextShadowEnabled = false;

const float UGameDebugStatics::kDefaultLifetime = -1.0f;
const float UGameDebugStatics::kDefaultThickness = 0.0f;

const FColor UGameDebugStatics::kDefaultCollisionQueryColor = FColor( 255, 221, 188 );

const FColor UGameDebugStatics::kDefaultCollisionBlockColor = FColor::Green;
const FColor UGameDebugStatics::kDefaultCollisionOverlapColor = FColor( 255, 252, 170 );
const FColor UGameDebugStatics::kDefaultCollisionInvalidColor = FColor::Red;
const FColor UGameDebugStatics::kDefaultCollisionInvalidPenetrationColor = FColor::Magenta;

const float UGameDebugStatics::kDefaultCollisionImpactPointSize = 8.0f;
const float UGameDebugStatics::kDefaultCollisionInvalidPenetrationPointSizeModifier = 2.0f;

const float UGameDebugStatics::UGameDebugStatics::kDefaultCollisionImpactNormalArrowLength = 75.0f;
const float UGameDebugStatics::UGameDebugStatics::kDefaultCollisionImpactNormalArrowSize = 24.0f;

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawText( const UWorld* World, const FVector& Location, const FString& Text, AActor* AttachmentActor, float Size, const FColor& Color, float Lifetime, bool ShadowEnabled )
{
	// Draw text in world space (optionally attached to the passed-in AttachmentActor).
	DrawDebugString( World, Location, Text, AttachmentActor, Color, Lifetime, ShadowEnabled, Size );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawScreenText( const FString& Text, FVector2D Size, const FColor& Color, float Lifetime )
{
	// Print a message to the screen (text appears above the last string printed).
	GEngine->AddOnScreenDebugMessage( INDEX_NONE, Lifetime, Color, Text, true, Size );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawScreenText_Verbose( const FString& Text, FVector2D Size, float Lifetime )
{
	// Print a message to the screen with importance akin to a "Verbose" log messuage.
	DebugDrawScreenText( Text, Size, kDefaultTextColor_Verbose, Lifetime );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawScreenText_Log( const FString& Text, FVector2D Size, float Lifetime )
{
	// Print a message to the screen with importance akin to a "Log" log messuage.
	DebugDrawScreenText( Text, Size, kDefaultTextColor_Log, Lifetime );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawScreenText_Warning( const FString& Text, FVector2D Size, float Lifetime )
{
	// Print a message to the screen with importance akin to a "Log" log messuage.
	DebugDrawScreenText( Text, Size, kDefaultTextColor_Warning, Lifetime );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawScreenText_Error( const FString& Text, FVector2D Size, float Lifetime )
{
	// Print a message to the screen with importance akin to a "Log" log messuage.
	DebugDrawScreenText( Text, Size, kDefaultTextColor_Error, Lifetime );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawPoint( const UWorld* World, const FVector& Position, float Size, const FColor& Color, float Lifetime )
{
	check( IsValid( World ) );

	DrawDebugPoint( World, Position, Size, Color, false, Lifetime, 0 );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawLine( const UWorld* World, const FVector& Start, const FVector& End, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugLine( World, Start, End, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawBox( const UWorld* World, const FVector& Center, const FVector& Extent, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugBox( World, Center, Extent, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawBox( const UWorld* World, const FVector& Center, const FQuat& Rotation, const FVector& Extent, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugBox( World, Center, Extent, Rotation, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawSphere( const UWorld* World, const FVector& Center, float Radius, int32 Segments, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugSphere( World, Center, Radius, Segments, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawCylinder( const UWorld* World, const FVector& Start, const FVector& End, float Radius, int32 Segments, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugCylinder( World, Start, End, Radius, Segments, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawCapsule( const UWorld* World, const FVector& Center, float HalfHeight, float Radius, const FQuat& Rotation, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugCapsule( World, Center, HalfHeight, Radius, Rotation, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawCone( const UWorld* World, const FVector& Origin, const FVector& Direction, float Length, float AngleWidth, float AngleHeight, int32 Segments, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugCone( World, Origin, Direction, Length, AngleWidth, AngleHeight, Segments, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawDirectionalArrow( const UWorld* World, const FVector& Start, const FVector& End, float ArrowSize, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugDirectionalArrow( World, Start, End, ArrowSize, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawShape( const UWorld* World, const FVector& Location, const FCollisionShape& Shape, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	switch( Shape.ShapeType )
	{
	case ECollisionShape::Box:
		DebugDrawBox( World, Location, Shape.GetExtent( ), Color, Lifetime, Thickness );
		return;

	case ECollisionShape::Sphere:
		DebugDrawSphere( World, Location, Shape.GetSphereRadius( ), 16, Color, Lifetime, Thickness );
		return;

	case ECollisionShape::Capsule:
		DebugDrawCapsule( World, Location, Shape.GetCapsuleHalfHeight( ), Shape.GetCapsuleRadius( ), FQuat::Identity, Color, Lifetime, Thickness );
		return;
	}

	check( false );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawShape( const UWorld* World, const FVector& Location, const FQuat& Rotation, const FCollisionShape& Shape, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	switch( Shape.ShapeType )
	{
	case ECollisionShape::Box:
		DebugDrawBox( World, Location, Rotation, Shape.GetExtent( ), Color, Lifetime, Thickness );
		return;

	case ECollisionShape::Sphere:
		DebugDrawSphere( World, Location, Shape.GetSphereRadius( ), 16, Color, Lifetime, Thickness );
		return;

	case ECollisionShape::Capsule:
		DebugDrawCapsule( World, Location, Shape.GetCapsuleHalfHeight( ), Shape.GetCapsuleRadius( ), Rotation, Color, Lifetime, Thickness );
		return;
	}

	check( false );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawCrosshairs( const UWorld* World, const FVector& AxialLocation, const FRotator& AxialRotation, float Size, const FColor& Color, float Lifetime )
{
	check( IsValid( World ) );

	DrawDebugCrosshairs( World, AxialLocation, AxialRotation, Size, Color, false, Lifetime, 0 );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawCoordinateSystem( const UWorld* World, const FVector& AxialLocation, const FRotator& AxialRotation, float Size, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugCoordinateSystem( World, AxialLocation, AxialRotation, Size, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawFrustum( const UWorld* World, const FMatrix& FrustumToWorldMatrix, const FColor& Color, float Lifetime, float Thickness )
{
	check( IsValid( World ) );

	DrawDebugFrustum( World, FrustumToWorldMatrix, Color, false, Lifetime, 0, Thickness );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawCamera( const UWorld* World, const FVector& Location, const FRotator& Rotation, float FOVDegrees, float Size, const FColor& Color, float Lifetime )
{
	check( IsValid( World ) );

	DrawDebugCamera( World, Location, Rotation, FOVDegrees, Size, Color, false, Lifetime, 0 );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawHistogram( const UWorld* World, const FDebugFloatHistory& HistoryFloatData, const FVector& Location, const FVector2D& Size, const FColor& Color, const float Lifetime )
{
	check( IsValid( World ) );

	DrawDebugFloatHistory( const_cast< UWorld& >( *World ), HistoryFloatData, Location, Size, Color, false, Lifetime, 0 );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawHistogram( const UWorld* World, const FDebugFloatHistory& HistoryFloatData, const FTransform& Transform, const FVector2D& Size, const FColor& Color, const float Lifetime )
{
	check( IsValid( World ) );

	DrawDebugFloatHistory( const_cast< UWorld& >( *World ), HistoryFloatData, Transform, Size, Color, false, Lifetime, 0 );
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawLineTraceHelper( const UWorld* World, const FVector& Start, const FVector& End, const TArray< FHitResult >& Hits, const FColor& ColorQuery, const FColor& ColorBlock, const FColor& ColorOverlap, const FColor& ColorInvalid, float Lifetime, float Thickness )
{
#if ENABLE_DRAW_DEBUG
	// Helper method to consolidate the code for a single line trace and a multi line trace.
	const bool hitsEmpty = ( Hits.Num( ) <= 0 );
	const bool isBlocking = !hitsEmpty && ( FHitResult::GetNumBlockingHits( Hits ) > 0 );
	const bool isOverlap = !hitsEmpty && !isBlocking && ( FHitResult::GetNumOverlapHits( Hits ) > 0 );

	if( hitsEmpty || ( !isBlocking && !isOverlap ) )
	{
		bool startedInPenetration = false;
		if( !hitsEmpty )
		{
			// If the trace wasn't a hit or an overlap then it failed; check to see if it started in penetration to draw the result differently than if it just didn't query anything.
			const FHitResult& hit = Hits[0];
			startedInPenetration = hit.bStartPenetrating;
			if( startedInPenetration )
			{
				// Draw the impact point (should be the same as the trace start point though) using the ::kDefaultCollisionInvalidPenetrationColor -- to differentiate it from a failed trace due to more "natural" reasons.
				DebugDrawPoint( World, hit.ImpactPoint, kDefaultCollisionImpactPointSize*kDefaultCollisionInvalidPenetrationPointSizeModifier, kDefaultCollisionInvalidPenetrationColor, Lifetime );
			}
		}

		// Draw the query trace using one of the two invalid colors (standard trace failure or started-in-penetration failure).
		DebugDrawLine( World, Start, End, ( startedInPenetration ) ? kDefaultCollisionInvalidPenetrationColor : ColorInvalid, Lifetime, Thickness );
		return;
	}

	// The line trace resulted in either a hit or an overlap; draw using the query color up to the first blocking hit (then switch to the blocking color) or, if no blocking hit, the first overlap (then switch to the overlap color).

	int32 firstHitIdx = 0;
	if( isBlocking )
	{
		// Find the first hit result (the static method in FHitResult doesn't like the const array).
		for( int32 i = 0; i < Hits.Num( ); ++i )
		{
			if( Hits[i].bBlockingHit )
			{
				firstHitIdx = i;
				break;
			}
		}
	}

	const FHitResult& hitFirst = Hits[firstHitIdx];
	FVector queryEnd = End;
	if( isBlocking )
	{
		// Just set the query endpoint to the first blocking hit's impact location.
		queryEnd = hitFirst.ImpactPoint;
	}
	else
	{
		// Get (and check) the overlapped primitive component.
		const UPrimitiveComponent* pPrimitiveComponent = hitFirst.GetComponent( );
		check( IsValid( pPrimitiveComponent ) );

		float distance = FVector::Distance( Start, pPrimitiveComponent->GetComponentLocation( ) );
		queryEnd = Start + ( ( End - Start ).GetSafeNormal( )*distance );
	}

	// Draw the first half of the trace line.
	DebugDrawLine( World, Start, queryEnd, ColorQuery, Lifetime, Thickness );

	// Draw the second half of the trace line.
	DebugDrawLine( World, queryEnd, End, ( isOverlap ) ? ColorOverlap : ColorBlock, Lifetime, Thickness );

	for( int32 i = 0; i < Hits.Num( ); ++i )
	{
		const FHitResult& hit = Hits[0];
		if( !hit.IsValidBlockingHit( ) )
		{
			// Do an overlap point render.
			DebugDrawPoint( World, hit.ImpactPoint, kDefaultCollisionImpactPointSize, ColorOverlap, Lifetime );
			continue;
		}
		
		// Draw the impact point as well as the impact normal.
		DebugDrawPoint( World, hit.ImpactPoint, kDefaultCollisionImpactPointSize, ColorBlock, Lifetime );

		// Draw the impact normal as a directional arrow as well.
		DebugDrawDirectionalArrow( World, hit.ImpactPoint, hit.ImpactPoint + hit.ImpactNormal*kDefaultCollisionImpactNormalArrowLength, kDefaultCollisionImpactNormalArrowSize, ColorBlock, Lifetime, Thickness );
	}
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawLineTraceResult( const UWorld* World, const FVector& Start, const FVector& End, const FHitResult& Hit, float Lifetime, float Thickness )
{
#if ENABLE_DRAW_DEBUG
	// Alternate version of the UE4's line trace (single) rendering function (I didn't like how it handled the trace results).
	TArray< FHitResult > hitArrayOfOne;
	hitArrayOfOne.Add( Hit );

	DebugDrawLineTraceHelper( World, Start, End, hitArrayOfOne, kDefaultCollisionQueryColor, kDefaultCollisionBlockColor, kDefaultCollisionOverlapColor, kDefaultCollisionInvalidColor, Lifetime, Thickness );
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawLineTraceResult( const UWorld* World, const FVector& Start, const FVector& End, const FHitResult& Hit, const FColor& ColorQueryOverride, const FColor& ColorBlockOverride, const FColor& ColorOverlapOverride, const FColor& ColorInvalidOverride, float Lifetime, float Thickness )
{
#if ENABLE_DRAW_DEBUG
	// Alternate version of the UE4's line trace (single) rendering function (I didn't like how it handled the trace results).
	TArray< FHitResult > hitArrayOfOne;
	hitArrayOfOne.Add( Hit );

	DebugDrawLineTraceHelper( World, Start, End, hitArrayOfOne, ColorQueryOverride, ColorBlockOverride, ColorOverlapOverride, ColorInvalidOverride, Lifetime, Thickness );
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawLineTraceResults( const UWorld* World, const FVector& Start, const FVector& End, const TArray< FHitResult >& Hits, float Lifetime, float Thickness )
{
#if ENABLE_DRAW_DEBUG
	// Alternate version of the UE4's line trace (multi) rendering function (I didn't like how it handled the trace results). With color overrides.
	DebugDrawLineTraceHelper( World, Start, End, Hits, kDefaultCollisionQueryColor, kDefaultCollisionBlockColor, kDefaultCollisionOverlapColor, kDefaultCollisionInvalidColor, Lifetime, Thickness );
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawLineTraceResults( const UWorld* World, const FVector& Start, const FVector& End, const TArray< FHitResult >& Hits, const FColor& ColorQueryOverride, const FColor& ColorBlockOverride, const FColor& ColorOverlapOverride, const FColor& ColorInvalidOverride, float Lifetime, float Thickness )
{
#if ENABLE_DRAW_DEBUG
	// Alternate version of the UE4's line trace (multi) rendering function (I didn't like how it handled the trace results). With color overrides.
	DebugDrawLineTraceHelper( World, Start, End, Hits, ColorQueryOverride, ColorBlockOverride, ColorOverlapOverride, ColorInvalidOverride, Lifetime, Thickness );
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawSweepResult( const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const FHitResult& Hit, float Lifetime )
{
#if ENABLE_DRAW_DEBUG
	// The single-result sweep wrapper method (no rotation applied).
	TArray< FHitResult > hitArrayOfOne;
	hitArrayOfOne.Add( Hit );

	DebugDrawSweepResults( World, Start, End, FQuat::Identity, Shape, hitArrayOfOne, Lifetime );
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawSweepResult( const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape& Shape, const FHitResult& Hit, float Lifetime )
{
#if ENABLE_DRAW_DEBUG
	// The single-result sweep wrapper method (rotation applied).
	TArray< FHitResult > hitArrayOfOne;
	hitArrayOfOne.Add( Hit );

	DebugDrawSweepResults( World, Start, End, Rotation, Shape, hitArrayOfOne, Lifetime );
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawSweepResults( const UWorld* World, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const TArray< FHitResult >& Hits, float Lifetime )
{
#if ENABLE_DRAW_DEBUG
	// The multi-result sweep wrapper method (no rotation applied).
	DebugDrawSweepResults( World, Start, End, FQuat::Identity, Shape, Hits, Lifetime );
#endif	// ENABLE_DRAW_DEBUG.
}

//----------------------------------------------------------------------------------------------------
void UGameDebugStatics::DebugDrawSweepResults( const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape& Shape, const TArray< FHitResult >& Hits, float Lifetime )
{
#if ENABLE_DRAW_DEBUG
	// The multi-result sweep wrapper method (rotation applied).
	check( IsValid( World ) );

	switch( Shape.ShapeType )
	{
	case ECollisionShape::Box:
		DrawBoxSweeps( World, Start, End, Shape.GetExtent( ), Rotation, Hits, Lifetime );
		return;

	case ECollisionShape::Sphere:
		DrawSphereSweeps( World, Start, End, Shape.GetSphereRadius( ), Hits, Lifetime );
		return;

	case ECollisionShape::Capsule:
		DrawCapsuleSweeps( World, Start, End, Shape.GetCapsuleHalfHeight( ), Shape.GetCapsuleRadius( ), Rotation, Hits, Lifetime );
		return;
	}

	check( false );
#endif	// ENABLE_DRAW_DEBUG.
}