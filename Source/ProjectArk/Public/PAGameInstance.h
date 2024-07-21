// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectArk.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "PAGameInstance.generated.h"

#pragma region ItemData
UENUM(BlueprintType)
enum class EItemKind :uint8 {
	IKE_WEAPON UMETA(DisplayName = "무기"),
	IKE_COOKING UMETA(DisplayName = "요리"),
	IKE_CONSUMABLE UMETA(DisplayName = "소비"),
	IKE_POTION UMETA(DisplayName = "물약"),
	IKE_COLLECTABLE UMETA(DisplayName = "수집품"),
	IKE_SHILLING UMETA(DisplayName = "실링"),
	IKE_NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EItemGrade : uint8 {
	IGE_NORMAL UMETA(DisplayName = "일반"),
	IGE_RARE UMETA(DisplayName = "희귀"),
	IGE_HEROIC UMETA(Displayname = "영웅"),
	IGE_LEGENDARY UMETA(Displayname = "전설")
};

UENUM(BlueprintType)
enum class EBoundState : uint8 {
	BSE_CHARACTER UMETA(DisplayName = "캐릭터"),
	BSE_ROSTER UMETA(DisplayName = "원정대"),
	BSE_NONE UMETA(DisplayName = "없음")
};

USTRUCT(BlueprintType)
struct FPAItemData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FPAItemData(): itemID(0), itemName(TEXT("")),eKind(EItemKind::IKE_NONE),
	eGrade(EItemGrade::IGE_NORMAL), eBoundState(EBoundState::BSE_CHARACTER)
	, description(TEXT("")),bCanbeSold(true), bCanbeDestroyed(true),
	bCanbeDismantled(true),sources(TArray<FString>()), buyPrice(0), sellPrice(0)
	, count(0){};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 itemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString itemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemKind eKind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EItemGrade eGrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EBoundState eBoundState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bCanbeSold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bCanbeDestroyed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bCanbeDismantled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FString> sources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int buyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int sellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int count;

};

USTRUCT(BlueprintType)
struct FPACookingData : public FPAItemData {
	GENERATED_BODY()

	public:
	FPACookingData() { };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float coolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int weaponPowerEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int basicStatEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int vitalityStatEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float combatResourceNaturalRecoveryEffect;
};

USTRUCT(BlueprintType)
struct FPAConsumableData: public FPAItemData {
	GENERATED_BODY()
	
public:
	FPAConsumableData() {};
};

USTRUCT(BlueprintType)
struct FPAPotionData: public FPAItemData {
	GENERATED_BODY()
	
public:
	FPAPotionData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int availableLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float coolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int healAmount;
};

USTRUCT(BlueprintType)
struct FPACollectableData : public FPAItemData {
	GENERATED_BODY()
	
public:
	FPACollectableData() {
		bCanbeDismantled = false;
	};
};
#pragma endregion

/**
 * 
 */
UCLASS()
class PROJECTARK_API UPAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPAGameInstance();

	virtual void Init() override;
	
	
private:
	UPROPERTY()
	class UDataTable* PACookingItemTable;
	
	UPROPERTY()
	class UDataTable* PAConsumableItemTable;

	UPROPERTY()
	class UDataTable* PAPotionItemTable;

	UPROPERTY()
	class UDataTable* PACollectableItemTable;

public:
	FStreamableManager Streamablemanager;

	TPair<FString, int32>  ChooseItemID(EItemKind eKind);
	FPAItemData* GetPAItemData(EItemKind eKind, FString ItemID);
	
};
