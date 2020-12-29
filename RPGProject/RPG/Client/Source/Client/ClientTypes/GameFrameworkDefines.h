#pragma once




///游戏状态
UENUM(Blueprintable)
enum class EGameStatus : uint8
{
	Login = 1,			///账号登录
	LoginLoading,		///账号登录加载
	SelectRole,			///角色选择
	SelectRoleLoading,	///角色选择加载
	BackToSelectRoleLoading, ///返回角色选择加载
	CreateRole,			///角色创建
	CreateRoleLoading,	///角色创建加载
	Teleport,			///传送
	TeleportLoading,	///传送加载
	InWorld,			///游戏世界
	InWorldLoading,		///游戏世界加载
};

///登录阶段
UENUM(Blueprintable)
enum class ELoginStage : uint8
{
	NoLoginStage = 1,		///未登录阶段
	TryLoginStage,			///尝试登录阶段 
	AccountLoginStage,		///账号登录阶段
	RoleLoginStage			///角色登录阶段
};

///加载类型
UENUM(Blueprintable)
enum class ELoadingType : uint8
{
	None = 1,					///没有在加载
	LoginLoading,				///账号登录加载
	SelectRoleLoading,			///角色选择加载	
	BackToSelectRoleLoading,	///返回角色选择加载
	CreateRoleLoading,			///角色创建加载
	TeleportLoading,			///传送加载
	InWorldLoading,				///游戏世界加载
};