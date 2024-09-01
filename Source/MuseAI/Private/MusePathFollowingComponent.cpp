// Fill out your copyright notice in the Description page of Project Settings.


#include "MusePathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void UMusePathFollowingComponent::FollowPathSegment(float DeltaTime)
{
  DrawDebugSphere(GetWorld(),GetCurrentTargetLocation(),25.0f,10.0f,FColor::Red,false,-1);
  UPathFollowingComponent::FollowPathSegment(DeltaTime);
}
