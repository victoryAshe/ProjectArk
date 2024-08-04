// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MonsterCharacter.h"
#include "PACharacter.h"

// 블랙보드 Key 
const FName AMonsterAIController::HomePosKey(TEXT("HomePos"));                      // Monster 생성 위치 값 
const FName AMonsterAIController::PatrolPosKey(TEXT("PatrolPos"));                  // 순찰할 위치 정보 
const FName AMonsterAIController::TargetKey(TEXT("Target"));                        // 플레이어 위치 
const FName AMonsterAIController::ShouldReturnHomeKey(TEXT("bShouldReturnHome"));   // 기존 위치로 돌아가?

AMonsterAIController::AMonsterAIController()
{
	RepeatInterval = 3.0f;

	// 비헤이비어 트리, 블랙보드 데이터 로드

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
	
	// MonsterAIController 가동 시, 
	// 비헤이비어 트리 애셋과 같은 폴더에 위치한 블랙보드 애셋, 비헤이비어 트리가 함께 동작함

	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		// 몬스터의 초기 위치를 HomePosKey에 저장
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			PALOG(Error, TEXT("AIController coudn't run behavior tree!"));
		}

		// 3초 간격으로 거리 체크 (OnRepeatTimer() 호출)
		GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMonsterAIController::OnRepeatTimer, RepeatInterval, true);

	}
}


void AMonsterAIController::OnRepeatTimer()
{
	PrintDistanceToTarget();
}


// 몬스터 - 타겟 거리 출력
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