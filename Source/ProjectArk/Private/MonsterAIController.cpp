// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterCharacter.h"
#include "ProjectArkCharacter.h"

const FName AMonsterAIController::HomePosKey(TEXT("HomePos")); // Monster ���� ��ġ �� 
const FName AMonsterAIController::PatrolPosKey(TEXT("PatrolPos")); // ������ ��ġ ���� 
const FName AMonsterAIController::TargetKey(TEXT("Target")); // �÷��̾� ��ġ 

AMonsterAIController::AMonsterAIController()
{
	RepeatInterval = 3.0f;

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
	
	// MonsterAIController ���� �� �����̺�� Ʈ�� �ּ°� ���� ������ ��ġ�� ������ �ּ�, �����̺�� Ʈ������ �Բ� �����Ѵ�!
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		// ������ ��ġ������ HomePosKey�� ����
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());

		// HomePosKey ���
		// FVector LoggedHomePos = Blackboard->GetValueAsVector(HomePosKey);
		// PALOG(Warning, TEXT("HomePosKey value: %s"), *LoggedHomePos.ToString());

		if (!RunBehaviorTree(BTAsset))
		{
			PALOG(Error, TEXT("AIController coudn't run behavior tree!"));
		}

		// 3�� �������� �Ÿ� üũ 
		GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMonsterAIController::OnRepeatTimer, RepeatInterval, true);

	}
}

void AMonsterAIController::OnRepeatTimer()
{
	PrintDistanceToTarget();
}

void AMonsterAIController::PrintDistanceToTarget()
{
	auto ControllingPawn = GetPawn();
	if (nullptr == ControllingPawn)
	{
		PALOG(Error, TEXT("ControllingPawn is null"));
		return;
	}

	auto Target = Cast<AProjectArkCharacter>(Blackboard->GetValueAsObject(TargetKey));
	if (nullptr == Target)
	{
		PALOG(Error, TEXT("Target is null"));
		return;
	}

	float Distance = Target->GetDistanceTo(ControllingPawn);
	PALOG(Warning, TEXT("Target distance: %s"), *FString::SanitizeFloat(Distance));
}