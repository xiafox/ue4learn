#pragma once

#include "EntityDeclareDefine.h"
#include "ClientEntity.h"

/**
*游戏对象基础类
*所有扩展出的游戏对象都应该继承于该类
*/
class ServerGameEntity : public ClientEntity
{
	KBE_DECLARE_ENTITY_MAP();
	typedef ClientEntity Supper;

public:

	ServerGameEntity();
	~ServerGameEntity();

	void OnRep_modelParts(const KBEngine::FVariantMap& newModelParts, const KBEngine::FVariantMap& oldModelParts);
	void OnRep_modelExtParts(const KBEngine::FVariantMap& newModelParts, const KBEngine::FVariantMap& oldModelParts);

protected:
	virtual void OnEnterWorld() override;       ///< Entity进入世界时，此方法被调用
	virtual void OnLeaveWorld() override;       ///< 当Entity离开世界（被销毁时）时，此方法被调用
	virtual void OnEnterScenes()override;		///<开始场景

	virtual void Set_Position(const FVector &oldVal) override;///<设置位置
	virtual void Set_Direction(const FVector &oldVal) override;///<设置朝向
	virtual void OnUpdateVolatileData() override;

	virtual void BindActorToEntity(AServerCharacter* InServerCharacter);
	virtual void PostBindActorToEntity();///<绑定Actor到Entity之后的操作
	virtual void UnBindActorToEntity();///<将Actor和对应的Entity解除绑定

	virtual void BindActorToInterface();
	virtual void UnBindActorToInterface();
	virtual void SpwawnServerCharacter();///<生成Actor
	virtual void PostSpwawnServerCharacter();///<生成Actor

	virtual UClass* GetEntityModelClassAsset();///< 获取实体模型的类
};


