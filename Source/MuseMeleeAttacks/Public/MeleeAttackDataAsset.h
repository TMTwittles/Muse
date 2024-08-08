// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimSequence.h"
#include "MeleeAttackDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MUSEMELEEATTACKS_API UMeleeAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()

private:
  UPROPERTY(EditAnywhere, Category="Animation", meta=(AllowPrivateAccess="true"))
  TObjectPtr<UAnimMontage> MeleeMontage;

  UPROPERTY(EditAnywhere, Category="Animation", meta=(AllowPrivateAccess="true"))
  float BlendInTime;

  UPROPERTY(EditAnywhere, Category = "Animation", meta=(AllowPrivateAccess="true"))
  float BlendOutTime;

public:
  UFUNCTION()
  inline UAnimMontage* GetMontage() const { return MeleeMontage; };
};
