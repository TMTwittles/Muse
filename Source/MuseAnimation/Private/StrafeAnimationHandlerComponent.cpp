// Fill out your copyright notice in the Description page of Project Settings.
#include "StrafeAnimationHandlerComponent.h"
#include "MuseAnimation.h"
#include "GameFramework/CharacterMovementComponent.h"

const float NumStrafeDirections = 4;
const float ForwardStrafeDirectionDegrees = 0.0f;
const float StrafeRangeDegrees = 45.0f;
const float DegreesPositiveLimit = 180.0f;
const float DegreesNegativeLimit = -180.0f;

void UStrafeAnimationHandlerComponent::BeginPlay()
{
  CharacterMovement = GetOwner()->GetComponentByClass<UCharacterMovementComponent>();
  check(CharacterMovement);
}

UStrafeAnimationHandlerComponent::UStrafeAnimationHandlerComponent()
{
  ActiveStrafeDirection = EStrafeDirection::NONE;
}

void UStrafeAnimationHandlerComponent::UpdateActiveStrafeDirection()
{
  UpdateSignedStrafeDirectionDegrees();
  UE_LOG(LogMuseAnimation, Log, TEXT("%f"), SignedMovementDirectionDegrees);
  if (InvalidMovementDirection(SignedMovementDirectionDegrees) || ActiveStrafeDirection == EStrafeDirection::NONE) return;
  if (StrafeDirectionInRange(ActiveStrafeDirection, SignedMovementDirectionDegrees)) return;

  TArray<EStrafeDirection> Keys;
  MovementRangesMap.GetKeys(Keys);
  for (EStrafeDirection Direction : Keys)
  {
    if (StrafeDirectionInRange(Direction, SignedMovementDirectionDegrees))
    {
      ActiveStrafeDirection = Direction;
      StrafeDirectionChanged.Broadcast(ActiveStrafeDirection);
      return;
    }
  }

  UE_LOG(LogMuseAnimation, Error, TEXT("No movement range for %f"), SignedMovementDirectionDegrees);
}

float UStrafeAnimationHandlerComponent::GetAngleRelativeToActiveStrafeDirection(const float InMovementDirectionDegrees) const
{
  return GetAngleRelativeToStrafeDirection(ActiveStrafeDirection, InMovementDirectionDegrees);
}

float UStrafeAnimationHandlerComponent::GetStrafeDirectionDegrees(const EStrafeDirection InStrafeDirection) const
{
  return MovementRanges[(int)InStrafeDirection].StrafeDirectionDegrees;
}

void UStrafeAnimationHandlerComponent::SetMovementRange(const EStrafeDirection InStrafeDirection, const float InClockwiseDirectionDegrees, const float InRangeDegreesLeft, const float InRangeDegreesRight)
{
  if (MovementRangesMap.Contains(InStrafeDirection))
  {
    UE_LOG(LogMuseAnimation, Error, TEXT("Movement range for strafe direction %s already exists."), *UEnum::GetValueAsString(InStrafeDirection));
    return;
  }
  if (InClockwiseDirectionDegrees < 0.0f)
  {
    UE_LOG(LogMuseAnimation, Error, TEXT("Input direction for %s must be clockwise"), *UEnum::GetValueAsString(InStrafeDirection));
    return;
  }
  MovementRangesMap.Add(InStrafeDirection, BuildStrafeMovementRange(InClockwiseDirectionDegrees, InRangeDegreesLeft, InRangeDegreesRight));

  if (ActiveStrafeDirection == EStrafeDirection::NONE)
  {
    ActiveStrafeDirection = InStrafeDirection;
  }
}

void UStrafeAnimationHandlerComponent::UpdateSignedStrafeDirectionDegrees()
{
  const FVector DirectionFacingNormalized = GetOwner()->GetActorForwardVector().GetSafeNormal();
  const FVector VelocityNormalized = CharacterMovement->Velocity.GetSafeNormal();

  // Clamp in range 1.0 to -1.0 to avoid floating point errors. 
  const float DotProduct = FMath::Clamp(FVector::DotProduct(DirectionFacingNormalized, VelocityNormalized), -1.0f, 1.0f);
  const float UnsignedStrafeMovementAngleRadians = FMath::Acos(DotProduct);

  // Calculate the Z component of the cross product to determine if moving left or right. 
  const float CrossProductZ = DirectionFacingNormalized.X * VelocityNormalized.Y - DirectionFacingNormalized.Y * VelocityNormalized.X;

  // Determine the sign of the angle (clockwise or counterclockwise)
  const bool bMovingClockwise = CrossProductZ < 0;
  
  const float SignedStrafeMovementAngleRadians = bMovingClockwise ? UnsignedStrafeMovementAngleRadians * -1.0f : UnsignedStrafeMovementAngleRadians;
  SignedMovementDirectionDegrees = ConvertToClockWiseRotationDegrees(FMath::RadiansToDegrees(SignedStrafeMovementAngleRadians));
}

float UStrafeAnimationHandlerComponent::GetAngleRelativeToStrafeDirection(const EStrafeDirection InStrafeDirection, float InMovementDirectionDegrees) const
{
  if (MovementRangesMap.Contains(InStrafeDirection) == false)
  {
    UE_LOG(LogMuseAnimation, Error, TEXT("No entry for strafe direction %s exists."), *UEnum::GetValueAsString(InStrafeDirection));
    return -1.0f;
  }

  const FStrafeMovementRange MovementRange = MovementRangesMap[InStrafeDirection];
  if (MovementRange.StrafeDirectionDegrees == 0 || InMovementDirectionDegrees == 0)
  {
    // If the strafe direction degrees is 0, InMovementDirectionDegrees would already be relative in range 90,-90 degrees.
    return InMovementDirectionDegrees;
  }
  return MovementRange.StrafeDirectionDegrees - ConvertToClockWiseRotationDegrees(InMovementDirectionDegrees);
}

FStrafeMovementRange UStrafeAnimationHandlerComponent::BuildStrafeMovementRange(const float InStrafeDirectionDegrees, const float InRangeDegreesLeft, const float InRangeDegreesRight) const
{
  FStrafeMovementRange ConstructedRange;
  ConstructedRange.StrafeDirectionDegrees = InStrafeDirectionDegrees;
  ConstructedRange.StrafeDirectionDegreesUnwind = FMath::UnwindDegrees(InStrafeDirectionDegrees);
  ConstructedRange.StrafeRangeLeft = InRangeDegreesLeft;
  ConstructedRange.StrafeRangeRight = InRangeDegreesRight;
  return ConstructedRange;
}

bool UStrafeAnimationHandlerComponent::StrafeDirectionInRange(const EStrafeDirection Direction, const float InSignedDirectionDegrees) const
{
  if (MovementRangesMap.Contains(Direction) == false)
  {
    UE_LOG(LogMuseAnimation, Error, TEXT("No entry for strafe direction %s exists."), *UEnum::GetValueAsString(Direction));
    return false;
  }

  float RelativeAngle = GetAngleRelativeToStrafeDirection(Direction, InSignedDirectionDegrees);
  FStrafeMovementRange MovementRange = MovementRangesMap[Direction];
  return IsFloatInRange(RelativeAngle, MovementRange.StrafeRangeLeft, MovementRange.StrafeRangeRight);
}

bool UStrafeAnimationHandlerComponent::InvalidMovementDirection(const float InMovementDirectionDegrees) const
{
  if (InMovementDirectionDegrees > DegreesPositiveLimit || InMovementDirectionDegrees < DegreesNegativeLimit)
  {
    UE_LOG(LogMuseAnimation, Error, TEXT("Input direction: %f does not fit in limits: %f and %f"), InMovementDirectionDegrees, DegreesPositiveLimit, DegreesNegativeLimit);
    return true;
  }
  return false;
}

float UStrafeAnimationHandlerComponent::ConvertToClockWiseRotationDegrees(const float InSignedRotationDegrees) const
{
  const float FullRotationDegrees = 360.0f;
  float ClockWiseRotation = InSignedRotationDegrees;
  float Sign = FMath::Sign(InSignedRotationDegrees);
  if (Sign < 0.0f)
  {
    const float UnsignedRotationDegrees = FMath::Abs(InSignedRotationDegrees);
    int NumClockWiseRotations = InSignedRotationDegrees / FullRotationDegrees;
    NumClockWiseRotations = NumClockWiseRotations == 0 ? 1 : NumClockWiseRotations;
    ClockWiseRotation = InSignedRotationDegrees + NumClockWiseRotations * FullRotationDegrees;
  }
  return ClockWiseRotation;
}

bool UStrafeAnimationHandlerComponent::IsFloatInRange(const float Value, const float Bound01, const float Bound02) const
{
  bool bFloatInRange = false;
  if (Bound01 <= Bound02)
  {
    bFloatInRange = FMath::IsWithinInclusive(Value, Bound01, Bound02);
  }
  else if (Bound01 > Bound02)
  {
    bFloatInRange = FMath::IsWithinInclusive(Value, Bound02, Bound01);
  }
  return bFloatInRange;
}
