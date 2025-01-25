// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MeleeAnimationPhase.h"
#include "AnimNotifyState_MeleeAttackPhase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeleeAttackPhaseStarted, EMeleeAnimationPhase, AnimationPhaseStarted);

/**
 * 
 */
UCLASS(Blueprintable)
class MUSE_API UAnimNotifyState_MeleeAttackPhase : public UAnimNotifyState
{
	GENERATED_BODY()

private:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  EMeleeAnimationPhase ENotifyMeleeAnimationPhase;
  bool bPropertyActive = false;

public:
  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

  UPROPERTY()
  FMeleeAttackPhaseStarted MeleeAttackPhaseStarted;

  UFUNCTION(BlueprintPure)
  inline EMeleeAnimationPhase GetMeleeAnimPhase() { return ENotifyMeleeAnimationPhase; }

  UFUNCTION(BlueprintPure)
  inline bool IsActivePhase() { return bPropertyActive; }
};
