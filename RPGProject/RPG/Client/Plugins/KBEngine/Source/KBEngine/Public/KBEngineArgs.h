#pragma once

#include "KBEDefine.h"

// ÿ֡�����ʱ��  MessageReader::Process()
// FrameMaxProcessTime == 0.0  ����֡����
#define FrameMaxProcessTime 0.004

namespace KBEngine
{
	class KBENGINE_API KBEngineArgs
	{
	public:
		// ��¼ip�Ͷ˿�
		FString host = "127.0.0.1";
		uint16 port = 20013;

		// �ͻ�������
		// Reference: http://www.kbengine.org/docs/programming/clientsdkprogramming.html, client types
		CLIENT_TYPE clientType = CLIENT_TYPE::CLIENT_TYPE_MINI;

		// �־û������Ϣ�� ���磺�ӷ���˵����Э����Գ־û������أ��´ε�¼�汾�������ı�
		// ����ֱ�Ӵӱ��ؼ������ṩ��¼�ٶ�
		FString persistentDataPath = TEXT("Application.PersistentDataPath");

		// Allow synchronization role position information to the server
		// �Ƿ����Զ�ͬ�������Ϣ������ˣ���Ϣ����λ���뷽��
		// �Ǹ�ʵʱ����Ϸ����Ҫ�������ѡ��
		bool syncPlayer = true;

		// �Ƿ�ʹ�ñ�������
		// ���������ѡ�������kbengine_defs.xml::cellapp/aliasEntityID�Ĳ�������һ��
		bool useAliasEntityID = true;

		// ��Entity��ʼ��ʱ�Ƿ񴥷����Ե�set_*�¼�(callPropertysSetMethods)
		bool isOnInitCallPropertysSetMethods = true;

		// ���ͻ����С
		uint32 SEND_BUFFER_MAX = 65535;

		// ���ջ�������С
		uint32 RECV_BUFFER_MAX = 327675;

		// ���������ͼ�������Խ�̣���Ϊ���ߵ�ʱ��ҲԽ�̣�����λ����
		// ���Ϊ0���򲻷���
		// ע�⣺��ֵ����С��kbengine_defs.xml��kbengine.xml��<channelCommon><timeout><external>�����õĳ�ʱ����
		int32 tickInterval = 55;

		// ͨ��Э����ܣ�blowfishЭ��
		TArray<uint8> encryptedKey;

		// �������ͻ��˵İ汾���Լ�Э��MD5
		FString clientVersion = "1.2.2";
		FString clientScriptVersion = "0.1.0";
	};
}