// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeleeHitType.h"
#include "MeleeHitReactsDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MUSEMELEEATTACKS_API UMeleeHitReactsDataAsset : public UDataAsset
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
  UAnimMontage* RightHitReactMontage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
  UAnimMontage* LeftHitReactMontage;

  UPROPERTY()
  TMap<EMeleeHitType, UAnimMontage*> HitMontageMap;

  void BuildMontageMap();

public:

  UFUNCTION(BlueprintCallable)
  UAnimMontage* GetAnimMontage(EMeleeHitType HitType);
};
