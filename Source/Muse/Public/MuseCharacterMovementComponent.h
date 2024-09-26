// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MoveMode/MuseMoveModes.h"
#include "MoveMode/MuseMoveMode.h"
#include "MuseCharacterMovementComponent.generated.h"

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
  UFUNCTION(BlueprintCallable)
  void AddMovementMode(const EMuseMoveMode& InMoveMode);
  UFUNCTION(BlueprintCallable)
  void EnterMoveMode(const EMuseMoveMode& InMoveMode);
  void MoveDelta(const float& DeltaTime, const FVector& DeltaPosition, const FQuat& DeltaRotation);

  UFUNCTION(BlueprintPure)
  bool IsCustomMovementMode(EMuseMoveMode InCustomMovementMode) const;
};
