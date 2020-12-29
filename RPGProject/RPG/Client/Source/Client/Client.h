// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if !WITH_EDITOR
#define client_checkNoReentry() checkNoReentry()
#else
#define client_checkNoReentry()
#endif

#define UINT64_TO_FSTRING(UInt64Value) (FString::Printf(TEXT("%I64u"),UInt64Value))
#define FSTRING_TO_UINT64(FStringValue) (FCString::Strtoui64(*FStringValue, nullptr, 10))
