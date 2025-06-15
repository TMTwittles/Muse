// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterCameraController.h"

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
  TObjectPtr<UCharacterCameraController> CameraController;

public:
  UCameraMode();
  virtual void OnEnterCameraMode();
  virtual void TickCameraMode(const float DeltaTime);
  bool TryConfigure(UCharacterCameraController* InCameraController);
  virtual bool PostConfigure() = 0;
  inline const bool ShouldTick() const { return bTickCameraMode; }
};
