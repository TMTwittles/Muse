// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimSequence.h"
#include "Engine/DataAsset.h"
#include "EquipmentAnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MUSE_API UEquipmentAnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  TObjectPtr<UAnimSequence> EquipmentRestingPose;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UAnimSequence> EquipmentAimPose;

  UPROPERTY(EditAnywhere)
  float TwoHandedBlendWeight;

  UPROPERTY(EditAnywhere)
  float OneHandedBlendWeight;

  UFUNCTION(BlueprintCallable)
  inline float GetTwoHandedBlendWeight() const { return TwoHandedBlendWeight; }

  UFUNCTION(BlueprintCallable)
  inline float GetOneHandedBlendWeight() const { return OneHandedBlendWeight; }

  UFUNCTION(BlueprintCallable)
  inline UAnimSequence* GetEquipmentRestingPose() const { return EquipmentRestingPose; }

  UFUNCTION(BlueprintCallable)
  inline UAnimSequence* GetEquipmentAimPose() const { return EquipmentAimPose; }

};
