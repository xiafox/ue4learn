
#include "EntityDeclare.h"
#include "KBEngine.h"
#include "Entity/Account/Account.h"
#include "Entity/Role/Role.h"

void EntityDeclare()
{
	ENTITY_DECLARE(TEXT("Account"), Account);
	ENTITY_DECLARE(TEXT("Role"), Role);
}
