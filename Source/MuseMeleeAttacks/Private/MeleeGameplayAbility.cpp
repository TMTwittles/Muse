// Fill out your copyright notice in the Description page of Project Settings.
#include "MeleeGameplayAbility.h"
#include "AbilityTask_PlayMeleeMontage.h"

void UMeleeGameplayAbility::InputPressed(
  const FGameplayAbilitySpecHandle Handle,
  const FGameplayAbilityActorInfo* ActorInfo,
  const FGameplayAbilityActivationInfo ActivationInfo)
{
  bPlayerTriggeredNextMeleeMontageTask = true;
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

  for (uint32 ComboIndex = 0; ComboIndex < ComboData->GetNumComboAttacks(); ComboIndex++)
  {
    const UMeleeAttackDataAsset* MeleeAttackData = ComboData->GetMeleeAttack(ComboIndex);
    FName MeleeMontageTaskName = FName(FString::Printf(TEXT("PlayMeleeMontageTask_%d_%s"), ComboIndex, *MeleeAttackData->GetMontage()->GetName()));
    PlayMeleeMontageTasks.Add(UAbilityTask_PlayMeleeMontage::CreatePlayMeleeMontageProxy(this, MeleeMontageTaskName, ComboData->GetMeleeAttack(ComboIndex)));
  }

  check(PlayMeleeMontageTasks.Num() > 0);
  if (PlayMeleeMontageTasks.Num() > 0)
  {
    bPlayerTriggeredNextMeleeMontageTask = false;
    ActiveMeleeMontageTaskIndex = 0;
    PlayNextMeleeMontageTask();
  }
  else
  {
    constexpr bool bReplicateEndAbility = true;
    constexpr bool bWasCancelled = true;
    EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
  }
}

void UMeleeGameplayAbility::PlayNextMeleeMontageTask()
{
  UAbilityTask_PlayMeleeMontage* NextMeleeMontage = PlayMeleeMontageTasks[ActiveMeleeMontageTaskIndex];
  check(NextMeleeMontage);
  if (NextMeleeMontage)
  {
    NextMeleeMontage->MeleeMontageTaskEnded.AddDynamic(this, &UMeleeGameplayAbility::OnMeleeMontageTaskCompleted);
    NextMeleeMontage->Activate();
  }
}

void UMeleeGameplayAbility::OnMeleeMontageTaskCompleted()
{
  UAbilityTask_PlayMeleeMontage* CompletedMeleeMontage = PlayMeleeMontageTasks[ActiveMeleeMontageTaskIndex];
  check(CompletedMeleeMontage);
  check(CompletedMeleeMontage->MeleeMontageTaskEnded.IsBound());
  if (CompletedMeleeMontage && CompletedMeleeMontage->MeleeMontageTaskEnded.IsBound())
  {
    CompletedMeleeMontage->MeleeMontageTaskEnded.RemoveDynamic(this, &UMeleeGameplayAbility::OnMeleeMontageTaskCompleted);
  }
  // Play the next melee montage task if it was triggered by the player and there still montages left to play. 
  bool bPlayNextMeleeMontageTask = bPlayerTriggeredNextMeleeMontageTask &&
    !ComboData->EndOfCombo(ActiveMeleeMontageTaskIndex);

  bPlayerTriggeredNextMeleeMontageTask = false;
  if (bPlayNextMeleeMontageTask)
  {
    ActiveMeleeMontageTaskIndex += ActiveMeleeMontageTaskIndex + 1 <= ComboData->GetNumComboAttacks() - 1;
    PlayNextMeleeMontageTask();
  }
  // End ability if the player did not trigger another montage to play.
  else
  {
    EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
  }
}
