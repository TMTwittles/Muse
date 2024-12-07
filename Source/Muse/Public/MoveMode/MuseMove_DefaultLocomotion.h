// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveMode/MuseMoveMode.h"
#include "MuseMove_DefaultLocomotion.generated.h"

/**
 * 
 */
UCLASS()
class MUSE_API UMuseMove_DefaultLocomotion : public UMuseMoveMode
{
	GENERATED_BODY()

private:


public:
  virtual void TickMoveMode(const float DeltaTime) override;
  virtual void EnterMoveMode() override;
  virtual void ExitMoveMode() override;
};
