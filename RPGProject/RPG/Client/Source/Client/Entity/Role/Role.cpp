
#include "Role.h"
#include "ClientGameInstance.h"
#include "GameFramework/Actor.h"
#include "Character/Player/PlayerCharacter.h"

KBE_BEGIN_ENTITY_METHOD_MAP(Role, Supper)
DECLARE_REMOTE_METHOD(CLIENT_OnRepJumpState, &Role::CLIENT_OnRepJumpState, uint8, FVector, FVector)
KBE_END_ENTITY_METHOD_MAP()

KBE_BEGIN_ENTITY_PROPERTY_MAP(Role, Supper)
KBE_END_ENTITY_PROPERTY_MAP()


Role::Role()
{
}

Role::~Role()
{

}

void Role::BASE_Logoff()
{
	KBEngine::FVariantArray args;
	BaseCall(TEXT("BASE_logoff"), args);
}

void Role::BASE_Logout()
{
	KBEngine::FVariantArray args;
	BaseCall(TEXT("BASE_logout"), args);
}

void Role::CLIENT_OnRepJumpState(uint8 JumpState, FVector InVelocity, FVector InPos)
{
	AActor* actor = Actor();
	if (actor != nullptr)
	{
		APlayerCharacter* PlayerCharacter = dynamic_cast<APlayerCharacter*>(actor);
		if (PlayerCharacter != nullptr)
		{
			PlayerCharacter->OnRepJumpState(JumpState, InVelocity, InPos);
		}
	}
}

void Role::OnEnterWorld()
{
	Supper::OnEnterWorld();
	if (IsPlayer())
	{
		UClientGameInstance::Instance->LoginManager->OnPlayerEnterWorld();
	}
}

void Role::OnLeaveWorld()
{
	Supper::OnLeaveWorld();
}

