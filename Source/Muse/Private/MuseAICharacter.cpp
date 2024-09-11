// Fill out your copyright notice in the Description page of Project Settings.
#include "MuseAICharacter.h"
#include "MuseMovement/Public/MuseAICharacterMovementComponent.h"

AMuseAICharacter::AMuseAICharacter(const FObjectInitializer& ObjectInitializer) :
  Super(ObjectInitializer.SetDefaultSubobjectClass<UMuseAICharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
  MuseAIMovement = Cast<UMuseAICharacterMovementComponent>(GetCharacterMovement());
  MeleeHitRegister = CreateDefaultSubobject<UMeleeHitRegisterComponent>("Melee Hit Register");
}

// Called when the game starts or when spawned
void AMuseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMuseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMuseAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

