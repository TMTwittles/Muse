// Fill out your copyright notice in the Description page of Project Settings.
#include "Ranged/RangedAttackComponent.h"
#include "Statics/MuseGameplayStatics.h"

DEFINE_LOG_CATEGORY(LogRangedAttackComponent);

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
  OwningMovementComponent = Cast<UCharacterMovementComponent>(OwningCharacter->GetMovementComponent());
  check(OwningMovementComponent);
  EquipmentManagerComponent = GetOwner()->GetComponentByClass<UEquipmentManagerComponent>();
  check(EquipmentManagerComponent);
}


// Called every frame
void URangedAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (bIsAiming)
  {
    TickAimComponent(DeltaTime);
  }
}

void URangedAttackComponent::EnterAim()
{
  bIsAiming = true;
  OwningMovementComponent->bOrientRotationToMovement = false;
  OwningMovementComponent->bUseControllerDesiredRotation = true;
}

void URangedAttackComponent::ExitAim()
{
  bIsAiming = false;
  OwningMovementComponent->bOrientRotationToMovement = true;
  OwningMovementComponent->bUseControllerDesiredRotation = false;
}

void URangedAttackComponent::FireWeapon()
{
  EquipmentManagerComponent->SetActiveEquipment(EWeapon::RIFLE);
}

void URangedAttackComponent::TickAimComponent(const float DeltaTime)
{
  FVector AimDirection = OwningCharacter->GetControlRotation().Vector();
  FVector CharacterForward = OwningCharacter->GetActorForwardVector();
  float SignedAngle = UMuseGameplayStatics::GetSignedAngle(CharacterForward, AimDirection, -GetOwner()->GetActorRightVector());
  AimPitch = FMath::RadiansToDegrees(SignedAngle);
  AimPitch = 0.0f;
}

