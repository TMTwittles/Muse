// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveMode/MuseMoveMode.h"
#include "MuseMove_DefaultLocomotion.generated.h"

/**
 * Lazy wrapper for move walk.
 */
UCLASS()
class MUSE_API UMuseMove_DefaultLocomotion : public UMuseMoveMode
{
	GENERATED_BODY()

private:
  // TODO: Store this in a better struct. 
  float InitialMaxAcceleration;
  float InitialMaxSpeed;
  float MaxAcceleration;
  float MaxSpeed;

public:
  virtual void TickMoveMode(const float DeltaTime, int32 Iterations) override;
  virtual void EnterMoveMode() override;
  virtual void ExitMoveMode() override;

  // TODO: Have this configuring exposed via blueprint. Preferably a data asset.
  inline void SetMaxAcceleration(const float InMaxAcceleration) { MaxAcceleration = InMaxAcceleration; };
  inline void SetMaxSpeed(const float InMaxSpeed) { MaxSpeed = InMaxSpeed; };
};
