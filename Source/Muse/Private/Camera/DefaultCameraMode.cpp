// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DefaultCameraMode.h"

void UDefaultCameraMode::OnEnterCameraMode()
{
  CameraSpringArm->bUsePawnControlRotation = true;
}
