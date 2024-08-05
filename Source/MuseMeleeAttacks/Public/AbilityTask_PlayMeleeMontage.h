// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MeleeComboDataAsset.h"
#include "AbilityTask_PlayMeleeMontage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeMontageDelegate);

/**
 * 
 */
UCLASS()
class MUSEMELEEATTACKS_API UAbilityTask_PlayMeleeMontage : public UAbilityTask
{
	GENERATED_BODY()

public:
  UPROPERTY(BlueprintAssignable)
  FMeleeMontageDelegate AllMeleeMontagesCompleted;

  virtual void Activate() override;

  UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMeleeMontage",
    HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
  static UAbilityTask_PlayMeleeMontage* CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility,
    FName TaskInstanceName, UMeleeComboDataAsset* InMeleeComboData);

private:
  void OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
  void PlayCurrentComboMeleeMontage();

public:
  void QueueNextComboAttack();

private:
  FOnMontageEnded MontageEndedDelegate;
  uint32 CurrentComboIndex;
  bool bPlayNextMeleeCombo;

  UPROPERTY()
  TObjectPtr<UMeleeComboDataAsset> MeleeComboData;
};
