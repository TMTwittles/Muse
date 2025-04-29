// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Equipment.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MUSE_API UEquipmentComponent : public USceneComponent
{
	GENERATED_BODY()

private:
  UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<USceneComponent> EquipmentRoot;

  UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"))
  TObjectPtr<UStaticMeshComponent> WeaponMesh;

  UPROPERTY(EditAnywhere, meta = (AllowprivateAccess = "true"))
  TObjectPtr<USceneComponent> IKWeaponAttachPoint;

public:
  UEquipmentComponent();

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline FTransform GetIKWeaponJointTransform() const { return IKWeaponAttachPoint->GetComponentTransform(); }

  UFUNCTION(BlueprintCallable)
  void SetWeaponMeshVisibility(const bool bMeshVisible);
};
