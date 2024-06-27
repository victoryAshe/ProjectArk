// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectArk.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "ProjectArkPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AProjectArkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectArkPlayerController();

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void OnSpawnItem(); //temporary

private:
	FVector Destination;
	bool bInputPressed; // Input is bring pressed
	bool bMoving;
	FVector MoveCompletedVector;
};


