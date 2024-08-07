// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectArk.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "PAItem.h"
#include "PAPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class PAItem;

/**
 * 
 */
UCLASS()
class PROJECTARK_API APAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APAPlayerController();

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnCallInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(APAItem* NewItem);

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
	/*Move*/
	FVector Destination;
	bool bInputPressed; // Input is bring pressed
	bool bMoving;
	FVector MoveCompletedVector;

	/*Finincial*/
	int32 shilling;

	/*Inventory widget*/
	bool bInventoryOpened;

	UPROPERTY(VisibleAnywhere, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> UI_InventoryClass;

	UPROPERTY(VisibleAnywhere, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* InventoryWidget;

	UPROPERTY(VisibleAnywhere, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<APAItem*> InvenArray;
	
};
