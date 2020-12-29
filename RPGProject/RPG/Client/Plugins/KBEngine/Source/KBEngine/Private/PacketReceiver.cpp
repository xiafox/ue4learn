#include "PacketReceiver.h"
#include "KBEnginePrivatePCH.h"
#include "NetworkInterface.h"
#include "MessageReader.h"
#include "AllowWindowsPlatformTypes.h"
#include "HideWindowsPlatformTypes.h"
#include "Containers/CircularQueue.h"

void SocketErrorLog(int id)
{
	FString errStr;

	switch (id)
	{
	case WSANOTINITIALISED: errStr = TEXT("PacketReceiver::BackgroundRecv: not initialized"); break;
	case WSASYSNOTREADY:    errStr = TEXT("PacketReceiver::BackgroundRecv: sub sys not ready"); break;
	case WSAHOST_NOT_FOUND: errStr = TEXT("PacketReceiver::BackgroundRecv: name server not found"); break;
	case WSATRY_AGAIN:      errStr = TEXT("PacketReceiver::BackgroundRecv: server fail"); break;
	case WSANO_RECOVERY:    errStr = TEXT("PacketReceiver::BackgroundRecv: no recovery"); break;
	case WSAEINPROGRESS:    errStr = TEXT("PacketReceiver::BackgroundRecv: socket blocked by other prog"); break;
	case WSANO_DATA:        errStr = TEXT("PacketReceiver::BackgroundRecv: no data record"); break;
	case WSAEINTR:          errStr = TEXT("PacketReceiver::BackgroundRecv: blocking call canciled"); break;
	case WSAEPROCLIM:       errStr = TEXT("PacketReceiver::BackgroundRecv: limit exceeded"); break;
	case WSAEFAULT:         errStr = TEXT("PacketReceiver::BackgroundRecv: lpWSAData in startup not valid."); break;
	case WSAECONNABORTED:   errStr = TEXT("PacketReceiver::BackgroundRecv: connect aborted!"); break;
	default:				errStr = FString::Printf(TEXT("PacketReceiver::OnBackgroundRecv: unknown error id: %d"), id); break;
	};

	KBE_ERROR(TEXT("%s"), *errStr);
}

namespace KBEngine
{
	PacketReceiver::PacketReceiver(NetworkInterface* networkInterface, uint32 buffLength)
	{
		networkInterface_ = networkInterface;
	}

	PacketReceiver::~PacketReceiver()
	{
		KBE_DEBUG(TEXT("PacketReceiver::~PacketReceiver()"));
		StopBackgroundRecv();
	}

	void PacketReceiver::StartBackgroundRecv()
	{
		KBE_ASSERT(!thread_);
		thread_ = FRunnableThread::Create(this, *FString::Printf(TEXT("KBEnginePacketReceiver:%p"), this));
	}

	void PacketReceiver::StopBackgroundRecv()
	{
		if (thread_)
		{
			breakThread_ = true;
			// 阻塞等待线程结束
			thread_->WaitForCompletion();
			delete thread_;
			thread_ = nullptr;
		}
	}

	uint32 PacketReceiver::Run()
	{
		while (!breakThread_ && networkInterface_ && networkInterface_->Valid() && networkInterface_->messageReader_)
		{
			FReceivedPacket IncomingPacket;

			if (networkInterface_->Socket()->Wait(ESocketWaitConditions::WaitForRead, 100))
			{
				int32 BytesRead = 0;

				IncomingPacket.PacketBytes.AddUninitialized(ReceivedPacketSize);

				bool isRecv = networkInterface_->Socket()->Recv(IncomingPacket.PacketBytes.GetData(), IncomingPacket.PacketBytes.Num(), BytesRead);
				if (isRecv && BytesRead == 0)
				{
					continue;
				}
				else if (!isRecv)
				{
#if PLATFORM_WINDOWS
					int ErrorId = GetLastError();
					
					if (!willClose_)
					{
						SocketErrorLog(ErrorId);
					}
#endif					
					breakThread_ = true;
					if (!willClose_)
					{
						networkInterface_->WillClose();
					}
					break;
				}

				IncomingPacket.PacketBytes.SetNum(FMath::Max(BytesRead, 0));

				while (true)
				{
					if (networkInterface_->messageReader_->SocketRecvQueue.IsFull())
					{
						FPlatformProcess::Sleep(0.1);

						if (!breakThread_ && networkInterface_ && networkInterface_->Valid() && networkInterface_->messageReader_)  //防止在等待时关闭连接等
						{
							continue;
						}
						else
						{
							goto break_while;
						}
					}
					networkInterface_->messageReader_->SocketRecvQueue.Enqueue(MoveTemp(IncomingPacket));
					break;
				}
				break_while:;
			}
			else
			{
#if PLATFORM_WINDOWS
				int ErrorId = GetLastError();

				if (ErrorId != ESocketErrors::SE_NO_ERROR)
				{
					if (!willClose_)
					{
						SocketErrorLog(ErrorId);
					}
					breakThread_ = true;
					if (!willClose_)
					{
						networkInterface_->WillClose();
					}
				}
#endif		
			}
		}

		return 0;
	}
}