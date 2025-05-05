// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MuseCharacter.generated.h"

/*
* TODO: Changed dependencies to not use forward declarations if in same module. Perform
* this once modules have been better consolidated. In this case creating a gameplay module to store alot of this code.
*/

class UEquipmentManagerComponent;
class UEquipmentComponent;
class UEquipmentDataAsset;

class UMuseCharacterMovementComponent;

class UStrafeAnimationHandlerComponent;

class URotationComponent;

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UMeleeAttackComponent;
class UMeleeComboDataAsset;

class URangedAttackComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class MUSE_API AMuseCharacter : public ACharacter
{
	GENERATED_BODY()

  /** Muse character movement component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMuseCharacterMovementComponent> MuseCharacterMovement;

  /** Equipment */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UEquipmentManagerComponent> EquipmentManager;

  /** Strafe animation handler */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StrafeAnimation, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UStrafeAnimationHandlerComponent> StrafeAnimationHandler;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

  /** Melee */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Melee, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMeleeAttackComponent> MeleeAttack;

  /** Ranged */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ranged, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<URangedAttackComponent> RangedAttack;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess="true"))
  TObjectPtr<URotationComponent> RotationComp;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

  /** Melee Input action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> MeleeAction;

  /** Fire Input action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> FireAction;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> AimAction;

  /** Sprint Action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UInputAction> SprintAction;

  float SignedAngularRotationDegrees;
  UPROPERTY(EditAnywhere)
  float RotationIncreaseDegrees = 0.25f;
  float AngularRotation = 90.0f;

public:
	AMuseCharacter(const FObjectInitializer& ObjectInitializer);
  virtual void Tick(float DeltaTime) override;
  virtual void PossessedBy(AController* NewController) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

private:
  /** Called for melee input */
  void Melee();

  /** Called for firing rifle input */
  void FireRanged();
  void StartAimRanged();
  void ExitAimRanged();

  /** Called for sprint input */
  void EnterSprint();
  void ExitSprint();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

