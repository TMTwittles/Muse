// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "MuseCharacter.generated.h"

class UMuseCharacterMovementComponent;

class USpringArmComponent;
class UCameraComponent;

class UAbilitySystemComponent;
class UPlayerGameplayAbilitiesDataAsset;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMuseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

  /** Muse character movement component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMuseCharacterMovementComponent> MuseCharacterMovement;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

  /** Character GAS component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UAbilitySystemComponent> AbilitySystem;

  //** Player abilities */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UPlayerGameplayAbilitiesDataAsset> PlayerAbilities;

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

public:
	AMuseCharacter(const FObjectInitializer& ObjectInitializer);

  virtual void PossessedBy(AController* NewController) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// To add mapping context
	virtual void BeginPlay();

private:
  
  void InitAbilitySystem();
  void BindAbilitySystemInputs(UEnhancedInputComponent* EnhancedInputComponent);
  void AbilityInputPressed(int32 InputId);
  void AbilityInputReleased(int32 InputId);

public:
  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

