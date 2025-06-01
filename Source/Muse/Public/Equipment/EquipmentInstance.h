// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Equipment/Equipment.h"
#include "Equipment/EquipmentDataAsset.h"
#include "EquipmentInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MUSE_API UEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
  UEquipmentInstance();
  UEquipmentInstance(AEquipment* InEquipment, UEquipmentDataAsset* InDataAsset);
  void Initialize(AEquipment* InEquipment, UEquipmentDataAsset* InDataAsset);

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline AEquipment* GetEquipment() const { return Equipment; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline UEquipmentDataAsset* GetEquipmentData() const { return Data; }
private:
  UPROPERTY()
  AEquipment* Equipment;

  UPROPERTY()
  UEquipmentDataAsset* Data;
};
