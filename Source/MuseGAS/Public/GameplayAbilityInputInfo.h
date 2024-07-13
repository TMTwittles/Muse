// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityInputInfo.generated.h"

class UInputAction;

/**
 * 
 */
USTRUCT()
struct FGameplayAbilityInputInfo
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, Category = "GameplayAbiltityInputInfo")
  TSubclassOf<UGameplayAbility> GameplayAbilityClass;

  UPROPERTY(EditAnywhere, Category = "GameplayAbilityInputInfo")
  TObjectPtr<UInputAction> InputAction;

  UPROPERTY(VisibleAnywhere, Category="GameplayAbilityInputInfo")
  int32 InputId;

  bool IsValid() const
  {
    return GameplayAbilityClass && InputAction;
  }

  friend uint32 GetTypeHash(const FGameplayAbilityInputInfo& Item)
  {
    return HashCombine(GetTypeHash(Item.GameplayAbilityClass), GetTypeHash(Item.InputAction));
  }
};
