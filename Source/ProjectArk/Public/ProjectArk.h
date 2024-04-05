// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectArk, Log, All);

#define PALOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) +TEXT(")"))
#define PALOG_S(Verbosity) UE_LOG(LogProjectArk, Verbosity, TEXT("%s"), *PALOG_CALLINFO)
#define PALOG(Verbosity, Format, ...) UE_LOG(LogProjectArk, Verbosity, TEXT("%s %s"), *PALOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//ABCHECK ��ũ�� ���� ������ ���� �ƴҰ�� ���� �α�
#define PACHECK(Expr, ...) {if(!(Expr)) {PALOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}