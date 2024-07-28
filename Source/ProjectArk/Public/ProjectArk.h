// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectArk, Log, All);

#define PALOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) +TEXT(")"))
#define PALOG_S(Verbosity) UE_LOG(LogProjectArk, Verbosity, TEXT("%s"), *PALOG_CALLINFO)
#define PALOG(Verbosity, Format, ...) UE_LOG(LogProjectArk, Verbosity, TEXT("%s %s"), *PALOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//ABCHECK 매크로 내의 조건이 참이 아닐경우 붉은 로그
#define PACHECK(Expr, ...) {if(!(Expr)) {PALOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

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