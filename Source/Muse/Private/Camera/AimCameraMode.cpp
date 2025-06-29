// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/AimCameraMode.h"
#include "Camera/CameraComponent.h"

void UAimCameraMode::OnExitCameraMode()
{
  // TODO: Make a method that will switch to the last used camera before this.
  SwitchToDefaultCamera();
}

void UAimCameraMode::OnEnterCameraMode()
{
  SwitchToFreeCamera();
}

void UAimCameraMode::TickCameraMode(const float DeltaTime)
{
  APawn* Pawn = Cast<APawn>(OwningActor);
  FQuat ControlRotationQuat = FQuat::Identity;

  if (Pawn && Pawn->GetController())
  {
    ControlRotationQuat = Pawn->GetController()->GetControlRotation().Quaternion();
  }

  // Set target screen positions (In range 0.0f - 1.0f)
  const float TargetScreenPosX = 0.65f;
  const float TargetScreenPosY = 0.5f;
  // Calculate screen offsets, moving target screen positions to range (-0.5f, 0.5f);
  const float ScreenOffsetX = (TargetScreenPosX - 0.5f);
  const float ScreenOffsetY = (TargetScreenPosY - 0.5f);

 
  const float FovY = FreeCamera->GetCameraComponent().FieldOfView;
  const float FovX = FreeCamera->GetCameraComponent().AspectRatio * FovY;
  const float AngleYaw = FMath::DegreesToRadians(ScreenOffsetX * FovX);
  const float AnglePitch = FMath::DegreesToRadians(ScreenOffsetY * FovY);
  const FQuat ScreenPitchRot = FQuat(FVector::RightVector, AnglePitch);
  const FQuat ScreenYawRot = FQuat(FVector::UpVector, AngleYaw);
  
  FQuat NewCameraRotation = ScreenYawRot * ScreenPitchRot * ControlRotationQuat;

  FreeCamera->SetActorRotation(NewCameraRotation);

  // TODO: Change this to be data oriented. 
  const float DistanceFromPlayer = 600.0f;
  const FVector DesiredCameraPosition = OwningActor->GetActorLocation() + -ControlRotationQuat.GetForwardVector() * DistanceFromPlayer;
  FreeCamera->SetActorLocation(DesiredCameraPosition);
}

bool UAimCameraMode::PostConfigure()
{
  bTickCameraMode = true;
  return true;
}

