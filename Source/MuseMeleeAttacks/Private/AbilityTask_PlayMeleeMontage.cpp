// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilityTask_PlayMeleeMontage.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

void UAbilityTask_PlayMeleeMontage::QueueMontage(UAnimMontage* NextMontage)
{
  MontageComboStack.EmplaceAt(0, NextMontage);
}

UAbilityTask_PlayMeleeMontage* UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, const UMeleeAttackDataAsset* InMeleeData)
{
  UAbilityTask_PlayMeleeMontage* PlayMeleeMontageTask = NewAbilityTask<UAbilityTask_PlayMeleeMontage>(OwningAbility, TaskInstanceName);
  PlayMeleeMontageTask->QueueMontage(InMeleeData->GetMontage());
  return PlayMeleeMontageTask;
}

void UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
  if (MontageComboStack.Num() == 0)
  {
    if (AllMeleeMontagesCompleted.IsBound())
    {
      AllMeleeMontagesCompleted.Broadcast();
    }
    EndTask();
  }
  else
  {
    PlayNextMeleeMontage();
  }
}

void UAbilityTask_PlayMeleeMontage::PlayNextMeleeMontage()
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

  UAnimMontage* NextMeleeMontage = MontageComboStack.Pop();
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

  ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
  if (Character && (Character->GetLocalRole() == ROLE_Authority ||
    (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
  {
    Character->SetAnimRootMotionTranslationScale(1.0f);
  }

  PlayNextMeleeMontage();
}

void UAbilityTask_PlayMeleeMontage::ExternalCancel()
{
}

FString UAbilityTask_PlayMeleeMontage::GetDebugString() const
{
  return FString();
}

void UAbilityTask_PlayMeleeMontage::OnDestroy(bool bAbilityEnded)
{
}
