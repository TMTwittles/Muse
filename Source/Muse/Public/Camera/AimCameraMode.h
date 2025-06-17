// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraMode.h"
#include "AimCameraMode.generated.h"

/**
 * 
 */
UCLASS()
class MUSE_API UAimCameraMode : public UCameraMode
{
	GENERATED_BODY()

public:
  virtual void OnEnterCameraMode() override;
  virtual void TickCameraMode(const float DeltaTime) override;
};
