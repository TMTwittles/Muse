// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MUSE_API UMuseGameplayStatics : public UObject
{
public:
  static float GetSignedAngle(const FVector& InVectorA, const FVector& InVectorB);
  static float GetSignedAngle(const FQuat& QuatA, const FQuat& QuatB);
};
