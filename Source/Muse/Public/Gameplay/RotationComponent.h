// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotationComponent.generated.h"

DECLARE_DELEGATE(FRotationComplete);
DECLARE_DELEGATE(FRotationCanceled);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSE_API URotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
  FRotationComplete RotationComplete;
  FRotationCanceled RotationCanceled;
  
	// Sets default values for this component's properties
	URotationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  void SmoothRotateToVector(const FVector& InVector, const float InTimeRotateToVector);
  void CancelSmoothRotation();

private:
  void PerformSmoothRotation();

private:
  FQuat InitialRotation;
  FQuat TargetRotation;
  bool bPerformSmoothRotation;
  float CurrRotationTime;
  float TargetRotationTime;
};
