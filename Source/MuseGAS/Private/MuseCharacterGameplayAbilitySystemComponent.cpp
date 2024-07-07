// Fill out your copyright notice in the Description page of Project Settings.
#include "MuseCharacterGameplayAbilitySystemComponent.h"
#include "MuseGAS.h"
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
  check(AbilitySystem);
  //check(Ability);

  //// Only give abilities on the server.
  //if (GetOwner()->HasAuthority())
  //{
  //  AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
  //}

  //// Actor responsible/owns the ability system.
  //AActor* OwningActor = GetOwner();
  //// Actor through which the ability system acts, uses abilities etc.
  //AActor* Avatar = GetOwner();
  //// Tell ability system of owner and avatar.
  //AbilitySystem->InitAbilityActorInfo(OwningActor, Avatar);
}


// Called every frame
void UMuseCharacterGameplayAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UMuseCharacterGameplayAbilitySystemComponent::BindAbilitySystemInputs(UInputComponent* PlayerInputComponent)
{
  check(AbilitySystem != nullptr);
  FTopLevelAssetPath InputEnumPath = FTopLevelAssetPath(TEXT("/Script/Muse"), TEXT("ECharacterAbilityInput"));
  FGameplayAbilityInputBinds InputBinds = FGameplayAbilityInputBinds(TEXT("ConfirmInput"), TEXT("CancelInput"), InputEnumPath);
  AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, InputBinds);
}

void UMuseCharacterGameplayAbilitySystemComponent::RefreshAbilitySystem()
{
  check(AbilitySystem != nullptr);
  AbilitySystem->RefreshAbilityActorInfo();
}

