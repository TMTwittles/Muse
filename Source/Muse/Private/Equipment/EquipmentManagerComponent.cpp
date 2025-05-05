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

void UEquipmentManagerComponent::ConstructEquipment(USkeletalMeshComponent* InSkeletalMesh, const FName& InSocketName)
{
  // TODO: Please forgive me for the lack of modularity in this current spawning implementation.

  if (SwordEquipmentData)
  {
    FActorSpawnParameters SwordSpawnParameters;
    SwordSpawnParameters.Owner = GetOwner();
    Sword = GetWorld()->SpawnActor<AEquipment>(
      SwordEquipmentData->GetEquipment(),
      FVector::Zero(),
      FRotator::ZeroRotator,
      SwordSpawnParameters);
    Sword->SetWeaponMeshVisibility(false);
    Sword->AttachToComponent(InSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
    FEquipmentInstance SwordEquipmentInstance;
    SwordEquipmentInstance.Data = SwordEquipmentData;
    SwordEquipmentInstance.Equipment = Sword;
    SetEquipment(EWeapon::SWORD, SwordEquipmentInstance);
  }
  else
  {
    UE_LOG(LogEquipmentManagerComponent, Warning, TEXT("No sword equipment has been set"));
  }

  if (RifleEquipmentData)
  {
    FActorSpawnParameters RifleSpawnParameters;
    RifleSpawnParameters.Owner = GetOwner();
    Rifle = GetWorld()->SpawnActor<AEquipment>(
      RifleEquipmentData->GetEquipment(),
      FVector::Zero(),
      FRotator::ZeroRotator,
      RifleSpawnParameters);
    Rifle->SetWeaponMeshVisibility(false);
    Rifle->AttachToComponent(InSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
    FEquipmentInstance RifleEquipmentInstance;
    RifleEquipmentInstance.Data = RifleEquipmentData;
    RifleEquipmentInstance.Equipment = Rifle;
    SetEquipment(EWeapon::RIFLE, RifleEquipmentInstance);
  }
  else
  {
    UE_LOG(LogEquipmentManagerComponent, Warning, TEXT("No rifle equipment has been set"));
  }
}

void UEquipmentManagerComponent::SetEquipment(const EWeapon WeaponType, const FEquipmentInstance& InEquipmentInstance)
{
  MappedEquipment.Add(WeaponType);
  MappedEquipment[WeaponType] = InEquipmentInstance;
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
    ActiveEquipmentInstance = nullptr;
    SetEquipmentState(EEquipmentState::HOLSTERED);
    return;
  }

  ActiveEquipmentInstance = &MappedEquipment[WeaponType];
  ActiveEquipmentInstance->GetEquipment()->SetWeaponMeshVisibility(true);
  SetEquipmentState(EEquipmentState::EQUIPPED);
  ActiveEquipmentChanged.Broadcast(ActiveEquipmentInstance->GetEquipmentData()->AnimationData);
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
  if (ActiveEquipmentInstance != nullptr)
  {
    ActiveEquipmentInstance->GetEquipment()->SetWeaponMeshVisibility(false);
  }
  ActiveWeapon = EWeapon::NONE;
  ActiveEquipmentInstance = nullptr;
}
