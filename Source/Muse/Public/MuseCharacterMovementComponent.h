// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MuseCharacterMovementComponent.generated.h"

enum EMuseMoveMode : uint8;
class UMuseMoveMode;

/**
 * 
 */
UCLASS()
class MUSE_API UMuseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

  UPROPERTY()
  TMap<TEnumAsByte<EMuseMoveMode>, TObjectPtr<UMuseMoveMode>> MoveModeMap;

public:
  virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
  virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
  void ClearMovementModes();
  UFUNCTION(BlueprintCallable)
  void AddMovementMode(const EMuseMoveMode& InMoveMode);
  UFUNCTION(BlueprintCallable)
  void EnterMoveMode(const EMuseMoveMode& InMoveMode);
  UFUNCTION(BlueprintCallable)
  void ExitCustomMoveMode();
  void MoveDelta(const float& DeltaTime, const FVector& DeltaPosition, const FQuat& DeltaRotation);

  UFUNCTION(BlueprintPure)
  bool IsCustomMovementMode(EMuseMoveMode InCustomMovementMode) const;
};
