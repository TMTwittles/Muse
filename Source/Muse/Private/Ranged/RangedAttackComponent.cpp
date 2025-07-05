// Fill out your copyright notice in the Description page of Project Settings.
#include "Ranged/RangedAttackComponent.h"
#include "Statics/MuseGameplayStatics.h"
#include "Gameplay/RotationComponent.h"
#include "Camera/CameraComponent.h"

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
  ConfigureActiveCamera();
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

void URangedAttackComponent::ConfigureActiveCamera()
{
  MusePlayerController = Cast<AMusePlayerController>(GetOwner()->GetWorld()->GetFirstPlayerController());
  check(MusePlayerController);
  MusePlayerController->OnViewTargetChanged.AddDynamic(this, &URangedAttackComponent::UpdateActiveCamera);
  UpdateActiveCamera(MusePlayerController->GetViewTarget());
}

void URangedAttackComponent::UpdateActiveCamera(AActor* NewViewTarget)
{
  ActivePlayerCamera = NewViewTarget->GetComponentByClass<UCameraComponent>();
  check(ActivePlayerCamera);
}

void URangedAttackComponent::EnterAim()
{
  bIsAiming = true;
  OwningMovementComponent->bOrientRotationToMovement = false;
}

void URangedAttackComponent::ExitAim()
{
  bIsAiming = false;
  OwningMovementComponent->bOrientRotationToMovement = true;
}

void URangedAttackComponent::FireWeapon()
{
  EquipmentManagerComponent->SetActiveEquipment(EWeapon::RIFLE);
}

void URangedAttackComponent::TickAimComponent(const float DeltaTime)
{
  FVector TargetAimLocation = ActivePlayerCamera->GetComponentLocation() + ActivePlayerCamera->GetForwardVector() * 1750.0f;
  FVector AimDirection = TargetAimLocation - GetOwner()->GetActorLocation();

  //float PitchSignedAngle = UMuseGameplayStatics::GetSignedAngle(CharacterForward, AimDirection, -GetOwner()->GetActorRightVector());
  //float YawSignedAngle = UMuseGameplayStatics::GetSignedAngle(CharacterForward, AimDirection, GetOwner()->GetActorUpVector());
  //GetOwner()->GetComponentByClass<URotationComponent>()->SmoothRotateToVector(AimDirection, 0.1f);

  FRotator AimDirectionRotation = AimDirection.Rotation();
  AimDirectionRotation.Pitch = 0.0f;
  GetOwner()->SetActorRotation(AimDirectionRotation);
}

