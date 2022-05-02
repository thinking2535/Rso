#include "Server.h"

namespace rso::game
{
	void CServer::_DBCallback(TOutObject& OutObject_)
	{
		if (_DBQueries.front().MasterSessionIndex == _MasterSessionCounter) // Master와 재 접속이 된경우 이전 접속시 날린 쿼리는 버리기 위함
		{
			_DBCallbackFunc(_DBQueries.front().itSession, OutObject_);
			_SessionSubDBPushedCount(_DBQueries.front().itSession);
		}
	}
	void CServer::_SendSmSessionEnd(TSessionsIt itSession_)
	{
		_NetM.Send(itSession_->second.MasterKey, SHeader(EProto::SmSessionEnd), SSmSessionEnd(itSession_->first, itSession_->second.SessionCode));
	}
	void CServer::_SessionEnd(TSessionsIt itSession_)
	{
		if (itSession_->second.IsValid())
			return;

		_SendSmSessionEnd(itSession_);
		_Sessions.erase(itSession_);
	}
	void CServer::SessionHold(TSessionsIt itSession_)
	{
		if (itSession_->second.Hold)
			return;

		itSession_->second.Hold = true;
	}
	void CServer::_SessionUnHold(TSessionsIt itSession_)
	{
		if (!itSession_->second.Hold)
			return;

		itSession_->second.Hold = false;
		_SessionEnd(itSession_);
	}
	void CServer::_SessionUnSendToServer(TSessionsIt itSession_)
	{
		if (!itSession_->second.SendToServer)
			return;

		itSession_->second.SendToServer = false;
		_SessionEnd(itSession_);
	}
	void CServer::_SessionSubDBPushedCount(TSessionsIt itSession_)
	{
		if (itSession_ != _Sessions.end())
		{
			if (itSession_->second.DBPushedCount <= 0)
				return;

			--itSession_->second.DBPushedCount;
			_SessionEnd(itSession_);
		}
		_DBQueries.pop_front();
	}
	void CServer::_LinkM(const CKey& Key_)
	{
		_NetM.Send(Key_.PeerNum, SHeader(EProto::SmServerOn), SSmServerOn(_ClientBindNamePortPub, _NetS.GetPort()));
		_LinkFuncM(Key_);
	}
	void CServer::_LinkFailM(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_LinkFailFuncM(PeerNum_, NetRet_);
	}
	void CServer::_UnLinkM(const CKey& Key_, ENetRet NetRet_)
	{
		++_MasterSessionCounter;
		_NetC.CloseAll();	// 모든 유저를 끊어버리는 이유는 특정 서버가 마스터와 끊어지고 다른 서버는 연결 정상일 경우
							// 특정서버에 접속한 유저는 다른 서버로 또다시 접속 가능해지므로(중복로그인)
		_UnLinkFuncM(Key_, NetRet_);
	}
	void CServer::_RecvM(const CKey& Key_, CStream& Stream_)
	{
		SHeader Header;
		Stream_ >> Header;

		switch (Header.Proto)
		{
		case EProto::MsMasterOn: return _RecvMsMasterOn(Key_, Stream_);
		case EProto::MsOtherMasterOn: return _RecvMsOtherMasterOn(Key_, Stream_);
		case EProto::MsOtherMasterOff: return _RecvMsOtherMasterOff(Key_, Stream_);
		case EProto::MsServerOn: return _RecvMsServerOn(Key_, Stream_);
		case EProto::MsOtherServerOn: return _RecvMsOtherServerOn(Key_, Stream_);
		case EProto::MsOtherMasterServerOn: return _RecvMsOtherMasterServerOn(Key_, Stream_);
		case EProto::MsOtherServerOff: return _RecvMsOtherServerOff(Key_, Stream_);
		case EProto::MsOtherMasterServerOff: return _RecvMsOtherMasterServerOff(Key_, Stream_);
		case EProto::MsChangeNick: return _RecvMsChangeNick(Key_, Stream_);
		case EProto::MsChangeNickFail: return _RecvMsChangeNickFail(Key_, Stream_);
		case EProto::MsCheck: return _RecvMsCheck(Key_, Stream_);
		case EProto::MsLogin: return _RecvMsLogin(Key_, Stream_);
		case EProto::MsAddFriendRequest: return _RecvMsAddFriendRequest(Key_, Stream_);
		case EProto::MsAddFriend: return _RecvMsAddFriend(Key_, Stream_);
		case EProto::MsAddFriendFail: return _RecvMsAddFriendFail(Key_, Stream_);
		case EProto::MsAllowFriend: return _RecvMsAllowFriend(Key_, Stream_);
		case EProto::MsAllowFriendFail: return _RecvMsAllowFriendFail(Key_, Stream_);
		case EProto::MsDenyFriend: return _RecvMsDenyFriend(Key_, Stream_);
		case EProto::MsDenyFriendFail: return _RecvMsDenyFriendFail(Key_, Stream_);
		case EProto::MsBlockFriend: return _RecvMsBlockFriend(Key_, Stream_);
		case EProto::MsBlockFriendFail: return _RecvMsBlockFriendFail(Key_, Stream_);
		case EProto::MsUnBlockFriend: return _RecvMsUnBlockFriend(Key_, Stream_);
		case EProto::MsUnBlockFriendFail: return _RecvMsUnBlockFriendFail(Key_, Stream_);
		case EProto::MsChangeState: return _RecvMsChangeState(Key_, Stream_);
		case EProto::MsChangeStateFail: return _RecvMsChangeStateFail(Key_, Stream_);
		case EProto::MsFriendStateChanged: return _RecvMsFriendStateChanged(Key_, Stream_);
		case EProto::MsFriendStateChangedOffline: return _RecvMsFriendStateChangedOffline(Key_, Stream_);
		case EProto::MsToServer: return _RecvMsToServer(Key_, Stream_);
		case EProto::MsSessionHold: return _RecvMsSessionHold(Key_, Stream_);
		case EProto::MsPunish: return _RecvMsPunish(Key_, Stream_);
		case EProto::MsUserProto: return _RecvFuncM(Key_, Stream_);
		default: return _NetM.Close(Key_.PeerNum);
		}
	}
	void CServer::_RecvMsMasterOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsMasterOn Proto;
		Stream_ >> Proto;

		// clear ///////////////////
		for (auto& m : _OtherMasters)
		{
			for (auto& s : m.second.Servers)
				_NetS.ErasePeer(s.ServerBindNamePort);
		}

		_OtherMasters.clear();
		////////////////////////////////////////

		for (auto& i : Proto.OtherMasters)
		{
			_OtherMasters.emplace(TRangeUID(i.RangeUID), _SMaster(i.Servers));

			for (auto& s : i.Servers)
				_NetS.InsertPeer(s.ServerBindNamePort);
		}
	}
	void CServer::_RecvMsOtherMasterOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsOtherMasterOn Proto;
		Stream_ >> Proto;

		_OtherMasters.emplace(TRangeUID(Proto.RangeUID), _SMaster(Proto.Servers));

		for (auto& s : Proto.Servers)
			_NetS.InsertPeer(s.ServerBindNamePort);
	}
	void CServer::_RecvMsOtherMasterOff(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsOtherMasterOff Proto;
		Stream_ >> Proto;

		auto itMaster = _OtherMasters.find(TRangeUID(Proto.MasterRangeUID));
		if (itMaster == _OtherMasters.end())
			return;

		for (auto& i : itMaster->second.Servers)
			_NetS.ErasePeer(i.ServerBindNamePort);

		_OtherMasters.erase(itMaster);
	}
	void CServer::_RecvMsServerOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsServerOn Proto;
		Stream_ >> Proto;

		// clear ///////////////////
		for (auto& s : _OtherServers)
			_NetS.ErasePeer(s.ServerBindNamePort);

		_OtherServers.clear();

		for (auto& m : _OtherMasters)
		{
			for (auto& s : m.second.Servers)
				_NetS.ErasePeer(s.ServerBindNamePort);
		}

		_OtherMasters.clear();
		////////////////////////////////////////

		_ServerBindNamePort = Proto.ServerBindNamePort;

		for (auto& s : Proto.OtherServers)
		{
			_OtherServers.emplace(s.ServerBindNamePort);
			_NetS.InsertPeer(s.ServerBindNamePort);
		}

		for (auto& m : Proto.OtherMasters)
		{
			_OtherMasters.emplace(m.RangeUID, _SMaster(m.Servers));

			for (auto& s : m.Servers)
				_NetS.InsertPeer(s.ServerBindNamePort);
		}
	}
	void CServer::_RecvMsOtherServerOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsOtherServerOn Proto;
		Stream_ >> Proto;

		_OtherServers.emplace(Proto.Server);
		_NetS.InsertPeer(Proto.Server.ServerBindNamePort);
	}
	void CServer::_RecvMsOtherMasterServerOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsOtherMasterServerOn Proto;
		Stream_ >> Proto;

		auto itMaster = _OtherMasters.find(TRangeUID(Proto.MasterRangeUID));
		if (itMaster == _OtherMasters.end())
			return;

		itMaster->second.Servers.emplace(Proto.Server);
		_NetS.InsertPeer(Proto.Server.ServerBindNamePort);
	}
	void CServer::_RecvMsOtherServerOff(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsOtherServerOff Proto;
		Stream_ >> Proto;

		_OtherServers.erase(Proto.Server.ServerBindNamePort);
		_NetS.ErasePeer(Proto.Server.ServerBindNamePort);
	}
	void CServer::_RecvMsOtherMasterServerOff(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsOtherMasterServerOff Proto;
		Stream_ >> Proto;

		auto itMaster = _OtherMasters.find(TRangeUID(Proto.MasterRangeUID));
		if (itMaster == _OtherMasters.end())
			return;

		itMaster->second.Servers.erase(Proto.Server.ServerBindNamePort);
		_NetS.ErasePeer(Proto.Server.ServerBindNamePort);
	}
	void CServer::_RecvMsChangeNick(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsChangeNick Proto;
		Stream_ >> Proto;

		auto itClient = _Clients.get(Proto.ClientKey.PeerNum);
		if (!itClient)
			return;

		if (!(*itClient)->second.HasKey(Proto.ClientKey))
			return;

		(*itClient)->second.Account.Nick = Proto.Nick;
		(*itClient)->second.IsChangingNick = false;
		_ChangeNickFunc(Proto.ClientKey, EGameRet::Ok);
	}
	void CServer::_RecvMsChangeNickFail(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsChangeNickFail Proto;
		Stream_ >> Proto;

		auto itClient = _Clients.get(Proto.ClientKey.PeerNum);
		if (!itClient)
			return;

		if (!(*itClient)->second.HasKey(Proto.ClientKey))
			return;

		(*itClient)->second.IsChangingNick = false;
		_ChangeNickFunc(Proto.ClientKey, Proto.GameRet);
	}
	void CServer::_RecvMsCheck(const CKey& Key_, CStream& Stream_)
	{
		// throw 하더라도 서버가 끊어져 버리면 Master의 세션을 알아서 삭제됨
		SMsCheck Proto;
		Stream_ >> Proto;

		auto ib = _Sessions.emplace(Proto.UID, _SClient(Key_, Proto.SessionCode, _SCheck(Proto.UserKey, Proto.AuthKey)));
		if (!ib.second)
		{
			ib.first->second.SessionCode = Proto.SessionCode;
			ib.first->second.AddCheck(Proto.SessionCode, _SCheck(Proto.UserKey, Proto.AuthKey));
		}

		_CheckFunc(ib.first, Proto.SessionCode); // throw 하더라도 서버가 끊어져 버리면 Master의 세션을 알아서 삭제됨
	}
	void CServer::_RecvMsLogin(const CKey& Key_, CStream& Stream_)
	{
		// throw 하더라도 서버가 끊어져 버리면 Master의 세션을 알아서 삭제됨
		SMsLogin Proto;
		Stream_ >> Proto;

		auto it = _Sessions.find(Proto.UID);
		if (it == _Sessions.end())
		{
			_Sessions.emplace(Proto.UID, _SClient(Key_, Proto));
		}
		else
		{
			it->second.MsLogin(Proto);
		}

		_SessionTimer.emplace(system_clock::now() + seconds(30), _SSession(SUIDPair(Proto.UID, Proto.SubUID), Proto.SessionCode));
		_NetM.Send(Key_.PeerNum, SHeader(EProto::SmLogin), SSmLogin(Proto.UserKey, Proto.SessionCode));
	}
	void CServer::_RecvMsAddFriendRequest(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsAddFriendRequest Proto;
		Stream_ >> Proto;

		auto itSession = _Sessions.find(Proto.ToUID);
		if (itSession == _Sessions.end())
			return;

		itSession->second.Friends.emplace(Proto.FromUID, SFriend(SFriendDB(Proto.FromNick, EFriendState::Request), SFriendInfo(SNamePort(), c_Default_State)));
		_SendC(itSession, SHeader(EProto::ScAddFriendRequest), SScAddFriendRequest(Proto.FromUID, Proto.FromNick));
	}
	void CServer::_RecvMsAddFriend(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsAddFriend Proto;
		Stream_ >> Proto;

		auto itSession = _Sessions.find(Proto.FromUID);
		if (itSession == _Sessions.end())
			return;

		auto ibFriend = itSession->second.Friends.emplace(Proto.ToUID, SFriend(SFriendDB(Proto.ToNick, EFriendState::Normal), SFriendInfo(SNamePort(), c_Default_State)));
		_SendC(itSession, SHeader(EProto::ScAddFriend), SScAddFriend(Proto.ToUID, ibFriend.first->second));
	}
	void CServer::_RecvMsAddFriendFail(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsAddFriendFail Proto;
		Stream_ >> Proto;

		auto itSession = _Sessions.find(Proto.FromUID);
		if (itSession == _Sessions.end())
			return;

		_SendC(itSession, SHeader(EProto::ScError), SScError(Proto.GameRet));
	}
	void CServer::_RecvMsAllowFriend(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsAllowFriend Proto;
		Stream_ >> Proto;

		auto itSession = _Sessions.find(Proto.UID);
		if (itSession == _Sessions.end())
			return;

		auto itFriend = itSession->second.Friends.find(Proto.FriendUID);
		if (itFriend == itSession->second.Friends.end())
			return;

		itFriend->second.FriendState = EFriendState::Normal;
		_NetC.Send(Proto.UserKey, SHeader(EProto::ScAllowFriend), SScAllowFriend(Proto.FriendUID));
	}
	void CServer::_RecvMsAllowFriendFail(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsAllowFriendFail Proto;
		Stream_ >> Proto;

		_NetC.Send(Proto.UserKey, SHeader(EProto::ScError), SScError(Proto.GameRet));
	}
	void CServer::_RecvMsDenyFriend(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsDenyFriend Proto;
		Stream_ >> Proto;

		auto itSession = _Sessions.find(Proto.UID);
		if (itSession == _Sessions.end())
			return;

		auto itFriend = itSession->second.Friends.find(Proto.FriendUID);
		if (itFriend == itSession->second.Friends.end())
			return;

		itFriend->second.FriendState = EFriendState::Blocked;
		_NetC.Send(Proto.UserKey, SHeader(EProto::ScDenyFriend), SScDenyFriend(Proto.FriendUID));
	}
	void CServer::_RecvMsDenyFriendFail(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsDenyFriendFail Proto;
		Stream_ >> Proto;

		_NetC.Send(Proto.UserKey, SHeader(EProto::ScError), SScError(Proto.GameRet));
	}
	void CServer::_RecvMsBlockFriend(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsBlockFriend Proto;
		Stream_ >> Proto;

		auto itSession = _Sessions.find(Proto.UID);
		if (itSession == _Sessions.end())
			return;

		auto itFriend = itSession->second.Friends.find(Proto.FriendUID);
		if (itFriend == itSession->second.Friends.end())
			return;

		itFriend->second.FriendState = Proto.FriendState;
		_NetC.Send(Proto.UserKey, SHeader(EProto::ScBlockFriend), SScBlockFriend(Proto.FriendUID));
	}
	void CServer::_RecvMsBlockFriendFail(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsBlockFriendFail Proto;
		Stream_ >> Proto;

		_NetC.Send(Proto.UserKey, SHeader(EProto::ScError), SScError(Proto.GameRet));
	}
	void CServer::_RecvMsUnBlockFriend(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsUnBlockFriend Proto;
		Stream_ >> Proto;

		auto itSession = _Sessions.find(Proto.UID);
		if (itSession == _Sessions.end())
			return;

		auto itFriend = itSession->second.Friends.find(Proto.FriendUID);
		if (itFriend == itSession->second.Friends.end())
			return;

		itFriend->second.FriendState = Proto.FriendState;
		_NetC.Send(Proto.UserKey, SHeader(EProto::ScUnBlockFriend), SScUnBlockFriend(Proto.FriendUID));
	}
	void CServer::_RecvMsUnBlockFriendFail(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsUnBlockFriendFail Proto;
		Stream_ >> Proto;

		_NetC.Send(Proto.UserKey, SHeader(EProto::ScError), SScError(Proto.GameRet));
	}
	void CServer::_RecvMsChangeState(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsChangeState Proto;
		Stream_ >> Proto;

		_NetC.Send(Proto.UserKey, SHeader(EProto::ScChangeState), SScChangeState(Proto.State));
	}
	void CServer::_RecvMsChangeStateFail(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsChangeStateFail Proto;
		Stream_ >> Proto;

		_NetC.Send(Proto.UserKey, SHeader(EProto::ScError), SScError(Proto.GameRet));
	}
	void CServer::_RecvMsFriendStateChanged(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsFriendStateChanged Proto;
		Stream_ >> Proto;

		for (auto& i : Proto.UIDFriendInfos)
		{
			auto itSession = _Sessions.find(i.UID);
			if (itSession == _Sessions.end())
				continue;

			auto itFriend = itSession->second.Friends.find(Proto.FriendUID);
			if (itFriend != itSession->second.Friends.end())
				itFriend->second.Info = i.FriendInfo;

			_SendC(itSession, SHeader(EProto::ScFriendStateChanged), SScFriendStateChanged(Proto.FriendUID, i.FriendInfo.State));
		}
	}
	void CServer::_RecvMsFriendStateChangedOffline(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsFriendStateChangedOffline Proto;
		Stream_ >> Proto;

		for (auto& i : Proto.Friends)
		{
			auto itSession = _Sessions.find(i.first);
			if (itSession == _Sessions.end())
				continue;

			_SendC(itSession, SHeader(EProto::ScFriendStateChangedOffline), SScFriendStateChangedOffline(std::move(i.second)));
		}
	}
	void CServer::_RecvMsSessionHold(const CKey& Key_, CStream& Stream_)
	{
		TUID UID = 0;
		Stream_ >> UID;

		TSessionCode SessionCode = 0;
		Stream_ >> SessionCode;

		CStream Stream;
		Stream_ >> Stream;

		auto ib = _Sessions.emplace(UID, _SClient(Key_, SessionCode, true));
		if (!ib.second)
			ib.first->second.SessionCode = SessionCode;

		_RecvSessionHoldFunc(ib.first, Stream);
	}
	void CServer::_RecvMsToServer(const CKey& Key_, CStream& Stream_)
	{
		TUID UID = 0;
		Stream_ >> UID;

		TSessionCode SessionCode = 0;
		Stream_ >> SessionCode;

		CStream Stream;
		Stream_ >> Stream;

		auto ib = _Sessions.emplace(UID, _SClient(Key_, SessionCode, false));
		if (!ib.second)
		{
			ib.first->second.SessionCode = SessionCode;
			ib.first->second.SendToServer = true; // _RecvMsToServer 를 받기전에 SendToServer 는 false 가 됨을 보장.
		}

		try
		{
			_RecvSendToServerFunc(ib.first, Stream);
		}
		catch (...)
		{
			_SessionUnSendToServer(ib.first);
			throw;
		}

		_SessionUnSendToServer(ib.first);
	}
	void CServer::_RecvMsPunish(const CKey& /*Key_*/, CStream& Stream_)
	{
		SMsPunish Proto;
		Stream_ >> Proto;

		_RecvPunishFunc(Proto.UID, Proto.EndTime);
	}

	void CServer::_LinkS(TIPEndPoint /*IPEndPoint_*/)
	{
	}
	void CServer::_UnLinkS(TIPEndPoint /*IPEndPoint_*/)
	{
	}
	void CServer::_RecvS(CUDP::SRecvData& Data_)
	{
		SHeader Header;
		Data_.Stream >> Header;

		switch (Header.Proto)
		{
		case EProto::SsMessageSend: return _RecvSsMessageSend(Data_);
		default: THROWEX();
		}
	}
	void CServer::_RecvSsMessageSend(CUDP::SRecvData& Data_)
	{
		SSsMessageSend Proto;
		Data_.Stream >> Proto;

		_SendScMessageReceived(Proto.ToUID, SScMessageReceived(Proto.FromUID, Proto.Message));
	}
	void CServer::_SendScMessageReceived(TUID ToUID_, const SScMessageReceived& Proto_)
	{
		auto itSession = _Sessions.find(ToUID_);
		if (itSession == _Sessions.end())
			return;

		auto itFriend = itSession->second.Friends.find(Proto_.FromUID);
		if (itFriend == itSession->second.Friends.end())
			return;

		if (itFriend->second.FriendState != EFriendState::Normal)
			return;

		_SendC(itSession, SHeader(EProto::ScMessageReceived), Proto_);
	}
	void CServer::_LinkC(const CKey& Key_)
	{
		if (!_NetM.IsLinked(0))
			_NetC.Close(Key_.PeerNum);
	}
	void CServer::_UnLinkC(const CKey& Key_, ENetRet NetRet_)
	{
		auto itClient = _Clients.get(Key_.PeerNum);
		if (itClient)
		{
			_UnLinkFuncC(*itClient, Key_, NetRet_); // _LogoutFunc 에서 Session 을 사용할 수 있으므로 _SessionLogout 호출 전에 호출

			if ((*itClient)->second.UnLink(Key_))
				_SessionEnd(*itClient);

			_Clients.erase(itClient);
		}
	}
	void CServer::_RecvC(const CKey& Key_, CStream& Stream_)
	{
		SHeader Header;
		Stream_ >> Header;

		auto itClient = _Clients.get(Key_.PeerNum);
		if (itClient)
		{
			switch (Header.Proto)
			{
			case EProto::CsAddFriend: return _RecvCsAddFriend(*itClient, Key_, Stream_);
			case EProto::CsAllowFriend: return _RecvCsAllowFriend(*itClient, Key_, Stream_);
			case EProto::CsDenyFriend: return _RecvCsDenyFriend(*itClient, Key_, Stream_);
			case EProto::CsBlockFriend: return _RecvCsBlockFriend(*itClient, Key_, Stream_);
			case EProto::CsUnBlockFriend: return _RecvCsUnBlockFriend(*itClient, Key_, Stream_);
			case EProto::CsChangeState: return _RecvCsChangeState(*itClient, Key_, Stream_);
			case EProto::CsMessageSend: return _RecvCsMessageSend(*itClient, Key_, Stream_);
			case EProto::CsUserProto: return _RecvFuncC(*itClient, Key_, Stream_);
			default: return _NetC.Close(Key_.PeerNum);
			}
		}
		else
		{
			switch (Header.Proto)
			{
			case EProto::CsLogin: return _RecvCsLogin(Key_, Stream_);
			default: return _NetC.Close(Key_.PeerNum);
			}
		}
	}
	void CServer::_RecvCsLogin(const CKey& Key_, CStream& Stream_)
	{
		// catch 하지 못한 exception 이 있어도 Server가 Master와 끊어지므로 마스터의 세션이 삭제되어 상관 없음.
		SCsLogin Proto;
		Stream_ >> Proto;

		try
		{
			if (Proto.Version.Main != _Version.Main ||
				Proto.Version.Data != _Version.Data)
				throw EGameRet::InvalidVersion;

			// 여기서 SessionTimer의 값을 찾아 삭제하지 않고 SessionTimer는 무조건 TimeOut 처리되고, 그 시점에 필요시 만료가 안되도록 처리

			auto itSession = _Sessions.find(Proto.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidSession;

			CKey DupKey;
			if (!itSession->second.CsLogin(Proto.ID, _AllowDupLogin ? Proto.SubUID : 0, Proto.SessionCode, Key_, DupKey)) // CmMaster에서 클라가 SubUID 를 _AllowDupLogin 에 맞지 않게 보냈다면 여기서 로그인 실패
				throw EGameRet::InvalidSession;

			if (DupKey)
				_NetC.Close(DupKey);

			_Clients.emplace_at(Key_.PeerNum, itSession); // _RecvC 에서 이미 Key_.PeerNum 이 비었을음 체크했으므로 !itClient 체크하지 않음.
			_LinkFuncC(itSession, Key_, Proto.Create, Proto.Stream); // 여기서 exception 날 경우 _UnLinkC 가 호출되어 _Clients, _Session 에서 안전하게 삭제됨.
		}
		catch (const EGameRet GameRet_)
		{
			_NetC.Send(Key_.PeerNum, SHeader(EProto::ScLoginFail), SScLoginFail(GameRet_));
			_NetC.WillClose(Key_.PeerNum, milliseconds(3000));
		}
	}
	void CServer::_RecvCsAddFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
	{
		SCsAddFriend Proto;
		Stream_ >> Proto;

		try
		{
			if (itSession_->second.Account.Nick == Proto.Nick)
				throw EGameRet::InvalidNick;

			_NetM.SendAll(SHeader(EProto::SmAddFriend), SSmAddFriend(Proto.Nick, itSession_->first, itSession_->second.Account.Nick));
		}
		catch (const EGameRet& GameRet_)
		{
			_NetC.Send(Key_.PeerNum, SHeader(EProto::ScError), SScError(GameRet_));
		}
	}
	void CServer::_RecvCsAllowFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
	{
		SCsAllowFriend Proto;
		Stream_ >> Proto;

		try
		{
			auto itFriend = itSession_->second.Friends.find(Proto.FriendUID);
			if (itFriend == itSession_->second.Friends.end())
				throw EGameRet::InvalidUID;

			if (itFriend->second.FriendState != EFriendState::Request)
				throw EGameRet::InvalidUID;

			_NetM.SendAll(SHeader(EProto::SmAllowFriend), SSmAllowFriend(itSession_->first, Proto.FriendUID, Key_));
		}
		catch (const EGameRet& GameRet_)
		{
			_NetC.Send(Key_.PeerNum, SHeader(EProto::ScError), SScError(GameRet_));
		}
	}
	void CServer::_RecvCsDenyFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
	{
		SCsDenyFriend Proto;
		Stream_ >> Proto;

		try
		{
			auto itFriend = itSession_->second.Friends.find(Proto.FriendUID);
			if (itFriend == itSession_->second.Friends.end())
				throw EGameRet::InvalidUID;

			if (itFriend->second.FriendState != EFriendState::Request)
				throw EGameRet::InvalidUID;

			_NetM.SendAll(SHeader(EProto::SmDenyFriend), SSmDenyFriend(itSession_->first, Proto.FriendUID, Key_));
		}
		catch (const EGameRet& GameRet_)
		{
			_NetC.Send(Key_.PeerNum, SHeader(EProto::ScError), SScError(GameRet_));
		}
	}
	void CServer::_RecvCsBlockFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
	{
		SCsBlockFriend Proto;
		Stream_ >> Proto;

		try
		{
			auto itFriend = itSession_->second.Friends.find(Proto.FriendUID);
			if (itFriend == itSession_->second.Friends.end())
				throw EGameRet::InvalidUID;

			if (itFriend->second.FriendState != EFriendState::Normal)
				throw EGameRet::InvalidUID;

			_NetM.SendAll(SHeader(EProto::SmBlockFriend), SSmBlockFriend(itSession_->first, Proto.FriendUID, Key_));
		}
		catch (const EGameRet& GameRet_)
		{
			_NetC.Send(Key_.PeerNum, SHeader(EProto::ScError), SScError(GameRet_));
		}
	}
	void CServer::_RecvCsUnBlockFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
	{
		SCsUnBlockFriend Proto;
		Stream_ >> Proto;

		try
		{
			auto itFriend = itSession_->second.Friends.find(Proto.FriendUID);
			if (itFriend == itSession_->second.Friends.end())
				throw EGameRet::InvalidUID;

			if (itFriend->second.FriendState != EFriendState::Blocked)
				throw EGameRet::InvalidUID;

			_NetM.SendAll(SHeader(EProto::SmUnBlockFriend), SSmUnBlockFriend(itSession_->first, Proto.FriendUID, Key_));
		}
		catch (const EGameRet& GameRet_)
		{
			_NetC.Send(Key_.PeerNum, SHeader(EProto::ScError), SScError(GameRet_));
		}
	}
	void CServer::_RecvCsChangeState(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
	{
		SCsChangeState Proto;
		Stream_ >> Proto;

		_NetM.SendAll(SHeader(EProto::SmChangeState), SSmChangeState(itSession_->first, Proto.State, Key_));
	}
	void CServer::_RecvCsMessageSend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
	{
		SCsMessageSend Proto;
		Stream_ >> Proto;

		try
		{
			auto itFriend = itSession_->second.Friends.find(Proto.ToUID);
			if (itFriend == itSession_->second.Friends.end())
				throw EGameRet::InvalidUID;

			if (itFriend->second.FriendState != EFriendState::Normal)
				throw EGameRet::InvalidUID;

			if (itFriend->second.Info.State == c_Default_State)
				return; // just ok, because State can not be modified by me

			if (itFriend->second.Info.ServerBindNamePort == _ServerBindNamePort)
				_SendScMessageReceived(Proto.ToUID, SScMessageReceived(itSession_->first, Proto.Message));
			else
				_NetS.Send(itFriend->second.Info.ServerBindNamePort, SHeader(EProto::SsMessageSend), SSsMessageSend(itSession_->first, Proto.ToUID, Proto.Message));
		}
		catch (const EGameRet& GameRet_)
		{
			_NetC.Send(Key_.PeerNum, SHeader(EProto::ScError), SScError(GameRet_));
		}
	}

	CServer::CServer(
		EAddressFamily AddressFamily_,
		TCheckFunc CheckFunc_, net::TLinkFunc LinkFuncM_, TLinkFailFunc LinkFailFuncM_, net::TUnLinkFunc UnLinkFuncM_, net::TRecvFunc RecvFuncM_,
		TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TRecvFunc RecvFuncC_,
		TRecvSessionHoldFunc RecvSessionHoldFunc_, TRecvSendToServerFunc RecvSendToServerFunc_, TRecvPunishFunc RecvPunishFunc_,
		TChangeNickFunc ChangeNickFunc_, TDBCallback DBCallbackFunc_,
		const SVersion& Version_, const SDBOption& DBOption_,
		const CNamePort& MasterNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_, TPort ServerBindPort_,
		bool AllowDupLogin_) :
		_CheckFunc(CheckFunc_), _LinkFuncM(LinkFuncM_), _LinkFailFuncM(LinkFailFuncM_), _UnLinkFuncM(UnLinkFuncM_), _RecvFuncM(RecvFuncM_),
		_LinkFuncC(LinkFuncC_), _UnLinkFuncC(UnLinkFuncC_), _RecvFuncC(RecvFuncC_),
		_RecvSessionHoldFunc(RecvSessionHoldFunc_), _RecvSendToServerFunc(RecvSendToServerFunc_), _RecvPunishFunc(RecvPunishFunc_),
		_ChangeNickFunc(ChangeNickFunc_), _DBCallbackFunc(DBCallbackFunc_),
		_Version(Version_),
		_ClientBindNamePortPub(ClientBindNamePortPub_),
		_AllowDupLogin(AllowDupLogin_),
		_NetM(
			std::bind(&CServer::_LinkM, this, _1),
			std::bind(&CServer::_LinkFailM, this, _1, _2),
			std::bind(&CServer::_UnLinkM, this, _1, _2),
			std::bind(&CServer::_RecvM, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 5,
			milliseconds(3000)),
		_NetS(
			EAddressFamily::INET, CNamePort(ServerBindPort_),
			std::bind(&CServer::_LinkS, this, _1),
			std::bind(&CServer::_UnLinkS, this, _1),
			std::bind(&CServer::_RecvS, this, _1),
			true, 0, 65535),
		_NetC(
			AddressFamily_,
			std::bind(&CServer::_LinkC, this, _1),
			std::bind(&CServer::_UnLinkC, this, _1, _2),
			std::bind(&CServer::_RecvC, this, _1, _2),
			true, 10240000, 10240000,
			milliseconds(120000), milliseconds(), NetCThreadCnt_, 0,
			ClientBindNamePort_, SOMAXCONN),
		_DB(DBOption_, std::bind(&CServer::_DBCallback, this, _1))
	{
		_NetM.Connect(MasterNamePort_, 0);
	}
	void CServer::Proc(void)
	{
		_DB.Proc();
		_NetM.Proc();
		_NetS.Proc();
		_NetC.Proc();

		if (_Period.CheckAndNextLoose())
		{
			auto Now = system_clock::now();

			auto it = _SessionTimer.begin();
			for (; it != _SessionTimer.end(); ++it)
			{
				if (it->first > Now)
					break;

				auto itSession = _Sessions.find(it->second.UID);
				if (itSession == _Sessions.end())
					continue;

				if (!itSession->second.TimeOut(it->second.SubUID, it->second.SessionCode))
					continue;

				_SessionEnd(itSession);
			}

			_SessionTimer.erase(_SessionTimer.begin(), it);
		}
	}
	void CServer::Certify(TPeerCnt PeerNum_)
	{
		_NetC.Certify(PeerNum_);
		_NetC.Send(PeerNum_, SHeader(EProto::ScLogin), _Clients[PeerNum_]->second.GetScLogin());
	}
	bool CServer::Certify(const CKey& Key_)
	{
		auto itClient = _Clients.get(Key_.PeerNum);
		if (!itClient)
			return false;

		if (!_NetC.Certify(Key_))
			return false;

		_NetC.Send(Key_.PeerNum, SHeader(EProto::ScLogin), (*itClient)->second.GetScLogin());

		return true;
	}
	void CServer::SetOpened(bool Opened_)
	{
		if (_Opened == Opened_)
			return;

		_Opened = Opened_;
		_NetM.SendAll(SHeader(EProto::SmSetOpened), SSmSetOpened(Opened_));
	}
	bool CServer::ChangeNick(const SSmChangeNick& Proto_)
	{
		auto& Client = _Clients[Proto_.ClientKey.PeerNum];

		if (Client->second.IsChangingNick)
			return false;

		// Master 와 접속이 끊어지면 모든 Client 를 Close 하므로 여기서 Master와의 접속여부 체크할 필요 없음.
		_NetM.SendAll(SHeader(EProto::SmChangeNick), Proto_);
		Client->second.IsChangingNick = true;
		return true;
	}
	void CServer::SendCheck(TSessionsIt itSession_, TSessionCode SessionCode_, const CStream& Stream_)
	{
		const auto Check = itSession_->second.DelCheck(SessionCode_);
		_NetM.Send(itSession_->second.MasterKey, SHeader(EProto::SmCheck), SSmCheck(itSession_->first, Check.UserKey, Check.AuthKey, Stream_));
		_SessionEnd(itSession_);
	}
}
