// Fill out your copyright notice in the Description page of Project Settings.
#include "Equipment/EquipmentManagerComponent.h"

DEFINE_LOG_CATEGORY(LogEquipmentManagerComponent);

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
  SetEquipmentState(EEquipmentState::HOLSTERED);
}

// Called every frame
void UEquipmentManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentManagerComponent::SetEquipment(const EWeapon WeaponType, const FEquipment& Equipment)
{
  if (!MappedEquipment.Contains(WeaponType))
  {
    MappedEquipment.Add(WeaponType);
  }
  MappedEquipment[WeaponType].AnimationData = Equipment.AnimationData;
  MappedEquipment[WeaponType].EquipmentMesh = Equipment.EquipmentMesh;
}

void UEquipmentManagerComponent::SetActiveEquipment(const EWeapon WeaponType)
{
  if (ActiveWeapon == WeaponType)
  {
    return;
  }

  ResetActiveEquipment();

  if (WeaponType == EWeapon::NONE)
  {
    ActiveEquipmentMesh = nullptr;
    ActiveEquipmentAnimationData = nullptr;
    SetEquipmentState(EEquipmentState::HOLSTERED);
    return;
  }

  ActiveEquipmentMesh = MappedEquipment[WeaponType].EquipmentMesh;
  ActiveEquipmentAnimationData = MappedEquipment[WeaponType].AnimationData;
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

void UEquipmentManagerComponent::SetUseFullBodyAnims(const bool bInUseFullBodyAnims)
{
  if (bUseFullBodyAnims == bInUseFullBodyAnims)
  {
    UE_LOG(LogEquipmentManagerComponent, Warning, TEXT("Trying to set bUseFullBody anims to %d when variable is already %d."), bUseFullBodyAnims, bUseFullBodyAnims);
    return;
  }
  bUseFullBodyAnims = bInUseFullBodyAnims;
  EquipmentAnimStateChanged.Broadcast(bUseFullBodyAnims);
}

void UEquipmentManagerComponent::ResetActiveEquipment()
{
  if (ActiveEquipmentMesh != nullptr)
  {
    ActiveEquipmentMesh->SetVisibility(false);
  }
  ActiveWeapon = EWeapon::NONE;
  ActiveEquipmentMesh = nullptr;
  ActiveEquipmentAnimationData = nullptr;
}
