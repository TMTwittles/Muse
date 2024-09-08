// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CollisionAtBone.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AnimNotifyState_CollisionAtBone, Log, All);

/**
 * 
 */
UCLASS(Blueprintable)
class MUSEMELEEATTACKS_API UAnimNotifyState_CollisionAtBone : public UAnimNotifyState
{
	GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, Category="Runtime", meta = (AllowPrivateAccess = "true"))
  FVector CurrentBoneLocation;
  UPROPERTY(BlueprintReadOnly, Category="Runtime", meta = (AllowPrivateAccess = "true"))
  FVector PreviousBoneLocation;
  UPROPERTY(BlueprintReadOnly, Category="Runtime", meta=(AllowPrivateAccess="true"))
  TArray<FName> HitActors;

public:

  UPROPERTY(EditAnywhere, Category="Bone information")
  FName BoneName;

  UPROPERTY(EditAnywhere, Category = "Collision settings")
  float CollisionSphereRadius;

  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
  virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
  void PerformSphereTrace(UWorld* InWorld, const FCollisionQueryParams& InParams);
};
