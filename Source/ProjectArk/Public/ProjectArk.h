// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectArk, Log, All);

#define PALOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) +TEXT(")"))
#define PALOG_S(Verbosity) UE_LOG(LogProjectArk, Verbosity, TEXT("%s"), *PALOG_CALLINFO)
#define PALOG(Verbosity, Format, ...) UE_LOG(LogProjectArk, Verbosity, TEXT("%s %s"), *PALOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//ABCHECK ��ũ�� ���� ������ ���� �ƴҰ�� ���� �α�
#define PACHECK(Expr, ...) {if(!(Expr)) {PALOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

UENUM(BlueprintType)
enum class EItemKind :uint8 {
	IKE_WEAPON UMETA(DisplayName = "����"),
	IKE_COOKING UMETA(DisplayName = "�丮"),
	IKE_CONSUMABLE UMETA(DisplayName = "�Һ�"),
	IKE_POTION UMETA(DisplayName = "����"),
	IKE_COLLECTABLE UMETA(DisplayName = "����ǰ"),
	IKE_SHILLING UMETA(DisplayName = "�Ǹ�"),
	IKE_NONE UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EItemGrade : uint8 {
	IGE_NORMAL UMETA(DisplayName = "�Ϲ�"),
	IGE_RARE UMETA(DisplayName = "���"),
	IGE_HEROIC UMETA(Displayname = "����"),
	IGE_LEGENDARY UMETA(Displayname = "����")
};

UENUM(BlueprintType)
enum class EBoundState : uint8 {
	BSE_CHARACTER UMETA(DisplayName = "ĳ����"),
	BSE_ROSTER UMETA(DisplayName = "������"),
	BSE_NONE UMETA(DisplayName = "����")
};