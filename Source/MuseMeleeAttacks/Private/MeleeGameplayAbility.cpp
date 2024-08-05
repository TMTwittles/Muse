// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeGameplayAbility.h"
#include "AbilityTask_PlayMeleeMontage.h"

void UMeleeGameplayAbility::InputPressed(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo)
{
  check(PlayMeleeMontageTask);
  PlayMeleeMontageTask->QueueNextComboAttack();
}

void UMeleeGameplayAbility::ActivateAbility(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo,
  const FGameplayEventData* TriggerEventData)
{
  	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
      constexpr bool bReplicateEndAbility = true;
      constexpr bool bWasCancelled = true;
      EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    }

    PlayMeleeMontageTask = UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(this, FName(TEXT("Play melee montage")), ComboData);
    PlayMeleeMontageTask->AllMeleeMontagesCompleted.AddDynamic(this, &UMeleeGameplayAbility::OnAllMeleeMontagesCompleted);
    PlayMeleeMontageTask->Activate();
}

void UMeleeGameplayAbility::OnAllMeleeMontagesCompleted()
{
  EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
