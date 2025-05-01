// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Equipment.h"

AEquipment::AEquipment()
{
  EquipmentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root component"));
  SetRootComponent(EquipmentRoot);

  WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
  WeaponMesh->SetupAttachment(EquipmentRoot);

  IKWeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Scene component"));
  IKWeaponAttachPoint->SetupAttachment(WeaponMesh);
}

void AEquipment::SetWeaponMeshVisibility(const bool bMeshVisible)
{
  WeaponMesh->SetVisibility(bMeshVisible);
}
