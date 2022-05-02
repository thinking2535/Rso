#pragma once

#include "Base.h"
#include <Rso/Net/Server.h>
#include <map>

namespace rso::monitor
{
	using namespace net;
	using namespace stdext;

	class CServer final
	{
	private:
		using TCallbackFunc = function<void(const CKey&, CStream&)>;
		struct _SAgent
		{
			SAgentValue Agent;
			SProcValue Proc;

			_SAgent(const SAgent& Agent_) :
				Agent(Agent_)
			{
			}
		};
		using TAgents = map<CKey, _SAgent>;

		CNamePort _ClientNamePort;
		unique_ptr<net::CServer> _NetC;
		unique_ptr<net::CServer> _NetA;
		TCallbackFunc _TryLoginFunc;
		TCallbackUserProto _CallbackUserProto;
		TAgents _Agents;

		void _LinkA(const CKey& Key_);
		void _UnLinkA(const CKey& Key_, ENetRet NetRet_);
		void _RecvA(const CKey& Key_, CStream& Stream_);
		void _RecvAsAgentOn(const CKey& Key_, CStream& Stream_);
		void _RecvAsProcOn(const CKey& Key_, CStream& Stream_);
		void _RecvAsProcOff(const CKey& Key_, CStream& Stream_);
		void _RecvAsAgentOption(const CKey& Key_, CStream& Stream_);
		void _RecvAsAgentStat(const CKey& Key_, CStream& Stream_);
		void _RecvAsProcStat(const CKey& Key_, CStream& Stream_);
		void _RecvAsNotifyToClient(const CKey& Key_, CStream& Stream_);

		void _RecvC(const CKey& Key_, CStream& Stream_);
		void _RecvCsLogin(const CKey& Key_, CStream& Stream_);
		void _RecvCsToAgent(const CKey& Key_, const list<SKey>& AgentKeys_, CStream& Stream_);

	public:
		CServer(
			EAddressFamily AddressFamily_,
			TLinkFunc LinkCFunc_, TUnLinkFunc UnLinkCFunc_, TCallbackFunc TryLoginFunc_, TCallbackUserProto CallbackUserProto_,
			const CNamePort& ClientNamePort_, const CNamePort& AgentNamePort_);
	private:
		void _Certify(TPeerCnt PeerNum_);
	public:
		void Certify(TPeerCnt PeerNum_);
		void Certify(const CKey& Key_);
		inline void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
		{
			_NetC->WillClose(PeerNum_, WaitMilliseconds_);
		}
		inline void WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
		{
			_NetC->WillClose(Key_, WaitMilliseconds_);
		}
		void Proc(void);
	};
}
