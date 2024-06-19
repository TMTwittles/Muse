// Copyright Epic Games, Inc. All Rights Reserved.

#include "MuseGameMode.h"
#include "MuseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMuseGameMode::AMuseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
