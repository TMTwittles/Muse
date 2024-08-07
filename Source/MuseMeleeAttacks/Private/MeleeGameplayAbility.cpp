// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeGameplayAbility.h"
#include "MeleeAttackDataAsset.h"
#include "AbilityTask_PlayMeleeMontage.h"

void UMeleeGameplayAbility::InputPressed(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo)
{
  UE_LOG(LogTemp, Log, TEXT("%s Queing next combo for %s"), *this->GetName(), *PlayMeleeMontageTask->GetName());
  check(PlayMeleeMontageTask);
  PlayMeleeMontageTask->QueueNextComboAttack();
}

void UMeleeGameplayAbility::ActivateAbility(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo,
  const FGameplayEventData* TriggerEventData)
{
  UE_LOG(LogTemp, Log, TEXT("%s Activating ability."), *this->GetName());
  if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
  {
    constexpr bool bReplicateEndAbility = true;
    constexpr bool bWasCancelled = true;
    EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
  }

  for (int ComboIndex = 0; ComboIndex < ComboData->GetNumComboAttacks(); ComboIndex++)
  {
    UMeleeAttackDataAsset* MeleeAttackData = ComboData->GetMeleeAttack[ComboIndex];
    FName MeleeMontageTaskName = FName(FString::Printf(TEXT("PlayMeleeMontage")))
    PlayMeleeMontageTasks.Add(UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(this, FName(TEXT("Play melee montage")), ComboData->GetMeleeAttack[ComboIndex]));
  }

  PlayMeleeMontageTask = UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(this, FName(TEXT("Play melee montage")), ComboData);
  PlayMeleeMontageTask->AllMeleeMontagesCompleted.AddDynamic(this, &UMeleeGameplayAbility::OnAllMeleeMontagesCompleted);
  PlayMeleeMontageTask->Activate();
}

void UMeleeGameplayAbility::OnAllMeleeMontagesCompleted()
{
  check(PlayMeleeMontageTask && PlayMeleeMontageTask->AllMeleeMontagesCompleted.IsBound());
  if (PlayMeleeMontageTask && PlayMeleeMontageTask->AllMeleeMontagesCompleted.IsBound())
  {
    PlayMeleeMontageTask->AllMeleeMontagesCompleted.RemoveDynamic(this, &UMeleeGameplayAbility::OnAllMeleeMontagesCompleted);
  }
  EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
