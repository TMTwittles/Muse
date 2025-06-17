// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/CameraMode.h"

DEFINE_LOG_CATEGORY(LogCameraMode);

UCameraMode::UCameraMode()
{
}

void UCameraMode::OnExitCameraMode()
{
}

void UCameraMode::OnEnterCameraMode()
{

}

void UCameraMode::TickCameraMode(const float DeltaTime)
{
  check(bTickCameraMode);
}

bool UCameraMode::TryConfigure(const CameraModeConfigureContainer& InContainer)
{
  DefaultCameraSpringArm = InContainer.DefaultCameraSpringArm;
  SpawnedFreeCamera = InContainer.SpawnedFreeCamera;
  OwningActor = InContainer.OwningActor;
  PostConfigure();
  return true;
}

bool UCameraMode::PostConfigure()
{
  return false;
}

void UCameraMode::SwitchToFreeCamera()
{
  APlayerController* PC = OwningActor->GetWorld()->GetFirstPlayerController();
  if (PC && SpawnedFreeCamera)
  {
    PC->SetViewTargetWithBlend(SpawnedFreeCamera, 1.0f);
  }
}

void UCameraMode::SwitchToDefaultCamera()
{
  APlayerController* PC = OwningActor->GetWorld()->GetFirstPlayerController();
  if (PC && OwningActor)
  {
    PC->SetViewTargetWithBlend(OwningActor, 1.0f);
  }
}
