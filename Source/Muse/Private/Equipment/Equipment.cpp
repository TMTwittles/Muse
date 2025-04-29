// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Equipment.h"

UEquipmentComponent::UEquipmentComponent()
{
  EquipmentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root component"));
  EquipmentRoot->SetupAttachment(this);

  WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
  WeaponMesh->SetupAttachment(EquipmentRoot);

  IKWeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Scene component"));
  IKWeaponAttachPoint->SetupAttachment(WeaponMesh);
}

void UEquipmentComponent::SetWeaponMeshVisibility(const bool bMeshVisible)
{
  WeaponMesh->SetVisibility(bMeshVisible);
}
