// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilityTask_PlayMeleeMontage.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

void UAbilityTask_PlayMeleeMontage::QueueMontage(UAnimMontage* NextMontage)
{
  MontageComboStack.Add(NextMontage);
}

UAbilityTask_PlayMeleeMontage* UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, const UMeleeAttackDataAsset* InMeleeData)
{
  UAbilityTask_PlayMeleeMontage* PlayMeleeMontageTask = NewAbilityTask<UAbilityTask_PlayMeleeMontage>(OwningAbility, TaskInstanceName);
  PlayMeleeMontageTask->QueueMontage(InMeleeData->GetMontage());
  return PlayMeleeMontageTask;
}

void UAbilityTask_PlayMeleeMontage::Activate()
{
  if (Ability == nullptr)
  {
    return;
  }

  UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
  if (!ASC)
  {
    return;
  }

  const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
  UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
  if (!AnimInstance)
  {
    return;
  }

  if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageComboStack.Pop(), 1.0f, FName(TEXT("")), 0.0f) > 0.f)
  {
    // Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
    if (ShouldBroadcastAbilityTaskDelegates() == false)
    {
      return;
    }

    //InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UAbilityTask_PlayMontageAndWait::OnGameplayAbilityCancelled);

    //BlendingOutDelegate.BindUObject(this, &UAbilityTask_PlayMontageAndWait::OnMontageBlendingOut);
    //AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

    //MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMontageAndWait::OnMontageEnded);
    //AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

    ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
    if (Character && (Character->GetLocalRole() == ROLE_Authority ||
      (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
    {
      Character->SetAnimRootMotionTranslationScale(1.0f);
    }
  }
}

void UAbilityTask_PlayMeleeMontage::ExternalCancel()
{
}

FString UAbilityTask_PlayMeleeMontage::GetDebugString() const
{
  return FString();
}

void UAbilityTask_PlayMeleeMontage::OnDestroy(bool AbilityEnded)
{
}
