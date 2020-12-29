#pragma once

#include "ModelDefine.h"

const TMap<MODEL_PART_TYPE, FName> ModelPartType_To_SockName = {
	{MODEL_PART_TYPE::MODEL_PART_LWEAPON, TEXT("Socket_Weapon_L")},
	{MODEL_PART_TYPE::MODEL_PART_RWEAPON, TEXT("Socket_Weapon_R")},
};

const int ONE_JUMP_HEIGHT = 300;	// 一段跳高度
const int TWO_JUMP_HEIGHT = 200;	// 二段跳高度

// effectState 效果状态
const int EFFECT_STATE_DIZZY = 1;		// 2 ^ 0	眩晕效果
const int EFFECT_STATE_FIX = 2;			// 2 ^ 1	定身效果
const int EFFECT_STATE_SLEEP = 4;		// 2 ^ 2	睡眠效果

// 效果状态优先级
const TArray<int> EFFECT_STATE_PRIORITY = { EFFECT_STATE_SLEEP, EFFECT_STATE_DIZZY, EFFECT_STATE_FIX };

const int32 RESOURCE_LOAD_DISTANCE[3] = { 1000, 3000, 5000 };