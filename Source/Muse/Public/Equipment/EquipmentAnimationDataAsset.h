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

  UPROPERTY(EditAnywhere, Category = "RightHandedEquipmentProperties")
  bool bApplyRightHandedBlendWeight;

  UPROPERTY(EditAnywhere, Category = "RightHandedEquipmentProperties")
  float RightHandedBlendWeight;

  UPROPERTY(EditAnywhere, Category = "LeftHandedEquipmentProperties")
  bool bApplyLeftHandedBlendWeight;

  UPROPERTY(EditAnywhere, Category = "LeftHandedEquipmentProperties")
  float LeftHandedBlendWeight;

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline bool ApplyRightHandedBlendWeight() const { return bApplyRightHandedBlendWeight; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline bool ApplyLeftHandedBlendWeight() const { return bApplyLeftHandedBlendWeight; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline float GetLeftHandedBlendWeight() const { return LeftHandedBlendWeight; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline float GetRightHandedBlendWeight() const { return RightHandedBlendWeight; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline UAnimSequence* GetEquipmentRestingPose() const { return EquipmentRestingPose; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline UAnimSequence* GetEquipmentAimPose() const { return EquipmentAimPose; }

};
