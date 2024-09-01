// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "MusePathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class MUSEAI_API UMusePathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

protected:
  virtual void FollowPathSegment(float DeltaTime) override;
};
