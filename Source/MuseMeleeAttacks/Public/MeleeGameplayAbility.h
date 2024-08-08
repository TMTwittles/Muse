// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MeleeComboDataAsset.h"
#include "MeleeGameplayAbility.generated.h"

class UAbilityTask_PlayMeleeMontage;

/**
 * Plays a melee attack, listens for input to trigger comboable melee attacks based off input ComboData.
 */
UCLASS()
class MUSEMELEEATTACKS_API UMeleeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
  TObjectPtr<UMeleeComboDataAsset> ComboData;

  UPROPERTY()
  bool bPlayerTriggeredNextMeleeMontageTask;

  UPROPERTY()
  uint32 ActiveMeleeMontageTaskIndex;

  UPROPERTY()
  TArray<TObjectPtr<UAbilityTask_PlayMeleeMontage>> PlayMeleeMontageTasks;

public:
  virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
  virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
  void PlayNextMeleeMontageTask();

  UFUNCTION()
  void OnMeleeMontageTaskCompleted();
};
