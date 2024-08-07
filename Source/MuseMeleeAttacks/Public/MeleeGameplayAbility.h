// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MeleeComboDataAsset.h"
#include "MeleeGameplayAbility.generated.h"

class UAbilityTask_PlayMeleeMontage;

/**
 * 
 */
UCLASS()
class MUSEMELEEATTACKS_API UMeleeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  TObjectPtr<UMeleeComboDataAsset> ComboData;

public:
  virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
  virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
  UFUNCTION()
  void OnAllMeleeMontagesCompleted();

  UPROPERTY()
  uint32 ActiveMeleeMontageTask;

  UPROPERTY() 
  TArray<TObjectPtr<UAbilityTask_PlayMeleeMontage>> PlayMeleeMontageTasks;
};
