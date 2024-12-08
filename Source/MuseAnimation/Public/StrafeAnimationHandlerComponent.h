// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/ActorComponent.h"
#include "StrafeAnimationHandlerComponent.generated.h"

// NOTE: Order of enums matter for construction. 
UENUM(BlueprintType)
enum class EStrafeDirection : uint8 {
  FORWARD UMETA(DisplayName = "Forward"),
  RIGHT UMETA(DisplayName = "Right"),
  BACKWARD UMETA(DisplayName = "Backward"),
  LEFT UMETA(DisplayName = "Left"),
  NONE UMETA(DisplayName = "NONE")
};

/*
* House the range for strafe movement. Note, all values are in degrees and positive.
* For example forward strafe direction would have a strafe direction of 0 with min range of 45 and max range of 315 degrees.
*/
USTRUCT()
struct FStrafeMovementRange
{
  GENERATED_USTRUCT_BODY()
  float StrafeDirectionDegrees;
  float StrafeDirectionDegreesUnwind;
  float StrafeRangeLeft;
  float StrafeRangeRight;
};

class UCharacterMovementComponent;

/**
 * 
 */
UCLASS(BlueprintType)
class MUSEANIMATION_API UStrafeAnimationHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

  UPROPERTY()
  TMap<EStrafeDirection, FStrafeMovementRange> MovementRangesMap;
  UPROPERTY()
  TArray<FStrafeMovementRange> MovementRanges;
  UPROPERTY()
  float SignedMovementDirectionDegrees;
  UPROPERTY()
  EStrafeDirection ActiveStrafeDirection;
  UPROPERTY()
  TObjectPtr<UCharacterMovementComponent> CharacterMovement;

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrafeDirectionChanged, EStrafeDirection, NewDirection);
  UPROPERTY(BlueprintAssignable)
  FOnStrafeDirectionChanged StrafeDirectionChanged;

  UStrafeAnimationHandlerComponent();

  UFUNCTION(BlueprintCallable)
  void UpdateActiveStrafeDirection();
  UFUNCTION(BlueprintCallable, BlueprintPure)
  float GetAngleRelativeToActiveStrafeDirection(const float InMovementDirectionDegrees) const;
  UFUNCTION(BlueprintCallable, BlueprintPure)
  float GetStrafeDirectionDegrees(const EStrafeDirection InStrafeDirection) const;
  // Establishes a movement for the inserted direction, using a clock wise direction as the starting point with left and right range.
  UFUNCTION(BlueprintCallable)
  void SetMovementRange(const EStrafeDirection InStrafeDirection, const float InClockwiseDirectionDegrees, const float InRangeDegreesLeft, const float InRangeDegreesRight);
private:
  void UpdateSignedStrafeDirectionDegrees();
  float GetAngleRelativeToStrafeDirection(const EStrafeDirection InStrafeDirection, float InMovementDirectionDegrees) const;
  FStrafeMovementRange BuildStrafeMovementRange(const float InStrafeDirectionDegrees, const float InRangeDegreesLeft, const float InRangeDegreesRight) const;
  bool StrafeDirectionInRange(const EStrafeDirection Direction, const float InSignedDirectionDegrees) const;
  bool InvalidMovementDirection(const float InMovementDirectionDegrees) const;
  float ConvertToClockWiseRotationDegrees(const float InSignedRotationDegrees) const;
  bool IsFloatInRange(const float Value, const float Bound01, const float Bound02) const;
public:
  UFUNCTION(BlueprintCallable, BlueprintPure)
  FORCEINLINE float GetSignedStrafeMovementAngleDegrees() const { return SignedMovementDirectionDegrees; }
  UFUNCTION(BlueprintCallable, BlueprintPure)
  FORCEINLINE EStrafeDirection GetActiveStrafeDirection() const { return ActiveStrafeDirection; }
};
