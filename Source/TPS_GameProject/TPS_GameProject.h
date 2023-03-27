// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//메크로 함수이름(줄번호)
#define  CAllINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")")) // FString 형태로 실행되는 라인을 반환하는 함수를 매크로 지정한다.

#define PRINT_CALLINFO() UE_LOG(LogTemp, Warning, TEXT("%s"), *CAllINFO) //치환 

//가변인자 매크로 ##__VA_ARGS__ 을 ... 에 int %d 처럼 ##__VA_ARGS__
#define PRINT_LOG(fmt, ...) UE_LOG(LogTemp, Warning, TEXT("%s %s"), *CAllINFO, *FString::Printf(fmt, ##__VA_ARGS__))

