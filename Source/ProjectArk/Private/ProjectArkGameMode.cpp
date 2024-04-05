// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectArkGameMode.h"
#include "ProjectArkPlayerController.h"
#include "ProjectArkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectArkGameMode::AProjectArkGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectArkPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}