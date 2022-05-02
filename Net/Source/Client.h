#pragma once

#include "Net.h"

namespace rso::net
{
	class CClient : public CNet, public IClient
	{
		struct _SConnect
		{
			ENetRet NetRet = ENetRet::Null;
			CKey Key;
			CSocket Socket;
			TIPEndPoint IPEndPoint;

			_SConnect(ENetRet NetRet_, const CKey& Key_, CSocket&& Socket_, TIPEndPoint&& IPEndPoint_) :
				NetRet(NetRet_), Key(Key_), Socket(std::move(Socket_)), IPEndPoint(std::move(IPEndPoint_))
			{
			}
			_SConnect(ENetRet NetRet_, const CKey& Key_) :
				NetRet(NetRet_), Key(Key_)
			{
			}
		};
		struct _SConnectingInfo
		{
			CKey Key;
			CNamePort NamePort;
		};

		long _ConnectTimeOutSec = 0;
		TLinkFunc _LinkFunc;
		TLinkFailFunc _LinkFailFunc;
		TUnLinkFunc _UnLinkFunc;
		CList<CKey> _PeersAndConnectings;
		CLFQueue<_SConnectingInfo> _ConnectingInfos;
		CLFQueue<_SConnect> _Connects;

		CThread _ConnectorThread;

		void _LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _Link(const CKey& Key_);
		void _UnLink(const CKey& Key_, ENetRet NetRet_);
		void _Connector(const volatile bool* Exit_);
		_SConnect _Connect(const _SConnectingInfo& ConnectInfo_);

	public:
		CClient(
			TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
			bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
			milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, long ConnectTimeOutSec_);
		inline bool IsConnecting(TPeerCnt PeerNum_) const
		{
			return bool(_PeersAndConnectings.get(PeerNum_));
		}
		CKey Connect(const CNamePort& NamePort_, TPeerCnt PeerNum_);
		inline CKey Connect(const CNamePort& NamePort_)
		{
			return Connect(NamePort_, TPeerCnt(_PeersAndConnectings.new_index()));
		}
		void Close(TPeerCnt PeerNum_);
		bool Close(const CKey& Key_);
		void CloseAll(void);
		void Proc(void);
	};
}