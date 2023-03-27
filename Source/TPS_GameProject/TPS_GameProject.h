// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//��ũ�� �Լ��̸�(�ٹ�ȣ)
#define  CAllINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")")) // FString ���·� ����Ǵ� ������ ��ȯ�ϴ� �Լ��� ��ũ�� �����Ѵ�.

#define PRINT_CALLINFO() UE_LOG(LogTemp, Warning, TEXT("%s"), *CAllINFO) //ġȯ 

//�������� ��ũ�� ##__VA_ARGS__ �� ... �� int %d ó�� ##__VA_ARGS__
#define PRINT_LOG(fmt, ...) UE_LOG(LogTemp, Warning, TEXT("%s %s"), *CAllINFO, *FString::Printf(fmt, ##__VA_ARGS__))

