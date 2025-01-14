// Fill out your copyright notice in the Description page of Project Settings.
#include "MeleeAttackComponent.h"
#include "AnimNotifyState_MeleeAttackPhase.h"
#include "MeleeAnimationPhase.h"

DEFINE_LOG_CATEGORY(LogMeleeAttackComponent)

// Sets default values for this component's properties
UMeleeAttackComponent::UMeleeAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
  AnimInstance = GetOwner()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
}


// Called every frame
void UMeleeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UMeleeAttackComponent::TryTriggerAttack()
{
  bool bAttackTriggered = false;
  return bAttackTriggered;
}

void UMeleeAttackComponent::ConfigureContainer(FMeleeAttackContainer& InContainer, const uint32 MeleeAttackIndex)
{
  InContainer.AnimMontage = MeleeComboData->GetMeleeAttack(MeleeAttackIndex)->GetMontage();
  if (!InContainer.AnimMontage)
  {
    UE_LOG(LogMeleeAttackComponent, Error, TEXT("Melee montage in melee combo is not configured %s."), *MeleeComboData.GetName());
    return;
  }

  // Reset melee attack phases for new anim montage.
  InContainer.MeleeAttackAnimPhases.SetNum(3);
  InContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::SETUP] = nullptr;
  InContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::ATTACK] = nullptr;
  InContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::RECOVERY] = nullptr;

  for (FAnimNotifyEvent AnimNotifyEvent : InContainer.AnimMontage->Notifies)
  {
    UAnimNotifyState_MeleeAttackPhase* MeleeAttackPhaseNotify = Cast<UAnimNotifyState_MeleeAttackPhase>(AnimNotifyEvent.NotifyStateClass);
    if (MeleeAttackPhaseNotify)
    {
      uint8 MeleeAttackPhaseNotifyIndex = (uint8)MeleeAttackPhaseNotify->GetMeleeAnimPhase();
      InContainer.MeleeAttackAnimPhases[MeleeAttackPhaseNotifyIndex] = MeleeAttackPhaseNotify;
      // Ensure order is maintained, setup must appear before attack etc.
      check(MeleeAttackPhaseNotifyIndex == 0 || InContainer.MeleeAttackAnimPhases[MeleeAttackPhaseNotifyIndex - 1] != nullptr);
    }
  }
}

void UMeleeAttackComponent::MeleeMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
  if (Montage == CurrMeleeContainer.AnimMontage)
  {
    CurrMeleeContainer.Clear();
  }
}

EMeleeAnimationPhase UMeleeAttackComponent::GetActiveMeleeAnimationPhase()
{
  if (!MeleeAnimationPlaying())
  {
    return EMeleeAnimationPhase::NONE;
  }

  for (UAnimNotifyState_MeleeAttackPhase* MeleeAttackAnimNotify : CurrMeleeContainer.MeleeAttackAnimPhases)
  {
    if (MeleeAttackAnimNotify->IsActivePhase())
    {
      return MeleeAttackAnimNotify->GetMeleeAnimPhase();
    }
  }
  UE_LOG(LogMeleeAttackComponent, Error, TEXT("Unable to retun an active melee animation phase, ensure montage has been configured correctly."));
  return EMeleeAnimationPhase::NONE;
}


