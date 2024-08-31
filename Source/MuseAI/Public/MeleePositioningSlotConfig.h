// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MeleePositioningSlotConfig.generated.h"

/**
 * 
 */
UCLASS()
class MUSEAI_API UMeleePositioningSlotConfig : public UDataAsset
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere, Category="Slot settings")
  uint32 NumSlots;

public:
  UFUNCTION()
  inline uint32 GetNumSlots() { return NumSlots; }
};
