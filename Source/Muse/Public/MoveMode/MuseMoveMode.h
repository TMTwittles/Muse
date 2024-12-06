// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MuseMoveMode.generated.h"

class UMuseCharacterMovementComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class MUSE_API UMuseMoveMode : public UObject
{
	GENERATED_BODY()

protected:
  TObjectPtr<UMuseCharacterMovementComponent> MovementComp;

public:
  void SetMovementComponent(UMuseCharacterMovementComponent* MovementComponent);
  virtual void TickMoveMode(const float DeltaTime);
  virtual void EnterMoveMode();
  virtual void ExitMoveMode();
};
