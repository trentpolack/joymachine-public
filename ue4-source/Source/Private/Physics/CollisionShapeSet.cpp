// Copyright 2015-2017 Joy Machine, LLC. All rights reserved.

#include "steelhunters.h"

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "WorldCollision.h"

#include "CollisionShapeSet.h"

//----------------------------------------------------------------------------------------------------
FCollisionShapeSet::FCollisionShapeSet( )
{	}

//----------------------------------------------------------------------------------------------------
FCollisionShapeSet( ECollisionShape ShapeTypeIn )
: ShapeType( ShapeTypeIn )
{	}

//----------------------------------------------------------------------------------------------------
FCollisionShapeSet::FCollisionShapeSet( const FCollisionShapeSet& ShapeSetInstance )
{
	ShapeType = ShapeSetInstance.ShapeType;
	ShapeBox = ShapeSetInstance.ShapeBox;
	ShapeSphere = ShapeSetInstance.ShapeSphere;
	ShapeCapsule = ShapeSetInstance.ShapeCapsule;
}

//----------------------------------------------------------------------------------------------------
FCollisionShapeSet::FCollisionShapeSet( const FCollisionShape& ShapeInstance )
{
	// Create the various shape instances based on the type of shape passed in.
	if( ShapeInstance.IsBox( ) )
	{
		this = MakeShapeBox( ShapeInstance.GetExtent( ) );
	}
	else if( ShapeInstance.IsSphere( ) )
	{
		this = MakeShapeSphere( ShapeInstance.GetSphereRadius( ) );
	}
	else if( ShapeInstance.IsCapsule( ) )
	{
		this = MakeShapeCapsule( ShapeInstance.GetCapsuleRadius( ), ShapeInstance.GetCapsuleHalfHeight( ) );
	}
}

//----------------------------------------------------------------------------------------------------
FCollisionShapeSet FCollisionShapeSet::MakeShapeBox( const FVector& HalfExtents )
{
	// Static method.
	FCollisionShapeSet ShapeSet( ECollisionShape::Box );

	// Make a box from the passed-in half-extents.
	ShapeSet.ShapeBox = FCollisionShape::MakeBox( HalfExtents );

	// Make the remaining shapes based on the extents.
	ShapeSet.ShapeSphere = FCollisionShape::MakeSphere( FMath::Max< float >( HalfExtents.X, FMath::Max< float>( HalfExtents.Y, HalfExtents.Z ) ) );
	ShapeSet.ShapeCapsule = FCollisionShape::MakeCapsule( FMath::Max< float >( HalfExtents.X, HalfExtents.Y ), HalfExtents.Z );

	return ShapeSet;
}

//----------------------------------------------------------------------------------------------------
FCollisionShapeSet FCollisionShapeSet::MakeShapeSphere( float Radius )
{
	// Static method.
	FCollisionShapeSet ShapeSet( ECollisionShape::Sphere );

	// Make a sphere from the passed-in radius.
	ShapeSet.ShapeSphere = FCollisionShape::MakeSphere( Radius );

	// Make the remaining shapes based on the radius (the capsule likely being the most incorrect).
	ShapeSet.ShapeBox = FCollisionShape::MakeBox( FVector( Radius, Radius, Radius ) );
	ShapeSet.ShapeCapsule = FCollisionShape::MakeCapsule( Radius, Radius );

	return ShapeSet;
}

//----------------------------------------------------------------------------------------------------
FCollisionShapeSet FCollisionShapeSet::MakeShapeCapsule( float Radius, float HalfHeight )
{
	// Static method.
	FCollisionShapeSet ShapeSet( ECollisionShape::Capsule );

	// Make a capsule from the passed-in radius/half-height.
	ShapeSet.ShapeCapsule = FCollisionShape::MakeCapsule( Radius, HalfHeight );

	// Make a sphere using the maximum value from the box extents.
	ShapeSet.ShapeBox = FCollisionShape::MakeBox( FVector( Radius, Radius, HalfHeight ) );
	ShapeSet.ShapeSphere = FCollisionShape::MakeSphere( FMath::Max< float >( Radius, HalfHeight ) );

	return ShapeSet;
}

//----------------------------------------------------------------------------------------------------
void FCollisionShapeSet::UpdateShapeBox( FVector HalfExtents )
{
	// Update the box extents.
	ShapeBox.SetBox( HalfExtents );

	// Update the remaining shapes based on the extents.
	ShapeSphere.SetSphere( FMath::Max< float >( HalfExtents.X, FMath::Max< float>( HalfExtents.Y, HalfExtents.Z ) ) );
	ShapeCapsule.SetCapsule( FMath::Max< float >( HalfExtents.X, HalfExtents.Y ), HalfExtents.Z );
}

//----------------------------------------------------------------------------------------------------
void FCollisionShapeSet::UpdateShapeSphere( float Radius )
{
	// Update the sphere radius.
	ShapeSphere.SetSphere( Radius );

	// Update the remaining shapes based on the radius.
	ShapeBox.SetBox( FVector( Radius, Radius, Radius ) );
	ShapeCapsule.SetCapsule( Radius, Radius );
}

//----------------------------------------------------------------------------------------------------
void FCollisionShapeSet::UpdateShapeCapsule( float Radius, float HalfHeight )
{
	// Update the capsule with the given parameters.
	ShapeCapsule.SetCapsule( Radius, HalfHeight );

	// Update the remaining shapes based on the extents.
	ShapeBox.SetBox( FVector( Radius, Radius, HalfHeight ) );
	ShapeSphere.SetSphere( FMath::Max< float >( Radius, HalfHeight ) );
}

//----------------------------------------------------------------------------------------------------
void FCollisionShapeSet::UpdateShapeCapsuleRadius( float Radius )
{
	// Update the capsule radius.
	ShapeCapsule.SetCapsule( Radius, ShapeCapsule.GetCapsuleHalfHeight( ) );

	// Update the remaining shapes based on the new capsule radius.
	ShapeBox.SetBox( FVector( Radius, Radius, ShapeCapsule.GetCapsuleHalfHeight( ) ) );
	ShapeSphere.SetSphere( FMath::Max< float >( Radius, ShapeCapsule.GetCapsuleHalfHeight( ) ) );
}

//----------------------------------------------------------------------------------------------------
void FCollisionShapeSet::UpdateShapeCapsuleHalfHeight( float HalfHeight )
{
	// Update the capsule half-height.
	ShapeCapsule.SetCapsule( ShapeCapsule.GetCapsuleRadius( ), HalfHeight );

	// Update the remaining shapes based on the extents.
	ShapeBox.SetBox( FVector( ShapeCapsule.GetCapsuleRadius( ), ShapeCapsule.GetCapsuleRadius( ), HalfHeight ) );
	ShapeSphere.SetSphere( FMath::Max< float >( ShapeSphere.GetSphereRadius( ), HalfHeight ) );
}

//----------------------------------------------------------------------------------------------------
FVector FCollisionShapeSet::GetHalfExtents( ) const
{
	return( ShapeBox.GetExtent( ) );
}

//----------------------------------------------------------------------------------------------------
FVector FCollisionShapeSet::GetExtents( ) const
{
	return( ShapeBox.GetExtent( )*2.0f );
}

//----------------------------------------------------------------------------------------------------
float FCollisionShapeSet::GetRadius( ) const
{
	return( ( ShapeType == ECollisionShape::Sphere ) ? ShapeSphere.GetSphereRadius( ) : ShapeCapsule.GetCapsuleRadius( ) );
}

//----------------------------------------------------------------------------------------------------
float FCollisionShapeSet::GetHalfHeight( ) const
{
	return( ( ShapeType == ECollisionShape::Capsule ) ? ShapeCapsule.GetCapsuleHalfHeight( ) : ShapeBox.GetExtent( ).Z );
}
