// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeleeHitReactsDataAsset.generated.h"

UENUM(BlueprintType)
enum class EMeleeHitType : uint8
{
  FORWARD_HIT = 0 UMETA(DisplayName = "Forward hit"),
  RIGHT_HIT = 1 UMETA(DisplayName = "Right hit"),
  LEFT_HIT = 2 UMETA(DisplayName = "Left hit")
};

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
