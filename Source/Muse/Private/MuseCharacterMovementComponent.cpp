// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseCharacterMovementComponent.h"
#include "MoveMode/MuseMoveMode.h"
#include "MoveMode/MuseMoveModes.h"
#include "MoveMode/MuseMoveModeBuilder.h"

DEFINE_LOG_CATEGORY(LogMuseCharacterMovementComponent);

void UMuseCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
  Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UMuseCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
  Super::PhysCustom(DeltaTime, Iterations);
  check(MoveModeMap.Contains((EMuseMoveMode)CustomMovementMode));
  MoveModeMap[(EMuseMoveMode)CustomMovementMode]->TickMoveMode(DeltaTime, Iterations);
}

void UMuseCharacterMovementComponent::PhysCustom(const TEnumAsByte<enum EMovementMode> InMove, float DeltaTime, int32 Iterations)
{
  switch (InMove)
  {
    case MOVE_Walking:
      PhysWalking(DeltaTime, Iterations);
      break;
    default:
      UE_LOG(LogMuseCharacterMovementComponent, Error, TEXT("I havent added that move mode yet. Soz."));
      break;
  }
}

void UMuseCharacterMovementComponent::ClearMovementModes()
{
  MoveModeMap.Empty();
}

void UMuseCharacterMovementComponent::AddMovementMode(const EMuseMoveMode& InMoveMode)
{
  check(MoveModeMap.Num() == 0 || !MoveModeMap.Contains(InMoveMode));
  MoveModeMap.Add(InMoveMode);
  MoveModeMap[InMoveMode] = UMuseMoveModeBuilder::BuildMoveMode(this, InMoveMode);
}

void UMuseCharacterMovementComponent::EnterMoveMode(const EMuseMoveMode& InMoveMode)
{
  check(MoveModeMap.Contains(InMoveMode));
  
  if (MovementMode == MOVE_Custom)
  {
    MoveModeMap[InMoveMode]->ExitMoveMode();
  }
  SetMovementMode(MOVE_Custom, InMoveMode);
  MoveModeMap[InMoveMode]->EnterMoveMode();
}

void UMuseCharacterMovementComponent::ExitCustomMoveMode()
{
  SetMovementMode(MOVE_Walking);
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

template<class TMuseMoveMode>
TMuseMoveMode* UMuseCharacterMovementComponent::GetMoveMode(TEnumAsByte<EMuseMoveMode> InMoveMode) const
{
  if (MoveModeMap.Contains(InMoveMode) == false)
  {
    UE_LOG(LogMuseCharacterMovementComponent, Error, TEXT("No move mode entry added"));
    return nullptr;
  }

  return static_cast<TMuseMoveMode>(MoveModeMap[InMoveMode]);
}
