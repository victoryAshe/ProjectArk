// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectArk.h"
#include "GameFramework/Character.h"
#include "ProjectArkCharacter.generated.h"

UCLASS(Blueprintable)
class AProjectArkCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectArkCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	void SetCameraScope(float NewAxisValue); // Set CameraBoom's ArmLength

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/*Minimap camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* SceneCaptureComponent2D;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* MinimapCameraBoom;

	/*Minimap widget*/
	UPROPERTY(VisibleAnywhere, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> UI_MinimapClass;

	UPROPERTY(VisibleAnywhere, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	class UUserWidget* MinimapWidget;

	/*Minimap: character indicator*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* IndicatorSpriteComponent;

private:
	bool bisZoomed;
	float ArmLengthSpeed; // The CameraBoom's ArmLegnth Changing Speed
	float TargetArmLength;
	FRotator ZoomedRotator;
	FRotator NotZoomedRotator;
	float ArmRotationSpeed;
};

