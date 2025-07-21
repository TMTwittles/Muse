// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraMode.h"
#include "DefaultCameraMode.generated.h"

/**
 * 
 */
UCLASS()
class MUSE_API UDefaultCameraMode : public UCameraMode
{
	GENERATED_BODY()

public:
  virtual void OnEnterCameraMode() override;
	
};
