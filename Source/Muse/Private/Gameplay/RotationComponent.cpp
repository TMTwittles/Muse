// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/RotationComponent.h"

// Sets default values for this component's properties
URotationComponent::URotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URotationComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void URotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (CurrRotationTime <= TargetRotationTime)
  {
    PerformSmoothRotation();
    CurrRotationTime += DeltaTime;
  }
}

void URotationComponent::SmoothRotateToVector(const FVector& InVector, const float InTimeRotateToVector)
{
  InitialRotation = GetOwner()->GetActorQuat();
  TargetRotation = InVector.Rotation().Quaternion();
  TargetRotationTime = InTimeRotateToVector;
  CurrRotationTime = 0.0f;
  bPerformSmoothRotation = true;
}

void URotationComponent::PerformSmoothRotation()
{
  const double SlerpAlpha = (CurrRotationTime / TargetRotationTime);
  const FQuat SmoothedRotation = FQuat::Slerp(InitialRotation, TargetRotation, SlerpAlpha);
  GetOwner()->SetActorRotation(SmoothedRotation);
}

