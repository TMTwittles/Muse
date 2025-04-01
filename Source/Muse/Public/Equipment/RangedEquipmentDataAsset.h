// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquipmentDataAsset.h"
#include "RangedAnimationDataAsset.h"
#include "RangedEquipmentDataAsset.generated.h"


/**
 * 
 */
UCLASS()
class MUSE_API URangedEquipmentDataAsset : public UEquipmentDataAsset
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere, Category = "RangedEquipementData|")
  TObjectPtr<URangedEquipmentDataAsset> RangedEquipmentAnimationData;
};
