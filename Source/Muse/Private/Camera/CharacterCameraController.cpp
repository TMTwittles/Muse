// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/CharacterCameraController.h"
#include "Camera/DefaultCameraMode.h"
#include "Camera/AimCameraMode.h"

// Sets default values for this component's properties
UCharacterCameraController::UCharacterCameraController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCharacterCameraController::BeginPlay()
{
	Super::BeginPlay();

  CameraSpringArm = GetOwner()->GetComponentByClass<USpringArmComponent>();

  // Spawn free camera
  SpawnedFreeCamera = GetOwner()->GetWorld()->SpawnActor<AMuseFreeCamera>(FreeCameraClass);

  // Build camera modes.
  bHasBuiltCameraModes = TryBuildCameraModes();
}

// Called every frame
void UCharacterCameraController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  if (!bHasBuiltCameraModes)
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
  if (ActiveCameraMode == NewCameraMode)
  {
    return;
  }
  ActiveCameraMode = NewCameraMode;
  CameraModeMap[ActiveCameraMode]->OnEnterCameraMode();
}

bool UCharacterCameraController::TryBuildCameraModes()
{
  CameraModeConfigureContainer Container;
  Container.DefaultCameraSpringArm = CameraSpringArm;
  Container.OwningActor = GetOwner();
  Container.SpawnedFreeCamera = SpawnedFreeCamera;

  TryBuildCameraMode<UDefaultCameraMode>(Container, ECameraMode::DEFAULT);
  TryBuildCameraMode<UAimCameraMode>(Container, ECameraMode::AIM);
  SwitchCameraModes(ECameraMode::DEFAULT);
  return true;
}

template<class TCameraMode>
bool UCharacterCameraController::TryBuildCameraMode(const CameraModeConfigureContainer& InContainer, const ECameraMode InNewCameraMode)
{
  check(CameraModeMap.Contains(InNewCameraMode) == false);

  TObjectPtr<TCameraMode> NewCameraMode = NewObject<TCameraMode>();
  if (!NewCameraMode->TryConfigure(InContainer))
  {
    NewCameraMode->ConditionalBeginDestroy();
    return false;
  }

  CameraModeMap.Add(InNewCameraMode);
  CameraModeMap[InNewCameraMode] = NewCameraMode;
  return true;
}

