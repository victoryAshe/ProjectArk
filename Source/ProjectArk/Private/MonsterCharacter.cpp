// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterCharacter.h"
#include "MonsterAIController.h"


// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 매쉬 입히기
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
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

	// 몬스터 콜리전 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCharacter"));

	// 움직임 및 회전 동작 조정
	bUseControllerRotationYaw = false; // 컨트롤러의 Yaw 회전값을 사용할건가?
	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향에 따라 회전할건가?
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // 회전 방향을 컨트롤러의 희망 방향으로 사용할건가?
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f); // 회전 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 200.0f; // 속도 플레이어보다 느리게 설정 
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
