// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLockOnComponent, Log, All);

class UStrafeAnimationHandlerComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLockedOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLockOnCleared);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSELOCKON_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<AActor> LockOnTargetActor;
  bool bLockOnActive;
  bool bHardLockOnActive;
  bool bShouldLockOn;
  bool bTickLockOnDuration;
  float CurrentLockOnDuration;
  UStrafeAnimationHandlerComponent* OwnerStrafeAnimationHandler;
  USpringArmComponent* CameraSpringArm;
  APlayerController* PlayerController;

  float InitialCameraSpringArmLength;
  FVector InitialCameraSpringArmRelativeLocation;
  FRotator InitialCameraSpringArmRelativeRotation;
  FRotator InitialControlRotation;

public:

  FLockedOn LockedOn;
  FLockOnCleared LockedOnCleared;

	// Sets default values for this component's properties
	ULockOnComponent();

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline bool HasLockOnTarget() const { return LockOnTargetActor != nullptr; }

  UFUNCTION(BlueprintCallable, BlueprintPure)
  inline FVector GetLockOnTargetPosition() const { return LockOnTargetActor == nullptr ? FVector::Zero() : LockOnTargetActor->GetActorLocation(); }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  virtual void TickCameraLockOnLocation(float DeltaTime);

  UFUNCTION(BlueprintCallable, BlueprintPure)
  const FQuat GetRotationToLockOnTarget(FVector RelativeLocation, bool bIncludeYaw, float MidPointPercentage=1.0f) const;

  UFUNCTION(BlueprintCallable)
  void EnterLockOn();
  UFUNCTION(BlueprintCallable)
  void EnterLockOnForDuration(float LockOnDuration);
  UFUNCTION(BlueprintCallable)
  void EnterHardLockOn();
  UFUNCTION(BlueprintCallable)
  void ExitHardLockOn();
  UFUNCTION(BlueprintCallable)
  void ExitLockOn();
  UFUNCTION(BlueprintCallable)
  void SetCameraSpringArm(USpringArmComponent* InCameraSpringArm);
  UFUNCTION(BlueprintCallable)
  inline bool LockOnActive() const { return bLockOnActive; }

  UFUNCTION(BlueprintCallable)
  inline bool HardLockOnActive() const { return bHardLockOnActive; }

private:
  UFUNCTION(BlueprintCallable)
  bool TryUpdateLockOnTarget();

  UFUNCTION(BlueprintCallable)
  inline void ClearLockOnTarget() { LockOnTargetActor == nullptr; }
};
