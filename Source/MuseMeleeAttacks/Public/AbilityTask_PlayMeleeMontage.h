// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MeleeAttackDataAsset.h"
#include "Animation/AnimMontage.h"
#include "MeleeAnimationState.h"
#include "AbilityTask_PlayMeleeMontage.generated.h"

class UAbilitySystemComponent;
class UMuseMovementComponent;
class UMuseCharacterMovementComponent;
class ULockOnComponent;
class UEquipmentManagerComponent;
class UAnimInstance;
class ACharacter;
class UAnimNotifyState_MeleeAttackPhase;

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

  void SetMeleeAnimationState(const EMeleeAnimationState NewAnimationState);
  bool CanEndMeleeMontageTask();
  void EndMeleeMontageTaskOnEnterRecovery();
  void EndMeleeMontageTask();

private:
  void PlayMeleeMontage();
  void OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
  bool TrySetAvatarCharacterRootMotionScale(const float InRootMotionScale);

private:
  bool bEndTaskOnEnterRecovery;
  EMeleeAnimationState ActiveAnimationState;
  FOnMontageEnded MontageEndedDelegate;
  FOnMontageBlendingOutStarted MontageBlendingOutStartedDelegate;
  float InitialRootMotionTranslationScale;
  TArray<UAnimNotifyState_MeleeAttackPhase*> AnimationStateNotifyEvents;

  UPROPERTY()
  TObjectPtr<UMuseCharacterMovementComponent> AvatarMovementComponent;

  UPROPERTY()
  TObjectPtr<UEquipmentManagerComponent> AvatarEquipmentManagerComponent;

  UPROPERTY()
  TObjectPtr<ULockOnComponent> AvatarLockOnComponent;

  UPROPERTY()
  TObjectPtr<UMeleeAttackDataAsset> MeleeAttackData;

  UPROPERTY()
  TObjectPtr<UAbilitySystemComponent> AbilitySystemComp;

  UPROPERTY()
  TObjectPtr<UAnimInstance> AnimInstance;

  UPROPERTY()
  TObjectPtr<ACharacter> AvatarCharacter;
};
