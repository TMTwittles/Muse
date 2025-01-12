// Fill out your copyright notice in the Description page of Project Settings.
#include "AnimNotifyState_MeleeAttackPhase.h"
#include "AbilityTask_PlayMeleeMontage.h"

void UMeleeAttackAnimNotifyState::BindAbilityTask(UAbilityTask_PlayMeleeMontage* InAbilityTask)
{
  MeleeMontageAbilityTask = InAbilityTask;
}

void UMeleeAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
  SetMeleeAttackPhase((EMeleeAnimationState)MeleeAttackPhaseID);
}

void UMeleeAttackAnimNotifyState::SetMeleeAttackPhase(EMeleeAnimationState InMeleeAttackPhase)
{
  if (MeleeMontageAbilityTask)
  {
    MeleeMontageAbilityTask->SetMeleeAnimationState(InMeleeAttackPhase);
  }
}
