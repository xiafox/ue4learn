#include "KBEPersonality.h"
#include "KBEnginePrivatePCH.h"
#include "KBEDebug.h"

namespace KBEngine
{
	KBEPersonality* KBEPersonality::instance_ = nullptr;

	void KBEPersonality::Register(KBEPersonality * inst)
	{
		KBE_ASSERT(instance_ == nullptr);
		instance_ = inst;
	}

	void KBEPersonality::Deregister()
	{
		if (instance_)
			delete instance_;
			instance_ = nullptr;
	}
}
