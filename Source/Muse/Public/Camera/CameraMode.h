// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "CameraMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCameraMode, Log, All);

/**
 * 
 */
UCLASS()
class MUSE_API UCameraMode : public UObject
{
	GENERATED_BODY()

protected:
  UPROPERTY()
  bool bTickCameraMode = false;

  UPROPERTY()
  TObjectPtr<USpringArmComponent> CameraSpringArm;

  UPROPERTY()
  TObjectPtr<UCameraComponent> CameraComponent;

public:
  UCameraMode();
  virtual void OnEnterCameraMode();
  virtual void TickCameraMode(const float DeltaTime);
  virtual bool TryConfigure(AActor* InOwner);
  inline const bool ShouldTick() const { return bTickCameraMode; }
};
