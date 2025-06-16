// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraMode.h"
#include "MuseFreeCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharacterCameraController.generated.h"

UENUM()
enum class ECameraMode : uint8
{
  DEFAULT,
  AIM
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSE_API UCharacterCameraController : public UActorComponent
{
	GENERATED_BODY()

  UPROPERTY()
  bool bHasBuiltCameraModes = false;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"), Category="Camera Modes")
  TMap<ECameraMode, TObjectPtr<UCameraMode>> CameraModeMap;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Camera Modes")
  ECameraMode ActiveCameraMode;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Camera Modes")
  TSubclassOf<AMuseFreeCamera> FreeCameraClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Camera Object Dependencies")
  TObjectPtr<AMuseFreeCamera> SpawnedFreeCamera;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Camera Object Dependencies")
  TObjectPtr<USpringArmComponent> CameraSpringArm;

public:	
	// Sets default values for this component's properties
  UCharacterCameraController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  void SwitchCameraModes(ECameraMode NewCameraMode);
  FORCEINLINE void ModifySpringArmOrientation(const bool bRotateToControlRotation) { CameraSpringArm->bUsePawnControlRotation = bRotateToControlRotation; }

private:
  bool TryBuildCameraModes();

  template<class TCameraMode>
  bool TryBuildCameraMode(const ECameraMode InNewCameraMode);

  FORCEINLINE bool ValidActiveCameraMode() const { return CameraModeMap.Contains(ActiveCameraMode) && CameraModeMap[ActiveCameraMode] != nullptr; }
};
