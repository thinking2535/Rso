#pragma once

#include "Net.h"
#include <Rso/Net/Client.h>

namespace rso::mobile
{
	// 서버에 연결 시도할 경우 PeersExt에 먼저 추가되고, PeersNet에 나중에 추가됨.
	class CClient : public CNet, public INet, public IClient
	{
		struct _SPeerWillExpire
		{
			TTime ExpireTime;
			TPeerCnt PeerExtNum = c_PeerCnt_Null;
			bool NeedToConnect = false; // _LinkFail _UnLink 에서 바로 Connect하지 않고, NeedToConnect 변수를 사용하요 Proc() 에서 Connect하는 이유는
										// _LinkFail 의 경우 메모리 부족등의 이유로 호출된 경우 거기서 재 Connect하면 또다시 _LinkFail 가 호출되어 재귀호출 될 가능성 있음.

			_SPeerWillExpire(const TTime& ExpireTime_, TPeerCnt PeerExtNum_) :
				ExpireTime(ExpireTime_), PeerExtNum(PeerExtNum_)
			{
			}
		};
		using _TPeersWillExpire = CList<_SPeerWillExpire>;
		using _TPeersWillExpireIt = typename _TPeersWillExpire::iterator;
		struct _SPeerExt : public SPeerExt
		{
			_TPeersWillExpireIt itPeerWillExpire; // end() 이면 서버와 Mobile 연결됨.(IsLinked == true)
			CNamePort NamePort;
			CKey ServerExtKey;
			bool HaveBeenLinked = false; // 한번이라도 IsLinked 가 true 인 적이 있었는가? (한번 IsLinked 가 true가 되면 _SPeerExt가 제거되기 전까지 연결된 상태이므로 외부에서의 Linked)

			_SPeerExt(const CKey& Key_, const CNamePort& NamePort_) :
				SPeerExt(Key_), NamePort(NamePort_)
			{
			}
		};
		using _TPeersExt = CList<_SPeerExt>;
		using _TPeersExtIt = typename _TPeersExt::iterator;
		struct _SPeerNet
		{
			CKey ExtKey;

			_SPeerNet(const CKey& ExtKey_) :
				ExtKey(ExtKey_)
			{
			}
		};
		using _TPeersNet = CList<_SPeerNet>;
		using _TPeersNetIt = typename _TPeersNet::iterator;

		TLinkFailFunc _LinkFailFunc;
		TLinkFunc _LinkFuncSoft;
		TUnLinkFunc _UnLinkFuncSoft;
		net::CClient _Net;
		_TPeersNet _PeersNet;
		_TPeersExt _PeersExt;
		_TPeersWillExpire _PeersWillExpire;

		void _Close(_TPeersExtIt itPeerExt_, ENetRet NetRet_);
		void _Close(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _Close(_TPeersExtIt itPeerExt_);
		void _Link(const CKey& Key_);
		void _LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLink(const CKey& Key_, ENetRet NetRet_);
		void _Recv(const CKey& Key_, CStream& Stream_);
		void _RecvAck(_TPeersExtIt itPeerExt_);
		void _RecvLink(_TPeersNetIt itPeerNet_, _TPeersExtIt itPeerExt_, CStream& Stream_);
		void _RecvReLink(const CKey& NetKey_, _TPeersNetIt itPeerNet_, _TPeersExtIt itPeerExt_, CStream& Stream_);
		void _RecvUnLink(const CKey& NetKey_, CStream& Stream_);
		void _RecvUserProto(_TPeersExtIt itPeerExt_, CStream& Stream_);
		void _SendUnLink(TPeerCnt NetPeerNum_);

	public:
		CClient(
			TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
			bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
			milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, long ConnectTimeOutSec_,
			TLinkFunc LinkFuncSoft_, TUnLinkFunc UnLinkFuncSoft_, const milliseconds& KeepConnectDuration_);
		bool IsLinked(TPeerCnt PeerNum_) const;
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
		inline bool IsConnecting(TPeerCnt PeerNum_) const
		{
			return bool(_PeersExt.get(PeerNum_));
		}
	private:
		bool _Connect(_TPeersExtIt itPeerExt_);
	public:
		CKey Connect(const CNamePort& NamePort_, TPeerCnt PeerNum_);
		CKey Connect(const CNamePort& NamePort_);
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

			if (itPeerExt->itPeerWillExpire == _PeersWillExpire.end())
				_Net.Send(itPeerExt->NetKey.PeerNum, SHeaderCs(EProtoCs::UserProto), Args_...);
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

			if (itPeerExt->itPeerWillExpire == _PeersWillExpire.end())
				_Net.Send(itPeerExt->NetKey.PeerNum, SHeaderCs(EProtoCs::UserProto), Args_...);
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

				if (i.itPeerWillExpire == _PeersWillExpire.end())
					_Net.Send(i.NetKey.PeerNum, SHeaderCs(EProtoCs::UserProto), Args_...);
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

				if (i.itPeerWillExpire == _PeersWillExpire.end())
					_Net.Send(i.NetKey.PeerNum, SHeaderCs(EProtoCs::UserProto), Args_...);
			}
		}
	};
}