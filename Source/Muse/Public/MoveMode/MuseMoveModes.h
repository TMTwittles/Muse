// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MuseMoveModes.generated.h"

UENUM(BlueprintType)
enum EMuseMoveMode
{
  MMOVE_NONE UMETA(Hidden),
  MMOVE_MELEE_SUCK_TO_TARGET UMETA(DisplayName = "Melee suck to target")
};
