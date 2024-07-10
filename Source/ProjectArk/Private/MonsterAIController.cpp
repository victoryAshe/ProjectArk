// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMonsterAIController::HomePosKey(TEXT("HomePos")); // Monster 생성 위치 값 
const FName AMonsterAIController::PatrolPosKey(TEXT("PatrolPos")); // 순찰할 위치 정보 
const FName AMonsterAIController::TargetKey(TEXT("Target")); // 플레이어 위치 

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

	// MonsterAIController 가동 시 비헤이비어 트리 애셋과 같은 폴더에 위치한 블랙보드 애셋, 비헤이비어 트리가 함께 동작한다!
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		// 액터의 위치정보를 HomePosKey에 저장
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());

		// HomePosKey 출력
		// FVector LoggedHomePos = Blackboard->GetValueAsVector(HomePosKey);
		// PALOG(Warning, TEXT("HomePosKey value: %s"), *LoggedHomePos.ToString());

		if (!RunBehaviorTree(BTAsset))
		{
			PALOG(Error, TEXT("AIController coudn't run behavior tree!"));
		}
	}
}
