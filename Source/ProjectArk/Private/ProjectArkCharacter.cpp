// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectArkCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

AProjectArkCharacter::AProjectArkCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	MinimapCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapCameraBoom"));
	SpriteCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SpriteCaptureComponent2D"));
	IndicatorSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("IndicatorSprite"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

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
	ZoomedRotator = FRotator(-40.f, 0.f, 0.f);
	NotZoomedRotator = FRotator(-50.f, 0.f, 0.f);
	ArmRotationSpeed = 3.f;

	// Create a camera...
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a minimap camera boom
	MinimapCameraBoom->SetupAttachment(RootComponent);
	MinimapCameraBoom->SetUsingAbsoluteRotation(true);
	MinimapCameraBoom->TargetArmLength = 2400.f;
	MinimapCameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	MinimapCameraBoom->bDoCollisionTest = false;
	MinimapCameraBoom->bInheritYaw = false;
	MinimapCameraBoom->bInheritPitch = false;
	MinimapCameraBoom->bInheritRoll = false;

	// Create a sprite capture component
	SpriteCaptureComponent2D->SetupAttachment(MinimapCameraBoom, USpringArmComponent::SocketName);
	SpriteCaptureComponent2D->ProjectionType = ECameraProjectionMode::Orthographic;
	SpriteCaptureComponent2D->OrthoWidth = 1024.f;
	SpriteCaptureComponent2D->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> Minimap_SpriteRenderTargetObject
	(TEXT("/Game/ProjectArkContents/Textures/Minimap_SpriteRenderTarget2D.Minimap_SpriteRenderTarget2D"));
	if (Minimap_SpriteRenderTargetObject.Succeeded())
	{
		SpriteCaptureComponent2D->TextureTarget = Minimap_SpriteRenderTargetObject.Object;
	}

	IndicatorSpriteComponent->SetupAttachment(RootComponent);
	IndicatorSpriteComponent->SetWorldRotation(FRotator::MakeFromEuler(FVector(90.f, 0.f, -90.f)));
	IndicatorSpriteComponent->SetRelativeLocation(FVector(0.f, 0.f, 800.f));
	IndicatorSpriteComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	static ConstructorHelpers::FObjectFinder<UPaperSprite> SPRITE_INDICATOR(TEXT("/Game/ProjectArkContents/Textures/indicator_Sprite.indicator_Sprite"));
	if (SPRITE_INDICATOR.Succeeded())
	{
		IndicatorSpriteComponent->SetSprite(SPRITE_INDICATOR.Object);
	}
	IndicatorSpriteComponent->bVisibleInSceneCaptureOnly = true;
	IndicatorSpriteComponent->bOnlyOwnerSee = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_MINIMAP(TEXT("/Game/ProjectArkContents/UI/WB_Minimap.WB_Minimap_C"));
	if (UI_MINIMAP.Succeeded())
	{
		UI_MinimapClass = UI_MINIMAP.Class;
	}

}




void AProjectArkCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		PACHECK(nullptr != UI_MinimapClass);

		MinimapWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), UI_MinimapClass));
		PACHECK(nullptr != MinimapWidget);
		MinimapWidget->AddToViewport(10);

	}
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