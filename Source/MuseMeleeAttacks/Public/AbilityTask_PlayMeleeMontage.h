// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MeleeComboDataAsset.h"
#include "Animation/AnimMontage.h"
#include "AbilityTask_PlayMeleeMontage.generated.h"

class UAbilitySystemComponent;
class UAnimInstance;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeMontageDelegate);

/**
 * Plays a melee montage, different to PlayMontageAndWait because it allows for montages to qued to play one after the after
 * based off the data provided in the MeleeComboData.
 */
UCLASS()
class MUSEMELEEATTACKS_API UAbilityTask_PlayMeleeMontage : public UAbilityTask
{
	GENERATED_BODY()

public:
  UPROPERTY(BlueprintAssignable)
  FMeleeMontageDelegate AllMeleeMontagesCompleted;

private:
  virtual void OnDestroy(bool bInOwnerFinished) override;

public:
  virtual void Activate() override;

  UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMeleeMontage",
    HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
  static UAbilityTask_PlayMeleeMontage* CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility,
    FName TaskInstanceName, UMeleeComboDataAsset* InMeleeComboData);

private:
  void OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
  void PlayCurrentComboMeleeMontage();
  bool TrySetAvatarCharacterRootMotionScale(const float InRootMotionScale);

public:
  void QueueNextComboAttack();

private:
  FOnMontageEnded MontageEndedDelegate;

  // Determines which combo from the melee combo data to play.
  uint32 CurrentComboIndex;

  // Whether the task should play the next melee combo once the current one has finished.
  bool bPlayNextMeleeCombo;

  float InitialRootMotionTranslationScale;

  UPROPERTY()
  TObjectPtr<UMeleeComboDataAsset> MeleeComboData;

  UPROPERTY()
  TObjectPtr<UAbilitySystemComponent> ASC;

  UPROPERTY()
  TObjectPtr<UAnimInstance> AnimInstance;

  UPROPERTY()
  TObjectPtr<ACharacter> AvatarCharacter;
};
