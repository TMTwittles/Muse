// Fill out your copyright notice in the Description page of Project Settings.
#include "Statics/MuseGameplayStatics.h"

float UMuseGameplayStatics::GetSignedAngle(const FVector& InVectorA, const FVector& InVectorB)
{

  const FVector VectorANormalized = InVectorA.GetSafeNormal();
  const FVector VectorBNormalized = InVectorB.GetSafeNormal();

  // Calculate the angle in radians
  float Dot = FVector::DotProduct(VectorANormalized, VectorBNormalized);
  float AngleRad = FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f)); // Clamp to avoid floating point errors

  // Calculate the cross product to determine the sign of the angle
  FVector Cross = FVector::CrossProduct(VectorANormalized, VectorBNormalized);

  // If the cross product has a positive Z component, the angle is counterclockwise
  // If the cross product has a negative Z component, the angle is clockwise
  if (Cross.Z < 0)
  {
    // Angle is negative (clockwise)
    AngleRad = -AngleRad;
  }

  // Convert to degrees if needed
  float AngleDeg = FMath::RadiansToDegrees(AngleRad);

  return AngleDeg;
}

float UMuseGameplayStatics::GetSignedAngle(const FQuat& QuatA, const FQuat& QuatB)
{
  // Step 1: Calculate the relative quaternion (Delta quaternion)
  FQuat DeltaQuat = QuatA.Inverse() * QuatB;

  // Step 2: Extract the axis and angle from the Delta quaternion
  // The axis is given by the X, Y, Z components, and the angle is given by 2 * acos(W)
  FVector Axis = DeltaQuat.GetAxisX(); // We can use any axis (X, Y, or Z)
  float Angle = 2.0f * FMath::Acos(DeltaQuat.W); // Angle in radians

  // Step 3: Compute the signed angle
  // We need to check the direction of the rotation. 
  // This can be done by examining the sign of the component of the rotation axis (here we use the Z-axis as a convention).
  float SignedAngle = FMath::RadiansToDegrees(Angle); // Convert from radians to degrees

  // Use the Z component of the axis to determine the sign of the angle
  if (Axis.Z < 0.0f)
  {
    SignedAngle = -SignedAngle; // Flip the sign if the Z component is negative
  }

  return SignedAngle;
}
