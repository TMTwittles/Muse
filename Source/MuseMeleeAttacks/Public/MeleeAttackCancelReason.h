// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MeleeAttackCancelReason.generated.h"

UENUM(BlueprintType)
enum class EMeleeAttackCancelReason : uint8 {
  JUMP_PRESSED = 0 UMETA(DisplayName = "DOWN")
};
