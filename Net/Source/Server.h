#pragma once

#include "Net.h"
#include "BinderServer.h"
#include <Rso/Log/Log.h>

namespace rso::net
{
	using namespace log;

	class CServer : public CNet, public IServer
	{
		struct _SAccept
		{
			CSocket Socket;
			TIPEndPoint IPEndPoint;

			_SAccept(CSocket&& Socket_, TIPEndPoint&& IPEndPoint_) :
				Socket(std::move(Socket_)), IPEndPoint(std::move(IPEndPoint_))
			{
			}
		};

		CLFQueue<_SAccept> _Accepts;
		unique_ptr<CThread> _AcceptorThread;
		CBinderServer _Binder;

		void _Acceptor(const volatile bool* Exit_);

	public:
		CServer(
			EAddressFamily AddressFamily_,
			TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
			bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
			milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, TSize MaxRcvBPS_,
			const CNamePort& BindInfo_, int BackLog_);
		inline CNamePort GetBindNamePort(void) const
		{
			return _Binder.GetNamePort();
		}
		inline void Certify(TPeerCnt PeerNum_)
		{
			_Peers[PeerNum_].Certify();
		}
		bool Certify(const CKey& Key_);
		inline bool IsCertified(TPeerCnt PeerNum_) const
		{
			return _Peers[PeerNum_].IsCertified();
		}
		void Proc(void);
	};
}