// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeHitType.h"
#include "MeleeHitReactsDataAsset.h"
#include "MeleeHitRegisterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUSEMELEEATTACKS_API UMeleeHitRegisterComponent : public UActorComponent
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere, Category = "Data")
  TObjectPtr<UMeleeHitReactsDataAsset>  MeleeHitReactDataAsset;

  UPROPERTY()
  TWeakObjectPtr<UAnimInstance> AnimInstance;

public:	
	// Sets default values for this component's properties
	UMeleeHitRegisterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  void RegisterMeleeHit(const EMeleeHitType& InMeleeHitType);

private:
  void PlayMeleeHitReactAnimation();
		
};
