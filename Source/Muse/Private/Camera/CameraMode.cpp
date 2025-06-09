// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/CameraMode.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY(LogCameraMode);

UCameraMode::UCameraMode()
{
}

void UCameraMode::OnEnterCameraMode()
{

}

void UCameraMode::TickCameraMode(const float DeltaTime)
{
  check(bShouldTick);
}

bool UCameraMode::TryConfigure(const AActor& InOwner)
{
  CameraSpringArm = InOwner.GetComponentByClass<USpringArmComponent>();
  if (!CameraSpringArm)
  {
    UE_LOG(LogCameraMode, Warning, TEXT("Unable to locate camera spring arm in owner: %s"), *InOwner.GetName());
    return false;
  }
  return true;
}
