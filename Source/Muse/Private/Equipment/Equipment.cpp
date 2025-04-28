// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/Equipment.h"

AEquipment::AEquipment()
{
  EquipmentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root component"));
  SetRootComponent(EquipmentRoot);

  WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
  WeaponMesh->AttachToComponent(EquipmentRoot, FAttachmentTransformRules::KeepRelativeTransform);

  IKWeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Scene component"));
  IKWeaponAttachPoint->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void AEquipment::SetWeaponMeshVisibility(const bool bVisible)
{
  WeaponMesh->SetVisibility(bVisible);
}
