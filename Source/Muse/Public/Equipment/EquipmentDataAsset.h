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
  UPROPERTY(EditAnywhere)
  TObjectPtr<UStaticMesh> EquipmentMesh;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UEquipmentAnimationDataAsset> AnimationData;

  UFUNCTION(BlueprintCallable)
  inline UStaticMesh* GetEquipmentMesh() const { return EquipmentMesh; }

  UFUNCTION(BlueprintCallable)
  inline UEquipmentAnimationDataAsset* GetEquipmentAnimationData() const { return AnimationData; }
};
