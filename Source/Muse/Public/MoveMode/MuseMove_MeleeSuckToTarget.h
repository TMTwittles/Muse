// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveMode/MuseMoveMode.h"
#include "MuseMove_MeleeSuckToTarget.generated.h"

/**
 * 
 */
UCLASS()
class MUSE_API UMuseMove_MeleeSuckToTarget : public UMuseMoveMode
{
	GENERATED_BODY()

private:
  UPROPERTY()
  AActor* TargetActor;

  UPROPERTY()
  FVector TargetSuckPosition;

public:
  virtual void TickMoveMode(const float DeltaTime) override;
  virtual void EnterMoveMode() override;
  virtual void ExitMoveMode() override;

  bool TrySetMeleeTargetActor();
  void UpdateTargetSuckPosition();
  inline bool TargetActorFound() const { return TargetActor != nullptr; }
};
