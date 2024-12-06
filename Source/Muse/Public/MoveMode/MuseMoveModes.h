// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MuseMoveModes.generated.h"

UENUM(BlueprintType)
enum EMuseMoveMode : uint8
{
  MMOVE_NONE UMETA(Hidden),
  MMOVE_MELEE_SUCK_TO_TARGET UMETA(DisplayName = "Melee suck to target")
};
