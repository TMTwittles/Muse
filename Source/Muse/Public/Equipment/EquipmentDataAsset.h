// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Equipment.h"
#include "EquipmentAnimationDataAsset.h"
#include "EquipmentDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MUSE_API UEquipmentDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = "DefaultData")
  TObjectPtr<UEquipmentComponent> Equipment;

  UPROPERTY(EditAnywhere, Category = "DefaultData")
  TObjectPtr<UEquipmentAnimationDataAsset> AnimationData;

  UFUNCTION(BlueprintCallable, Category = "DefaultData")
  inline UEquipmentComponent* GetEquipment() const { return Equipment; }

  UFUNCTION(BlueprintCallable, Category ="DefaultData")
  inline UEquipmentAnimationDataAsset* GetEquipmentAnimationData() const { return AnimationData; }
};
