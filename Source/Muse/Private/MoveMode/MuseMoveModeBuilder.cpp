// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveMode/MuseMoveModeBuilder.h"
#include "MoveMode/MuseMoveModes.h"
#include "MuseCharacterMovementComponent.h"
#include "MoveMode/MuseMove_MeleeSuckToTarget.h"
#include "MoveMode/MuseMove_DefaultLocomotion.h"

DEFINE_LOG_CATEGORY(LogMuseMoveModeBuilder);

UMuseMoveMode* UMuseMoveModeBuilder::BuildMoveMode(UMuseCharacterMovementComponent* InMovementComp, const EMuseMoveMode& InMoveMode)
{
  UMuseMoveMode* BuiltMoveMode = nullptr;
  switch (InMoveMode)
  {
  case MMOVE_MELEE_SUCK_TO_TARGET:
    BuiltMoveMode = NewObject<UMuseMove_MeleeSuckToTarget>();
    break;
  default:
    UE_LOG(LogMuseMoveModeBuilder, Error, TEXT("No entry added for MoveMode"));
    break;
  }
  check(BuiltMoveMode);
  BuiltMoveMode->SetMovementComponent(InMovementComp);
  return BuiltMoveMode;
}
