// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/CameraMode.h"
#include "GameFramework/SpringArmComponent.h"

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

void UCameraMode::Configure(USpringArmComponent* InSpringArmComponent)
{
  SpringArmComponent = InSpringArmComponent;
}
