// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeAnimationState.h"
#include "AnimNotifyState_MeleeAttackPhase.generated.h"

class UAbilityTask_PlayMeleeMontage;

/**
 * 
 */
UCLASS(Blueprintable)
class MUSEMELEEATTACKS_API UMeleeAttackAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  uint8 MeleeAttackPhaseID;

  void BindAbilityTask(UAbilityTask_PlayMeleeMontage* InAbilityTask);
  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
private:
  void SetMeleeAttackPhase(EMeleeAnimationState InMeleeAttackPhase);
private:
  UAbilityTask_PlayMeleeMontage* MeleeMontageAbilityTask;
};
