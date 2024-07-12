// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterCharacter.h"
#include "MonsterAIController.h"


// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �Ž� ������
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	// �ִϸ��̼� �������Ʈ ����
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("/Game/ProjectArkContents/Animation/BP_MonsterAnim.BP_MonsterAnim"));
	if (AnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
	}

	// ĳ���� ������ �ڵ忡�� AI����
	// PlacedInWorldOrSpawned : �� MonsterCharacter���� MonsterAIController ���� ����
	// �÷��̾ �����ϴ� ĳ���͸� ������ ��� ĳ���ʹ� MonsterCharacter �� ���踦 �޴´�.
	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ���� �ݸ��� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCharacter"));

	// ������ �� ȸ�� ���� ����
	bUseControllerRotationYaw = false; // ��Ʈ�ѷ��� Yaw ȸ������ ����Ұǰ�?
	GetCharacterMovement()->bOrientRotationToMovement = true; // �̵� ���⿡ ���� ȸ���Ұǰ�?
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // ȸ�� ������ ��Ʈ�ѷ��� ��� �������� ����Ұǰ�?
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f); // ȸ�� �ӵ� ����
	GetCharacterMovement()->MaxWalkSpeed = 200.0f; // �ӵ� �÷��̾�� ������ ���� 
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

void AMonsterCharacter::Attack()
{
	if (IsAttacking)
	{
		PALOG(Warning, TEXT("Already on the attack"));
	}
	else
	{
		IsAttacking = true;
		PALOG(Warning, TEXT("Let's start the attack"));
		OnAttackMontageEnded(); // �ӽ� ���� 
	}
}

// void AMonsterCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
void AMonsterCharacter::OnAttackMontageEnded()
{
	PACHECK(IsAttacking);
	// ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	// AttackEndComboState();
	OnAttackEnd.Broadcast();
	PALOG(Warning, TEXT("OnAttackMontageEnded()"));
}
