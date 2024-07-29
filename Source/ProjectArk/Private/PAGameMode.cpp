// Fill out your copyright notice in the Description page of Project Settings.


#include "PAGameMode.h"
#include "PAPlayerController.h"
#include "PACharacter.h"
#include "UObject/ConstructorHelpers.h"

APAGameMode::APAGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APAPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ProjectArkContents/Characters/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/ProjectArkContents/Characters/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}