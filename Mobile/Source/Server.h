#pragma once

#include "Net.h"
#include <Rso/Net/Server.h>

namespace rso::mobile
{
	// 이쪽에서 끊을 경우 Net.SendNet.WillClose() 로 ExtPeerNum, NetPeerNum 은 같이 쓸 수 없음.
	// 클라이언트가 연결될 경우 PeersNet에 먼저 추가되고, PeersExt에 나중에 추가됨
	class CServer : public CNet, public INet, public IServer
	{
		struct _SPeerWillExpire
		{
			TTime ExpireTime;
			TPeerCnt PeerExtNum = c_PeerCnt_Null;

			_SPeerWillExpire(const TTime& ExpireTime_, TPeerCnt PeerExtNum_) :
				ExpireTime(ExpireTime_), PeerExtNum(PeerExtNum_)
			{
			}
		};
		using _TPeersWillExpire = CList<_SPeerWillExpire>;
		using _TPeersWillExpireIt = typename _TPeersWillExpire::iterator;
		struct _SPeerExt : public SPeerExt
		{
			CNamePort NamePort;
			_TPeersWillExpireIt itPeerWillExpire;
			bool Certified = false;

			_SPeerExt(const CKey& Key_, const CKey& NetKey_, const CNamePort& NamePort_) :
				SPeerExt(Key_, NetKey_), NamePort(NamePort_)
			{
			}
		};
		using _TPeersExt = CList<_SPeerExt>;
		using _TPeersExtIt = typename _TPeersExt::iterator;
		struct _SPeerNet
		{
			CKey ExtKey;
		};
		using _TPeersNet = CList<_SPeerNet>;
		using _TPeersNetIt = typename _TPeersNet::iterator;

		net::CServer _Net;
		_TPeersNet _PeersNet;
		_TPeersExt _PeersExt;
		_TPeersWillExpire _PeersWillExpire;

		void _Close(_TPeersExtIt itPeerExt_, ENetRet NetRet_);
		void _Close(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _Close(_TPeersExtIt itPeerExt_);
		void _Link(const CKey& Key_);
		void _UnLink(const CKey& Key_, ENetRet NetRet_);
		void _Recv(const CKey& Key_, CStream& Stream_);
		void _RecvAck(_TPeersExtIt itPeerExt_);
		void _RecvLink(const CKey& NetKey_, _TPeersNetIt itPeerNet_, CStream& Stream_);
		void _RecvReLink(const CKey& NetKey_, _TPeersNetIt itPeerNet_, CStream& Stream_);
		void _RecvUnLink(const CKey& NetKey_, CStream& Stream_);
		void _RecvUserProto(_TPeersExtIt itPeerExt_, CStream& Stream_);
		void _SendUnLink(TPeerCnt NetPeerNum_);

	public:
		CServer(
			EAddressFamily AddressFamily_,
			TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
			bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
			milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, TSize MaxRcvBPS_,
			const CNamePort& BindInfo_, int BackLog_, const milliseconds& KeepConnectDuration_);
		inline bool IsLinked(TPeerCnt PeerNum_) const
		{
			return bool(_PeersExt.get(PeerNum_));
		}
		inline const CNamePort& GetNamePort(TPeerCnt PeerNum_) const
		{
			return _PeersExt[PeerNum_].NamePort;
		}
		inline void Close(TPeerCnt PeerNum_)
		{
			_Close(_PeersExt.get(PeerNum_));
		}
		bool Close(const CKey& Key_);
		void CloseAll(void);
		void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_);
		bool WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_);
		inline TPeerCnt GetPeerCnt(void) const
		{
			return TPeerCnt(_PeersExt.size());
		}
		inline milliseconds Latency(TPeerCnt PeerNum_) const
		{
			return _Net.Latency(_PeersExt[PeerNum_].NetKey.PeerNum);
		}
		void Proc(void);
		void Certify(TPeerCnt PeerNum_);
		bool Certify(const CKey& Key_);
		inline bool IsCertified(TPeerCnt PeerNum_) const
		{
			return _PeersExt[PeerNum_].Certified;
		}
		template<typename... TTypes>
		void Send(TPeerCnt PeerNum_, const TTypes&... Args_)
		{
			auto itPeerExt = _PeersExt.get(PeerNum_);

			if (itPeerExt->DoesWillClose())
				return;

			try
			{
				itPeerExt->Send(Args_...);
			}
			catch (...)
			{
				itPeerExt->WillClose(milliseconds(0));
				return;
			}

			if (itPeerExt->NetKey)
				_Net.Send(itPeerExt->NetKey.PeerNum, SHeaderSc(EProtoSc::UserProto), Args_...);
		}
		template<typename... TTypes>
		void Send(const CKey& Key_, const TTypes&... Args_)
		{
			auto itPeerExt = _PeersExt.get(Key_.PeerNum);
			if (!itPeerExt)
				return;

			if (itPeerExt->Key.PeerCounter != Key_.PeerCounter)
				return;

			if (itPeerExt->DoesWillClose())
				return;

			try
			{
				itPeerExt->Send(Args_...);
			}
			catch (...)
			{
				itPeerExt->WillClose(milliseconds(0));
				return;
			}

			if (itPeerExt->NetKey)
				_Net.Send(itPeerExt->NetKey.PeerNum, SHeaderSc(EProtoSc::UserProto), Args_...);
		}
		template<typename... TTypes>
		void SendAll(const TTypes&... Args_)
		{
			for (auto& i : _PeersExt)
			{
				if (i.DoesWillClose())
					continue;

				try
				{
					i.Send(Args_...);
				}
				catch (...)
				{
					i.WillClose(milliseconds(0));
					continue;
				}

				if (i.NetKey)
					_Net.Send(i.NetKey.PeerNum, SHeaderSc(EProtoSc::UserProto), Args_...);
			}
		}
		template<typename... TTypes>
		void SendAllExcept(const CKey& Key_, const TTypes&... Args_)
		{
			for (auto& i : _PeersExt)
			{
				if (i.DoesWillClose())
					continue;

				if (i.Key == Key_)
					continue;

				try
				{
					i.Send(Args_...);
				}
				catch (...)
				{
					i.WillClose(milliseconds(0));
					continue;
				}

				if (i.NetKey)
					_Net.Send(i.NetKey.PeerNum, SHeaderSc(EProtoSc::UserProto), Args_...);
			}
		}
	};
}