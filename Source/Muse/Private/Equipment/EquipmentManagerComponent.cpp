// Fill out your copyright notice in the Description page of Project Settings.
#include "Equipment/EquipmentManagerComponent.h"

// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
  ResetActiveEquipment();
  SwordStaticMesh->SetVisibility(false);
  RifleStaticMesh->SetVisibility(false);
  SetEquipmentState(EEquipmentState::HOLSTERED);
}

// Called every frame
void UEquipmentManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentManagerComponent::SetActiveEquipment(const EWeapon WeaponType)
{
  if (ActiveWeapon == WeaponType)
  {
    return;
  }

  ResetActiveEquipment();
  switch (WeaponType)
  {
  case EWeapon::SWORD:
    ActiveEquipmentMesh = SwordStaticMesh;
    ActiveEquipmentAnimationData = SwordEquipmentAnimationData;
    break;
  case EWeapon::RIFLE:
    ActiveEquipmentMesh = RifleStaticMesh;
    ActiveEquipmentAnimationData = RifleEquipmentAnimationData;
    break;
  case EWeapon::NONE:
  default:
    ActiveEquipmentMesh = nullptr;
    ActiveEquipmentAnimationData = nullptr;
    SetEquipmentState(EEquipmentState::HOLSTERED);
    return;
  }

  ActiveEquipmentMesh->SetVisibility(true);
  SetEquipmentState(EEquipmentState::EQUIPPED);
  ActiveEquipmentChanged.Broadcast(ActiveEquipmentAnimationData);
}

void UEquipmentManagerComponent::SetActiveEquipmentForDuration(const EWeapon InWeapon, const float EquipDuration)
{
}

void UEquipmentManagerComponent::SetEquipmentState(const EEquipmentState InEquipmentState)
{
  if (ActiveEquipmentState == InEquipmentState)
  {
    return;
  }
  ActiveEquipmentState = InEquipmentState;
  EquipmentStateChanged.Broadcast(ActiveEquipmentState);
}

void UEquipmentManagerComponent::ResetActiveEquipment()
{
  if (ActiveEquipmentMesh != nullptr)
  {
    ActiveEquipmentMesh->SetVisibility(false);
  }
  ActiveEquipmentMesh = nullptr;
  ActiveEquipmentAnimationData = nullptr;
}
