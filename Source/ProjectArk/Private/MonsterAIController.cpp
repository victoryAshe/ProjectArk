// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterCharacter.h"
#include "PACharacter.h"

// ������ Key 
const FName AMonsterAIController::HomePosKey(TEXT("HomePos"));                      // Monster ���� ��ġ �� 
const FName AMonsterAIController::PatrolPosKey(TEXT("PatrolPos"));                  // ������ ��ġ ���� 
const FName AMonsterAIController::TargetKey(TEXT("Target"));                        // �÷��̾� ��ġ 
const FName AMonsterAIController::ShouldReturnHomeKey(TEXT("bShouldReturnHome"));   // ���� ��ġ�� ���ư�?

AMonsterAIController::AMonsterAIController()
{
	RepeatInterval = 3.0f;

	// �����̺�� Ʈ��, ������ ������ �ε�

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/ProjectArkContents/AI/BB_MonsterCharacter.BB_MonsterCharacter"));
	if (BBObject.Succeeded()) 
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/ProjectArkContents/AI/BT_MonsterCharacter.BT_MonsterCharacter"));
	if (BTObject.Succeeded()) 
	{
		BTAsset = BTObject.Object;
	}
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// MonsterAIController ���� ��, 
	// �����̺�� Ʈ�� �ּ°� ���� ������ ��ġ�� ������ �ּ�, �����̺�� Ʈ���� �Բ� ������

	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		// ������ �ʱ� ��ġ�� HomePosKey�� ����
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			PALOG(Error, TEXT("AIController coudn't run behavior tree!"));
		}

		// 3�� �������� �Ÿ� üũ (OnRepeatTimer() ȣ��)
		GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMonsterAIController::OnRepeatTimer, RepeatInterval, true);

	}
}


void AMonsterAIController::OnRepeatTimer()
{
	PrintDistanceToTarget();
}


// ���� - Ÿ�� �Ÿ� ���
void AMonsterAIController::PrintDistanceToTarget()
{
	auto ControllingPawn = GetPawn();
	if (nullptr == ControllingPawn)
	{
		PALOG(Error, TEXT("ControllingPawn is null"));
		return;
	}

	auto Target = Cast<APACharacter>(Blackboard->GetValueAsObject(TargetKey));
	if (nullptr == Target)
	{
		PALOG(Warning, TEXT("Target is null"));
		return;
	}

	float Distance = Target->GetDistanceTo(ControllingPawn);
	PALOG(Log, TEXT("Target distance: %s"), *FString::SanitizeFloat(Distance));
}