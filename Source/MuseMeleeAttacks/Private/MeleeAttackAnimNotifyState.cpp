// Fill out your copyright notice in the Description page of Project Settings.
#include "MeleeAttackAnimNotifyState.h"
#include "AbilityTask_PlayMeleeMontage.h"

void UAnimNotifyState_MeleeAttackPhase::BindAbilityTask(UAbilityTask_PlayMeleeMontage* InAbilityTask)
{
  MeleeMontageAbilityTask = InAbilityTask;
}

void UAnimNotifyState_MeleeAttackPhase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
  if (MeleeMontageAbilityTask)
  {
    MeleeMontageAbilityTask->SetMeleeAnimationState(AnimationState);
  }
}
