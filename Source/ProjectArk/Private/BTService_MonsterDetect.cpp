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

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();  // 현재 AI제어하는 Pawn 가져오기
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;                    // 감지 원의 반지름

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	
	// HomePos에서의 거리 계산
	FVector HomePos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMonsterAIController::HomePosKey);
	float DistanceFromHome = FVector::Dist(Center, HomePos);

	if (DistanceFromHome > 2000.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bShouldReturnHome"), true);
		return;
	}
	else if (DistanceFromHome < 1.0f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bShouldReturnHome"), false);
	}
	
	// 주변 충돌체 검출 
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,                             // 탐색된 모든 객체를 담는 TArray
		Center,                                     // 전체 위치에서 탐색 시작
		FQuat::Identity,                            // 충돌 검사 수행 전, 충돌 형상에 적용할 회전으로, 충돌 검사에 회전을 적용하지 않음
		ECollisionChannel::ECC_GameTraceChannel2,   // 충돌 검사에 사용할 트레이스 채널 선택
		FCollisionShape::MakeSphere(DetectRadius),  // 충돌 모양 및 범위
		CollisionQueryParam                         // 단순 충돌 모양만 고려 
	);

	
	// 검색 전 타겟은 초기화를 해줘야 없어졌을때 패트롤 모드로 간다. 아직도 복수의 타겟 처리가 안되었다.
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, nullptr);

	// 무엇인가, 탐색 성공 시 
	if (bResult) 
	{
		// 디텍팅된 플레이어가 없어졌을 경우의 처리가 없음
		
		// 탐색된 오브젝트 중에서 플레이어 캐릭터 찾기 
		for (auto const& OverlapResult : OverlapResults)
		{
			// 충돌체의 액터 가져오기 : 해당 액터가 플레이어 캐릭터라면 ProjectArkCharacter 변수에 할당, 아니라면 nullptr
			AProjectArkCharacter* ProjectArkCharacter = Cast<AProjectArkCharacter>(OverlapResult.GetActor());

			// ProjectArkCharacter가 참 && 플레이어를 제어하는 컨트롤러인지 확인
			if (ProjectArkCharacter && ProjectArkCharacter->GetController()->IsPlayerController())
			{
				// AMonsterAIController의 블랙보드에 플레이어를 타겟으로 설정
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, ProjectArkCharacter);
				
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);    // 감지되면 초록색으로 표시
				DrawDebugPoint(World, ProjectArkCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ProjectArkCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
			
		}
	}

	// OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetKey, nullptr);

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);                  // 감지 안 되면 빨간색으로 표시
}