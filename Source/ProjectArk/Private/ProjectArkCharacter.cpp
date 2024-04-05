// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectArkCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AProjectArkCharacter::AProjectArkCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1400.f;
	CameraBoom->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	bisZoomed = false;
	ArmLengthSpeed = 15.f;
	TargetArmLength = 1400.f;
	ZoomedRotator = FRotator(-40.f, 0.f,0.f);
	NotZoomedRotator = FRotator(-50.f,0.f,0.f);
	ArmRotationSpeed = 3.f;

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}



void AProjectArkCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (bisZoomed == true)
	{
		if (CameraBoom->TargetArmLength > TargetArmLength)
		{
			CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - ArmLengthSpeed, 300, 1400);
		}
		CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), ZoomedRotator, DeltaSeconds, ArmRotationSpeed));
	}
	else if (bisZoomed == false)
	{
		if (CameraBoom->TargetArmLength < TargetArmLength)
		{
			CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + ArmLengthSpeed, 300, 1400);
		}
		CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), NotZoomedRotator, DeltaSeconds, ArmRotationSpeed));
	}
}

// Called to bind functionality to input
void AProjectArkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("SetCameraScope"), this, &AProjectArkCharacter::SetCameraScope);
}

void AProjectArkCharacter::SetCameraScope(float NewAxisValue)
{
	PACHECK(CameraBoom != nullptr);
	if (NewAxisValue == 0) return;

	if (NewAxisValue < 0)
	{
		if (bisZoomed == true) return;
		bisZoomed = true;
		TargetArmLength = 300.f;
	}
	else
	{
		if (bisZoomed == false) return;
		bisZoomed = false;
		TargetArmLength = 1400.f;
	}
	
	PALOG(Warning, TEXT("CameraBooom's RelativeRotation: %s"), *CameraBoom->GetRelativeRotation().ToString());

}