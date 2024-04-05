// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class PROJECTARK_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	// 몬스터 캐릭터 데이터
	int Hp;
	float Power;
	float Defense;
	float CoolTime;
	FName Name;
	TArray<FString> Scripts;

public:
	// Sets default values for this character's properties
	AMonsterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
