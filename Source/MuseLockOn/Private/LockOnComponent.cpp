// Fill out your copyright notice in the Description page of Project Settings.
#include "LockOnComponent.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

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

	// ...
	
}


// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

const FQuat ULockOnComponent::GetRotationToLockOnTarget() const
{
  check(LockOnTargetActor != nullptr);
  FVector Target = GetLockOnTargetPosition();
  FVector Start = GetOwner()->GetActorLocation();
  FVector LockOnDirection = (Target - Start).GetSafeNormal();
  LockOnDirection.Z = 0.0f;
  return FRotationMatrix::MakeFromX(LockOnDirection).ToQuat();;
}

bool ULockOnComponent::TryUpdateLockOnTarget()
{
  LockOnTargetActor = nullptr;
  TArray<APawn*> NearbyPawns;

  FCollisionShape Sphere = FCollisionShape::MakeSphere(1000.0f);
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

