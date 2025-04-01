// Fill out your copyright notice in the Description page of Project Settings.
#include "Statics/MuseGameplayStatics.h"

float UMuseGameplayStatics::GetSignedAngle(const FVector& InVectorA, const FVector& InVectorB, const FVector& InReferenceNormal)
{
  const FVector VectorANormalized = InVectorA.IsNormalized() ? InVectorA : InVectorA.GetSafeNormal();
  const FVector VectorBNormalized = InVectorB.IsNormalized() ? InVectorB : InVectorB.GetSafeNormal();
  float UnsignedAngleBetweenTwoVectors = acos(FVector::DotProduct(VectorANormalized, VectorBNormalized));
  FVector CrossProduct = FVector::CrossProduct(VectorANormalized, VectorBNormalized);
  check(InReferenceNormal.IsNormalized());
  float Dot = FVector::DotProduct(CrossProduct, InReferenceNormal);
  return FMath::Sign(Dot) * UnsignedAngleBetweenTwoVectors;
}
