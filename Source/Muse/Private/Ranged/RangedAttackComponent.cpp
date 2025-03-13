// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranged/RangedAttackComponent.h"

// Sets default values for this component's properties
URangedAttackComponent::URangedAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URangedAttackComponent::BeginPlay()
{
	Super::BeginPlay();
  OwningCharacter = Cast<AMuseCharacter>(GetOwner());
  check(OwningCharacter);
  EquipmentManagerComponent = GetOwner()->GetComponentByClass<UEquipmentManagerComponent>();
  check(EquipmentManagerComponent);
}


// Called every frame
void URangedAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (bIsAiming)
  {
    
  }
}

void URangedAttackComponent::EnterAim()
{
  bIsAiming = true;
  EquipmentManagerComponent->SetActiveEquipment(EWeapon::RIFLE);
}

void URangedAttackComponent::ExitAim()
{
  bIsAiming = false;
}

void URangedAttackComponent::FireWeapon()
{
  EquipmentManagerComponent->SetActiveEquipment(EWeapon::RIFLE);
}

