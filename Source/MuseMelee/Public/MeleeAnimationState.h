// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMeleeAnimationState : uint8
{
  SETUP = 0 UMETA(DisplayName = "Setup"),
  ATTACK = 1 UMETA(DisplayName = "Attack"),
  RECOVERY = 2 UMETA(DisplayName = "Recovery")
};
