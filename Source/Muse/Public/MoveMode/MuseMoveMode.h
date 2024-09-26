// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../MuseCharacterMovementComponent.h"
#include "MuseMoveMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MUSE_API UMuseMoveMode : public UObject
{
	GENERATED_BODY()

public:
  virtual void TickMoveMode(const float DeltaTime, UMuseCharacterMovementComponent& MovementComponent);
  virtual void EnterMoveMode();
  virtual void ExitMoveMode();
};
