// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeAnimationState.h"
#include "MeleeAttackAnimNotifyState.generated.h"

class UAbilityTask_PlayMeleeMontage;

/**
 * 
 */
UCLASS(Blueprintable)
class MUSEMELEE_API UAnimNotifyState_MeleeAttackPhase : public UAnimNotifyState
{
	GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  EMeleeAnimationState AnimationState;

  void BindAbilityTask(UAbilityTask_PlayMeleeMontage* InAbilityTask);
  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
private:
  UAbilityTask_PlayMeleeMontage* MeleeMontageAbilityTask;
};
