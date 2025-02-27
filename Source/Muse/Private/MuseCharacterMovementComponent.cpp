// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseCharacterMovementComponent.h"
#include "MoveMode/MuseMoveModes.h"
#include "MoveMode/MuseMoveMode.h"
#include "MoveMode/MuseMoveModeBuilder.h"
#include "GameFramework/Character.h"
#include "Statics/MuseGameplayStatics.h"

DEFINE_LOG_CATEGORY(LogMuseCharacterMovementComponent);

void UMuseCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  PrevRotation = GetOwner()->GetActorRotation();
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

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

void UMuseCharacterMovementComponent::OverrideWalkMovementSettings(const float NewAcceleration, const float NewMaxSpeed)
{
  // Store initial settings.
  bHasOverridenSettings = true;
  InitialAcceleration = MaxAcceleration;
  InitialMaxSpeed = MaxWalkSpeed;

  MaxAcceleration = NewAcceleration;
  MaxWalkSpeed = NewMaxSpeed;
}

void UMuseCharacterMovementComponent::ClearWalkMovementSettings()
{
  if (!bHasOverridenSettings)
  {
    return;
  }

  MaxAcceleration = InitialAcceleration;
  MaxWalkSpeed = InitialMaxSpeed;
}

float UMuseCharacterMovementComponent::GetCurrentAngularVelocity(const float DeltaTime) const
{
  const FRotator CurrentRotation = GetOwner()->GetActorRotation();
  const FRotator DeltaRotation = CurrentRotation - PrevRotation;
  const float CurrentAngularVelocity = DeltaRotation.Yaw != 0.0f ?
    FMath::DegreesToRadians(DeltaRotation.Yaw) / DeltaTime :
    0.0f;
  return CurrentAngularVelocity;
}

float UMuseCharacterMovementComponent::GetMaxAngularVelocity(const float DeltaTime) const
{
  const float MaxAngularVelocity = RotationRate.Yaw != 0.0f ?
    RotationRate.Yaw * DeltaTime :
    0.0f;
  return MaxAngularVelocity;
}

