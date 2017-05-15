// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#pragma once

#include "steelhunters.h"

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "WorldCollision.h"

#include "CollisionShapeSet.generated.h"

// FCollisionShapeSet which, when updates for a given shape, will propagate to all other shapes to ensure that no *specific* shape is required by anything.
USTRUCT( )
struct STEELHUNTERS_API FCollisionShapeSet
{
	GENERATED_BODY( )

public:
	FCollisionShapeSet( );
	FCollisionShapeSet( ECollisionShape::Type ShapeTypeIn );
	FCollisionShapeSet( const FCollisionShapeSet& ShapeSetInstance );

public:
	// Base shape type.
	//	NOTE (trent, 5/9/17): This may not actually be necessary.
	ECollisionShape::Type ShapeType;

	// FCollisionShape is implemented as a union, so only one shape is ever fully supported. So, this is a way to support any shape (when the desired shape is unknown from code).
	FCollisionShape ShapeBox;
	FCollisionShape ShapeSphere;
	FCollisionShape ShapeCapsule;

public:
	static float CapsuleRadiusFromBoxExtent;

public:
	static FCollisionShapeSet MakeShapeBox( const FVector& HalfExtents );
	static FCollisionShapeSet MakeShapeSphere( float Radius );
	static FCollisionShapeSet MakeShapeCapsule( float Radius, float HalfHeight );

public:
	// Update the existing shapes.
	void UpdateShapeBox( FVector HalfExtents );
	void UpdateShapeSphere( float Radius );
	void UpdateShapeCapsule( float Radius, float HalfHeight );
	void UpdateShapeCapsuleRadius( float Radius );
	void UpdateShapeCapsuleHalfHeight( float HalfHeight );

	// Accessors (regardless of shape type; all shape data is updated whenever a given shape is updated).
	FVector GetHalfExtents( ) const;
	FVector GetExtents( ) const;
	float GetRadius( ) const;
	float GetHalfHeight( ) const;
};
