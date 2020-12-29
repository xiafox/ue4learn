#pragma once

#include "BaseManager.h"
#include "ClientTypes/GameFrameworkDefines.h"
#include "Engine/EngineTypes.h"
#include "Entity/Alias/RoleInfo.h"
#include "LoginManager.generated.h"

/**
 *提供登陆的交互，管理游戏登陆流程，提供登陆各个阶段状态的回调。
 */
UCLASS(Blueprintable)
class ULoginManager : public UBaseManager
{
	GENERATED_BODY()
public:
	ULoginManager();
	~ULoginManager();

	static ULoginManager* GetInstance();

	/**
	*BlueprintCallable，请求账号登陆
	*
	*@param server 服务器
	*@param account 账号
	*@param password 密码
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		void Login(FString account, FString password);

	/**
	*BlueprintCallable，请求创建角色
	*
	*@param playerName 玩家名
	*@param profession 玩家职业
	*@param camp 玩家阵营
	*@param gender 玩家性别
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		void CreateRole(FString playerName, int32 profession, int32 camp, int32 gender);

	/**
	*BlueprintCallable，请求删除角色
	*
	*@param dbid 角色数据库唯一标识
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		void DeleteRole(FString dbid);

	/**
	*BlueprintCallable，请求进入游戏
	*
	*@param dbid 玩家DBID
	*
	*@return 无
	*/
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		void EnterGame(FString dbid);

	///<BlueprintCallable,客户端从游戏状态返回到登陆界面/直接退出
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		void Logoff();

	///<BlueprintCallable,客户端从游戏状态返回到角色选择
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		void Logout();

	///<BlueprintCallable,获取账号里面所有角色数据信息
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		TArray<FROLEINFO> GetRoleInfos();

	///<BlueprintCallable,获取账号里面某个DBID角色的数据信息,没找到则返回false
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		bool GetRoleInfoByDBid(const FString& dbid, FROLEINFO& OutRoleInfo);

	///<BlueprintCallable，进入创建角色
	UFUNCTION(BlueprintCallable, Category = "LoginManager")
		void EnterCreateRole();

	///<请求账号角色信息数据回调
	void OnQueryRoles(const FROLEINFO_T& InRoleInfos);
	///<新增角色成功回调
	void OnAddRole(const FROLEINFO& RoleInfo);
	///<删除角色成功回调
	void OnDeleteRole(const uint64& dbid);

	void OnLoginSuccessed();///<登陆成功回调
	void OnLoginFailed();///<登陆失败回调
	void OnDisconnect();///<失去服务器连接
	void OnPlayerEnterWorld();///<玩家进入世界

	///获取当前是否是登录状态
	bool GetIsLoginStatus();
	///<获取当前登陆阶段
	ELoginStage GetCurrLoginStage();

private:
	void NotifyServerLogin( FString account, FString password);///<通知服务器账号登陆
	void OnTryLoginTimeOut();///<尝试登录阶段超时
	void SetLoginStage(ELoginStage inLoginStage);///<设置当前登陆阶段
	void OnLoginStageChanged(ELoginStage newLoginStage);///<登陆阶段改变
	void SetRoleInfos(const FROLEINFO_T& RoleInfos);//保存服务器发送过来的玩家角色数据
	void AddRoleInfo(const FROLEINFO& RoleInfo);
	void DeleteRoleInfo(uint64 dbid);

private:
	TArray<FROLEINFO> roleInfoDatas;///<玩家信息列表
	ELoginStage currLoginStage = ELoginStage::NoLoginStage;
	FTimerHandle TryLoginHander;///<尝试登录阶段定时器
};