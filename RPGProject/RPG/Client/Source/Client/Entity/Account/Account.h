#pragma once

#include "EntityDeclareDefine.h"
#include "Entity/ClientEntity.h"


class Account : public ClientEntity
{
	KBE_DECLARE_ENTITY_MAP();

	typedef ClientEntity Supper;


public:
	Account();
	~Account();

	/**
	*Define method
	*服务器客户端Account创建完成可以通信了回调通知客户端
	*
	*@return 无
	*/
	void CLIENT_OnAccountCreateSuccessed();

	/**
	*Exposed method
	*请求查询账号角色信息数据
	*
	*@return 无
	*/
	void BASE_QueryRoles();

	/**
	*Define method
	*服务器账号创建完成返回账号角色信息数据
	*
	*@param val ，角色数据列表（FVariant&）
	*
	*@return 无
	*/
	void CLIENT_OnQueryRoles(const FVariant& val);

	/**
	*Exposed method
	*请求添加角色
	*
	*@param playerName ，角色名称（FString）
	*@param profession ，角色职业（int32）
	*@param camp ，角色阵营（int32）
	*@param gender ，角色性别（int32）
	*
	*@return 无
	*/
	void BASE_AddRole(FString playerName, int32 profession, int32 camp, int32 gender);

	/**
	*Define method
	*服务器角色添加成功回调
	*
	*@param val ，角色数据（FVariant&）
	*
	*@return 无
	*/
	void CLIENT_OnAddRole(const FVariant& val);

	/**
	*Exposed method
	*请求删除角色
	*
	*@param dbid ，角色数据库唯一标识（uint64）
	*
	*@return 无
	*/
	void BASE_DeleteRole(uint64 dbid);

	/**
	*Define method
	*角色删除成功服务器回调通知
	*
	*@param dbid ，角色数据库唯一标识（uint64）
	*
	*@return 无
	*/
	void CLIENT_OnDeleteRole(uint64 dbid);///<删除角色服务器数据

	/**
	*Exposed method
	*请求进入游戏
	*
	*@param dbid ，角色数据库唯一标识（uint64）
	*
	*@return 无
	*/
	void BASE_EnterGame(uint64 dbid);

	/**
	*Exposed method
	*请求退出游戏
	*
	*
	*@return 无
	*/
	void BASE_Logoff();
};