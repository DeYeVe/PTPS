// Copyright Epic Games, Inc. All Rights Reserved.

#include "PTPSGameMode.h"
#include "PTPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

APTPSGameMode::APTPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
