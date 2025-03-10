// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Equipment/EquipmentAnimationDataAsset.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "RangedAnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MUSE_API URangedAnimationDataAsset : public UEquipmentAnimationDataAsset
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere, Category = "RangedEquipmentProperties|AimOffsets")
  TObjectPtr<UAimOffsetBlendSpace> StandardAimOffset;
};
