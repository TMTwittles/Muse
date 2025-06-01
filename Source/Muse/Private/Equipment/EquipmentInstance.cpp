// Fill out your copyright notice in the Description page of Project Settings.
#include "Equipment/EquipmentInstance.h"

UEquipmentInstance::UEquipmentInstance()
{
  Data = nullptr;
  Equipment = nullptr;
}

UEquipmentInstance::UEquipmentInstance(AEquipment* InEquipment, UEquipmentDataAsset* InDataAsset)
{
  Data = InDataAsset;
  Equipment = InEquipment;
}

void UEquipmentInstance::Initialize(AEquipment* InEquipment, UEquipmentDataAsset* InDataAsset)
{
  Data = InDataAsset;
  Equipment = InEquipment;
}
