// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CameraMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCameraMode, Log, All);

class USpringArmComponent;

/**
 * 
 */
UCLASS()
class MUSE_API UCameraMode : public UObject
{
	GENERATED_BODY()

protected:
  UPROPERTY()
  bool bShouldTick = false;

  UPROPERTY()
  TObjectPtr<USpringArmComponent> CameraSpringArm;

public:
  UCameraMode();
  virtual void OnEnterCameraMode();
  virtual void TickCameraMode(const float DeltaTime);
  virtual bool TryConfigure(const AActor& InOwner);
  inline const bool ShouldTick() const { return bShouldTick; }
};
