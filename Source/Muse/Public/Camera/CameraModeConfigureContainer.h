// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "MuseFreeCamera.h"
#include "GameFramework/SpringArmComponent.h"

struct CameraModeConfigureContainer
{
  AActor* OwningActor;
  AMuseFreeCamera* SpawnedFreeCamera;
  USpringArmComponent* DefaultCameraSpringArm;
};

