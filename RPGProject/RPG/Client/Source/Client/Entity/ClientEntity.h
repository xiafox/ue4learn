#pragma once

#include "Entity.h"
#include "EntityDeclareDefine.h"

class UEntityInterfaceObject;
/**
*Entity基类
*/
class ClientEntity : public KBEngine::Entity
{
	KBE_DECLARE_ENTITY_MAP();
	typedef Entity Super;

public:
	ClientEntity();

	~ClientEntity();
	/**
	*初始化参数
	*
	*@param scriptModule ，脚本组件（ScriptModule &）
	*
	*@return 无
	*/
	virtual void InitProperties(KBEngine::ScriptModule& scriptModule)override;

	virtual void OnEnterScenes();///<开始场景
	virtual void OnEnterWorld()override;///<当Entity进入世界时，此方法被调用
	virtual void OnLeaveWorld()override;///<当Entity离开世界（被销毁时）时，此方法被调用
	virtual void OnDestroy()override;

	void RegisterRemoteMethod();		/// 注册服务器通知的客户端远程方法
	void RegisterUpdateProperty();		/// 注册服务器更新的客户端属性

	virtual void OnRemoteMethodCall(const FString &name, const TArray<FVariant> &args) override;
	virtual void OnUpdateProperty(const FString &name, const FVariant &newVal, const FVariant &oldVal) override;

	/**
	*将Interface和对应的Entity解除绑定
	*
	*@return 无
	*/
	virtual void UnBindInterfaceToEntity();
	template<class T>
	T* GetInterfaceObject(const FString &name);///<获取接口对象

	bool IsPlayerRole(); ///<是否是玩家自己
	bool IsPlayerAccount(); ///<是否是当前客户端账号

public:
	TMap<FString, UEntityInterfaceObject *> InterfaceObjects;///< 接口对像字典
	TMap<FString, UEntityInterfaceObject *> InterfaceFuncObj;
	TMap<FString, UEntityInterfaceObject *>	InterfaceProObj;
};



