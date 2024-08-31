// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"

class UMeleeSlotPositioningComponent;

UCLASS()
class MUSEAI_API AAIManager : public AActor
{
	GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, Category="Positioning", meta=(AllowPrivateAccess="true"))
  TObjectPtr<UMeleeSlotPositioningComponent> MeleeSlotPositioning;

public:	
	// Sets default values for this actor's properties
	AAIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintPure)
  inline UMeleeSlotPositioningComponent* GetMeleeSlotPositioningComponent() const { return MeleeSlotPositioning; }
};
