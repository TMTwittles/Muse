// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeHitRegisterComponent.h"

// Sets default values for this component's properties
UMeleeHitRegisterComponent::UMeleeHitRegisterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeHitRegisterComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UMeleeHitRegisterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMeleeHitRegisterComponent::RegisterMeleeHit(const EMeleeHitType& InMeleeHitType)
{
  check(MeleeHitReactDataAsset);
  USkeletalMeshComponent* MeshComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
  check(MeshComp);
  UAnimMontage* HitReactMontage = MeleeHitReactDataAsset->GetAnimMontage(InMeleeHitType);
  MeshComp->GetAnimInstance()->Montage_Play(HitReactMontage);
}

