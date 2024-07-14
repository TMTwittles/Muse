// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerGameplayAbilitiesDataAsset.h"

UPlayerGameplayAbilitiesDataAsset::UPlayerGameplayAbilitiesDataAsset(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{
}

const TSet<FGameplayAbilityInputInfo>& UPlayerGameplayAbilitiesDataAsset::GetInputAbilities() const
{
  return InputAbilities;
}

#if WITH_EDITOR
void UPlayerGameplayAbilitiesDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
  Super::PostEditChangeProperty(PropertyChangedEvent);

  const FProperty* Property = PropertyChangedEvent.Property;
  if (!Property)
  {
    return;
  }

  // Only modify input abilities if it has been modified and values exist.
  if (Property->GetFName() != GET_MEMBER_NAME_CHECKED(UPlayerGameplayAbilitiesDataAsset, InputAbilities)
    || InputAbilities.IsEmpty())
  {
    return;
  }

  // Reset input ability IDs.
  for (int32 i = 0; i < InputAbilities.Num(); ++i)
  {
    InputAbilities.Array()[i].InputId = i;
  }
}
#endif
