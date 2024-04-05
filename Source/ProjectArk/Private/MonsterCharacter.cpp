// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterCharacter.h"
#include "MonsterAIController.h"


// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, 90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	// 캐릭터 생성자 코드에서 AI구현
	// PlacedInWorldOrSpawned : 새 MonsterCharacter마다 MonsterAIController 액터 생성
	// 플레이어가 조종하는 캐릭터를 제외한 모든 캐릭터는 MonsterCharacter 의 지배를 받는다.
	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

