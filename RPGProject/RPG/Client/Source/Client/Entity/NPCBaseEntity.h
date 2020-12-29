#pragma once

#include "EntityDeclareDefine.h"
#include "Entity/ServerGameEntity.h"


class NPCBaseEntity : public ServerGameEntity
{
	KBE_DECLARE_ENTITY_MAP();
	typedef ServerGameEntity Supper;

public:
	NPCBaseEntity();
	~NPCBaseEntity();	

};

