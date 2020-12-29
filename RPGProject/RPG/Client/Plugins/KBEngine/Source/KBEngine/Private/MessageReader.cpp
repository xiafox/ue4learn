#include "MessageReader.h"
#include "KBEnginePrivatePCH.h"
#include "MessagesHandler.h"
#include "KBEngineArgs.h"

DECLARE_CYCLE_STAT(TEXT("Process"), STAT_Process, STATGROUP_KBEngine_MessageReader);
DECLARE_CYCLE_STAT(TEXT("Process_RecvQueue"), STAT_Process_RecvQueue, STATGROUP_KBEngine_MessageReader);
DECLARE_CYCLE_STAT(TEXT("Process_MessagesAnalysis"), STAT_Process_MessagesAnalysis, STATGROUP_KBEngine_MessageReader);
DECLARE_CYCLE_STAT(TEXT("Process_MessagesHandler"), STAT_Process_MessagesHandler, STATGROUP_KBEngine_MessageReader);

namespace KBEngine
{

	MessageReader::MessageReader(MessagesHandler* handler, Messages *messages, uint32 SocketRecvQueueSize) :
		SocketRecvQueue(SocketRecvQueueSize),
		messagesHandler_(handler),
		messages_(messages)
	{
		KBE_ASSERT(messages_);
		KBE_ASSERT(messagesHandler_);
	}


	MessageReader::~MessageReader()
	{
		
	}

	void MessageReader::Reset()
	{
		stream.Clear();
		SocketRecvQueue.Empty();
	}

	void MessageReader::Process()
	{
		SCOPE_CYCLE_COUNTER(STAT_Process);
		const double StartTime = FPlatformTime::Seconds();
		bool isFrameMaxProcess = false;

		while (true)
		{
			if (expectBuff.Num() == expectSize)
			{
				SCOPE_CYCLE_COUNTER(STAT_Process_MessagesAnalysis);
				//开始处理预期数据
				{
					if (state == READ_STATE::READ_STATE_MSGID)
					{
						stream.Append(expectBuff.GetData(), expectSize);
						msgid = stream.ReadUint16();
						stream.Clear();

						auto* msg = messages_->GetClientMessage(msgid);
						if (!msg)
						{
							KBE_ERROR(TEXT("MessageReader::Process_: unknown message(%d)!"), msgid);
							KBE_ASSERT(msg);
						}

						if (msg->MsgLen() == -1)
						{
							state = READ_STATE::READ_STATE_MSGLEN;
							expectSize = 2;
						}
						else if (msg->MsgLen() == 0)  // 如果是0个参数的消息，那么没有后续内容可读了，处理本条消息并且直接跳到下一条消息
						{
							{
								SCOPE_CYCLE_COUNTER(STAT_Process_MessagesHandler);
								msg->HandleMessage(&stream, messagesHandler_);
							}
							state = READ_STATE::READ_STATE_MSGID;
							expectSize = 2;
						}
						else
						{
							expectSize = msg->MsgLen();
							state = READ_STATE::READ_STATE_BODY;
						}
					}
					else if (state == READ_STATE::READ_STATE_MSGLEN)
					{
						stream.Append(expectBuff.GetData(), expectSize);

						msglen = stream.ReadUint16();
						stream.Clear();

						// 长度扩展
						if (msglen >= 65535)
						{
							state = READ_STATE::READ_STATE_MSGLEN_EX;
							expectSize = 4;
						}
						else
						{
							state = READ_STATE::READ_STATE_BODY;
							expectSize = msglen;
						}
					}
					else if (state == READ_STATE::READ_STATE_MSGLEN_EX)
					{
						stream.Append(expectBuff.GetData(), expectSize);

						expectSize = stream.ReadUint32();
						stream.Clear();

						state = READ_STATE::READ_STATE_BODY;
					}
					else if (state == READ_STATE::READ_STATE_BODY)
					{
						stream.Append(expectBuff.GetData(), expectSize);

						auto* msg = messages_->GetClientMessage(msgid);
						if (!msg)
						{
							KBE_ERROR(TEXT("MessageReader::Process_: unknown message(%d)!"), msgid);
							KBE_ASSERT(msg);
						}

						{
							SCOPE_CYCLE_COUNTER(STAT_Process_MessagesHandler);
							msg->HandleMessage(&stream, messagesHandler_);
						}

						stream.Clear();

						state = READ_STATE::READ_STATE_MSGID;
						expectSize = 2;
					}
				}
				//处理完预期数据
				expectBuff.Reset(expectSize);
			}
			else //预期数据还不够,继续读取缓冲区
			{
				SCOPE_CYCLE_COUNTER(STAT_Process_RecvQueue);
				if (nowProcessPacket.PacketBytes.Num() == nowProcessPacket.NextReadIndex)
				{
					if (!SocketRecvQueue.Dequeue(nowProcessPacket))
					{
						break;
					}
				}

				if (nowProcessPacket.PacketBytes.Num() != 0)
				{
					uint32 SurplusPacketBytes = nowProcessPacket.PacketBytes.Num() - nowProcessPacket.NextReadIndex;
					int32 nowExpectBuffNum = expectBuff.Num();
					uint32 ReadSize = FMath::Min(expectSize - nowExpectBuffNum, SurplusPacketBytes);
					expectBuff.AddUninitialized(ReadSize);
					FMemory::Memcpy(&expectBuff[nowExpectBuffNum], &nowProcessPacket.PacketBytes[nowProcessPacket.NextReadIndex], ReadSize);
					nowProcessPacket.NextReadIndex += ReadSize;
				}
			}

			//分帧处理
			const double ElapsedTime = FPlatformTime::Seconds() - StartTime;
			if (NowFrameMaxProcessTime && ElapsedTime >= NowFrameMaxProcessTime)
			{
				NowFrameMaxProcessTime += 0.0001;  //防止消息一直太多处理不过来
				isFrameMaxProcess = true;
				break;
			}
		}

		if (!isFrameMaxProcess)
		{
			NowFrameMaxProcessTime = FMath::Max(NowFrameMaxProcessTime - 0.0005, FrameMaxProcessTime);  //消息不堆积了,逐渐还原每帧最大处理时间
		}
	}

}