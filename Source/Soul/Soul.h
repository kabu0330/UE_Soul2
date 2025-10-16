// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 디버그
DECLARE_LOG_CATEGORY_EXTERN(LogSoul, Log, All);

// 로그 커스텀
// 출시 빌드에서는 모든 로그 비활성화
#if UE_BUILD_SHIPPING
#define LOG(Format, ...)
#define LOG_WARNING(Format, ...)
#define LOG_ERROR(Format, ...)
#define LOG_CYAN(Format, ...)

#else // 개발 빌드에서만 로그 활성화
#define LOG(Format, ...) \
UE_LOG(LogSoul, Log, TEXT(Format), ##__VA_ARGS__)

#define LOG_WARNING(Format, ...) \
UE_LOG(LogSoul, Warning, TEXT(Format), ##__VA_ARGS__)

#define LOG_ERROR(Format, ...) \
UE_LOG(LogSoul, Error, TEXT(Format), ##__VA_ARGS__)

#define DEBUG_MESSAGE(Format, ...) \
do { \
UE_LOG(LogSoul, Log, TEXT(Format), ##__VA_ARGS__); \
if (GEngine) \
{ \
GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, \
FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
} \
} while(0)

#define FIXED_DEBUG_MESSAGE(Index, Format, ...) \
if (GEngine) \
{ GEngine->AddOnScreenDebugMessage(Index, 5.f, FColor::Cyan, FString::Printf(TEXT(Format), ##__VA_ARGS__));}

#endif