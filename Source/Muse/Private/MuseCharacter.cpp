// Copyright Epic Games, Inc. All Rights Reserved.

#include "MuseCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilityInputInfo.h"
#include "MoveMode/MuseMoveModes.h"
#include "PlayerGameplayAbilitiesDataAsset.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMuseCharacter

AMuseCharacter::AMuseCharacter(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer.SetDefaultSubobjectClass<UMuseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
  // Cast movement component to character movement component.
  MuseCharacterMovement = Cast<UMuseCharacterMovementComponent>(GetCharacterMovement());
  

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create ability system component.
  AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("CharacterGameplayAbilities"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMuseCharacter::PossessedBy(AController* NewController)
{
  Super::PossessedBy(NewController);

  // Notify server actor has changed. 
  AbilitySystem->RefreshAbilityActorInfo();
}

void AMuseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

  // Add custom movement modes.
  MuseCharacterMovement->ClearMovementModes();
  MuseCharacterMovement->AddMovementMode(EMuseMoveMode::MMOVE_MELEE_SUCK_TO_TARGET);

  // Initialize ability system, granting character available abilities.
  InitAbilitySystem();
}

void AMuseCharacter::InitAbilitySystem()
{
  check(AbilitySystem);
  check(PlayerAbilities);

  const auto& InputAbilities = PlayerAbilities->GetInputAbilities();
  for (const auto& InputAbility : InputAbilities)
  {
    check(InputAbility.IsValid() == true);

    // Only give abilities on the server.
    if (HasAuthority())
    {
      constexpr int32 AbilityLevel = 1;
      const int32 InputId = InputAbility.InputId;
      const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(InputAbility.GameplayAbilityClass, AbilityLevel, InputId);
      AbilitySystem->GiveAbility(AbilitySpec);
    }
  }

  // Actor responsible/owns the ability system.
  AActor* OwningActor = this;
  // Actor through which the ability system acts, uses abilities etc.
  AActor* Avatar = this;
  // Tell ability system of owner and avatar.
  AbilitySystem->InitAbilityActorInfo(OwningActor, Avatar);
}

void AMuseCharacter::BindAbilitySystemInputs(UEnhancedInputComponent* EnhancedInputComponent)
{
  check(AbilitySystem != nullptr);
  check(PlayerAbilities != nullptr);
  const auto& InputAbilities = PlayerAbilities->GetInputAbilities();
  for (const auto& InputAbility : InputAbilities)
  {
    check(InputAbility.IsValid() == true);
    const UInputAction* InputAction = InputAbility.InputAction;
    const int32 InputId = InputAbility.InputId;
    EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &AMuseCharacter::AbilityInputPressed, InputId);
    EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &AMuseCharacter::AbilityInputReleased, InputId);
  }
}

void AMuseCharacter::AbilityInputPressed(int32 InputId)
{
  check(AbilitySystem);
  AbilitySystem->AbilityLocalInputPressed(InputId);
}

void AMuseCharacter::AbilityInputReleased(int32 InputId)
{
  check(AbilitySystem);
  AbilitySystem->AbilityLocalInputReleased(InputId);
}

UAbilitySystemComponent* AMuseCharacter::GetAbilitySystemComponent() const
{
  return AbilitySystem;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMuseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMuseCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMuseCharacter::Look);

    // Ability system inputs.
    BindAbilitySystemInputs(EnhancedInputComponent);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMuseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMuseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
