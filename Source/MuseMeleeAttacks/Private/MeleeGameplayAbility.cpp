// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeGameplayAbility.h"
#include "AbilityTask_PlayMeleeMontage.h"

void UMeleeGameplayAbility::InputPressed(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo)
{
  
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

    UAbilityTask_PlayMeleeMontage* PlayMeleeMontage = UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(this, FName(TEXT("Play melee montage")), ComboData->GetMeleeAttack(0));
    PlayMeleeMontage->AllMeleeMontagesCompleted.AddDynamic(this, &UMeleeGameplayAbility::OnAllMeleeMontagesCompleted);
    PlayMeleeMontage->Activate();
}

void UMeleeGameplayAbility::OnAllMeleeMontagesCompleted()
{
  EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
