// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_MonsterDetect.h"
#include "MonsterAIController.h"
#include "MonsterCharacter.h"
#include "ProjectArkCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

// 몬스터가 플레이어 감지하는 서비스 
UBTService_MonsterDetect::UBTService_MonsterDetect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f; // 1초마다 서비스 실행
}

void UBTService_MonsterDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 현재 AI제어하는 Pawn 가져오기
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 500.0f; // 감지 원의 반지름

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	// 주변 충돌체 검출 
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, // 충돌체 검출 결과 저장 배열
		Center, 
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius), // 구체 모양의 충돌체
		CollisionQueryParam
	);

	// 충돌체 발견하면
	if (bResult) 
	{
		// PALOG(Log, TEXT("bResult is true."));
		for (auto const& OverlapResult : OverlapResults)
		{
			// 충돌체의 액터 가져오기 : 해당 액터가 플레이어 캐릭터라면 ProjectArkCharacter 변수에 할당, 아니라면 nullptr
			AProjectArkCharacter* ProjectArkCharacter = Cast<AProjectArkCharacter>(OverlapResult.GetActor());

			// ProjectArkCharacter가 참 && 플레이어를 제어하는 컨트롤러인지 확인
			if (ProjectArkCharacter && ProjectArkCharacter->GetController()->IsPlayerController())
			{
				// AMonsterAIController의 블랙보드에 플레이어를 타겟으로 설정
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, ProjectArkCharacter);
				// 감지되면 초록색으로 표시
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, ProjectArkCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ProjectArkCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}
	// 감지 안 되면 빨간색으로 표시
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}