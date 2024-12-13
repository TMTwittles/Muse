// Fill out your copyright notice in the Description page of Project Settings.
#include "MoveMode/MuseMove_DefaultLocomotion.h"
#include "MuseCharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UMuseMove_DefaultLocomotion::TickMoveMode(const float DeltaTime, int32 Iterations)
{
  MovementComp->PhysCustom(MOVE_Walking, DeltaTime, Iterations);
}

void UMuseMove_DefaultLocomotion::EnterMoveMode()
{
  InitialMaxAcceleration = MovementComp->MaxAcceleration;
  InitialMaxSpeed = MovementComp->MaxWalkSpeed;
}

void UMuseMove_DefaultLocomotion::ExitMoveMode()
{
  MovementComp->MaxAcceleration = InitialMaxAcceleration;
  MovementComp->MaxWalkSpeed = InitialMaxSpeed;
}

