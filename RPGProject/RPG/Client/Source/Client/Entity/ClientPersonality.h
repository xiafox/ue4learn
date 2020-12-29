#pragma once
#include "KBEngine.h"
/*
 KBEngine回调接口
*/
class ClientPersonality : public KBEngine::KBEPersonality
{
public:
	/* 当某个space data被改变或设置时，此方法被触发 */
	virtual void OnSetSpaceData(uint32 spaceID, const FString &key, const FString &value)override;

	/* 当某个space data被删除时，此方法被触发 */
	virtual void OnDelSpaceData(uint32 spaceID, const FString &key)override;

	/* 当服务器中的Space加载了地图的导航数据时，此方法被触发 */
	virtual void OnAddSpaceGeometryMapping(uint32 spaceID, const FString &respath)override;

	/* 当登录失败时，此方法被触发 */
	virtual void OnLoginFailed(int32 errCode, const FString& errName, const FString& errDesc, const TArray<uint8>& serverDatas)override;

	/* 当与服务器的连接断开时，此方法被触发 */
	virtual void OnDisconnect()override;




	/* 当成功通过KBEngineApp::ReLoginBaseapp()修改密码后，此方法被触发*/
	virtual void OnReLoginBaseapp(int32 errCode, const FString& errName, const FString& errDesc)override;

	/* 当成功通过KBEngineApp::NewPassword()修改密码后，此方法被触发*/
	virtual void OnNewPassword(int32 errCode, const FString& errName, const FString& errDesc)override;

	/* 当成功通过KBEngineApp::BindAccountEmail()修改密码后，此方法被触发*/
	virtual void OnBindAccountEmail(int32 errCode, const FString& errName, const FString& errDesc)override;

	/* 当成功通过KBEngineApp::CreateAccount()创建了账号以后，此方法被触发 */
	virtual void OnCreateAccountSuccess(const FString& account)override;

	/* 当成功通过KBEngineApp::ResetPassword()重置账号密码后，此方法被触发 */
	virtual void OnResetPasswordSuccess(const FString& account)override;

};
