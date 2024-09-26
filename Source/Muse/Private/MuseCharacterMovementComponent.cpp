// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseCharacterMovementComponent.h"
#include "MoveMode/MuseMoveModeBuilder.h"

void UMuseCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
  Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UMuseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
  Super::PhysCustom(DeltaTime, Iterations);
  check(MoveModeMap.Contains((EMuseMoveMode)CustomMovementMode));
  MoveModeMap[(EMuseMoveMode)CustomMovementMode]->TickMoveMode(DeltaTime, *this);
}

void UMuseCharacterMovementComponent::AddMovementMode(const EMuseMoveMode& InMoveMode)
{
  check(!MoveModeMap.Contains(InMoveMode));
  MoveModeMap.Add(InMoveMode);
  MoveModeMap[InMoveMode] = UMuseMoveModeBuilder::BuildMoveMode(InMoveMode);
}

void UMuseCharacterMovementComponent::EnterMoveMode(const EMuseMoveMode& InMoveMode)
{
  check(MoveModeMap.Contains(InMoveMode));
  MovementMode == MOVE_Custom;
  CustomMovementMode = InMoveMode;
  MoveModeMap[(EMuseMoveMode)CustomMovementMode]->EnterMoveMode();
}

void UMuseCharacterMovementComponent::MoveDelta(const float& DeltaTime, const FVector& DeltaPosition, const FQuat& DeltaRotation)
{
  FVector OldLocation = UpdatedComponent->GetComponentLocation();
  FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
  FHitResult Hit(1.f);
  SafeMoveUpdatedComponent(DeltaPosition, DeltaRotation, true, Hit);

  if (Hit.Time < 1.f)
  {
    HandleImpact(Hit, DeltaTime, DeltaPosition);
    SlideAlongSurface(DeltaPosition, (1.f - Hit.Time), Hit.Normal, Hit, true);
  }

  // Update outgoing velocity
  if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
  {
    Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
  }
}

bool UMuseCharacterMovementComponent::IsCustomMovementMode(EMuseMoveMode InCustomMovementMode) const
{
  return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
