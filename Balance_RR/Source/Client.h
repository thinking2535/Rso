#pragma once

#include "Base.h"
#include "Protocol.h"
#include <Rso/Base/Base.h>
#include <Rso/Base/OptionStream.h>
#include <functional>
#include <Rso/Net/Client.h>

namespace rso::balance_rr
{
	using namespace net;

	class CClient
	{
		enum class _EState
		{
			WillConnectServer,
			Allocated,
			Max,
			Null
		};
		struct _SServer
		{
			COptionStream<CNamePort> NamePort;
			CKey Key;
			_EState State = _EState::Null;

			_SServer(const wstring& FileName_) :
				NamePort(FileName_, true)
			{
			}
		};
		CList<_SServer> _Servers;
		net::CClient _Net;
		TLinkFunc _LinkFunc;
		TLinkFailFunc _LinkFailFunc;
		TUnLinkFunc _UnLinkFunc;
		TRecvFunc _RecvFunc;

		void _Link(const CKey& Key_);
		void _LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLink(const CKey& Key_, ENetRet NetRet_);
		void _Recv(const CKey& Key_, CStream& Stream_);
		void _RecvAcServerToConnect(const CKey& Key_, CStream& Stream_);
		void _RecvScAllocated(const CKey& Key_, CStream& Stream_);

	public:
		CClient(
			TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_);
		bool IsLinked(TPeerCnt PeerNum_) const;
		inline bool IsConnecting(TPeerCnt PeerNum_) const
		{
			return bool(_Servers.get(PeerNum_));
		}
		inline void Close(TPeerCnt PeerNum_)
		{
			_Net.Close(PeerNum_);
		}
		inline void Close(const CKey& Key_)
		{
			_Net.Close(Key_);
		}
		inline void CloseAll(void)
		{
			_Net.CloseAll();
		}
		inline milliseconds Latency(TPeerCnt PeerNum_) const
		{
			return _Net.Latency(PeerNum_);
		}
		inline void Proc(void)
		{
			_Net.Proc();
		}
		bool Connect(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& AllocatorNamePort_);
		inline bool Connect(const wstring& DataPath_, const CNamePort& AllocatorNamePort_)
		{
			return Connect(TPeerCnt(_Servers.new_index()), DataPath_, AllocatorNamePort_);
		}
		template<typename... _TScProto>
		void Send(TPeerCnt PeerNum_, const _TScProto&... Protos_)
		{
			if (_Servers[PeerNum_].State != _EState::Allocated)
				return;

			_Net.Send(PeerNum_, EProto::CsUserProto, Protos_...);
		}
		template<typename... _TScProto>
		void Send(const CKey& Key_, const _TScProto&... Protos_)
		{
			auto itServer = _Servers.get(Key_.PeerNum);
			if (!itServer ||
				itServer->Key.PeerCounter != Key_.PeerCounter ||
				itServer->State != _EState::Allocated)
				return;

			_Net.Send(Key_.PeerNum, EProto::CsUserProto, Protos_...);
		}
		template<typename... _TScProto>
		void SendAll(const _TScProto&... Protos_)
		{
			for (auto& i : _Servers)
			{
				if (i.State != _EState::Allocated)
					continue;

				_Net.Send(i.Key.PeerNum, EProto::CsUserProto, Protos_...);
			}
		}
		template<typename... _TScProto>
		void SendAllExcept(const CKey& Key_, const _TScProto&... Protos_)
		{
			for (auto& i : _Servers)
			{
				if (i.State != _EState::Allocated)
					continue;

				if (i.Key == Key_)
					continue;

				_Net.Send(i.Key.PeerNum, EProto::CsUserProto, Protos_...);
			}
		}
		inline void ClearServer(TPeerCnt PeerNum_)
		{
			_Servers[PeerNum_].NamePort.Clear();
		}
	};
}
