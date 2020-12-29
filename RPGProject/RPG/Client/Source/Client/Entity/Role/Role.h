#pragma once

#include "EntityDeclareDefine.h"
#include "Entity/ServerGameEntity.h"

class Role : public ServerGameEntity
{
	KBE_DECLARE_ENTITY_MAP();

	typedef ServerGameEntity Supper;

public:
	Role();
	~Role();

	/**
	*Exposed method
	*请求退出游戏,客户端从游戏状态返回到登陆界面/直接退出
	*
	*
	*@return 无
	*/
	void BASE_Logoff();

	/**
	*Exposed method
	*请求小退（退出角色）,客户端从游戏状态返回到角色选择
	*
	*@return 无
	*/
	void BASE_Logout();

	void CLIENT_OnRepJumpState(uint8 JumpState, FVector InVelocity, FVector InPos);

protected:
	virtual void OnEnterWorld() override;       ///< Entity进入世界时，此方法被调用
	virtual void OnLeaveWorld() override;       ///< 当Entity离开世界（被销毁时）时，此方法被调用
};