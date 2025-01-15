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

  if (!MeleeAnimationPlaying())
  {
    ConfigureContainer();
    PlayActiveAnimation();
    return true;
  }
  check(CurrMeleeContainer.IsValid());
  EMeleeAnimationPhase ActiveAnimationPhase = GetActiveMeleeAnimationPhase();
  if (ActiveAnimationPhase == EMeleeAnimationPhase::RECOVERY)
  {
    ConfigureContainer();
    PlayActiveAnimation();
    return true;
  }
  else if (ActiveAnimationPhase == EMeleeAnimationPhase::ATTACK)
  {
    if (!CurrMeleeContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::RECOVERY]->MeleeAttackPhaseStarted.IsAlreadyBound(this, &UMeleeAttackComponent::MeleeAnimationPhaseStarted))
    {
      CurrMeleeContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::RECOVERY]->MeleeAttackPhaseStarted.AddDynamic(this, &UMeleeAttackComponent::MeleeAnimationPhaseStarted);
    }
  }
  return bAttackTriggered;
}

void UMeleeAttackComponent::ConfigureContainer()
{
  int32 NewComboID = 0;
  if (CurrMeleeContainer.AnimMontage != nullptr)
  {
    NewComboID = CurrMeleeContainer.ComboID == MeleeComboData->GetNumComboAttacks() - 1 ? 0 : NewComboID++;
  }
  CurrMeleeContainer.Clear();
  CurrMeleeContainer.ComboID = NewComboID;
  CurrMeleeContainer.AnimMontage = MeleeComboData->GetMeleeAttack(NewComboID)->GetMontage();
  if (!CurrMeleeContainer.AnimMontage)
  {
    UE_LOG(LogMeleeAttackComponent, Error, TEXT("Melee montage in melee combo is not configured %s."), *MeleeComboData.GetName());
    return;
  }

  // Reset melee attack phases for new anim montage.
  CurrMeleeContainer.MeleeAttackAnimPhases.SetNum(3);
  CurrMeleeContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::SETUP] = nullptr;
  CurrMeleeContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::ATTACK] = nullptr;
  CurrMeleeContainer.MeleeAttackAnimPhases[(uint8)EMeleeAnimationPhase::RECOVERY] = nullptr;

  for (FAnimNotifyEvent AnimNotifyEvent : CurrMeleeContainer.AnimMontage->Notifies)
  {
    UAnimNotifyState_MeleeAttackPhase* MeleeAttackPhaseNotify = Cast<UAnimNotifyState_MeleeAttackPhase>(AnimNotifyEvent.NotifyStateClass);
    if (MeleeAttackPhaseNotify)
    {
      uint8 MeleeAttackPhaseNotifyIndex = (uint8)MeleeAttackPhaseNotify->GetMeleeAnimPhase();
      CurrMeleeContainer.MeleeAttackAnimPhases[MeleeAttackPhaseNotifyIndex] = MeleeAttackPhaseNotify;
      // Ensure order is maintained, setup must appear before attack etc.
      check(MeleeAttackPhaseNotifyIndex == 0 || CurrMeleeContainer.MeleeAttackAnimPhases[MeleeAttackPhaseNotifyIndex - 1] != nullptr);
    }
  }
  bool bMeleeAttackAnimPhasesConfiguredCorrectly = true;
  for (int32 NotifyIndex = 0; NotifyIndex < CurrMeleeContainer.MeleeAttackAnimPhases.Num(); NotifyIndex++)
  {
    if (CurrMeleeContainer.MeleeAttackAnimPhases[NotifyIndex] == nullptr)
    {
      bMeleeAttackAnimPhasesConfiguredCorrectly = false;
      UE_LOG(LogMeleeAttackComponent, Warning, TEXT("Notify %d for montage: %s has not been configured."), NotifyIndex, *CurrMeleeContainer.AnimMontage->GetName());
    }
  }
  if (!bMeleeAttackAnimPhasesConfiguredCorrectly)
  {
    CurrMeleeContainer.MeleeAttackAnimPhases.Empty();
  }
}

void UMeleeAttackComponent::MeleeMontageFinished(UAnimMontage* Montage, bool bInterrupted)
{
  if (Montage == CurrMeleeContainer.AnimMontage)
  {
    CurrMeleeContainer.Clear();
    AnimInstance->OnMontageEnded.RemoveDynamic(this, &UMeleeAttackComponent::MeleeMontageFinished);
  }
}

void UMeleeAttackComponent::MeleeAnimationPhaseStarted(EMeleeAnimationPhase InPhase)
{
  // TODO: This code is super dodge will most definately lead to bugs.
  if (CurrMeleeContainer.MeleeAttackAnimPhases[(uint8)InPhase]->MeleeAttackPhaseStarted.IsAlreadyBound(this, &UMeleeAttackComponent::MeleeAnimationPhaseStarted))
  {
    CurrMeleeContainer.MeleeAttackAnimPhases[(uint8)InPhase]->MeleeAttackPhaseStarted.RemoveDynamic(this, &UMeleeAttackComponent::MeleeAnimationPhaseStarted);
  }
  ConfigureContainer();
  PlayActiveAnimation();
}

void UMeleeAttackComponent::PlayActiveAnimation()
{
  check(CurrMeleeContainer.AnimMontage);
  AnimInstance->Montage_Play(CurrMeleeContainer.AnimMontage);
  if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &UMeleeAttackComponent::MeleeMontageFinished))
  {
    AnimInstance->OnMontageEnded.AddDynamic(this, &UMeleeAttackComponent::MeleeMontageFinished);
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


