#pragma once
#include "KBEnginePrivatePCH.h"
#include "MessageReader.h"

namespace KBEngine
{
	class MessageReader;
	class NetworkInterface;

	class PacketReceiver : public FRunnable
	{

	public:
		PacketReceiver(NetworkInterface* networkInterface, uint32 buffLength = 65535);
		~PacketReceiver();

		void StartBackgroundRecv();
		void WillClose() { willClose_ = true; }

	public:
		// for FRunnable
		virtual uint32 Run() override;

	private:
		// 由于阻塞在socket中，所以这个接口可能会导致卡机，外部非测试理由别用
		void StopBackgroundRecv();

	private:
		NetworkInterface* networkInterface_ = NULL;

		FRunnableThread* thread_ = nullptr;
		bool breakThread_ = false;

		// 由NetworkInterface关闭网络时通知，
		// 以避免在主动关闭网络时也发出错误信息
		bool willClose_ = false;
	};

}