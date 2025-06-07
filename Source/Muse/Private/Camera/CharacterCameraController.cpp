// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/CharacterCameraController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/DefaultCameraMode.h"
#include "Camera/AimCameraMode.h"

// Sets default values for this component's properties
UCharacterCameraController::UCharacterCameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterCameraController::Configure(USpringArmComponent* InCameraSpringArm)
{
  CameraSpringArm = InCameraSpringArm;
}

// Called when the game starts
void UCharacterCameraController::BeginPlay()
{
	Super::BeginPlay();

  // Build camera modes.
  BuildCameraModes();
  bHasBuiltCameraModes = true;
}

// Called every frame
void UCharacterCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  if (bHasBuiltCameraModes)
  {
    return;
  }

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  check(ValidActiveCameraMode());
  if (CameraModeMap[ActiveCameraMode]->ShouldTick())
  {
    CameraModeMap[ActiveCameraMode]->TickCameraMode(DeltaTime);
  }
}

void UCharacterCameraController::SwitchCameraModes(ECameraMode NewCameraMode)
{

}

void UCharacterCameraController::BuildCameraModes()
{
  check(CameraSpringArm != nullptr);

  BuildCameraMode<UDefaultCameraMode>(ECameraMode::DEFAULT);
  BuildCameraMode<UAimCameraMode>(ECameraMode::AIM);
  SwitchCameraModes(ECameraMode::DEFAULT);
}

template<class TCameraMode>
void UCharacterCameraController::BuildCameraMode(const ECameraMode InNewCameraMode)
{
  check(CameraModeMap.Contains(InNewCameraMode) == false);
  CameraModeMap.Add(InNewCameraMode);
  CameraModeMap[InNewCameraMode] = NewObject<TCameraMode>();
  CameraModeMap[InNewCameraMode]->Configure(CameraSpringArm);
}

