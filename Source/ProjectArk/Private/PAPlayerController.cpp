// Fill out your copyright notice in the Description page of Project Settings.


#include "PAPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PACharacter.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "PAItem.h"

APAPlayerController::APAPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	Destination = FVector::ZeroVector;
	MoveCompletedVector = FVector(0.f, 0.f, -1.f);
	bMoving = false;
	bInputPressed = false;
	shilling = 0;

	bInventoryOpened = false;
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_INVENTORY(TEXT("/Game/ProjectArkContents/UI/WB_Inventory.WB_Inventory_C"));
	if (UI_INVENTORY.Succeeded())
	{
		UI_InventoryClass = UI_INVENTORY.Class;
	}

	PACHECK(nullptr != UI_InventoryClass);

	InventoryWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), UI_InventoryClass));
}

void APAPlayerController::PlayerTick(float DeltaTime)
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

			if (FVector::Distance(WorldDirection, MoveCompletedVector) < 0.01f)
			{
				Destination = FVector::ZeroVector;
				bMoving = false;
			}
		}
	}
}

void APAPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APAPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APAPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("SpawnItem", IE_Pressed, this, &APAPlayerController::OnSpawnItem);

	InputComponent->BindAction("CallInventory", IE_Pressed, this, &APAPlayerController::OnCallInventory);
}

void APAPlayerController::OnSetDestinationPressed()
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

void APAPlayerController::OnSetDestinationReleased()
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
	bInputPressed = false;
}


void APAPlayerController::OnSpawnItem() //Temporary
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		for (int i = 0; i < 1; i++)
		{
			auto NewItem = GetWorld()->SpawnActor<APAItem>(MyPawn->GetActorLocation(), FRotator(0, FMath::RandRange(0.f, 360.f), 0));

		}
	}
}

void APAPlayerController::OnCallInventory()
{
	PACHECK(nullptr != InventoryWidget);
	if (bInventoryOpened == true)
	{
		InventoryWidget->RemoveFromViewport();
		bInventoryOpened = false;

	}
	else
	{
		InventoryWidget->AddToViewport(11);
		bInventoryOpened = true;
	}
}

void APAPlayerController::AddItem(APAItem* NewItem)
{
	PACHECK(nullptr!=NewItem)

	if (NewItem->eItemKind == EItemKind::IKE_SHILLING)
	{
		shilling += NewItem->amount;
		PALOG(Warning, TEXT("Shillings: %d"), shilling);
	}
	else
	{
		InvenArray.Emplace(NewItem);
	}
}

