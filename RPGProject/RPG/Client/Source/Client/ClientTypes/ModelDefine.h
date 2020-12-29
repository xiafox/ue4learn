#pragma once

#include "ModelDefine.generated.h"

/**
* 模型数据
*/
UENUM(BlueprintType)
enum class MODEL_PART_TYPE : uint8
{
	///头部模型
	MODEL_PART_HEAD = 1,
	///脸部模型
	MODEL_PART_FACE = 2,
	///披风模型
	MODEL_PART_CLOAK = 3,
	///上身模型
	MODEL_PART_BODY = 4,
	///头发模型
	MODEL_PART_HAIR = 5,
	///头饰模型
	MODEL_PART_ADORN = 6,
	///帽子模型
	MODEL_PART_HAT = 7,
	///上衣模型
	MODEL_PART_COAT = 8,
	///护腕模型
	MODEL_PART_WRIST = 9,
	///手套模型
	MODEL_PART_HAND = 10,
	///腰带模型
	MODEL_PART_WAIST = 11,
	///裤子模型
	MODEL_PART_PANTS = 12,
	///鞋子模型
	MODEL_PART_SHOES = 13,
	///项链模型
	MODEL_PART_NECKLACE = 14,
	///左手戒指模型
	MODEL_PART_LRING = 15,
	///右手戒指模型
	MODEL_PART_RRING = 16,
	///左武器模型
	MODEL_PART_LWEAPON = 17,
	///右武器模型
	MODEL_PART_RWEAPON = 18,
	/// 默认静态模型
	MODEL_TYPE_STATIC = 19,
};
