// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveMode/MuseMoveMode.h"

void UMuseMoveMode::SetMovementComponent(UMuseCharacterMovementComponent* MovementComponent)
{
  MovementComp = MovementComponent;
}

void UMuseMoveMode::TickMoveMode(const float DeltaTime)
{
}

void UMuseMoveMode::EnterMoveMode()
{
}

void UMuseMoveMode::ExitMoveMode()
{
}
