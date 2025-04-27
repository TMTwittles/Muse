// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/MuseEquipment.h"

AMuseEquipment::AMuseEquipment()
{
  EquipmentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root component"));
  SetRootComponent(EquipmentRoot);

  WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
  SetRootComponent(WeaponMesh);

  IKWeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Scene component"));
  IKWeaponAttachPoint->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepRelativeTransform);
}
