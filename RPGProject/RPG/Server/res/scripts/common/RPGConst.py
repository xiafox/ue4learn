#-*-coding:utf-8-*-

CONFIG_PATCH = "res/scripts/data/"

# --------------------------------------------------------------------
# about login
# --------------------------------------------------------------------
LOGIN_ROLE_UPPER_LIMIT					= 5			# 最多可以创建多少个角色
ROLE_NAME_LENGTH_UPPER_LIMIT			= 14		# 角色名字长度上限

#角色状态roleState，有刚创建0，激活（1），删除（3）
ROLE_STATE_CREATE						= 0			# 刚创建
ROLE_STATE_ACTIVE						= 1			# 激活，创建完进入游戏就会设置为激活
ROLE_STATE_INACTIVE						= 2			# 不激活，目前没有使用	
ROLE_STATE_DELETE						= 3			# 删除角色


# --------------------------------------------------------------------
# ACTION_FORBID 行为开关
# --------------------------------------------------------------------
ACTION_FORBID_NONE						= 0					# 无
ACTION_FORBID_MOVE						= 0x00000001		# 2^0	禁止主动移动（只禁止主动位移，不会禁止受击被迫位移）
ACTION_FORBID_SPELL						= 0x00000002		# 2^1	禁止施法
ACTION_FORBID_USE_ITEM					= 0x00000004		# 2^2	禁止使用道具
ACTION_FORBID_BE_ATTACK					= 0x00000008		# 2^3	禁止受击
ACTION_FORBID_ATTACK					= 0x00000010		# 2^4	禁止攻击(普通物理攻击)
ACTION_FORBID_JUMP						= 0x00000020		# 2^5	禁止跳跃

ACTION_FLAG_LIST = [ ACTION_FORBID_MOVE, ACTION_FORBID_SPELL, ACTION_FORBID_USE_ITEM, ACTION_FORBID_BE_ATTACK, ACTION_FORBID_ATTACK, ACTION_FORBID_JUMP, ]
# --------------------------------------------------------------------
# state
# --------------------------------------------------------------------
ENTITY_STATE_NONE			= 0			# 无状态
ENTITY_STATE_FREE			= 1			# 自由状态
ENTITY_STATE_FIGHT			= 2			# 战斗状态
ENTITY_STATE_DEAD			= 3			# 死亡状态

STATE_TO_ACTION_FLAGS = {
	ENTITY_STATE_NONE : 0,
	ENTITY_STATE_FREE : 0,
	ENTITY_STATE_FIGHT : 0,
	ENTITY_STATE_DEAD : ACTION_FORBID_MOVE | ACTION_FORBID_SPELL | ACTION_FORBID_USE_ITEM | ACTION_FORBID_BE_ATTACK | ACTION_FORBID_ATTACK | ACTION_FORBID_JUMP,
	}
# --------------------------------------------------------------------
# effectState 效果状态
# --------------------------------------------------------------------
EFFECT_STATE_DIZZY						= 0x00000001		# 2^0	眩晕效果
EFFECT_STATE_FIX						= 0x00000002		# 2^1	定身效果
EFFECT_STATE_SLEEP						= 0x00000004		# 2^2	睡眠效果

EFFECT_STATE_LIST = [EFFECT_STATE_DIZZY, EFFECT_STATE_FIX, EFFECT_STATE_SLEEP ]


EFFECT_STATE_TO_ACTION_FLAGS = {
	EFFECT_STATE_DIZZY : ACTION_FORBID_MOVE | ACTION_FORBID_USE_ITEM | ACTION_FORBID_ATTACK | ACTION_FORBID_JUMP,
	EFFECT_STATE_FIX : ACTION_FORBID_MOVE | ACTION_FORBID_ATTACK | ACTION_FORBID_JUMP,
	EFFECT_STATE_SLEEP : ACTION_FORBID_MOVE | ACTION_FORBID_USE_ITEM | ACTION_FORBID_ATTACK | ACTION_FORBID_JUMP,
	}
# --------------------------------------------------------------------
# 模型
# --------------------------------------------------------------------
MODEL_PART_HEAD = 1			# 头部模型"headModel"
MODEL_PART_FACE = 2			# 脸部模型"faceModel"
MODEL_PART_CLOAK = 3		# 披风模型"cloakModel"
MODEL_PART_BODY = 4			# 上身模型"bodyModel"
MODEL_PART_HAIR = 5			# 头发模型"hairModel"
MODEL_PART_ADORN = 6		# 头饰模型"adornModel"
MODEL_PART_HAT = 7			# 帽子模型"hatModel"
MODEL_PART_COAT = 8			# 上衣模型"coatModel"
MODEL_PART_WRIST = 9		# 护腕模型"wristModel"
MODEL_PART_HAND = 10		# 手套模型"handModel"
MODEL_PART_WAIST = 11		# 腰带模型"waistModel"
MODEL_PART_PANTS = 12		# 裤子模型"pantsModel"
MODEL_PART_SHOES = 13		# 鞋子模型"shoesModel"
MODEL_PART_NECKLACE = 14	# 项链模型"necklaceModel"
MODEL_PART_LRING = 15		# 左手戒指模型"lringModel"
MODEL_PART_RRING = 16		# 右手戒指模型"rringModel"
MODEL_PART_LWEAPON = 17		# 左武器模型"weaponModel"
MODEL_PART_RWEAPON = 18		# 右武器模型"weaponModel"
MODEL_TYPE_STATIC = 19		# 默认静态模型

MODEL_PARTS = [MODEL_PART_HEAD, MODEL_PART_FACE, MODEL_PART_CLOAK, MODEL_PART_BODY, MODEL_PART_HAIR, MODEL_PART_ADORN,
	MODEL_PART_HAT, MODEL_PART_COAT, MODEL_PART_WRIST, MODEL_PART_HAND, MODEL_PART_WAIST, MODEL_PART_PANTS,
	MODEL_PART_SHOES, MODEL_PART_NECKLACE, MODEL_PART_LRING, MODEL_PART_RRING, MODEL_PART_LWEAPON, MODEL_PART_RWEAPON,
	MODEL_TYPE_STATIC ]