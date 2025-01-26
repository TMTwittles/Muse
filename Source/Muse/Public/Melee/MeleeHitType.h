// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMeleeHitType : uint8
{
  FORWARD_HIT = 0 UMETA(DisplayName = "Forward hit"),
  RIGHT_HIT = 1 UMETA(DisplayName = "Right hit"),
  LEFT_HIT = 2 UMETA(DisplayName = "Left hit")
};
