// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MeleeAttackDataAsset.h"
#include "Animation/AnimMontage.h"
#include "MuseMovement/Public/MuseCharacterMovementComponent.h"
#include "AbilityTask_PlayMeleeMontage.generated.h"

class UAbilitySystemComponent;
class UAnimInstance;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMeleeMontageDelegate);

/**
 * Plays a melee montage based of input MeleeAttackData. 
 */
UCLASS()
class MUSEMELEEATTACKS_API UAbilityTask_PlayMeleeMontage : public UAbilityTask
{
	GENERATED_BODY()

public:
  UPROPERTY(BlueprintAssignable)
  FMeleeMontageDelegate MeleeMontageTaskEnded;

private:
  virtual void OnDestroy(bool bInOwnerFinished) override;

public:
  virtual void Activate() override;

  UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMeleeMontage",
    HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
  static UAbilityTask_PlayMeleeMontage* CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility,
    FName TaskInstanceName, UMeleeAttackDataAsset* InMeleeAttackData);

private:
  void PlayMeleeMontage();
  void EndMeleeMontageTask();
  void OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
  bool TrySetAvatarCharacterRootMotionScale(const float InRootMotionScale);

private:
  FOnMontageEnded MontageEndedDelegate;
  float InitialRootMotionTranslationScale;

  UPROPERTY()
  TObjectPtr<UMuseCharacterMovementComponent> AvatarMovementComponent;

  UPROPERTY()
  TObjectPtr<UMeleeAttackDataAsset> MeleeAttackData;

  UPROPERTY()
  TObjectPtr<UAbilitySystemComponent> AbilitySystemComp;

  UPROPERTY()
  TObjectPtr<UAnimInstance> AnimInstance;

  UPROPERTY()
  TObjectPtr<ACharacter> AvatarCharacter;
};
