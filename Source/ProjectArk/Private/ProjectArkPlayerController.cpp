// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectArkPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectArkCharacter.h"
#include "Engine/World.h"

#include "PAItem.h"

AProjectArkPlayerController::AProjectArkPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	Destination = FVector::ZeroVector;
	MoveCompletedVector = FVector(0.f, 0.f, -1.f);
	bMoving = false;
	bInputPressed = false;
}

void AProjectArkPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bInputPressed)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		Destination = Hit.Location;
	}

	if (bMoving)
	{
		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			FVector WorldDirection = (Destination - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);

			if (FVector::Distance(WorldDirection, MoveCompletedVector)<0.01f)
			{
				Destination = FVector::ZeroVector;
				bMoving = false;
			}
		}
	}

}

void AProjectArkPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AProjectArkPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AProjectArkPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("SpawnItem", IE_Pressed, this, &AProjectArkPlayerController::OnSpawnItem);
}

void AProjectArkPlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	bMoving = true;

	// Log for the position
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	Destination = Hit.Location;
	//PALOG(Warning, TEXT("Cursor pointed: %s"), *Destination.ToString());

	//spawn cursor particles
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, Destination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
	
}

void AProjectArkPlayerController::OnSetDestinationReleased()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
	bInputPressed = false;
}

void AProjectArkPlayerController::OnSpawnItem()
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		for (int i = 0; i < 5; i++)
		{
			auto NewItem = GetWorld()->SpawnActor<APAItem>(MyPawn->GetActorLocation(), FRotator(0, FMath::RandRange(0.f, 360.f),0));
			
		}
	}
}
