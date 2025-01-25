// Fill out your copyright notice in the Description page of Project Settings.
#include "Melee/AnimNotifyState_MeleeAttackPhase.h"

void UAnimNotifyState_MeleeAttackPhase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
  MeleeAttackPhaseStarted.Broadcast(ENotifyMeleeAnimationPhase);
  bPropertyActive = true;
}

void UAnimNotifyState_MeleeAttackPhase::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
  bPropertyActive = false;
}
