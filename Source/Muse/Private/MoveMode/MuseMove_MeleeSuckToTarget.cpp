// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveMode/MuseMove_MeleeSuckToTarget.h"
#include "MuseCharacterMovementComponent.h"
#include "Engine/OverlapResult.h"

void UMuseMove_MeleeSuckToTarget::TickMoveMode(const float DeltaTime)
{
  if (!TargetActorFound())
  {
    return;
  }
 
  UpdateTargetSuckPosition();

  const float SuckToMeleePositionSpeed = 300.0f;
  const FVector SuckMovementDirection = (TargetSuckPosition - MovementComp->GetOwner()->GetActorLocation()).GetSafeNormal();
  const FVector TargetVelocity = SuckMovementDirection * SuckToMeleePositionSpeed;

  FVector Delta = TargetVelocity * DeltaTime;
  FQuat DeltaRotation = FRotationMatrix::MakeFromXZ(TargetVelocity.GetSafeNormal(), FVector::UpVector).ToQuat();

  MovementComp->MoveDelta(DeltaTime, Delta, DeltaRotation);

  return;
}

void UMuseMove_MeleeSuckToTarget::EnterMoveMode()
{
  TrySetMeleeTargetActor();
}

void UMuseMove_MeleeSuckToTarget::ExitMoveMode()
{
}

bool UMuseMove_MeleeSuckToTarget::TrySetMeleeTargetActor()
{
  TargetActor = nullptr;
  TArray<APawn*> NearbyPawns;

  FCollisionShape Sphere = FCollisionShape::MakeSphere(1000.0f);
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(MovementComp->GetOwner()); // Ignore self

  TArray<FOverlapResult> OverlapResults;
  bool bHit = MovementComp->GetWorld()->OverlapMultiByObjectType(
    OverlapResults,
    MovementComp->GetOwner()->GetActorLocation(),
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
      float Distance = FVector::DistSquared(MovementComp->GetOwner()->GetActorLocation(), Pawn->GetActorLocation());
      if (Distance < ClosestDistance)
      {
        ClosestDistance = Distance;
        TargetActor = Pawn;
      }
    }
  }

  return TargetActor != nullptr;
}

void UMuseMove_MeleeSuckToTarget::UpdateTargetSuckPosition()
{
  check(TargetActorFound());

  FVector Direction = MovementComp->GetOwner()->GetActorLocation() - TargetActor->GetActorLocation();
  Direction = Direction.GetSafeNormal();

  TargetSuckPosition = TargetActor->GetActorLocation() + Direction * 75.0f;

  DrawDebugSphere(MovementComp->GetWorld(), MovementComp->GetOwner()->GetActorLocation(), 50.0f, 10, FColor::Red, false, -1.0f);
}
