// Fill out your copyright notice in the Description page of Project Settings.


#include "MuseCharacterMovementComponent.h"
#include "MoveMode/MuseMoveModes.h"
#include "MoveMode/MuseMoveMode.h"
#include "MoveMode/MuseMoveModeBuilder.h"
#include "GameFramework/Character.h"
#include "Statics/MuseGameplayStatics.h"

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

float UMuseCharacterMovementComponent::CalculateMovementAngle(const float DeltaTime)
{
  FRotator CurrentRotation = GetOwner()->GetActorRotation();
  // Calculate the change in rotation
  FRotator DeltaRotation = CurrentRotation - PrevRotation;

  // Convert to angular velocity (degrees per second)
  float AngularVelocity = DeltaRotation.Yaw / DeltaTime;  // Yaw is the rotation around the Z-axis

  DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + DeltaRotation.Vector().GetSafeNormal() * 100.0f, FColor::Yellow);
  DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CurrentRotation.Vector().GetSafeNormal() * 100.0f, FColor::Red);
  DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + PrevRotation.Vector().GetSafeNormal() * 100.0f, FColor::Green);

  //UE_LOG(LogMuseCharacterMovementComponent, Log, TEXT("Angular velocity: %f"), AngularVelocity);

  float SignedAngle = UMuseGameplayStatics::GetSignedAngle(CurrentRotation.Vector(), PrevRotation.Vector());

  PrevRotation = CurrentRotation;

  return SignedAngle;

  //FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
  //CurrentRotation.DiagnosticCheckNaN(TEXT("CharacterMovementComponent::PhysicsRotation(): CurrentRotation"));

  //FRotator DesiredRotation = CurrentRotation;
  //if (bOrientRotationToMovement)
  //{
  //  FRotator DeltaRot = GetDeltaRotation(DeltaTime);
  //  DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRot);
  //}
  //else if (CharacterOwner->Controller && bUseControllerDesiredRotation)
  //{
  //  DesiredRotation = CharacterOwner->Controller->GetDesiredRotation();
  //}
  //else if (!CharacterOwner->Controller && bRunPhysicsWithNoController && bUseControllerDesiredRotation)
  //{
  //  if (AController* ControllerOwner = Cast<AController>(CharacterOwner->GetOwner()))
  //  {
  //    DesiredRotation = ControllerOwner->GetDesiredRotation();
  //  }
  //}
  //DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CurrentRotation.Vector().GetSafeNormal() * 100.0f, FColor::Red);
  //DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + DesiredRotation.Vector().GetSafeNormal() * 100.0f, FColor::Green);
  //float SignedAngle = UMuseGameplayStatics::GetSignedAngle(CurrentRotation.Vector(), DesiredRotation.Vector());

  //return SignedAngle;
}
