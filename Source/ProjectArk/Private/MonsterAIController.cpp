// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AMonsterAIController::AMonsterAIController()
{
	// RepeatInterval = 3.0f;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_MonsterCharacter.BB_MonsterCharacter"));
	if (BBObject.Succeeded()) 
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_MonsterCharacter.BT_MonsterCharacter"));
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
		if (!RunBehaviorTree(BTAsset))
		{
			// ABLOG(Error, TEXT("AIController coudn't run behavior tree!"));
		}
	}
}

/*
void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AMonsterAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	// ABCHECK(nullptr != CurrentPawn);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr != NavSystem) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		// ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
}
*/