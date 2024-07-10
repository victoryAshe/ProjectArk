// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMonsterAIController::HomePosKey(TEXT("HomePos")); // Monster ���� ��ġ �� 
const FName AMonsterAIController::PatrolPosKey(TEXT("PatrolPos")); // ������ ��ġ ���� 
const FName AMonsterAIController::TargetKey(TEXT("Target")); // �÷��̾� ��ġ 

AMonsterAIController::AMonsterAIController()
{
	// RepeatInterval = 3.0f;

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
	// GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMonsterAIController::OnRepeatTimer, RepeatInterval, true);

	// MonsterAIController ���� �� �����̺�� Ʈ�� �ּ°� ���� ������ ��ġ�� ������ �ּ�, �����̺�� Ʈ���� �Բ� �����Ѵ�!
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
	}
}
