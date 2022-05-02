#pragma once

#include <Rso/Base/List.h>
#include <Rso/Base/LFQueue.h>
#include <Rso/Base/Thread.h>
#include "WSA.h"
#include "IOCP.h"
#include "Peer.h"
#include "IPEndPointV4.h"
#include "IPEndPointV6.h"

namespace rso::net
{
	using namespace core;
	using namespace std;

	class CNet : public INet
	{
	protected:
		using _TPeers = CList<CPeer>;
		using _TPeersIt = _TPeers::iterator;
		using _TPeersCIt = _TPeers::const_iterator;

		struct _SLFIOCP
		{
			CKey Key;
			TSize TranBytes = 0;

			_SLFIOCP()
			{}
			_SLFIOCP(const CKey& Key_, TSize TranBytes_) :
				Key(Key_), TranBytes(TranBytes_)
			{}
		};
		using _TLFIOCPs = vector<CLFQueue<_SLFIOCP>>;

		size_t _HeaderSize = 0;
		TLinkFunc _LinkFunc;
		TUnLinkFunc _UnLinkFunc;
		TRecvFunc _RecvFunc;
		SNetInfo _NetInfo;
		int _RecvBuffSize = 0;
		int _SendBuffSize = 0;
		CPeriod<milliseconds> _Period{ milliseconds(4000) };
		TPeerCnt _Counter = 0;
		_TPeers _Peers;
		_TLFIOCPs _LFIOCPs;
		bool _NoDelay = false;
		CLog _Log{ Target_Console | Target_File, EPeriod::Day, L"NetLog/", L"Normal", ELocale::Korean };
		CWSA _WSA;
		vector<unique_ptr<CThread>> _WorkerThreads;
		CIOCP _IOCP;

		void _Worker(const volatile bool* pExit_, size_t ThreadNum_);
		void _ErrorLog(const wstring& Msg_);
		void _SendDone(CKey Key_, TSize Size_);
		CKey _NewKey(TPeerCnt PeerNum_);
		void _Link(const CKey& Key_, CSocket&& Socket_, TIPEndPoint&& IPEndPoint_);
		void _Link(CSocket&& Socket_, TIPEndPoint&& IPEndPoint_);
		void _Close(_TPeersIt itPeer_, ENetRet NetRet_);
		void _Recved(_TPeersIt itPeer_, TSize TranBytes_);

	public:
		CNet(
			TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
			bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
			milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, TSize MaxRcvBPS_);
		CNet(const CNet&&) = delete;
		virtual ~CNet() {}
		CNet& operator = (CNet&&) = delete;
		inline bool IsLinked(TPeerCnt PeerNum_) const
		{
			return bool(_Peers.get(PeerNum_));
		}
		inline CNamePort GetNamePort(TPeerCnt PeerNum_) const
		{
			return _Peers[PeerNum_].GetNamePort();
		}
		inline void Close(TPeerCnt PeerNum_)
		{
			_Close(_Peers.get(PeerNum_), ENetRet::UserClose);
		}
		bool Close(const CKey& Key_);
		void CloseAll(void);
		inline void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
		{
			_Peers[PeerNum_].WillClose(WaitMilliseconds_, ENetRet::UserClose);
		}
		bool WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_);
		inline TPeerCnt GetPeerCnt(void) const
		{
			return static_cast<TPeerCnt>(_Peers.size());
		}
		inline milliseconds Latency(TPeerCnt PeerNum_) const
		{
			return _Peers[PeerNum_].Latency();
		}
		void Proc(void);

		template<typename... TTypes>
		inline void Send(TPeerCnt PeerNum_, const TTypes&... Args_)
		{
			_Peers[PeerNum_].Send(Args_...);
		}
		template<typename... TTypes>
		void Send(const CKey& Key_, const TTypes&... Args_)
		{
			auto itPeer = _Peers.get(Key_.PeerNum);
			if (!itPeer)
				return;

			if (itPeer->Key().PeerCounter != Key_.PeerCounter)
				return;

			itPeer->Send(Args_...);
		}
		template<typename... TTypes>
		void SendAll(const TTypes&... Args_)
		{
			for (auto& i : _Peers)
				i.Send(Args_...);
		}
		template<typename... TTypes>
		void SendAllExcept(TPeerCnt PeerNum_, const TTypes&... Args_)
		{
			for (auto& i : _Peers)
			{
				if (i.Key().PeerNum == PeerNum_)
					continue;

				i.Send(Args_...);
			}
		}
		template<typename... TTypes>
		void SendAllExcept(const CKey& Key_, const TTypes&... Args_)
		{
			for (auto& i : _Peers)
			{
				if (i.Key() == Key_)
					continue;

				i.Send(Args_...);
			}
		}
	};
}