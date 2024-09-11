// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeHitRegisterComponent.h"
#include "MuseAICharacter.generated.h"

class UMuseAICharacterMovementComponent;

UCLASS()
class MUSE_API AMuseAICharacter : public ACharacter
{
	GENERATED_BODY()

private:
  /** Muse AI character movement component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMuseAICharacterMovementComponent> MuseAIMovement;

  /** Muse AI character movement component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeleeHitRegistration, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMeleeHitRegisterComponent> MeleeHitRegister;

public:
	// Sets default values for this character's properties
	AMuseAICharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
