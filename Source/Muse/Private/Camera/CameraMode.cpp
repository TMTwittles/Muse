// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/CameraMode.h"

DEFINE_LOG_CATEGORY(LogCameraMode);

UCameraMode::UCameraMode()
{
}

void UCameraMode::OnEnterCameraMode()
{

}

void UCameraMode::TickCameraMode(const float DeltaTime)
{
  check(bTickCameraMode);
}

bool UCameraMode::TryConfigure(UCharacterCameraController* InCameraController)
{
  CameraController = InCameraController;
  OwningActor = InCameraController->GetOwner();
  PostConfigure();
  return true;
}
