// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
#include "MonsterAIController.h"
#include "MonsterCharacter.h"
#include "PACharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MonsterCharacter = Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MonsterCharacter)
		return EBTNodeResult::Failed;

	auto Target = Cast<APACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	// 타겟의 위치와 몬스터 캐릭터의 위치를 이용해 두 위치 간의 방향 벡터를 계산
	FVector LookVector = Target->GetActorLocation() - MonsterCharacter->GetActorLocation();
	// Z 축(높이) 방향의 변화를 무시 : 몬스터 캐릭터가 수평 평면에서만 회전
	LookVector.Z = 0.0f; 
	// 방향 벡터(LookVector)를 이용해 회전 행렬을 생성하고, 그 행렬을 회전 값(FRotator)으로 변환
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// 부드럽게 회전 FMath::RInterpTo 는 선형 보간 함수
	MonsterCharacter->SetActorRotation(FMath::RInterpTo(MonsterCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}