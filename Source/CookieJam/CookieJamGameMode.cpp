// Copyright Epic Games, Inc. All Rights Reserved.

#include "CookieJamGameMode.h"
#include "CookieJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACookieJamGameMode::ACookieJamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
