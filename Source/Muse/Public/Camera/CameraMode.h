// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CameraModeConfigureContainer.h"
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
  TObjectPtr<AActor> OwningActor;

  UPROPERTY()
  TObjectPtr<AMuseFreeCamera> SpawnedFreeCamera;

  UPROPERTY()
  TObjectPtr<USpringArmComponent> DefaultCameraSpringArm;

  void SwitchToFreeCamera();
  void SwitchToDefaultCamera();

public:

  UCameraMode();
  virtual void OnExitCameraMode();
  virtual void OnEnterCameraMode();
  virtual void TickCameraMode(const float DeltaTime);
  bool TryConfigure(const CameraModeConfigureContainer& InContainer);
  virtual bool PostConfigure();
  inline const bool ShouldTick() const { return bTickCameraMode; }
};
