// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeleeAttackDataAsset.h"
#include "MeleeComboDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MUSEMELEEATTACKS_API UMeleeComboDataAsset : public UDataAsset
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
  TArray<TObjectPtr<UMeleeAttackDataAsset>> MeleeAttacks;

public:
  inline const bool EndOfCombo(const int32 ComboIndex) const { return ComboIndex >= MeleeAttacks.Num() - 1; }
  inline const float GetNumComboAttack() const { MeleeAttacks.Num(); }
  inline UMeleeAttackDataAsset* GetMeleeAttack(const int32 ComboIndex) const { return MeleeAttacks[ComboIndex]; };
};
