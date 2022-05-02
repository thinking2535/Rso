#include "Server.h"

namespace rso
{
	namespace balance
	{
		bool CServer::_IsReady(void) const noexcept
		{
			return _ChildCntMax > 0;
		}
		void CServer::_LinkU(const CKey& Key_)
		{
			_NetU.Send(Key_.PeerNum, SHeader(EProto::DuChildOn), SDuChildOn(SServer(_ClientBindNamePortPub, _GetCapacity()), _SessionCode, _NetD.GetBindNamePort()));
		}
		void CServer::_LinkFailU(TPeerCnt PeerNum_, ENetRet NetRet_)
		{
			_LinkFailFuncU(PeerNum_, NetRet_);
			_NetU.Connect(_MasterNamePort);
		}
		void CServer::_UnLinkU(const CKey& Key_, ENetRet NetRet_)
		{
			// 부모를 찾고 있는 중이라면 초기화 코드는 필요 없으나 직관적으로 처리하기 위해 그냥 초기화
			_ErrorCnt = 0;
			_ChildCntMax = 0;
			_SessionCode = 0;
			_NetD.CloseAll();
			_ChildsStandby.clear();
			_ChildsStandbyTimer.clear();

			if (_ParentNamePort) // 부모를 찾고 있는 중이라면 
			{
				_NetU.Connect(_ParentNamePort);
				_ParentNamePort.Clear();
			}
			else // 아니면
			{
				const auto IsLinked = (bool)((CNamePort&)_Parent.ClientBindNamePortPub);
				_Parent = SServer();

				if (IsLinked) // 부모가 설정(정상 할당)되었다면
					_UnLinkFuncU(Key_, NetRet_);
				else
					_LinkFailFuncU(Key_.PeerNum, NetRet_);
			}
		}
		void CServer::_RecvU(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			if (!_IsReady())
			{
				switch (Header.Proto)
				{
				case EProto::UdParentOn: return _RecvUdParentOn(Key_, Stream_);
				case EProto::UdNewParent: return _RecvUdNewParent(Key_, Stream_);
				default: return _NetU.Close(Key_.PeerNum);
				}
			}
			else
			{
				switch (Header.Proto)
				{
				case EProto::UdNewChild: return _RecvUdNewChild(Key_, Stream_);
				case EProto::UdCapacity: return _RecvUdCapacity(Key_, Stream_);
				case EProto::UdBroadCast: return _RecvUdBroadCast(Key_, Stream_);
				case EProto::UdUserProto: return _RecvFuncU(Key_, Stream_);
				default: return _NetU.Close(Key_.PeerNum);
				}
			}
		}
		void CServer::_RecvUdParentOn(const CKey& Key_, CStream& Stream_)
		{
			SUdParentOn Proto;
			Stream_ >> Proto;

			_Parent = Proto;
			_ChildCntMax = Proto.ChildCntMax;
			_ErrorCnt = Proto.ErrorCnt;
			_LinkFuncU(Key_);
			// UdCapacity 를 보내지 않음.(여기서는 자식이 없기 때문에)
		}
		void CServer::_RecvUdNewChild(const CKey& Key_, CStream& Stream_)
		{
			SUdNewChild Proto;
			Stream_ >> Proto;

			auto TimerIt = _ChildsStandbyTimer.emplace(system_clock::now() + seconds(30), CNamePort(Proto.ClientBindNamePortPub));
			try
			{
				auto ib = _ChildsStandby.emplace(CNamePort(Proto.ClientBindNamePortPub), _SChildStandby(Proto.SessionCode, TimerIt));
				if (ib.second)
					_NetU.Send(Key_.PeerNum, SHeader(EProto::DuNewChild), SDuNewChild(Proto.NewChildKey, Proto.SessionCode));
				else
					throw 0;
			}
			catch (...)
			{
				_ChildsStandbyTimer.erase(TimerIt);
				_NetU.Send(Key_.PeerNum, SHeader(EProto::DuNewChildFail), SDuNewChildFail(Proto.NewChildKey));
			}
		}
		void CServer::_RecvUdNewParent(const CKey& Key_, CStream& Stream_)
		{
			SUdNewParent Proto;
			Stream_ >> Proto;

			_ParentNamePort = Proto.ChildBindNamePort;
			_SessionCode = Proto.SessionCode;
			_NetU.Close(Key_.PeerNum);
		}
		void CServer::_RecvUdCapacity(const CKey& /*Key_*/, CStream& Stream_)
		{
			SCapacity Proto;
			Stream_ >> Proto;

			_Parent.Capacity = Proto;
			_SendUdCapacity(_GetCapacity() + Proto);
		}
		void CServer::_RecvUdBroadCast(const CKey& Key_, CStream& Stream_)
		{
			CStream Stream;
			Stream_ >> Stream;

			_NetD.SendAll(SHeader(EProto::UdBroadCast), Stream);
			_RecvFuncUBroadCast(Key_, Stream);
		}
		void CServer::_LinkD(const CKey& /*Key_*/) noexcept
		{
		}
		void CServer::_UnLinkD(const CKey& Key_, ENetRet NetRet_)
		{
			auto itChild = _Childs.find(Key_.PeerNum);
			if (itChild != _Childs.end())
			{
				_Childs.erase(itChild);
				_UnLinkFuncD(Key_, NetRet_);
			}
		}
		void CServer::_RecvD(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			auto Child = _Childs.find(Key_.PeerNum);
			if (Child == _Childs.end())
			{
				switch (Header.Proto)
				{
				case EProto::DuChildOn: return _RecvDuChildOn(Key_, Stream_);
				default: return _NetD.Close(Key_.PeerNum);
				}
			}
			else
			{
				switch (Header.Proto)
				{
				case EProto::DuNewChild: return _RecvDuNewChild(Child, Stream_);
				case EProto::DuNewChildFail: return _RecvDuNewChildFail(Child, Stream_);
				case EProto::DuCapacity: return _RecvDuCapacity(Child, Stream_);
				case EProto::DuUserProto: return _RecvFuncD(Key_, Stream_);
				default: return _NetD.Close(Key_.PeerNum);
				}
			}
		}
		void CServer::_RecvDuChildOn(const CKey& Key_, CStream& Stream_)
		{
			SDuChildOn Proto;
			Stream_ >> Proto;

			if (_MasterNamePort)
			{
				auto ChildStandby = _ChildsStandby.find(CNamePort(Proto.ClientBindNamePortPub));
				if (ChildStandby == _ChildsStandby.end() ||
					ChildStandby->second.SessionCode != Proto.SessionCode)
				{
					_NetD.Close(Key_.PeerNum);
					return;
				}

				_ChildsStandbyTimer.erase(ChildStandby->second.ChildsStandbyTimerIt);
				_ChildsStandby.erase(ChildStandby);
			}

			if (_Childs.size() < _ChildCntMax)
			{
				_Childs.emplace(Key_.PeerNum, _SChild(Proto, Proto.ChildBindNamePort));
				_NetD.Certify(Key_.PeerNum);
				_NetD.Send(Key_.PeerNum, SHeader(EProto::UdParentOn), SUdParentOn(SServer(_ClientBindNamePortPub, _GetCapacity()), _ChildCntMax, _ErrorCnt));
				_LinkFuncD(Key_);
				// DuCapacity 보내지 않음.(모든 자식에게서 DuCapacity를 받은 경우만 위로 전달 해야 하기 때문에)
			}
			else
			{
				auto NewParent = _Childs.begin();
				auto It = NewParent;

				for (++It; It != _Childs.end(); ++It)
				{
					if (It->second.GetServerCntAll() < NewParent->second.GetServerCntAll())
						NewParent = It;
				}

				++NewParent->second.ServerCntStandby;
				_NetD.Send(NewParent->first, SHeader(EProto::UdNewChild), SUdNewChild(Proto.ClientBindNamePortPub, (_MasterNamePort ? Proto.SessionCode : ++_SessionCounter), Key_));
			}
		}
		void CServer::_RecvDuNewChild(_TChildsIt Child_, CStream& Stream_)
		{
			SDuNewChild Proto;
			Stream_ >> Proto;

			if (Child_->second.ServerCntStandby > 0)
				--Child_->second.ServerCntStandby;

			++Child_->second.Capacity.ServerCnt;

			// Proto.NewChildKey 가 접종하여 Send 실패하더라도 Child_ _ChildsStandby 가 시간되면 날라가기 때문에 문제 없음.
			_NetD.Send(Proto.NewChildKey, SHeader(EProto::UdNewParent), SUdNewParent(Child_->second.ChildBindNamePort, Proto.SessionCode));
			_NetD.WillClose(Proto.NewChildKey, seconds(1));
		}
		void CServer::_RecvDuNewChildFail(_TChildsIt Child_, CStream& Stream_)
		{
			SDuNewChildFail Proto;
			Stream_ >> Proto;

			if (Child_->second.ServerCntStandby > 0)
				--Child_->second.ServerCntStandby;

			_NetD.Close(Proto.NewChildKey);
		}
		void CServer::_RecvDuCapacity(_TChildsIt Child_, CStream& Stream_)
		{
			SCapacity Proto;
			Stream_ >> Proto;

			Child_->second.Capacity = Proto;

			if (size_t(_ReceivedCapacityCnt + 1) < _Childs.size())
			{
				++_ReceivedCapacityCnt;
			}
			else
			{
				_ReceivedCapacityCnt = 0;

				SCapacity ChildrenCapacity;
				for (auto& i : _Childs)
					ChildrenCapacity += i.second.Capacity;

				_NetU.SendAll(SHeader(EProto::DuCapacity), _GetCapacity() + ChildrenCapacity);
			}
		}
		void CServer::_LinkC(const CKey& Key_)
		{
			if (!_IsReady())
				_NetC.Close(Key_.PeerNum);
		}
		void CServer::_UnLinkC(const CKey& Key_, ENetRet NetRet_)
		{
			if (_NetC.IsCertified(Key_.PeerNum))
				_UnLinkFuncC(Key_, NetRet_);
		}
		void CServer::_RecvC(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			if (_NetC.IsCertified(Key_.PeerNum))
			{
				switch (Header.Proto)
				{
				case EProto::CsUserProto: return _RecvFuncC(Key_, Stream_);
				default: return _NetC.Close(Key_.PeerNum);
				}
			}
			else
			{
				switch (Header.Proto)
				{
				case EProto::CsConnect: return _RecvCsConnect(Key_, Stream_);
				default: return _NetC.Close(Key_.PeerNum);
				}
			}
		}
		void CServer::_RecvCsConnect(const CKey& Key_, CStream& Stream_)
		{
			SCsConnect Proto;
			Stream_ >> Proto;

			auto MyCapacity = _GetCapacity();
			if (MyCapacity.ClientCnt > 0)
				--MyCapacity.ClientCnt; // 당 서버에 접속중인 나 역시 포함되므로 제외

			SServer* OtherServer = nullptr;

			if (_MasterNamePort)
			{
				OtherServer = &_Parent;

				for (auto& i : _Childs)
				{
					if (i.second.Capacity < OtherServer->Capacity)
						OtherServer = &i.second;
				}
			}
			else
			{
				auto It = _Childs.begin();
				if (It != _Childs.end())
				{
					OtherServer = &It->second;

					for (++It; It != _Childs.end(); ++It)
					{
						if (It->second.Capacity < OtherServer->Capacity)
							OtherServer = &It->second;
					}
				}
			}

			if (!OtherServer || SCapacityAvgClientCnt()(MyCapacity) <= SCapacityAvgClientCnt()(OtherServer->Capacity) + _ErrorCnt ||
				CNamePort(Proto.PrevClientBindNamePortPub) == OtherServer->ClientBindNamePortPub) // 클라이언트가 직전에 접속한 서버를 다시 할당했다면
			{
				_NetC.Send(Key_.PeerNum, SHeader(EProto::ScAllocated), SScAllocated());
				_NetC.Certify(Key_.PeerNum);
				_LinkFuncC(Key_);
			}
			else
			{
				_NetC.Send(Key_.PeerNum, SHeader(EProto::ScNewParent), SScNewParent(OtherServer->ClientBindNamePortPub));
				_NetC.WillClose(Key_.PeerNum, milliseconds(1000));
				++OtherServer->Capacity.ClientCnt;
			}
		}
		void CServer::_SendUdCapacity(const SCapacity& Capacity_)
		{
			auto TotalCapacity = Capacity_;

			for (auto& i : _Childs)
				TotalCapacity += i.second.Capacity;

			for (auto& i : _Childs)
				_NetD.Send(i.first, SHeader(EProto::UdCapacity), TotalCapacity - i.second.Capacity);
		}

		CServer::CServer(
			EAddressFamily AddressFamily_,
			TLinkFunc LinkFuncU_, TLinkFailFunc LinkFailFuncU_, TUnLinkFunc UnLinkFuncU_, TRecvFunc RecvFuncU_, TRecvFunc RecvFuncUBroadCast_,
			TLinkFunc LinkFuncD_, TUnLinkFunc UnLinkFuncD_, TRecvFunc RecvFuncD_,
			TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TRecvFunc RecvFuncC_,
			const CNamePort& MasterNamePort_, TPeerCnt ChildCntMax_, TPeerCnt ErrorCnt_, size_t NetCThreadCnt_, const CNamePort& ChildBindNamePort_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_) :
			_MasterNamePort(MasterNamePort_),
			_ChildCntMax(MasterNamePort_ ? 0 : (ChildCntMax_ <= 0 ? MaxValue<TPeerCnt>() : ChildCntMax_)), // 내가 Master가 아니면 부모로 부터 받아서 세팅
			_ErrorCnt(MasterNamePort_ ? 0 : (ErrorCnt_ < 0 ? 0 : ErrorCnt_)), // 내가 Master가 아니면 부모로 부터 받아서 세팅
			_ClientBindNamePortPub(ClientBindNamePortPub_),
			_LinkFuncU(LinkFuncU_), _LinkFailFuncU(LinkFailFuncU_), _UnLinkFuncU(UnLinkFuncU_), _RecvFuncU(RecvFuncU_), _RecvFuncUBroadCast(RecvFuncUBroadCast_),
			_LinkFuncD(LinkFuncD_), _UnLinkFuncD(UnLinkFuncD_), _RecvFuncD(RecvFuncD_),
			_LinkFuncC(LinkFuncC_), _UnLinkFuncC(UnLinkFuncC_), _RecvFuncC(RecvFuncC_),
			_NetU(
				std::bind(&CServer::_LinkU, this, _1),
				std::bind(&CServer::_LinkFailU, this, _1, _2),
				std::bind(&CServer::_UnLinkU, this, _1, _2),
				std::bind(&CServer::_RecvU, this, _1, _2),
				false, 1024000, 1024000,
				 milliseconds(), milliseconds(), 1, 5),
			_NetD(
				AddressFamily_,
				std::bind(&CServer::_LinkD, this, _1),
				std::bind(&CServer::_UnLinkD, this, _1, _2),
				std::bind(&CServer::_RecvD, this, _1, _2),
				true, 10240000, 10240000,
				milliseconds(), milliseconds(), NetCThreadCnt_, 0,
				ChildBindNamePort_, SOMAXCONN),
			_NetC(
				AddressFamily_,
				std::bind(&CServer::_LinkC, this, _1),
				std::bind(&CServer::_UnLinkC, this, _1, _2),
				std::bind(&CServer::_RecvC, this, _1, _2),
				true, 10240000, 10240000,
				milliseconds(120000), milliseconds(60000), NetCThreadCnt_, 0,
				ClientBindNamePort_, SOMAXCONN)
		{
		}
		void CServer::Proc(void)
		{
			_NetU.Proc();
			_NetD.Proc();
			_NetC.Proc();

			if (_PeriodCapacity.CheckAndNextLoose())
			{
				if (!_MasterNamePort) // 내가 Master이면 (Leaf 라도 상관 없음)
					_SendUdCapacity(_GetCapacity());
				else if (_Childs.empty() && _IsReady()) // 내가 Leaf 이고, 부모가 준비되었으면
					_NetU.SendAll(SHeader(EProto::DuCapacity), _GetCapacity());

				if (_PeriodReconnect.CheckAndNextLoose())
				{
					if (!_NetU.IsConnecting(0) && !_ParentNamePort && _MasterNamePort)
						_NetU.Connect(_MasterNamePort);

				}

				const auto Now = system_clock::now();
				for (auto it = _ChildsStandbyTimer.begin(); it != _ChildsStandbyTimer.end(); ++it)
				{
					if (it->first > Now)
					{
						if (it != _ChildsStandbyTimer.begin())
							_ChildsStandbyTimer.erase(_ChildsStandbyTimer.begin(), it);

						break;
					}

					_ChildsStandby.erase(it->second);
				}
			}
		}
	}
}