// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraMode.h"
#include "CharacterCameraController.generated.h"

class USpringArmComponent;

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

  UPROPERTY()
  TObjectPtr<USpringArmComponent> CameraSpringArm;
  
  UPROPERTY()
  TMap<ECameraMode, TObjectPtr<UCameraMode>> CameraModeMap;

  UPROPERTY()
  ECameraMode ActiveCameraMode;

public:	
	// Sets default values for this component's properties
  UCharacterCameraController();
  void Configure(USpringArmComponent* InCameraSpringArm);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  void SwitchCameraModes(ECameraMode NewCameraMode);

private:
  void BuildCameraModes();

  template<class TCameraMode>
  void BuildCameraMode(const ECameraMode InNewCameraMode);

  inline const bool ValidActiveCameraMode() const { return CameraModeMap.Contains(ActiveCameraMode) && CameraModeMap[ActiveCameraMode] != nullptr; }
};
