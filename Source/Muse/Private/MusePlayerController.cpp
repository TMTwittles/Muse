// Fill out your copyright notice in the Description page of Project Settings.
#include "MusePlayerController.h"

void AMusePlayerController::SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
  Super::SetViewTargetWithBlend(NewViewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
  OnViewTargetChanged.Broadcast();
}
