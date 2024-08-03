// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MeleeAttackDataAsset.h"
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
  FMeleeMontageDelegate MeleeMontageCompleted;

  UPROPERTY(BlueprintAssignable)
  FMeleeMontageDelegate AllMeleeMontagesCompleted;

  UFUNCTION()
  void QueueMontage(UAnimMontage* NextMontage);

  UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMeleeMontage",
    HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
  static UAbilityTask_PlayMeleeMontage* CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility,
    FName TaskInstanceName, const UMeleeAttackDataAsset* InMeleeData);

  void OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
  void PlayNextMeleeMontage();
  virtual void Activate() override;
  /** Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task. */
  virtual void ExternalCancel() override;
  virtual FString GetDebugString() const override;
private:
  virtual void OnDestroy(bool bAbilityEnded) override;

  FOnMontageEnded MontageEndedDelegate;

  UPROPERTY()
  TArray<TObjectPtr<UAnimMontage>> MontageComboStack;

  UPROPERTY()
  TObjectPtr<UAnimMontage> ActiveMontage;
};
