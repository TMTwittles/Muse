// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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
  TObjectPtr<UStaticMesh> EquipmentMesh;

  UPROPERTY(EditAnywhere, Category = "DefaultData")
  TObjectPtr<UEquipmentAnimationDataAsset> AnimationData;

  UFUNCTION(BlueprintCallable, Category = "DefaultData")
  inline UStaticMesh* GetEquipmentMesh() const { return EquipmentMesh; }

  UFUNCTION(BlueprintCallable, Category ="DefaultData")
  inline UEquipmentAnimationDataAsset* GetEquipmentAnimationData() const { return AnimationData; }
};
