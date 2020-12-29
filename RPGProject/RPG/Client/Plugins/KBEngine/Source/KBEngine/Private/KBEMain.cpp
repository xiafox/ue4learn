// Fill out your copyright notice in the Description page of Project Settings.

#include "KBEMain.h"
#include "KBEnginePrivatePCH.h"
#include "KBEngineArgs.h"
#include "Event.h"

FKBEMain* FKBEMain::instance = nullptr;

// Sets default values
FKBEMain::FKBEMain(KBEngine::KBEPersonality* Imp)
{
	InitKBEngine();

	KBEngine::KBEPersonality::Register(Imp);
}

FKBEMain::~FKBEMain()
{
	if (pKBEApp)
	{
		delete pKBEApp;
		pKBEApp = nullptr;
	}
	if (KBEngine::KBEPersonality::Instance())
	{
		KBEngine::KBEPersonality::Deregister();
	}

	if (FKBEMain::instance)
	{
		FKBEMain::instance = nullptr;
	}
}


// Called every frame
void FKBEMain::Tick( float DeltaTime )
{
	KBEUpdate();
}

void FKBEMain::KBEUpdate()
{
	if (pKBEApp)
		pKBEApp->Process();

	KBEngine::KBEEvent::Instance()->ProcessAsyncEvents();
}

void FKBEMain::InitKBEngine()
{
	KBEngine::KBEngineArgs *args = new KBEngine::KBEngineArgs();

	args->host = host;
	args->port = (uint16)port;
	args->clientType = clientType;

	args->persistentDataPath = persistentDataPath;

	args->syncPlayer = syncPlayer;
	args->useAliasEntityID = useAliasEntityID;
	args->isOnInitCallPropertysSetMethods = isOnInitCallPropertysSetMethods;

	args->SEND_BUFFER_MAX = sendBufferMax;
	args->RECV_BUFFER_MAX = recvBufferMax;

	pKBEApp = new KBEngine::KBEngineApp(args);

	//KBEngine::Event::useSyncMode(!isMultiThreads);
}

void FKBEMain::SetIpAddr(const FString& Host, const int32& Port)
{
	if (host.Equals(Host) && port == Port)return;

	host = Host;
	port = Port;

	if (pKBEApp != nullptr)
	{
		delete pKBEApp;
		pKBEApp = nullptr;
	}

	KBEngine::KBEngineArgs *args = new KBEngine::KBEngineArgs();

	args->host = host;
	args->port = (uint16)port;
	args->clientType = clientType;

	args->persistentDataPath = persistentDataPath;

	args->syncPlayer = syncPlayer;
	args->useAliasEntityID = useAliasEntityID;
	args->isOnInitCallPropertysSetMethods = isOnInitCallPropertysSetMethods;

	args->SEND_BUFFER_MAX = sendBufferMax;
	args->RECV_BUFFER_MAX = recvBufferMax;

	pKBEApp = new KBEngine::KBEngineApp(args);
}

void FKBEMain::Login(const FString& Account, const FString& PassWords, const TArray<uint8>& Datas)
{
	if (pKBEApp)
	{
		pKBEApp->Login(Account, PassWords, Datas);
	}
}

void FKBEMain::CreateAccount(const FString& username, const FString& password, const TArray<uint8>& datas)
{
	if (pKBEApp)
	{
		pKBEApp->CreateAccount(username, password, datas);
	}
}

void FKBEMain::ResetPassword(const FString& username)
{
	if (pKBEApp)
	{
		pKBEApp->ResetPassword(username);
	}
}

void FKBEMain::ReLoginBaseapp()
{
	if (pKBEApp)
	{
		pKBEApp->ReLoginBaseapp();
	}
}

void FKBEMain::BindAccountEmail(const FString& emailAddress)
{
	if (pKBEApp)
	{
		pKBEApp->BindAccountEmail(emailAddress);
	}
}

void FKBEMain::NewPassword(const FString old_password, const FString new_password)
{
	if (pKBEApp)
	{
		pKBEApp->NewPassword(old_password, new_password);
	}
}

KBEngine::Entity* FKBEMain::FindEntity(int32 entityID)
{
	if (pKBEApp)
	{
		return pKBEApp->FindEntity(entityID);
	}
	return nullptr;
}

const TMap<int32, KBEngine::Entity*>* FKBEMain::GetAllEntities()
{
	if (pKBEApp)
	{
		return pKBEApp->Entities();
	}
	return nullptr;
}

