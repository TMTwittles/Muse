// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeSlotPositioningComponent.generated.h"

USTRUCT()
struct MUSEAI_API FMeleePositionSlot
{
  GENERATED_USTRUCT_BODY()

  FMeleePositionSlot()
  {
    SlotIndex = 0;
    SlotToken = FGuid::NewGuid();
    bSlotTaken = false;
  }

  FMeleePositionSlot(uint32 InSlotIndex)
  {
    SlotIndex = InSlotIndex;
    SlotToken = FGuid::NewGuid();
    bSlotTaken = false;
  }

  FGuid SlotToken;
  uint32 SlotIndex;
  bool bSlotTaken;
};

UCLASS()
class MUSEAI_API UMeleeSlotPositioningComponent : public UActorComponent
{
	GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, Category="Runtime", meta=(AllowPrivateAccess="true"))
  TObjectPtr<AActor> TargetSlotActor;

  UPROPERTY(EditAnywhere, Category="Config", meta=(AllowPrivateAccess="true"))
  uint32 NumSlots;

public:	
	UMeleeSlotPositioningComponent();
  void SetTargetSlotActor(AActor* InActor);
};
