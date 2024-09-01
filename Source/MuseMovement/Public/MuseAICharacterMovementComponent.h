// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MuseAICharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MUSEMOVEMENT_API UMuseAICharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
  
  class FSavedMove_MuseAICharacter : public FSavedMove_Character
  {
    typedef FSavedMove_Character Super;

    // TODO: Insert custom AI character movement variables for replication here.

    virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
    virtual void Clear() override;
    virtual uint8 GetCompressedFlags() const override;
    virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
    virtual void PrepMoveFor(ACharacter* C) override;
  };

  class FNetworkPredictionData_Client_MuseAICharacter : public FNetworkPredictionData_Client_Character
  {
  public:
    FNetworkPredictionData_Client_MuseAICharacter(const UCharacterMovementComponent& ClientMovement);
    typedef FNetworkPredictionData_Client_Character Super;
    virtual FSavedMovePtr AllocateNewMove() override;
  };

public:
  virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
protected:
  virtual void UpdateFromCompressedFlags(uint8 Flags) override;
  virtual void PhysNavWalking(float deltaTime, int32 Iterations) override;
};
