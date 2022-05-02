#pragma once

#include "Base.h"
#include <Rso/Base/OptionStream.h>
#include <Rso/Net/Client.h>

namespace rso
{
	using namespace base;
	using namespace net;

	namespace balance
	{
		class CClient : public INet
		{
			struct _SServer
			{
				TPeerCnt PeerCounter = 0;
				CNamePort PrevConnectNamePort;
				CList<CNamePort> ConnectNamePorts;
				COptionStream<CNamePort> ParentNamePort; // 접속 실패하더라도 Clear하지 않고 성공하면 갱신. (유저의 네트워크 상태, 유저 강종 등에 의해 접속 실패 할 수 있기 때문)
				bool Logon = false;

				_SServer(TPeerCnt PeerCounter_, const CNamePort& MasterNamePort_, const wstring& DataFile_) :
					PeerCounter(PeerCounter_), ParentNamePort(DataFile_, true)
				{
					ConnectNamePorts.emplace(MasterNamePort_);

					if (*ParentNamePort && *ParentNamePort != MasterNamePort_)
						ConnectNamePorts.emplace(*ParentNamePort);
				}
				CNamePort PopConnectNamePort(void)
				{
					if (ConnectNamePorts.empty())
						return CNamePort();

					auto ConnectNamePort = ConnectNamePorts.back();
					ConnectNamePorts.pop_back();

					return ConnectNamePort;
				}
				void Login(void)
				{
					Logon = true;
					ParentNamePort = PrevConnectNamePort;
				}
			};
			using _TServers = CList<_SServer>;
			using _TServersIt = _TServers::iterator;

			TPeerCnt _PeerCounter = 0;
			_TServers _Servers;

			TLinkFunc _LinkFuncS;
			TLinkFailFunc _LinkFailFuncS;
			TUnLinkFunc _UnLinkFuncS;
			TRecvFunc _RecvFuncS;

			net::CClient _NetS;

			bool _Connect(_TServersIt Server_);
			inline bool _Connect(TPeerCnt PeerNum_)
			{
				return _Connect(_Servers.get(PeerNum_));
			}
			void _LinkS(const CKey& Key_);
			void _LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_);
			void _UnLinkS(const CKey& Key_, ENetRet NetRet_);
			void _RecvS(const CKey& Key_, CStream& Stream_);
			void _RecvScNewParent(const CKey& Key_, CStream& Stream_);
			void _RecvScAllocated(const CKey& Key_, CStream& Stream_);

		public:
			CClient(
				TLinkFunc LinkFuncS_, TLinkFailFunc LinkFailFuncS_, TUnLinkFunc UnLinkFuncS_, TRecvFunc RecvFuncS_);
			virtual ~CClient() {}
			bool IsLinked(TPeerCnt PeerNum_) const;
			inline void Close(TPeerCnt PeerNum_)
			{
				_NetS.Close(PeerNum_);
			}
			inline bool Close(const CKey& Key_)
			{
				return _NetS.Close(Key_);
			}
			inline void CloseAll(void)
			{
				_NetS.CloseAll();
			}
			inline void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
			{
				_NetS.WillClose(PeerNum_, WaitMilliseconds_);
			}
			inline bool WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
			{
				return _NetS.WillClose(Key_, WaitMilliseconds_);
			}
			inline TPeerCnt GetPeerCnt(void) const
			{
				return _NetS.GetPeerCnt();
			}
			inline milliseconds Latency(TPeerCnt PeerNum_) const
			{
				return _NetS.Latency(PeerNum_);
			}
			CKey Connect(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& MasterNamePort_);
			inline CKey Connect(const wstring& DataPath_, const CNamePort& MasterNamePort_)
			{
				return Connect(TPeerCnt(_Servers.new_index()), DataPath_, MasterNamePort_);
			}
			inline void Proc(void)
			{
				_NetS.Proc();
			}
			inline bool IsConnecting(TPeerCnt PeerNum_) const
			{
				return bool(_Servers.get(PeerNum_));
			}
			template<typename... TProtos>
			void Send(TPeerCnt PeerNum_, const TProtos&... Protos_)
			{
				if (!_Servers[PeerNum_].Logon)
					return;

				_NetS.Send(PeerNum_, SHeader(EProto::CsUserProto), Protos_...);
			}
			template<typename... TProtos>
			void Send(const CKey& Key_, const TProtos&... Protos_)
			{
				if (_Servers[Key_.PeerNum].PeerCounter != Key_.PeerCounter)
					return;

				_NetS.Send(Key_.PeerNum, SHeader(EProto::CsUserProto), Protos_...);
			}
			template<typename... TProtos>
			void SendAll(const TProtos&... Protos_)
			{
				for (auto it = _Servers.begin(); it != _Servers.end(); ++it)
				{
					if (!it->Logon)
						continue;

					_NetS.Send(TPeerCnt(it.Index()), SHeader(EProto::CsUserProto), Protos_...);
				}
			}
		};
	}
}