// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "MeleeHitReactsDataAsset.h"
#include "MuseMeleeAttacks.h"

void UMeleeHitReactsDataAsset::BuildMontageMap()
{
  HitMontageMap.Add(EMeleeHitType::LEFT_HIT, LeftHitReactMontage);
  HitMontageMap.Add(EMeleeHitType::RIGHT_HIT, RightHitReactMontage);

  // Log missing warning
  for (const TPair<EMeleeHitType, UAnimMontage*>& HitMontagePair : HitMontageMap)
  {
    if (HitMontagePair.Value == nullptr)
    {
      UE_LOG(LogMuseMeleeAttack,
        Warning,
        TEXT("%s contains no hit react montage for entry %s"),
        *this->GetName(),
        *UEnum::GetValueAsString(HitMontagePair.Key))
    }
  }
}

UAnimMontage* UMeleeHitReactsDataAsset::GetAnimMontage(EMeleeHitType HitType)
{
  if (HitMontageMap.Num() == 0)
  {
    BuildMontageMap();
  }
  
  return HitMontageMap[HitType];
}
