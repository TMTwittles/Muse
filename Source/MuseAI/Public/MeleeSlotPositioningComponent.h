// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeSlotPositioningComponent.generated.h"

USTRUCT(BlueprintType)
struct MUSEAI_API FMeleePositionSlot
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
  FVector Position;
  UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
  bool bValidSlot = false;
  UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
  bool bSlotTaken = false;

  FMeleePositionSlot()
  {
    Position = FVector::Zero();
    bValidSlot = false;
    bSlotTaken = false;
  }
};

UCLASS(BlueprintType)
class MUSEAI_API UMeleeSlotPositioningComponent : public UActorComponent
{
	GENERATED_BODY()

  FCriticalSection TokenAccessMutex;

  UPROPERTY(BlueprintReadOnly, Category="Runtime", meta=(AllowPrivateAccess="true"))
  TObjectPtr<AActor> TargetSlotActor;

  UPROPERTY(BlueprintReadOnly, Category="Runtime", meta=(AllowPrivateAccess="true"))
  TArray<FMeleePositionSlot> Slots;

  UPROPERTY(BlueprintReadOnly, Category = "Runtime", meta = (AllowPrivateAccess = "true"))
  TMap<FName, int32> NameToSlotIndexMap; 

  UPROPERTY(EditAnywhere, Category="Positioning config", meta=(AllowPrivateAccess="true"))
  float HorizontalDistanceFromPlayer;

  UPROPERTY(EditAnywhere, Category = "Positioning config", meta = (AllowPrivateAccess = "true"))
  float MinHorizontalDistanceFromPlayer;

  UPROPERTY(EditAnywhere, Category = "Positioning config", meta = (AllowPrivateAccess = "true"))
  float VerticalDistanceFromPlayer;

  UPROPERTY(EditAnywhere, Category="Config", meta=(AllowPrivateAccess="true"))
  uint32 NumSlots;

public:	
	UMeleeSlotPositioningComponent();
  virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  const FVector GetMeleePosition(FName InName);
  inline const int32 FindMoveableSlotIndex() const;
  void ConfigureForActor(AActor* InActor);
};
