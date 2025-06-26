// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MusePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FViewTargetChanged, AActor*, NewViewTarget);

/**
 * 
 */
UCLASS()
class MUSE_API AMusePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
  UPROPERTY()
  FViewTargetChanged OnViewTargetChanged;

  virtual void SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing) override;

};
