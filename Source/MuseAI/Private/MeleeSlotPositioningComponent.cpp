// Fill out your copyright notice in the Description page of Project Settings.
#include "MeleeSlotPositioningComponent.h"
#include "DrawDebugHelpers.h"

UMeleeSlotPositioningComponent::UMeleeSlotPositioningComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UMeleeSlotPositioningComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  if (!TargetSlotActor)
  {
    return;
  }

  FRotator SlotRotator;
  FVector ActorLocation = TargetSlotActor->GetActorLocation();
  FVector SlotRotationEuler = FVector::Zero();
  float RotationStepAmount = 360.0f / NumSlots;

  for (uint32 SlotIndex = 0; SlotIndex < NumSlots; SlotIndex++)
  {
    SlotRotator = SlotRotator.MakeFromEuler(SlotRotationEuler);
    FVector Direction = SlotRotator.RotateVector(FVector::ForwardVector);
    FVector HorizontalTraceStart = ActorLocation;
    FVector HorizontalTraceEnd = ActorLocation + Direction * HorizontalDistanceFromPlayer;
    FHitResult OutHitResult;

    bool bHorizontalHit = GetWorld()->LineTraceSingleByChannel(
      OutHitResult,
      HorizontalTraceStart,
      HorizontalTraceEnd,
      ECollisionChannel::ECC_GameTraceChannel1);

    if (bHorizontalHit)
    {
      HorizontalTraceEnd = ActorLocation + Direction * MinHorizontalDistanceFromPlayer;
      bool bMinHorizontalHit = GetWorld()->LineTraceSingleByChannel(
        OutHitResult,
        HorizontalTraceStart,
        HorizontalTraceEnd,
        ECollisionChannel::ECC_GameTraceChannel1);
      if (bMinHorizontalHit)
      {
        Slots[SlotIndex].bValidSlot = false;
        SlotRotationEuler.Z += RotationStepAmount;
        continue;
      }
    }

    FVector VerticalTraceStart = HorizontalTraceEnd;
    FVector VerticalTraceEnd = HorizontalTraceEnd + -FVector::UpVector * VerticalDistanceFromPlayer;
    bool bVerticalHit = GetWorld()->LineTraceSingleByChannel(
      OutHitResult,
      VerticalTraceStart,
      VerticalTraceEnd,
      ECollisionChannel::ECC_GameTraceChannel1);

    if (!bVerticalHit)
    {
      Slots[SlotIndex].bValidSlot = false;
      SlotRotationEuler.Z += RotationStepAmount;
      continue;
    }

    Slots[SlotIndex].Position = OutHitResult.ImpactPoint;
    Slots[SlotIndex].bValidSlot = true;
    SlotRotationEuler.Z += RotationStepAmount;
  }

  for (uint32 SlotIndex = 0; SlotIndex < NumSlots; SlotIndex++)
  {
    if (Slots[SlotIndex].bValidSlot)
    {
      DrawDebugSphere(GetWorld(), Slots[SlotIndex].Position, 25.0f, 10, FColor::Red);
    }
  }
}

const FVector UMeleeSlotPositioningComponent::GetMeleePosition(FName InName)
{
  const FMeleePositionSlot Slot;
  FScopeLock Lock(&TokenAccessMutex);
  if (!NameToSlotIndexMap.Contains(InName))
  {
    NameToSlotIndexMap.Add(InName);
    NameToSlotIndexMap[InName] = FindMoveableSlotIndex();
  }
  else
  {
    if (!Slots[NameToSlotIndexMap[InName]].bValidSlot)
    {
      Slots[NameToSlotIndexMap[InName]].bSlotTaken = false;
      NameToSlotIndexMap[InName] = FindMoveableSlotIndex();
    }
  }
  Slots[NameToSlotIndexMap[InName]].bSlotTaken = true;
  return Slots[NameToSlotIndexMap[InName]].Position;
}

inline const int32 UMeleeSlotPositioningComponent::FindMoveableSlotIndex() const
{
  uint32 ValidSlotIndex = 0;
  for (uint32 SlotIndex = 0; SlotIndex < NumSlots; SlotIndex++)
  {
    if (!Slots[SlotIndex].bSlotTaken && Slots[SlotIndex].bValidSlot)
    {
      ValidSlotIndex = SlotIndex;
      break;
    }
  }
  return ValidSlotIndex;
}



void UMeleeSlotPositioningComponent::ConfigureForActor(AActor* InActor)
{
  TargetSlotActor = InActor;
  PrimaryComponentTick.bCanEverTick = true;
  for (uint32 i = 0; i < NumSlots; i++)
  {
    Slots.Add(FMeleePositionSlot());
  }
}


