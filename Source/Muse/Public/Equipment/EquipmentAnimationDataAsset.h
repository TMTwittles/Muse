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
  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|AnimSequences")
  TObjectPtr<UAnimSequence> EquipmentRestingPose;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|AnimSequences")
  TObjectPtr<UAnimSequence> EquipmentAimPose;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|FullbodyEquipmentProperties")
  bool bIsFullBody;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|FullbodyEquipmentProperties")
  float FullBodyBlendWeight;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|RightHandedEquipmentProperties")
  bool bApplyRightHandedBlendWeight;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|RightHandedEquipmentProperties")
  float RightHandedBlendWeight;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|LeftHandedEquipmentProperties")
  bool bApplyLeftHandedBlendWeight;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|LeftHandedEquipmentProperties")
  float LeftHandedBlendWeight;

  UPROPERTY(EditAnywhere, Category = "DefaultEquipmentAnimationProperties|IKHandLocation", meta = (EditCondition="bApplyLeftHandedBlendWeight && bApplyRightHandedBlendWeight"))
  FTransform EquipmentIKHandLocation;

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

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline FTransform GetEquipmentIKHandLocation() const { return EquipmentIKHandLocation; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline bool IsFullBodyAnim() const { return bIsFullBody; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline float GetFullBodyBlendWeight() const { return FullBodyBlendWeight; }
};
