// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MeleePositioningSlotService.generated.h"

class UMeleePositioningSlotConfig;

/**
 * 
 */
UCLASS()
class MUSEAI_API UMeleePositioningSlotService : public UBTService
{
	GENERATED_BODY()

  virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

  UPROPERTY(EditAnywhere, Category = "Config")
  FBlackboardKeySelector TargetActor;

  UPROPERTY(EditAnywhere, Category="Config")
  TObjectPtr<UMeleePositioningSlotConfig> Config;


};
