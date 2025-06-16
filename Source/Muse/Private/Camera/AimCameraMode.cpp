// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/AimCameraMode.h"

void UAimCameraMode::OnEnterCameraMode()
{
  CameraController->ModifySpringArmOrientation(false);
}

void UAimCameraMode::TickCameraMode(const float DeltaTime)
{
  APawn* Pawn = Cast<APawn>(OwningActor);
  FQuat ControlRotation = FQuat::Identity;
  if (Pawn && Pawn->GetController())
  {
    ControlRotation = Pawn->GetController()->GetControlRotation().Quaternion();
  }

  // Calculate rotation required to have camera face the aim target location.
  const FVector CameraForwardVector = ;
  const FVector AimTargetLocation = OwningActor->GetActorLocation() + FVector::UpVector * 500.0f;
  const FVector CameraToAimTargetLocation = AimTargetLocation - CameraForwardVector;

  const float LookAtRotation = FMath::Acos(FVector::DotProduct(CameraToAimTargetLocation.GetSafeNormal(), AimTargetLocation.GetSafeNormal()));
  const FVector CameraRightVector = FVector::CrossProduct(CameraToAimTargetLocation.GetSafeNormal(), CameraForwardVector.GetSafeNormal());
  FQuat LookAtRotationQuat = FQuat(CameraRightVector, LookAtRotation);
  FQuat NewCameraRotation = ControlRotation * LookAtRotationQuat;
  
}

bool UAimCameraMode::PostConfigure()
{
  
}
