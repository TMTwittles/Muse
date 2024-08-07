// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilityTask_PlayMeleeMontage.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

UAbilityTask_PlayMeleeMontage* UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, UMeleeComboDataAsset* InMeleeComboData)
{
  UAbilityTask_PlayMeleeMontage* PlayMeleeMontageTask = NewAbilityTask<UAbilityTask_PlayMeleeMontage>(OwningAbility, TaskInstanceName);
  PlayMeleeMontageTask->MeleeComboData = InMeleeComboData;
  return PlayMeleeMontageTask;
}

void UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
  if (!bPlayNextMeleeCombo || MeleeComboData->EndOfCombo(CurrentComboIndex))
  {
    if (AllMeleeMontagesCompleted.IsBound())
    {
      AllMeleeMontagesCompleted.Broadcast();
    }
    EndTask();
  }
  else
  {
    SafeIncrementComboIndex();
    bPlayNextMeleeCombo = false;
    PlayCurrentComboMeleeMontage();
  }
}

void UAbilityTask_PlayMeleeMontage::QueueNextComboAttack()
{
  bPlayNextMeleeCombo = !MeleeComboData->EndOfCombo(CurrentComboIndex);
}

void UAbilityTask_PlayMeleeMontage::PlayCurrentComboMeleeMontage()
{
  // TODO: This code should not happen but will due to timing of inputs,
  // consider alternative approaches to ending tasks. 
  if (MeleeComboData->EndOfCombo(CurrentComboIndex))
  {
    if (AllMeleeMontagesCompleted.IsBound())
    {
      AllMeleeMontagesCompleted.Broadcast();
    }
    EndTask();
  }

  UAnimMontage* NextMeleeMontage = MeleeComboData->GetMeleeAttack(CurrentComboIndex)->GetMontage();
  if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), NextMeleeMontage, 1.0f, FName(TEXT("")), 0.0f) > 0.f)
  {
    MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded);
    AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, NextMeleeMontage);
  }
}

bool UAbilityTask_PlayMeleeMontage::TrySetAvatarCharacterRootMotionScale(const float InRootMotionScale)
{
  bool bRootMotionScaleSet = false;
  check(AvatarCharacter)
  if (AvatarCharacter && (AvatarCharacter->GetLocalRole() == ROLE_Authority ||
    (AvatarCharacter->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
  {
    AvatarCharacter->SetAnimRootMotionTranslationScale(InitialRootMotionTranslationScale);
    bRootMotionScaleSet = true;
  }
  return bRootMotionScaleSet;
}

void UAbilityTask_PlayMeleeMontage::SafeIncrementComboIndex()
{
  CurrentComboIndex = CurrentComboIndex < MeleeComboData->GetNumComboAttack() - 1 ?
    CurrentComboIndex + 1 :
    CurrentComboIndex;
}

void UAbilityTask_PlayMeleeMontage::OnDestroy(bool bInOwnerFinished)
{
  if (AllMeleeMontagesCompleted.IsBound())
  {
    AllMeleeMontagesCompleted.Clear();
  }

  if (MontageEndedDelegate.IsBoundToObject(this))
  {
    MontageEndedDelegate.Unbind();
  }

  CurrentComboIndex = 0;
  bPlayNextMeleeCombo = false;
  MeleeComboData = nullptr;
  ASC = nullptr;

  if (AvatarCharacter)
  {
    TrySetAvatarCharacterRootMotionScale(InitialRootMotionTranslationScale);
    AvatarCharacter = nullptr;
  }

  Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_PlayMeleeMontage::Activate()
{
  check(Ability != nullptr);
  if (Ability == nullptr)
  {
    return;
  }

  const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
  AnimInstance = ActorInfo->GetAnimInstance();
  check(AnimInstance != nullptr);
  if (!AnimInstance)
  {
    return;
  }

  ASC = AbilitySystemComponent.Get();
  check(ASC != nullptr);
  if (!ASC)
  {
    return;
  }

  AvatarCharacter = Cast<ACharacter>(GetAvatarActor());
  check(AvatarCharacter != nullptr);
  if (!AvatarCharacter)
  {
    return;
  }

  InitialRootMotionTranslationScale = AvatarCharacter->GetAnimRootMotionTranslationScale();
  bool bSetRootMotionTranslationScale = TrySetAvatarCharacterRootMotionScale(1.0f);
  check(bSetRootMotionTranslationScale);
  if (!bSetRootMotionTranslationScale)
  {
    return;
  }

  MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded);
  CurrentComboIndex = 0;
  bPlayNextMeleeCombo = false;
  
  PlayCurrentComboMeleeMontage();
}
