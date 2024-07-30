// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CharacterAbilityInput.generated.h"

UENUM(BlueprintType)
enum class ECharacterAbilityInput : uint8
{
  UseAbility1 UMETA(DisplayName = "Use Ability01"),
  UseAbility2 UMETA(DisplayName = "Use Ability02"),
  UseAbility3 UMETA(DisplayName = "Use Ability03")
};
