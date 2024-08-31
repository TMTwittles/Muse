// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeSlotPositioningComponent.h"

UMeleeSlotPositioningComponent::UMeleeSlotPositioningComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UMeleeSlotPositioningComponent::SetTargetSlotActor(AActor* InActor)
{
  TargetSlotActor = InActor;
}


