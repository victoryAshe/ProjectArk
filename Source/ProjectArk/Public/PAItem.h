// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectArk.h"
#include "GameFramework/Actor.h"
#include "PAGameInstance.h"
#include "PAItem.generated.h"


UCLASS()
class PROJECTARK_API APAItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APAItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(VisibleAnywhere, Category = Item)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Item)
	UStaticMeshComponent* Item;

	UPROPERTY(VisibleAnywhere, Category = Item)
	EItemKind eItemKind;

	UPROPERTY(VisibleAnywhere, Category = Item)
	int amount = 1;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* ItemNameWidget;
	
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;
	
	struct FPAItemData* ItemData = nullptr;


	//Item StaticMesh 지연로드
private:
	void OnAssetLoadCompleted();

private:
	FSoftObjectPath ItemAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	
private:
	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/*
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);
	*/
};
