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
  if (!bPlayNextMeleeCombo)
  {
    if (AllMeleeMontagesCompleted.IsBound())
    {
      AllMeleeMontagesCompleted.Broadcast();
    }
    EndTask();
  }
  else
  {
    CurrentComboIndex += 1;
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
  const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
  UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
  if (!AnimInstance)
  {
    return;
  }

  UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
  if (!ASC)
  {
    return;
  }

  bPlayNextMeleeCombo = false;
  UAnimMontage* NextMeleeMontage = MeleeComboData->GetMeleeAttack(CurrentComboIndex)->GetMontage();
  if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), NextMeleeMontage, 1.0f, FName(TEXT("")), 0.0f) > 0.f)
  {
    MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded);
    AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, NextMeleeMontage);

    ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
  }
}

void UAbilityTask_PlayMeleeMontage::Activate()
{
  if (Ability == nullptr)
  {
    return;
  }

  MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded);
  CurrentComboIndex = 0;
  ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
  if (Character && (Character->GetLocalRole() == ROLE_Authority ||
    (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
  {
    Character->SetAnimRootMotionTranslationScale(1.0f);
  }

  PlayCurrentComboMeleeMontage();
}
