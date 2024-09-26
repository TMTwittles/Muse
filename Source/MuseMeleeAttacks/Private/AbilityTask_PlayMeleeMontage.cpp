// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilityTask_PlayMeleeMontage.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Pawn.h"

UAbilityTask_PlayMeleeMontage* UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, UMeleeAttackDataAsset* InMeleeAttackData)
{
  UAbilityTask_PlayMeleeMontage* PlayMeleeMontageTask = NewAbilityTask<UAbilityTask_PlayMeleeMontage>(OwningAbility, TaskInstanceName);
  PlayMeleeMontageTask->MeleeAttackData = InMeleeAttackData;
  const FGameplayAbilityActorInfo* ActorInfo = PlayMeleeMontageTask->Ability->GetCurrentActorInfo();
  if (ActorInfo)
  {
    PlayMeleeMontageTask->AnimInstance = ActorInfo->GetAnimInstance();
  }
  PlayMeleeMontageTask->AbilitySystemComp = PlayMeleeMontageTask->AbilitySystemComponent.Get();
  ACharacter* AvatarCharacter = Cast<ACharacter>(PlayMeleeMontageTask->GetAvatarActor());
  UMuse0CharacterMovementComponent* AvatarMovementComponent = AvatarCharacter != nullptr ?
    AvatarCharacter->FindComponentByClass<UMuse0CharacterMovementComponent>() : nullptr;
  PlayMeleeMontageTask->AvatarCharacter = AvatarCharacter;
  PlayMeleeMontageTask->AvatarMovementComponent = AvatarMovementComponent;
  return PlayMeleeMontageTask;
}

void UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
  EndMeleeMontageTask();
}

void UAbilityTask_PlayMeleeMontage::PlayMeleeMontage()
{
  AvatarMovementComponent->EnterMeleeAttack();
  UAnimMontage* MeleeMontage = MeleeAttackData->GetMontage();
  if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MeleeMontage, 1.0f, FName(TEXT("")), 0.0f) > 0.f)
  {
    MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded);
    AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MeleeMontage);
  }
}

void UAbilityTask_PlayMeleeMontage::EndMeleeMontageTask()
{
  AvatarMovementComponent->EndMeleeAttack();
  if (MeleeMontageTaskEnded.IsBound())
  {
    MeleeMontageTaskEnded.Broadcast();
  }
  EndTask();
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

void UAbilityTask_PlayMeleeMontage::OnDestroy(bool bInOwnerFinished)
{
  if (MeleeMontageTaskEnded.IsBound())
  {
    MeleeMontageTaskEnded.Clear();
  }

  if (MontageEndedDelegate.IsBoundToObject(this))
  {
    MontageEndedDelegate.Unbind();
  }

  AnimInstance = nullptr;
  AbilitySystemComp = nullptr;

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
    EndTask();
    return;
  }

  check(AnimInstance != nullptr);
  if (!AnimInstance)
  {
    EndTask();
    return;
  }

  check(AbilitySystemComp != nullptr);
  if (!AbilitySystemComp)
  {
    EndTask();
    return;
  }

  check(AvatarCharacter != nullptr);
  if (!AvatarCharacter)
  {
    EndTask();
    return;
  }

  InitialRootMotionTranslationScale = AvatarCharacter->GetAnimRootMotionTranslationScale();
  bool bSetRootMotionTranslationScale = TrySetAvatarCharacterRootMotionScale(1.0f);
  check(bSetRootMotionTranslationScale);
  if (!bSetRootMotionTranslationScale)
  {
    EndTask();
    return;
  }

  MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMeleeMontage::OnMeleeMontageEnded);
  PlayMeleeMontage();
}
