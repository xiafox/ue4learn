
#include "Account.h"
#include "ClientGameInstance.h"
#include "Manager/LoginManager.h"

KBE_BEGIN_ENTITY_METHOD_MAP(Account, Supper)
DECLARE_REMOTE_METHOD(CLIENT_OnAccountCreateSuccessed, &Account::CLIENT_OnAccountCreateSuccessed)
DECLARE_REMOTE_METHOD(CLIENT_OnQueryRoles, &Account::CLIENT_OnQueryRoles, const FVariant&)
DECLARE_REMOTE_METHOD(CLIENT_OnAddRole, &Account::CLIENT_OnAddRole, const FVariant&)
DECLARE_REMOTE_METHOD(CLIENT_OnDeleteRole, &Account::CLIENT_OnDeleteRole, uint64)
KBE_END_ENTITY_METHOD_MAP()

KBE_BEGIN_ENTITY_PROPERTY_MAP(Account, Supper)
KBE_END_ENTITY_PROPERTY_MAP()


Account::Account()
{
}

Account::~Account()
{
}

void Account::CLIENT_OnAccountCreateSuccessed()
{
	UClientGameInstance::Instance->LoginManager->OnLoginSuccessed();
}

void Account::BASE_QueryRoles()
{
	KBEngine::FVariantArray args;
	BaseCall(TEXT("BASE_queryRoles"), args);
}

void Account::CLIENT_OnQueryRoles(const FVariant& val)
{
	//参数形式{"infos":[{"dbid":...,"playerName":...},...]}
	FROLEINFO_T inst(val);
	UClientGameInstance::Instance->LoginManager->OnQueryRoles(inst);
}

void Account::BASE_AddRole(FString playerName, int32 profession, int32 camp, int32 gender)
{
	KBEngine::FVariantArray args;
	args.Add(playerName);
	args.Add(profession);
	args.Add(camp);
	args.Add(gender);
	BaseCall(TEXT("BASE_createRole"), args);
}

void Account::CLIENT_OnAddRole(const FVariant& val)
{
	FROLEINFO roleInfo(val);
	UClientGameInstance::Instance->LoginManager->OnAddRole(roleInfo);
}

void Account::BASE_DeleteRole(uint64 dbid)
{
	KBEngine::FVariantArray args;
	args.Add(dbid);
	BaseCall(TEXT("BASE_deleteRole"), args);
}

void Account::CLIENT_OnDeleteRole(uint64 dbid)
{
	UClientGameInstance::Instance->LoginManager->OnDeleteRole(dbid);
}

void Account::BASE_EnterGame(uint64 dbid)
{
	KBEngine::FVariantArray args;
	args.Add(dbid);
	BaseCall(TEXT("BASE_enterWorld"), args);
	EntityCallEnable(false); //关闭account继续给服务器发送消息的通道
}

void Account::BASE_Logoff()
{
	KBEngine::FVariantArray args;
	BaseCall(TEXT("BASE_logoff"), args);
}

