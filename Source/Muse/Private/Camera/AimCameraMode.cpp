// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/AimCameraMode.h"

void UAimCameraMode::OnEnterCameraMode()
{
  const FVector CameraForwardVector = CameraComponent->GetForwardVector();
  const FVector AimTargetLocation = OwningActor->GetActorLocation();
}

bool UAimCameraMode::TryConfigure(AActor* InOwner)
{
  bool bConfiguredAimCameraMode = Super::TryConfigure(InOwner);
  OwningActor = InOwner;
  return bConfiguredAimCameraMode;
}
