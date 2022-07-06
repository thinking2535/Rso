#pragma once

#include "Base.h"
#include <Rso/Net/Net.h>

namespace rso::mobile
{
	using namespace net;

	class CNet
	{
	protected:
		struct SPeerExt
		{
			CKey Key;
			CKey NetKey;
			TTime CloseTime;
			TProtoSeq ProtoSeqMustRecv = 0;
			TProtoSeq ProtoSeqFirstForSendProtos = 0; // SendProtos 의 첫번째 프로토콜 번호
			TSendProtos SendProtos;

			SPeerExt(const CKey& Key_) :
				Key(Key_)
			{
			}
			SPeerExt(const CKey& Key_, const CKey& NetKey_) :
				Key(Key_), NetKey(NetKey_)
			{
			}
			template<typename... TTypes>
			void Send(const TTypes&... Args_)
			{
				SendProtos.emplace()->Send(Args_...);
			}
			void RecvAck(void)
			{
				auto it = SendProtos.begin();
				it->clear();
				SendProtos.erase(it);
				++ProtoSeqFirstForSendProtos;
			}
			inline bool DoesWillClose(void) const
			{
				return (CloseTime.time_since_epoch().count() != 0);
			}
			inline void WillClose(const milliseconds& WaitMilliseconds_)
			{
				CloseTime = system_clock::now() + WaitMilliseconds_;
			}
		};

		TPeerCnt _PeerCounter = 0;
		CPeriod<milliseconds> _PeriodUnLinked{ milliseconds(1000) };
		milliseconds _KeepConnectDuration;
		TLinkFunc _LinkFunc;
		TUnLinkFunc _UnLinkFunc;
		TRecvFunc _RecvFunc;

	public:
		CNet(TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_, const milliseconds& KeepConnectDuration_);
		virtual ~CNet() {}
	};
}