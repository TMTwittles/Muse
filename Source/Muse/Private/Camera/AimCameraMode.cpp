// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/AimCameraMode.h"

void UAimCameraMode::OnEnterCameraMode()
{
  CameraSpringArm->bUsePawnControlRotation = false;

  // Calculate rotation required to have camera face the aim target location.
  const FVector CameraForwardVector = CameraComponent->GetForwardVector();
  const FVector AimTargetLocation = OwningActor->GetActorLocation() + FVector::UpVector * 500.0f;
  const FVector CameraToAimTargetLocation = AimTargetLocation - CameraForwardVector;

  const float LookAtRotation = FMath::Acos(FVector::DotProduct(CameraToAimTargetLocation.GetSafeNormal(), AimTargetLocation.GetSafeNormal()));
  const FVector CameraRightVector = FVector::CrossProduct(CameraToAimTargetLocation.GetSafeNormal(), CameraForwardVector.GetSafeNormal());
  FQuat LookAtRotationQuat = FQuat(CameraRightVector, LookAtRotation);
  FQuat NewCameraRotation = CameraSpringArm->GetRelativeRotation().Quaternion() * LookAtRotationQuat;
  CameraSpringArm->SetRelativeRotation(NewCameraRotation);
  
}

bool UAimCameraMode::TryConfigure(AActor* InOwner)
{
  if (!Super::TryConfigure(InOwner))
  {
    return false;
  }
  OwningActor = InOwner;
  return true;
}
