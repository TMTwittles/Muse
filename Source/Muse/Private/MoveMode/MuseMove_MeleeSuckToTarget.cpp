// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveMode/MuseMove_MeleeSuckToTarget.h"

void UMuseMove_MeleeSuckToTarget::TickMoveMode(const float DeltaTime, UMuseCharacterMovementComponent& MovementComponent)
{
  if (!TargetActorFound())
  {
    return;
  }
 
  UpdateTargetSuckPosition();

  const float SuckToMeleePositionSpeed = 300.0f;
  const FVector SuckMovementDirection = (SuckMovementDirection - MovementComponent.GetOwner()->GetActorLocation()).GetSafeNormal();
  const FVector TargetVelocity = SuckMovementDirection * SuckToMeleePositionSpeed;

  FVector Delta = TargetVelocity * DeltaTime;
  FQuat DeltaRotation = FRotationMatrix::MakeFromXZ(TargetVelocity.GetSafeNormal(), FVector::UpVector).ToQuat();

  MovementComponent.MoveDelta(DeltaTime, Delta, DeltaRotation);

  return;
}

void UMuseMove_MeleeSuckToTarget::EnterMoveMode()
{
}

void UMuseMove_MeleeSuckToTarget::ExitMoveMode()
{
}

bool UMuseMove_MeleeSuckToTarget::TrySetMeleeTargetActor()
{


  return false;
}

void UMuseMove_MeleeSuckToTarget::UpdateTargetSuckPosition()
{

}
