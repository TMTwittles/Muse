// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MuseEquipment.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MUSE_API AMuseEquipment : public AActor
{
	GENERATED_BODY()

public:
  AMuseEquipment();

private:
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess="true"))
  TObjectPtr<USceneComponent> EquipmentRoot;

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess="true"))
  TObjectPtr<UStaticMeshComponent> WeaponMesh;

  UPROPERTY(EditAnywhere, meta = (AllowprivateAccess = "true"))
  TObjectPtr<USceneComponent> IKWeaponAttachPoint;
};
