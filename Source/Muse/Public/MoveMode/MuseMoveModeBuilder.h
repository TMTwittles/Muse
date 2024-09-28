// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MuseMoveModes.h" // TODO: Forward declare this. 
#include "MuseMoveModeBuilder.generated.h"

class UMuseMoveMode;
class UMuseCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class MUSE_API UMuseMoveModeBuilder : public UObject
{
	GENERATED_BODY()
public:
  static UMuseMoveMode* BuildMoveMode(UMuseCharacterMovementComponent* InMovementComp, const EMuseMoveMode& InMoveMode);
};
