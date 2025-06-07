// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CameraMode.generated.h"

class USpringArmComponent;

/**
 * 
 */
UCLASS()
class MUSE_API UCameraMode : public UObject
{
	GENERATED_BODY()

  TObjectPtr<USpringArmComponent> SpringArmComponent;

protected:
  bool bShouldTick = false;

public:
  UCameraMode();
  virtual void OnEnterCameraMode();
  virtual void TickCameraMode(const float DeltaTime);
  void Configure(USpringArmComponent* InSpringArmComponent);

  inline const bool ShouldTick() const { return bShouldTick; }
};
