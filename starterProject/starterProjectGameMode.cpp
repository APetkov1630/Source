// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "starterProject.h"
#include "starterProjectGameMode.h"
#include "starterProjectCharacter.h"

AstarterProjectGameMode::AstarterProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
