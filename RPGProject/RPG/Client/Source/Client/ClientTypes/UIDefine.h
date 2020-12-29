#pragma once

#include "UIDefine.generated.h"

/**
* 界面类型
*/
UENUM(BlueprintType)
enum class WINDOW_TYPE_ENUM : uint8
{
	///没有界面类型
	WINDOW_TYPE_NO = 0,
	///登录界面
	WINDOW_TYPE_LOGIN,
	///角色创建界面
	WINDOW_TYPE_CREATE_ROLE,
	///角色选择界面
	WINDOW_TYPE_SELECT_ROLE,
	///场景转换界面
	WINDOW_TYPE_SWITCH_SCENE,
	///主界面
	WINDOW_TYPE_MAIN
};
