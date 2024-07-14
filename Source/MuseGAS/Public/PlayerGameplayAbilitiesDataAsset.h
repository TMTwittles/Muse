// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilityInputInfo.h"
#include "PlayerGameplayAbilitiesDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MUSEGAS_API UPlayerGameplayAbilitiesDataAsset : public UDataAsset
{
	GENERATED_UCLASS_BODY()

protected:
  UPROPERTY(EditAnywhere, Category="AbilitySystem")
  TSet<FGameplayAbilityInputInfo> InputAbilities;

public:
  const TSet<FGameplayAbilityInputInfo>& GetInputAbilities() const;

#if WITH_EDITOR
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
