// Fill out your copyright notice in the Description page of Project Settings.
#include "Camera/CharacterCameraController.h"
#include "Camera/DefaultCameraMode.h"
#include "Camera/AimCameraMode.h"

DEFINE_LOG_CATEGORY(LogCharacterCameraController)

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

  TryConfigurePostBeginPlay();
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
  CameraModeMap[ActiveCameraMode]->OnExitCameraMode();
  ActiveCameraMode = NewCameraMode;
  CameraModeMap[ActiveCameraMode]->OnEnterCameraMode();
}

bool UCharacterCameraController::TryConfigurePostBeginPlay()
{
  CameraSpringArm = GetOwner()->GetComponentByClass<USpringArmComponent>();
  if (!CameraSpringArm)
  {
    UE_LOG(LogCharacterCameraController, Warning, TEXT("No spring arm located in owner. This will be used by non-free cam camera modes."));
    return false;
  }

  // Spawn free camera
  FActorSpawnParameters FreeCameraSpawnParams;
  FreeCameraSpawnParams.Name = FName(*FString::Printf(TEXT("%s_FreeCamera"), *GetOwner()->GetName()));
  FreeCameraSpawnParams.Owner = GetOwner();
  SpawnedFreeCamera = GetOwner()->GetWorld()->SpawnActor<AMuseFreeCamera>(FreeCameraClass, FreeCameraSpawnParams);
  if (!SpawnedFreeCamera)
  {
    UE_LOG(LogCharacterCameraController, Warning, TEXT("Unable to spawn free camera, please ensure valid class has been set in settings."));
    return false;
  }

  // Build camera modes.
  bHasBuiltCameraModes = TryBuildCameraModes();
  if (!bHasBuiltCameraModes)
  {
    UE_LOG(LogCharacterCameraController, Warning, TEXT("Unable to successfully build camera modes."));
    return false;
  }
  OnCameraModesBuilt.Broadcast();

  return true;
}

bool UCharacterCameraController::TryBuildCameraModes()
{
  CameraModeConfigureContainer Container;
  Container.DefaultCameraSpringArm = CameraSpringArm;
  Container.OwningActor = GetOwner();
  Container.SpawnedFreeCamera = SpawnedFreeCamera;

  // TODO: Add error checking here. 
  TryBuildCameraMode<UDefaultCameraMode>(Container, ECameraMode::DEFAULT);
  TryBuildCameraMode<UAimCameraMode>(Container, ECameraMode::AIM);

  SwitchCameraModes(ECameraMode::DEFAULT);

  return true;
}

bool UCharacterCameraController::TryGetCameraMode(const ECameraMode InCameraMode, UCameraMode* OutCameraMode)
{
  if (CameraModeMap.Contains(InCameraMode) && CameraModeMap[InCameraMode] != nullptr)
  {
    OutCameraMode = CameraModeMap[InCameraMode];
  }
  return OutCameraMode != nullptr;
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

