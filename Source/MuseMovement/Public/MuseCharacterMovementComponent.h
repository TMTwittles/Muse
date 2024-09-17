// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MuseCharacter.h"
#include "MuseCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum EMuseMovementMode
{
  CMOVE_NONE UMETA(Hidden),
  CMOVE_MELEE_ATTACK UMETA(DisplayName = "Melee Attack")
};

/**
 * 
 */
UCLASS()
class MUSEMOVEMENT_API UMuseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

  class FSavedMove_MuseCharacter : public FSavedMove_Character
  {
    typedef FSavedMove_Character Super;

    uint8 Saved_bInMeleeAttack:1;

    virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
    virtual void Clear() override;
    virtual uint8 GetCompressedFlags() const override;
    virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
    virtual void PrepMoveFor(ACharacter* C) override;
  };

  class FNetworkPredictionData_Client_MuseCharacter : public FNetworkPredictionData_Client_Character
  {
  public:
    FNetworkPredictionData_Client_MuseCharacter(const UCharacterMovementComponent& ClientMovement);
    typedef FNetworkPredictionData_Client_Character Super;
    virtual FSavedMovePtr AllocateNewMove() override;
  };

  bool bInMeleeAttack;
  bool Safe_bInMeleeAttack;

protected:
  virtual void InitializeComponent() override;

public:
  virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
protected:
  virtual void UpdateFromCompressedFlags(uint8 Flags) override;
  virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
  virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
public:
  UFUNCTION(BlueprintCallable)
  void EnterMeleeAttack();

private:
  void ExitMeleeAttack();
  void PhysMeleeAttack(float DeltaTime, int32 Iterations);

public:

  UFUNCTION(BlueprintPure)
  bool IsCustomMovementMode(EMuseMovementMode InCustomMovementMode) const;
};
