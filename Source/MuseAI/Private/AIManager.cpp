// Fill out your copyright notice in the Description page of Project Settings.
#include "AIManager.h"
#include "GameFramework/Character.h"
#include "MeleeSlotPositioningComponent.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  MeleeSlotPositioning = CreateDefaultSubobject<UMeleeSlotPositioningComponent>(TEXT("MeleeSlotPositioning"));
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();

  // Set target slot actor for melee position slot manager.
  MeleeSlotPositioning->SetTargetSlotActor(Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetCharacter()));
}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

