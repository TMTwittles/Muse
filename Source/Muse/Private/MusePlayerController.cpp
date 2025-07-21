// Fill out your copyright notice in the Description page of Project Settings.
#include "MusePlayerController.h"

void AMusePlayerController::SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
  Super::SetViewTargetWithBlend(NewViewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);

  // TODO: This is not the proper way to do this, should override the player camera manager and appropriately broadcast new view target from there,
  // with extra respect to the client, blend times, etc.
  BroadcastOnViewTargetChanged(NewViewTarget);
}

void AMusePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
  ClearOnViewTargetChanged();
}

void AMusePlayerController::OnUnPossess()
{
  Super::OnUnPossess();
  ClearOnViewTargetChanged();
}
