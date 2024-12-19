// Fill out your copyright notice in the Description page of Project Settings.
#include "LockOnComponent.h"
#include "Engine/OverlapResult.h"
#include "StrafeAnimationHandlerComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogLockOnComponent)

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();
  PlayerController = static_cast<APlayerController*>(Cast<ACharacter>(GetOwner())->GetController());
  OwnerStrafeAnimationHandler = GetOwner()->GetComponentByClass<UStrafeAnimationHandlerComponent>();
}

// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (!bShouldLockOn)
  {
    return;
  }

  if (!bHardLockOnActive && bTickLockOnDuration)
  {
    CurrentLockOnDuration -= DeltaTime;
    if (CurrentLockOnDuration <= 0.0f)
    {
      ExitLockOn();
      return;
    }
  }

  bLockOnActive = TryUpdateLockOnTarget();
  if (!bLockOnActive)
  {
    ExitHardLockOn();
    return;
  }

  if (OwnerStrafeAnimationHandler)
  {
    OwnerStrafeAnimationHandler->UpdateActiveStrafeDirection();
  }
  GetOwner()->SetActorRotation(GetRotationToLockOnTarget(GetOwner()->GetActorLocation(), false));

  if (bHardLockOnActive)
  {
    TickCameraLockOnLocation(DeltaTime);
  }
}

void ULockOnComponent::TickCameraLockOnLocation(float DeltaTime)
{
  if (CameraSpringArm == nullptr)
  {
    UE_LOG(LogLockOnComponent, Warning, TEXT("Camera objects are not set."));
    return;
  }

  float XLockOnOffsetAmount = 300.0f;
  float YLockOnOffsetAmount = 0.0f;
  FVector LockOnPositionOffsetX = GetOwner()->GetActorRightVector() * XLockOnOffsetAmount;
  FVector LockOnPositionOffsetY = GetOwner()->GetActorForwardVector() * YLockOnOffsetAmount;
  FVector LockOnCameraPosition = GetOwner()->GetActorLocation();
  LockOnCameraPosition += LockOnPositionOffsetX;
  LockOnCameraPosition -= LockOnPositionOffsetY;
  LockOnCameraPosition.Z = GetOwner()->GetActorLocation().Z + 100.0f;
  CameraSpringArm->SetWorldLocation(LockOnCameraPosition);
  //DrawDebugLine(GetWorld(), InitialCameraSpringArmLocation, InitialCameraSpringArmLocation + LockOnPositionOffsetY, FColor::Red);
  //DrawDebugLine(GetWorld(), InitialCameraSpringArmLocation, InitialCameraSpringArmLocation + LockOnPositionOffsetX, FColor::Red);
  //DrawDebugSphere(GetWorld(), LockOnCameraPosition, 10.0f, 10, FColor::Green);

  FVector CameraSpringArmLocation = CameraSpringArm->GetComponentLocation();
  FRotator CameraLockOnRotation = GetRotationToLockOnTarget(CameraSpringArmLocation, true, 0.65f).Rotator();
  CameraSpringArm->SetRelativeRotation(CameraLockOnRotation);
  PlayerController->SetControlRotation(CameraLockOnRotation);
  CameraSpringArm->TargetArmLength = 300.0f;
}

const FQuat ULockOnComponent::GetRotationToLockOnTarget(FVector RelativeLocation, bool bIncludeYaw, float MidPointPercentage) const
{
  check(LockOnTargetActor != nullptr);
  FVector Target = GetLockOnTargetPosition();
  FVector LockOnDirection = (Target - GetOwner()->GetActorLocation()).GetSafeNormal();
  Target = GetOwner()->GetActorLocation() + LockOnDirection * (FVector::Dist(Target, GetOwner()->GetActorLocation()) * MidPointPercentage);
  LockOnDirection = (Target - RelativeLocation).GetSafeNormal();
  //DrawDebugSphere(GetWorld(), Target, 10.0f, 10, FColor::Red);
  LockOnDirection.Z = !bIncludeYaw ? 0.0f : LockOnDirection.Z;
  return FRotationMatrix::MakeFromX(LockOnDirection).ToQuat();
}

void ULockOnComponent::EnterLockOn()
{
    bLockOnActive = TryUpdateLockOnTarget();
    bShouldLockOn = true;
    bTickLockOnDuration = false;
    CurrentLockOnDuration = 0.0f;
    if (bLockOnActive)
    {
      LockedOn.Broadcast();
    }
}

void ULockOnComponent::EnterLockOnForDuration(float LockOnDuration)
{
  bLockOnActive = TryUpdateLockOnTarget();
  bShouldLockOn = true;
  bTickLockOnDuration = true;
  CurrentLockOnDuration = LockOnDuration;
  if (bLockOnActive)
  {
    LockedOn.Broadcast();
  }
}

void ULockOnComponent::EnterHardLockOn()
{
  if (bHardLockOnActive)
  {
    return;
  }

  if (CameraSpringArm == nullptr)
  {
    UE_LOG(LogLockOnComponent, Warning, TEXT("Camera objects are not set."));
    return;
  }
  if (!TryUpdateLockOnTarget())
  {
    // TODO: Instead of just early exiting when we fail to lock on, maybe we just zoom the camera
    // in by a bit?
    return;
  }
  bHardLockOnActive = true;
  InitialCameraSpringArmLength = CameraSpringArm->TargetArmLength;
  InitialCameraSpringArmRelativeLocation = CameraSpringArm->GetRelativeLocation();
  InitialCameraSpringArmRelativeRotation = CameraSpringArm->GetRelativeRotation();
  InitialControlRotation = PlayerController->GetControlRotation();
  CameraSpringArm->bUsePawnControlRotation = false;
  EnterLockOn();
}

void ULockOnComponent::ExitHardLockOn()
{
  if (CameraSpringArm == nullptr)
  {
    UE_LOG(LogLockOnComponent, Warning, TEXT("Camera objects are not set."));
    return;
  }

  if (bHardLockOnActive)
  {
    bHardLockOnActive = false;
    CameraSpringArm->TargetArmLength = InitialCameraSpringArmLength;
    CameraSpringArm->SetRelativeLocation(InitialCameraSpringArmRelativeLocation);
    CameraSpringArm->SetRelativeRotation(InitialCameraSpringArmRelativeRotation);
    CameraSpringArm->bUsePawnControlRotation = true;
    PlayerController->SetControlRotation(InitialControlRotation);
  }
  ExitLockOn();
}

void ULockOnComponent::ExitLockOn()
{
  if (bHardLockOnActive)
  {
    return;
  }

  bLockOnActive = false;
  bShouldLockOn = false;
  bTickLockOnDuration = false;
  CurrentLockOnDuration = 0.0f;
  LockedOnCleared.Broadcast();
}

void ULockOnComponent::SetCameraSpringArm(USpringArmComponent* InCameraSpringArm)
{
  CameraSpringArm = InCameraSpringArm;
}

bool ULockOnComponent::TryUpdateLockOnTarget()
{
  LockOnTargetActor = nullptr;
  TArray<APawn*> NearbyPawns;

  FCollisionShape Sphere = FCollisionShape::MakeSphere(100000000.0f);
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(GetOwner()); // Ignore self

  TArray<FOverlapResult> OverlapResults;
  bool bHit = GetWorld()->OverlapMultiByObjectType(
    OverlapResults,
    GetOwner()->GetActorLocation(),
    FQuat::Identity,
    FCollisionObjectQueryParams(ECC_Pawn),
    Sphere,
    CollisionParams
  );

  float ClosestDistance = MAX_flt;
  for (const FOverlapResult& Result : OverlapResults)
  {
    if (APawn* Pawn = Cast<APawn>(Result.GetActor()))
    {
      float Distance = FVector::DistSquared(GetOwner()->GetActorLocation(), Pawn->GetActorLocation());
      if (Distance < ClosestDistance)
      {
        ClosestDistance = Distance;
        LockOnTargetActor = Pawn;
      }
    }
  }

  if (LockOnTargetActor)
  {
    DrawDebugSphere(GetWorld(), LockOnTargetActor->GetActorLocation(), 25.0f, 10, FColor::Red);
  }

  return LockOnTargetActor != nullptr;
}

