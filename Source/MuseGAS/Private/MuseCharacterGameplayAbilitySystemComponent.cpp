// Fill out your copyright notice in the Description page of Project Settings.
#include "MuseCharacterGameplayAbilitySystemComponent.h"
#include "MuseGAS.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
UMuseCharacterGameplayAbilitySystemComponent::UMuseCharacterGameplayAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

  AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability system"));
}


// Called when the game starts
void UMuseCharacterGameplayAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
  InitAbilitySystem();
}

// Called every frame
void UMuseCharacterGameplayAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMuseCharacterGameplayAbilitySystemComponent::InitAbilitySystem()
{
  check(AbilitySystem);
  check(PlayerAbilities);

  const auto& InputAbilities = PlayerAbilities->GetInputAbilities();
  for (const auto& InputAbility : InputAbilities)
  {
    check(InputAbility.IsValid() == true);

    // Only give abilities on the server.
    if (GetOwner()->HasAuthority())
    {
      constexpr int32 AbilityLevel = 1;
      const int32 InputId = InputAbility.InputId;
      const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(InputAbility.GameplayAbilityClass, AbilityLevel, InputId);
      AbilitySystem->GiveAbility(AbilitySpec);
    }
  }

  // Actor responsible/owns the ability system.
  AActor* OwningActor = GetOwner();
  // Actor through which the ability system acts, uses abilities etc.
  AActor* Avatar = GetOwner();
  // Tell ability system of owner and avatar.
  AbilitySystem->InitAbilityActorInfo(OwningActor, Avatar);
}

void UMuseCharacterGameplayAbilitySystemComponent::AbilityInputPressed(int32 InputId)
{
  check(AbilitySystem);
  AbilitySystem->AbilityLocalInputPressed(InputId);
}

void UMuseCharacterGameplayAbilitySystemComponent::AbilityInputReleased(int32 InputId)
{
  check(AbilitySystem);
  AbilitySystem->AbilityLocalInputReleased(InputId);
}

void UMuseCharacterGameplayAbilitySystemComponent::BindAbilitySystemInputs(UEnhancedInputComponent* EnhancedInputComponent)
{
  check(AbilitySystem != nullptr);
  check(PlayerAbilities != nullptr);
  const auto& InputAbilities = PlayerAbilities->GetInputAbilities();
  for (const auto& InputAbility : InputAbilities)
  {
    check(InputAbility.IsValid() == true);
    const UInputAction* InputAction = InputAbility.InputAction;
    const int32 InputId = InputAbility.InputId;
    EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UMuseCharacterGameplayAbilitySystemComponent::AbilityInputPressed, InputId);
    EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UMuseCharacterGameplayAbilitySystemComponent::AbilityInputReleased, InputId);
  }
}

void UMuseCharacterGameplayAbilitySystemComponent::RefreshAbilitySystem()
{
  check(AbilitySystem != nullptr);
  AbilitySystem->RefreshAbilityActorInfo();
}

