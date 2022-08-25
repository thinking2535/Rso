#include "Master.h"

namespace rso::game
{
	TState CMaster::_GetState(TState State_, EFriendState FriendState_)
	{
		return (FriendState_ == EFriendState::Normal ? State_ : c_Default_State);
	}
	void CMaster::_McLoginFail(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		_NetC.Send(PeerNum_, SHeader(EProto::McLoginFail), SMcLoginFail(GameRet_));
		_NetC.WillClose(PeerNum_, milliseconds(3000));
	}
	void CMaster::_DBCallback(TOutObject& OutObject_)
	{
		if (OutObject_.SPNum < _DBCallbacks.size())
		{
			_DBCallbacks[OutObject_.SPNum](OutObject_);
		}
		else
		{
			OutObject_.SPNum -= _DBCallbacks.size();
			_DBCallbackFunc(OutObject_);
		}
	}
	void CMaster::_EraseFriendStatesForSeeInMaster(const TRangeUID& RangeUID_)
	{
		for (auto it = _FriendInfosForSee.begin(); it != _FriendInfosForSee.end(); )
		{
			auto itCheck = it;
			++it;

			for (auto itFriend = itCheck->second.begin(); itFriend != itCheck->second.end(); )
			{
				auto itFriendCheck = itFriend;
				++itFriend;

				if (!RangeUID_.IsValid(itFriendCheck->first))
					continue;

				itCheck->second.erase(itFriendCheck);
			}

			if (itCheck->second.empty())
				_FriendInfosForSee.erase(itCheck);
		}
	}
	void CMaster::_InsertFriendState(const SMmFriendStateChanged& Proto_)
	{
		_TUIDFriendInfosForNet UIDFriendInfosForMe;

		for (auto& i : Proto_.UIDFriendInfos)
		{
			auto itFriendInfos = _FriendInfosForSee.find(i.UID);
			if (itFriendInfos != _FriendInfosForSee.end())
			{
				if (i.FriendInfo.State == c_Default_State)
				{
					itFriendInfos->second.erase(Proto_.FriendUID);
					if (itFriendInfos->second.empty())
						_FriendInfosForSee.erase(itFriendInfos);
				}
				else
				{
					itFriendInfos->second.insert_or_assign(Proto_.FriendUID, i.FriendInfo);
				}
			}
			else
			{
				if (i.FriendInfo.State != 0)
				{
					auto ibFriendStates = _FriendInfosForSee.emplace(i.UID, TFriendInfos());
					ibFriendStates.first->second.insert_or_assign(Proto_.FriendUID, i.FriendInfo);
				}
			}

			auto itSession = _Sessions.find(i.UID);
			if (itSession == _Sessions.end())
				continue;

			auto ib = UIDFriendInfosForMe.emplace(itSession->second.ServerKey.PeerNum, TUIDFriendInfos());
			ib.first->second.emplace_back(i);
		}

		for (auto& i : UIDFriendInfosForMe)
			_NetS.Send(i.first, SHeader(EProto::MsFriendStateChanged), SMsFriendStateChanged(i.second, Proto_.FriendUID));
	}
	void CMaster::_SendMsChangeNickFail(const CKey& ServerKey_, const SMsChangeNickFail& Proto_)
	{
		_NetS.Send(ServerKey_, SHeader(EProto::MsChangeNickFail), Proto_);
	}
	void CMaster::_SendMsAddFriendFail(const CKey& ServerKey_, const SMsAddFriendFail& Proto_)
	{
		_NetS.Send(ServerKey_, SHeader(EProto::MsAddFriendFail), Proto_);
	}
	void CMaster::_SendMaAddFriend(const SMaAddFriend& Proto_)
	{
		_NetA.SendAll(SHeader(EProto::MaAddFriend), Proto_);
	}
	void CMaster::_StateChangedByMe(TSessionsIt itSession_, TState OldState_)
	{
		TUIDFriendInfos UIDFriendInfosForMe;
		_TUIDFriendInfosForNet UIDFriendInfosForNet;

		for (auto it = itSession_->second.Friends.begin(); it != itSession_->second.Friends.end(); ++it)
			_GetUIDFriendInfos(itSession_, it, UIDFriendInfosForMe, UIDFriendInfosForNet, _GetState(OldState_, it->second));

		_StateChanged(itSession_, UIDFriendInfosForMe, UIDFriendInfosForNet);
	}
	void CMaster::_StateChangedByFriend(TSessionsIt itSession_, _TFriendsIt itFriend_, EFriendState OldFriendState_)
	{
		TUIDFriendInfos UIDFriendInfosForMe;
		_TUIDFriendInfosForNet UIDFriendInfosForNet;

		_GetUIDFriendInfos(itSession_, itFriend_, UIDFriendInfosForMe, UIDFriendInfosForNet, _GetState(itSession_->second.User.State, OldFriendState_));
		_StateChanged(itSession_, UIDFriendInfosForMe, UIDFriendInfosForNet);
	}
	void CMaster::_StateChanged(TSessionsIt itSession_, const TUIDFriendInfos& UIDFriendInfosForMe_, const _TUIDFriendInfosForNet& UIDFriendInfosForNet_)
	{
		_InsertFriendState(SMmFriendStateChanged(UIDFriendInfosForMe_, itSession_->first));

		for (auto& i : UIDFriendInfosForNet_)
			_NetM.Send(std::get<1>(_OtherMasters[i.first].first)->first, SHeader(EProto::MmFriendStateChanged), SMmFriendStateChanged(i.second, itSession_->first));
	}
	void CMaster::_GetUIDFriendInfos(TSessionsIt itSession_, _TFriendsIt itFriend_, TUIDFriendInfos& UIDFriendInfosForMe_, _TUIDFriendInfosForNet& UIDFriendInfosForNet_, TState OldState_)
	{
		const auto NewState = _GetState(itSession_->second.User.State, itFriend_->second);
		if (OldState_ == NewState)
			return;

		SNamePort ServerBindNamePort;
		if (NewState != c_Default_State)
			ServerBindNamePort = _Servers[itSession_->second.ServerKey.PeerNum].ServerBindNamePort;

		if (_RangeUID.IsValid(itFriend_->first))
		{
			UIDFriendInfosForMe_.emplace_back(SUIDFriendInfo(itFriend_->first, SFriendInfo(ServerBindNamePort, NewState)));
		}
		else
		{
			auto itMaster = _OtherMasters.find<0>(TRangeUID(itFriend_->first));
			if (itMaster == _OtherMasters.end<0>())
				return;

			auto ib = UIDFriendInfosForNet_.emplace(itMaster->second, TUIDFriendInfos());
			ib.first->second.emplace_back(SUIDFriendInfo(itFriend_->first, SFriendInfo(ServerBindNamePort, NewState)));
		}
	}
	bool CMaster::_IsMyUID(TUID UID_) const
	{
		return (UID_ >= _RangeUID.MinValue && UID_ < _RangeUID.MaxValue);
	}
	auto CMaster::_GetServer(TPeerCnt ServerNum_)
	{
		auto itServer = _Servers.end();

		if (ServerNum_ == c_PeerCnt_Null)
		{
			// Alloc Server (multimap 으로 서버 인원수 체크 할 경우 매번 재 삽입 해야 하므로 그냥 여기서 순회하면서 인원수 체크 할당)
			for (auto it = _Servers.begin(); it != _Servers.end(); ++it)
			{
				if (!it->Opened)
					continue;

				if (itServer != _Servers.end())
				{
					if (it->Sessions.size() < itServer->Sessions.size())
						itServer = it;
				}
				else
				{
					itServer = it;
				}
			}
		}
		else
		{
			itServer = _Servers.get(ServerNum_);
		}

		if (!itServer)
			throw EGameRet::NoServerToConnect;

		return itServer;
	}
	void CMaster::_SessionEnd(TSessionsIt itSession_)
	{
		const auto OldState = itSession_->second.User.State;
		itSession_->second.User.State = c_Default_State;
		_StateChangedByMe(itSession_, OldState);

		auto UID = itSession_->first;
		auto ServerNum = itSession_->second.ServerKey.PeerNum;
		_Sessions.erase(itSession_);
		_SessionEndFunc(ServerNum, UID);
	}
	CMaster::TSessionsIt CMaster::_SessionBegin(TUID UID_, TPeerCnt ServerNum_, const SMasterUser& User_, const _TFriends& Friends_)
	{
		auto itSession = _Sessions.find(UID_);
		if (itSession == _Sessions.end()) // 새로 할당
		{
			auto itServer = _GetServer(ServerNum_);
			auto ib = _Sessions.emplace(UID_, _SClient(User_, Friends_, _SessionCounter + 1, itServer->Key));

			try
			{
				itServer->Sessions.emplace(UID_, ib.first);
			}
			catch (...)
			{
				_Sessions.erase(ib.first);
				throw EGameRet::AddSessionFail;
			}

			itSession = ib.first;
			_SessionBeginFunc(ib.first->second.ServerKey.PeerNum, UID_);
		}
		else
		{
			itSession->second.SessionCode = _SessionCounter + 1;
			itSession->second.User = User_;
			itSession->second.Friends = Friends_;
		}

		++_SessionCounter;

		return itSession;
	}
	void CMaster::_SessionLogin(TUID UID_, TUID SubUID_, const TID& ID_, const SMasterUser& MasterUser_, const TFriendDBs& Friends_, const CKey& ClientKey_)
	{
		_TFriends FriendsForSession;
		for (auto& f : Friends_)
		{
			if (f.second.FriendState == EFriendState::Adding)
				continue;

			FriendsForSession.emplace(f.first, f.second.FriendState);
		}

		auto itSession = _SessionBegin(UID_, c_PeerCnt_Null, MasterUser_, FriendsForSession);
		TFriends FriendsForNet;

		auto itMyStates = _FriendInfosForSee.find(UID_);

		// if there are some friends not finished to add, then keep going
		for (auto& f : Friends_)
		{
			if (f.second.FriendState == EFriendState::Adding)
			{
				_SendMaAddFriend(SMaAddFriend(f.first, f.second.Nick, UID_, MasterUser_.Nick, itSession->second.ServerKey));
			}
			else
			{
				SFriendInfo Info;

				if (itMyStates != _FriendInfosForSee.end())
				{
					auto itState = itMyStates->second.find(f.first);
					if (itState != itMyStates->second.end())
						Info = itState->second;
				}

				FriendsForNet.emplace(f.first, SFriend(f.second, Info));
			}
		}

		// Send to me that friend states
		_NetS.Send(itSession->second.ServerKey.PeerNum, SHeader(EProto::MsLogin), SMsLogin(UID_, SAccount(ID_, MasterUser_.Nick), SubUID_, FriendsForNet, ClientKey_, itSession->second.SessionCode));

		// Send to my friends that my states
		_StateChangedByMe(itSession, c_Default_State);
	}
	void CMaster::_SessionCheck(TUID UID_, const CKey& UserKey_, const CKey& AuthKey_)
	{
		auto itSession = _SessionBegin(UID_, c_PeerCnt_Null, SMasterUser(), _TFriends());
		_NetS.Send(itSession->second.ServerKey.PeerNum, SHeader(EProto::MsCheck), SMsCheck(UID_, UserKey_, AuthKey_, itSession->second.SessionCode));
	}
	void CMaster::_DBCreate(TOutObject& OutObject_)
	{
		auto itAuth = _Auths.get(OutObject_.Key.PeerNum);
		if (!itAuth)
			return;

		if (*itAuth != OutObject_.Key)
			return;

		SMasterCreateInDb In;
		SKey UserKey;
		OutObject_.InStream >> In;
		OutObject_.InStream >> UserKey;

		try
		{
			if (OutObject_.Ret != 0)
				throw EGameRet::DataBaseError;

			if (OutObject_.SPRet == 2601 || OutObject_.SPRet == 2627)
				throw EGameRet::AlreadyExist;
			else if (OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			SMasterCreateOutDb Out;
			OutObject_.OutStream >> Out;

			_NetA.Send(OutObject_.Key, SHeader(EProto::MaCreate), SMaCreate(UserKey, In.UID));
		}
		catch (const EGameRet GameRet_)
		{
			_NetA.Send(OutObject_.Key, SHeader(EProto::MaCreateFail), SMaCreateFail(UserKey, GameRet_));
		}
	}
	void CMaster::_DBLogin(TOutObject& OutObject_)
	{
		auto itClient = _Clients.get(OutObject_.Key.PeerNum);
		if (!itClient)
			return;

		if (*itClient != OutObject_.Key)
			return;

		SMasterLoginInDb In;
		OutObject_.InStream >> In;

		TUID SubUID = 0;
		OutObject_.InStream >> SubUID;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			SMasterLoginOutDb Out;
			OutObject_.OutStream >> Out;

			if (Out.Users.empty())
				throw EGameRet::InvalidID;

			if (Out.Users.front().PunishEndTime > steady_now())
				throw EGameRet::Punished;

			_SessionLogin(In.UID, SubUID, In.ID, Out.Users.front(), Out.Friends, OutObject_.Key);
		}
		catch (const EGameRet GameRet_)
		{
			_McLoginFail(OutObject_.Key.PeerNum, GameRet_);
		}
	}
	void CMaster::_DBChangeNick(TOutObject& OutObject_)
	{
		SMasterChangeNickInDb In;
		OutObject_.InStream >> In;

		SAmChangeNick Proto;
		OutObject_.InStream >> Proto;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			_NetS.Send(Proto.ServerKey, SHeader(EProto::MsChangeNick), static_cast<SMsChangeNick&>(Proto)); // Proto.ServerKey 가 다르면 Send 안되므로 _Servers 에서 찾을 필요 없음.
		}
		catch (const EGameRet GameRet_)
		{
			_SendMsChangeNickFail(Proto.ServerKey, SMsChangeNickFail(GameRet_, Proto.ClientKey, In.UID));
		}
	}
	void CMaster::_DBAddFriendBegin(TOutObject& OutObject_)
	{
		SMasterAddFriendBeginInDb In;
		OutObject_.InStream >> In;

		TNick FromNick;
		SKey FromServerKey;
		OutObject_.InStream >> FromNick;
		OutObject_.InStream >> FromServerKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			_SendMaAddFriend(SMaAddFriend(In.FriendUID, In.FriendNick, In.UID, FromNick, FromServerKey));
		}
		catch (const EGameRet GameRet_)
		{
			_SendMsAddFriendFail(FromServerKey, SMsAddFriendFail(GameRet_, In.UID));
		}
	}
	void CMaster::_DBAddFriendRequest(TOutObject& OutObject_)
	{
		SMasterAddFriendRequestInDb In;
		OutObject_.InStream >> In;

		TNick ToNick;
		TUID FromUID;
		SKey FromServerKey;
		SKey FromMasterKey;
		OutObject_.InStream >> ToNick;
		OutObject_.InStream >> FromUID;
		OutObject_.InStream >> FromServerKey;
		OutObject_.InStream >> FromMasterKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			SMasterAddFriendRequestOutDb Out;
			OutObject_.OutStream >> Out;

			if (Out.RowCounts.front() > 0)
			{
				auto itSession = _Sessions.find(In.UID);
				if (itSession != _Sessions.end())
				{
					itSession->second.Friends.emplace(In.FriendUID, In.FriendState);
					_NetS.Send(itSession->second.ServerKey.PeerNum, SHeader(EProto::MsAddFriendRequest), SMsAddFriendRequest(In.UID, In.FriendUID, In.FriendNick));
				}
			}

			_NetA.SendAll(SHeader(EProto::MaAddFriendRequest), SMaAddFriendRequest(In.UID, ToNick, In.FriendUID, FromServerKey, FromMasterKey));
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(OutObject_.Key, SHeader(EProto::MaAddFriendRequestFail), SMaAddFriendRequestFail(GameRet_, In.UID, FromUID, FromServerKey, FromMasterKey));
		}
	}
	void CMaster::_DBAddFriendEnd(TOutObject& OutObject_)
	{
		SMasterAddFriendEndInDb In;
		OutObject_.InStream >> In;

		TNick ToNick;
		SKey FromServerKey;
		OutObject_.InStream >> ToNick;
		OutObject_.InStream >> FromServerKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			auto itSession = _Sessions.find(In.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidSession;

			auto ibFriend = itSession->second.Friends.emplace(In.FriendUID, In.FriendState);
			_NetS.Send(FromServerKey, SHeader(EProto::MsAddFriend), SMsAddFriend(In.FriendUID, ToNick, In.UID));
			_StateChangedByFriend(itSession, ibFriend.first, EFriendState::Adding);
		}
		catch (const EGameRet GameRet_)
		{
			_SendMsAddFriendFail(FromServerKey, SMsAddFriendFail(GameRet_, In.UID));
		}
	}
	void CMaster::_DBAddFriendFail(TOutObject& OutObject_)
	{
		SMasterAddFriendFailInDb In;
		OutObject_.InStream >> In;

		EGameRet GameRet = EGameRet::Null;
		SKey FromServerKey;
		OutObject_.InStream >> GameRet;
		OutObject_.InStream >> FromServerKey;

		if (OutObject_.Ret != 0)
			return;

		_SendMsAddFriendFail(FromServerKey, SMsAddFriendFail(GameRet, In.UID));
	}
	void CMaster::_DBAllowFriend(TOutObject& OutObject_)
	{
		SMasterAllowFriendInDb In;
		OutObject_.InStream >> In;

		SKey UserKey;
		OutObject_.InStream >> UserKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			auto itSession = _Sessions.find(In.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidSession;

			auto itFriend = itSession->second.Friends.find(In.FriendUID);
			if (itFriend == itSession->second.Friends.end())
				throw EGameRet::InvalidUID;

			const auto OldFriendState = itFriend->second;
			itFriend->second = In.FriendState;

			_NetS.Send(OutObject_.Key, SHeader(EProto::MsAllowFriend), SMsAllowFriend(In.UID, In.FriendUID, UserKey));
			_StateChangedByFriend(itSession, itFriend, OldFriendState);
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsAllowFriendFail), SMsAllowFriendFail(GameRet_, UserKey));
		}
	}
	void CMaster::_DBDenyFriend(TOutObject& OutObject_)
	{
		SMasterDenyFriendInDb In;
		OutObject_.InStream >> In;

		SKey UserKey;
		OutObject_.InStream >> UserKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			_NetS.Send(OutObject_.Key, SHeader(EProto::MsDenyFriend), SMsDenyFriend(In.UID, In.FriendUID, UserKey));
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsDenyFriendFail), SMsDenyFriendFail(GameRet_, UserKey));
		}
	}
	void CMaster::_DBBlockFriend(TOutObject& OutObject_)
	{
		SMasterBlockFriendInDb In;
		OutObject_.InStream >> In;

		SKey UserKey;
		OutObject_.InStream >> UserKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			auto itSession = _Sessions.find(In.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidSession;

			auto itFriend = itSession->second.Friends.find(In.FriendUID);
			if (itFriend == itSession->second.Friends.end())
				throw EGameRet::InvalidUID;

			const auto OldFriendState = itFriend->second;
			itFriend->second = In.FriendState;
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsBlockFriend), SMsBlockFriend(In.UID, In.FriendUID, In.FriendState, UserKey));
			_StateChangedByFriend(itSession, itFriend, OldFriendState);
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsBlockFriendFail), SMsBlockFriendFail(GameRet_, UserKey));
		}
	}
	void CMaster::_DBUnBlockFriend(TOutObject& OutObject_)
	{
		SMasterUnBlockFriendInDb In;
		OutObject_.InStream >> In;

		SKey UserKey;
		OutObject_.InStream >> UserKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			auto itSession = _Sessions.find(In.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidSession;

			auto itFriend = itSession->second.Friends.find(In.FriendUID);
			if (itFriend == itSession->second.Friends.end())
				throw EGameRet::InvalidUID;

			const auto OldFriendState = itFriend->second;
			itFriend->second = In.FriendState;
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsUnBlockFriend), SMsUnBlockFriend(In.UID, In.FriendUID, In.FriendState, UserKey));
			_StateChangedByFriend(itSession, itFriend, OldFriendState);
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsUnBlockFriendFail), SMsUnBlockFriendFail(GameRet_, UserKey));
		}
	}
	void CMaster::_DBChangeState(TOutObject& OutObject_)
	{
		SMasterChangeStateInDb In;
		OutObject_.InStream >> In;

		SKey UserKey;
		OutObject_.InStream >> UserKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			auto itSession = _Sessions.find(In.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidSession;

			const auto OldState = itSession->second.User.State;
			itSession->second.User.State = In.State;
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsChangeState), SMsChangeState(In.State, UserKey));

			_StateChangedByMe(itSession, OldState);
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(OutObject_.Key, SHeader(EProto::MsChangeStateFail), SMsChangeStateFail(GameRet_, UserKey));
		}
	}
	void CMaster::_DBCheck(TOutObject& OutObject_)
	{
		auto itAuth = _Auths.get(OutObject_.Key.PeerNum);
		if (!itAuth)
			return;

		if (*itAuth != OutObject_.Key)
			return;

		SMasterCheckInDb In;
		OutObject_.InStream >> In;

		SKey UserKey;
		OutObject_.InStream >> UserKey;

		try
		{
			if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
				throw EGameRet::DataBaseError;

			SMasterCheckOutDb Out;
			OutObject_.OutStream >> Out;

			if (Out.PunishEndTimes.empty())
				throw EGameRet::InvalidID;

			if (Out.PunishEndTimes.front() > steady_now())
				throw EGameRet::Punished;

			_SessionCheck(In.UID, UserKey, OutObject_.Key);
		}
		catch (const EGameRet GameRet_)
		{
			_NetA.Send(itAuth->PeerNum, SHeader(EProto::MaCheckFail), SMaCheckFail(UserKey, GameRet_));
		}
	}
	void CMaster::_DBPunish(TOutObject& OutObject_)
	{
		SMasterPunishInDb In;
		OutObject_.InStream >> In;

		SMasterPunishOutDb Out;
		OutObject_.InStream >> Out;
	}

	void CMaster::_LinkA(const CKey& Key_)
	{
		_Auths.emplace_at(Key_.PeerNum, Key_);

		TServerNets Servers;
		for (auto& s : _Servers)
			Servers.emplace(SServerNet(s.ServerBindNamePort));

		_NetA.Send(Key_.PeerNum, SHeader(EProto::MaMasterOn), SMaMasterOn(SMasterNet(_RangeUID, std::move(Servers)), _ClientBindNamePortPub, _NetM.GetPort()));
		_LinkFuncA(Key_);
	}
	void CMaster::_LinkFailA(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_LinkFailFuncA(PeerNum_, NetRet_);
	}
	void CMaster::_UnLinkA(const CKey& Key_, ENetRet NetRet_)
	{
		for (auto& i : _ChangeNickClients)
			_SendMsChangeNickFail(i.second.ServerKey, SMsChangeNickFail(EGameRet::AuthDisconnected, i.second.ClientKey, i.first));

		_ChangeNickClients.clear();

		if (_Auths.erase(Key_.PeerNum))
			_UnLinkFuncA(Key_, NetRet_);
		else
			_LinkFailFuncA(Key_.PeerNum, NetRet_);
	}
	void CMaster::_RecvA(const CKey& Key_, CStream& Stream_)
	{
		auto itAuth = _Auths.get(Key_.PeerNum);
		if (!itAuth)
			return;

		SHeader Header;
		Stream_ >> Header;

		switch (Header.Proto)
		{
		case EProto::AmMasterOn: return _RecvAmMasterOn(Key_, Stream_);
		case EProto::AmOtherMasterOn: return _RecvAmOtherMasterOn(Key_, Stream_);
		case EProto::AmOtherMasterOff: return _RecvAmOtherMasterOff(Key_, Stream_);
		case EProto::AmOtherMasterServerOn: return _RecvAmOtherMasterServerOn(Key_, Stream_);
		case EProto::AmOtherMasterServerOff: return _RecvAmOtherMasterServerOff(Key_, Stream_);
		case EProto::AmCreate: return _RecvAmCreate(itAuth, Stream_);
		case EProto::AmChangeNick: return _RecvAmChangeNick(itAuth, Stream_);
		case EProto::AmChangeNickFail: return _RecvAmChangeNickFail(itAuth, Stream_);
		case EProto::AmCheck: return _RecvAmCheck(itAuth, Stream_);
		case EProto::AmAddFriendGetUID: return _RecvAmAddFriendGetUID(itAuth, Stream_);
		case EProto::AmAddFriendGetUIDFail: return _RecvAmAddFriendGetUIDFail(itAuth, Stream_);
		case EProto::AmAddFriendRequest: return _RecvAmAddFriendRequest(itAuth, Stream_);
		case EProto::AmAddFriend: return _RecvAmAddFriend(itAuth, Stream_);
		case EProto::AmAddFriendFail: return _RecvAmAddFriendFail(itAuth, Stream_);
		case EProto::AmToServer: return _RecvAmToServer(itAuth, Stream_);
		case EProto::AmPunish: return _RecvAmPunish(itAuth, Stream_);
		case EProto::AmUserProto: return _RecvFuncA(Key_, Stream_);
		default: return _NetA.Close(Key_.PeerNum);
		}
	}
	void CMaster::_RecvAmMasterOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SAmMasterOn Proto;
		Stream_ >> Proto;

		// clear ///////////////////
		for (auto& m : _OtherMasters)
			_NetM.ErasePeer(std::get<1>(m.first)->first);

		_OtherMasters.clear();
		////////////////////////////////////////

		for (auto& i : Proto.OtherMasters)
		{
			_OtherMasters.emplace(_SMaster(i.Servers), TRangeUID(i.RangeUID), CNamePort(i.MasterBindNamePort));
			_NetM.InsertPeer(i.MasterBindNamePort);
		}

		_NetS.SendAll(SHeader(EProto::MsMasterOn), SMsMasterOn(Proto.OtherMasters));
	}
	void CMaster::_RecvAmOtherMasterOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SAmOtherMasterOn Proto;
		Stream_ >> Proto;

		_OtherMasters.emplace(_SMaster(Proto.Master.Servers), TRangeUID(Proto.Master.RangeUID), CNamePort(Proto.Master.MasterBindNamePort));
		_NetM.InsertPeer(Proto.Master.MasterBindNamePort);
		_NetS.SendAll(SHeader(EProto::MsOtherMasterOn), SMsOtherMasterOn(SMasterNet(Proto.Master.RangeUID, Proto.Master.Servers)));
	}
	void CMaster::_RecvAmOtherMasterOff(const CKey& /*Key_*/, CStream& Stream_)
	{
		SAmOtherMasterOff Proto;
		Stream_ >> Proto;

		auto itMaster = _OtherMasters.find<0>(TRangeUID(Proto.MasterRangeUID));
		if (itMaster == _OtherMasters.end<0>())
			return;

		_NetM.ErasePeer(std::get<1>(_OtherMasters[itMaster->second].first)->first);
		_NetS.SendAll(SHeader(EProto::MsOtherMasterOff), SMsOtherMasterOff(Proto.MasterRangeUID));
		_OtherMasters.erase(itMaster->second);
	}
	void CMaster::_RecvAmOtherMasterServerOn(const CKey& /*Key_*/, CStream& Stream_)
	{
		SAmOtherMasterServerOn Proto;
		Stream_ >> Proto;

		auto itMaster = _OtherMasters.find<0>(TRangeUID(Proto.MasterRangeUID));
		if (itMaster == _OtherMasters.end<0>())
			return;

		_OtherMasters[itMaster->second].second.Servers.emplace(CNamePort(Proto.Server.ServerBindNamePort));
		_NetS.SendAll(SHeader(EProto::MsOtherMasterServerOn), SMsOtherMasterServerOn(Proto.MasterRangeUID, Proto.Server));
	}
	void CMaster::_RecvAmOtherMasterServerOff(const CKey& /*Key_*/, CStream& Stream_)
	{
		SAmOtherMasterServerOff Proto;
		Stream_ >> Proto;

		auto itMaster = _OtherMasters.find<0>(Proto.MasterRangeUID);
		if (itMaster == _OtherMasters.end<0>())
			return;

		_OtherMasters[itMaster->second].second.Servers.erase(Proto.Server);
		_NetS.SendAll(SHeader(EProto::MsOtherMasterServerOff), SMsOtherMasterServerOff(Proto.MasterRangeUID, Proto.Server));
	}
	void CMaster::_RecvAmCreate(_TAuthsIt itAuth_, CStream& Stream_)
	{
		SAmCreate Proto;
		Stream_ >> Proto;

		_DBPush(*itAuth_, SMasterCreateInDb(Proto.UID, Proto.Account, Proto.State), Proto.UserKey);
	}
	void CMaster::_RecvAmChangeNick(_TAuthsIt itAuth_, CStream& Stream_)
	{
		SAmChangeNick Proto;
		Stream_ >> Proto;

		_ChangeNickClients.erase(Proto.UID);
		_DBPush(*itAuth_, SMasterChangeNickInDb(Proto.UID, Proto.Nick), Proto);
	}
	void CMaster::_RecvAmChangeNickFail(_TAuthsIt /*itAuth_*/, CStream& Stream_)
	{
		SAmChangeNickFail Proto;
		Stream_ >> Proto;

		_ChangeNickClients.erase(Proto.UID);
		_SendMsChangeNickFail(Proto.ServerKey, SMsChangeNickFail(Proto.GameRet, Proto.ClientKey, Proto.UID));
	}
	void CMaster::_RecvAmCheck(_TAuthsIt itAuth_, CStream& Stream_)
	{
		SAmCheck Proto;
		Stream_ >> Proto;

		_DBPush(*itAuth_, SMasterCheckInDb(Proto.UID), Proto.UserKey);
	}
	void CMaster::_RecvAmAddFriendGetUID(_TAuthsIt itAuth_, CStream& Stream_)
	{
		SAmAddFriendGetUID Proto;
		Stream_ >> Proto;

		_DBPush(*itAuth_, SMasterAddFriendBeginInDb(Proto.FromUID, Proto.ToUID, Proto.ToNick, EFriendState::Adding), Proto.FromNick, Proto.FromServerKey);
	}
	void CMaster::_RecvAmAddFriendGetUIDFail(_TAuthsIt /*itAuth_*/, CStream& Stream_)
	{
		SAmAddFriendGetUIDFail Proto;
		Stream_ >> Proto;

		_SendMsAddFriendFail(Proto.FromServerKey, SMsAddFriendFail(Proto.GameRet, Proto.FromUID));
	}
	void CMaster::_RecvAmAddFriendRequest(_TAuthsIt itAuth_, CStream& Stream_)
	{
		SAmAddFriendRequest Proto;
		Stream_ >> Proto;

		_DBPush(*itAuth_, SMasterAddFriendRequestInDb(Proto.ToUID, Proto.FromUID, Proto.FromNick, EFriendState::Request), Proto.ToNick, Proto.FromUID, Proto.FromServerKey, Proto.FromMasterKey);
	}
	void CMaster::_RecvAmAddFriend(_TAuthsIt itAuth_, CStream& Stream_)
	{
		SAmAddFriend Proto;
		Stream_ >> Proto;

		_DBPush(*itAuth_, SMasterAddFriendEndInDb(Proto.FromUID, Proto.ToUID, EFriendState::Normal), Proto.ToNick, Proto.FromServerKey);
	}
	void CMaster::_RecvAmAddFriendFail(_TAuthsIt itAuth_, CStream& Stream_)
	{
		SAmAddFriendFail Proto;
		Stream_ >> Proto;

		_DBPush(*itAuth_, SMasterAddFriendFailInDb(Proto.FromUID, Proto.ToUID), Proto.GameRet, Proto.FromServerKey);
	}
	void CMaster::_RecvAmToServer(_TAuthsIt /*itAuth_*/, CStream& Stream_)
	{
		TUID UID = 0;
		Stream_ >> UID;

		CStream Stream;
		Stream_ >> Stream;

		// 아래 함수는 오류 나더라도 외부에 통보하지 않음.
		// 서버간 통신은 오류 허용
		SendToServerStream(UID, Stream);
	}
	void CMaster::_RecvAmPunish(_TAuthsIt /*itAuth_*/, CStream& Stream_)
	{
		SAmPunish Proto;
		Stream_ >> Proto;

		Punish(Proto.UID, Proto.EndTime);
	}

	void CMaster::_LinkM(TIPEndPoint IPEndPoint_)
	{
		auto itMaster = _OtherMasters.find<1>(IPEndPoint_->GetNamePort());
		if (itMaster == _OtherMasters.end<1>())
			return;

		const auto& RangeUID = std::get<0>(_OtherMasters[itMaster->second].first)->first;

		list<SMmFriendStateChanged> MmFriendStateChangeds;
		for (auto& s : _Sessions)
		{
			const auto& ServerBindNamePort = _Servers[s.second.ServerKey.PeerNum].ServerBindNamePort;

			TUIDFriendInfos UIDFriendInfos;

			for (auto& f : s.second.Friends)
			{
				if (!RangeUID.IsValid(f.first))
					continue;

				const auto NewState = _GetState(s.second.User.State, f.second);
				if (NewState == c_Default_State)
					continue;

				UIDFriendInfos.emplace_back(SUIDFriendInfo(f.first, SFriendInfo(ServerBindNamePort, NewState)));
			}

			MmFriendStateChangeds.emplace_back(SMmFriendStateChanged(std::move(UIDFriendInfos), s.first));
		}

		_NetM.Send(IPEndPoint_->GetNamePort(), SHeader(EProto::MmFriendStateChangedRenew), SMmFriendStateChangedRenew(std::move(MmFriendStateChangeds)));
	}
	void CMaster::_UnLinkM(TIPEndPoint IPEndPoint_)
	{
		auto itMaster = _OtherMasters.find<1>(IPEndPoint_->GetNamePort());
		if (itMaster == _OtherMasters.end<1>())
			return;

		map<TPeerCnt, map<TUID, list<TUID>>> ServersHavingFriends;

		const auto& RangeUID = std::get<0>(_OtherMasters[itMaster->second].first)->first;
		for (auto& s : _Sessions)
		{
			auto itMyStates = _FriendInfosForSee.find(s.first);
			if (itMyStates == _FriendInfosForSee.end())
				continue;

			list<TUID> Friends;
			for (auto& f : s.second.Friends)
			{
				if (!RangeUID.IsValid(f.first))
					continue;

				auto itFriendState = itMyStates->second.find(f.first);
				if (itFriendState == itMyStates->second.end())
					continue;

				if (itFriendState->second.State == c_Default_State) // NowState is c_Default_State, so do not need to Notify
					continue;

				itFriendState->second.State = c_Default_State;
				Friends.emplace_back(f.first);
			}

			if (!Friends.empty())
			{
				auto ibFriendsInServer = ServersHavingFriends.emplace(s.second.ServerKey.PeerNum, map<TUID, list<TUID>>());
				ibFriendsInServer.first->second.emplace(s.first, std::move(Friends));
			}
		}

		for (auto& i : ServersHavingFriends)
			_NetS.Send(i.first, SHeader(EProto::MsFriendStateChangedOffline), SMsFriendStateChangedOffline(std::move(i.second)));

		_EraseFriendStatesForSeeInMaster(RangeUID);
	}
	void CMaster::_RecvM(CUDP::SRecvData& Data_)
	{
		SHeader Header;
		Data_.Stream >> Header;

		switch (Header.Proto)
		{
		case EProto::MmFriendStateChanged: return _RecvMmFriendStateChanged(Data_);
		case EProto::MmFriendStateChangedRenew: return _RecvMmFriendStateChangedRenew(Data_);
		default: THROWEX();
		}
	}
	void CMaster::_RecvMmFriendStateChanged(CUDP::SRecvData& Data_)
	{
		SMmFriendStateChanged Proto;
		Data_.Stream >> Proto;

		_InsertFriendState(Proto);
	}
	void CMaster::_RecvMmFriendStateChangedRenew(CUDP::SRecvData& Data_)
	{
		auto itMaster = _OtherMasters.find<1>(Data_.IPEndPoint->GetNamePort());
		if (itMaster == _OtherMasters.end<1>())
			return;

		// whatever received this protocol, _UnLinkM had not been called (it means that _LinkM have been called in there)
		SMmFriendStateChangedRenew Proto;
		Data_.Stream >> Proto;

		_EraseFriendStatesForSeeInMaster(std::get<0>(_OtherMasters[itMaster->second].first)->first);

		for (auto& i : Proto.MmFriendStateChangeds)
			_InsertFriendState(i);
	}

	void CMaster::_LinkS(const CKey& /*Key_*/)
	{
	}
	void CMaster::_UnLinkS(const CKey& Key_, ENetRet NetRet_)
	{
		auto itServer = _Servers.get(Key_.PeerNum);
		if (itServer)
		{
			for (auto it = itServer->Sessions.begin(); it != itServer->Sessions.end(); ++it)
				_SessionEnd(it->second);

			SServerNet Server(itServer->ServerBindNamePort);
			_NetA.SendAll(SHeader(EProto::MaServerOff), SMaServerOff(Server));
			_NetS.SendAll(SHeader(EProto::MsOtherServerOff), SMsOtherServerOff(Server));
			_Servers.erase(itServer);
			_UnLinkFuncS(Key_, NetRet_);
		}
	}
	void CMaster::_RecvS(const CKey& Key_, CStream& Stream_)
	{
		SHeader Header;
		Stream_ >> Header;

		auto itServer = _Servers.get(Key_.PeerNum);
		if (itServer)
		{
			switch (Header.Proto)
			{
			case EProto::SmLogin: return _RecvSmLogin(itServer, Stream_);
			case EProto::SmChangeNick: return _RecvSmChangeNick(itServer, Stream_);
			case EProto::SmCheck: return _RecvSmCheck(itServer, Stream_);
			case EProto::SmAddFriend: return _RecvSmAddFriend(itServer, Stream_);
			case EProto::SmAllowFriend: return _RecvSmAllowFriend(itServer, Stream_);
			case EProto::SmDenyFriend: return _RecvSmDenyFriend(itServer, Stream_);
			case EProto::SmBlockFriend: return _RecvSmBlockFriend(itServer, Stream_);
			case EProto::SmUnBlockFriend: return _RecvSmUnBlockFriend(itServer, Stream_);
			case EProto::SmChangeState: return _RecvSmChangeState(itServer, Stream_);
			case EProto::SmToServer: return _RecvSmToServer(itServer, Stream_);
			case EProto::SmSessionEnd: return _RecvSmSessionEnd(itServer, Stream_);
			case EProto::SmSetOpened: return _RecvSmSetOpened(itServer, Stream_);
			case EProto::SmUserProto: return _RecvFuncS(Key_, Stream_);
			default: return _NetS.Close(Key_.PeerNum);
			}
		}
		else
		{
			switch (Header.Proto)
			{
			case EProto::SmServerOn: return _RecvSmServerOn(Key_, Stream_);
			default: return _NetS.Close(Key_.PeerNum);
			}
		}
	}
	void CMaster::_RecvSmServerOn(const CKey& Key_, CStream& Stream_)
	{
		SSmServerOn Proto;
		Stream_ >> Proto;

		TMasterNets OtherMasters;
		for (auto& i : _OtherMasters)
			OtherMasters.emplace_back(SMasterNet(std::get<0>(i.first)->first, i.second.Servers));

		TServerNets OtherServers;
		for (auto& i : _Servers)
			OtherServers.emplace(SServerNet(i.ServerBindNamePort));

		auto itServer = _Servers.emplace_at(Key_.PeerNum, _SServer(Key_, CNamePort(Proto.ClientBindNamePortPub), CNamePort(_NetS.GetNamePort(Key_.PeerNum).Name, Proto.ServerBindPort)));
		_NetS.Certify(Key_.PeerNum);

		_NetS.Send(Key_.PeerNum, SHeader(EProto::MsServerOn), SMsServerOn(itServer->ServerBindNamePort, std::move(OtherServers), std::move(OtherMasters)));

		_NetS.SendAllExcept(Key_.PeerNum, SHeader(EProto::MsOtherServerOn),
			SMsOtherServerOn(
				SServerNet(itServer->ServerBindNamePort)));
		_NetA.SendAll(SHeader(EProto::MaServerOn), SMaServerOn(itServer->ServerBindNamePort));

		_LinkFuncS(Key_);
	}
	void CMaster::_RecvSmLogin(_TServersIt itServer_, CStream& Stream_)
	{
		SSmLogin Proto;
		Stream_ >> Proto;

		_NetC.Send(Proto.UserKey, SHeader(EProto::McLogin), SMcLogin(itServer_->ClientBindNamePortPub, Proto.SessionCode));
		_NetC.WillClose(Proto.UserKey, milliseconds(3000));
	}
	void CMaster::_RecvSmChangeNick(_TServersIt itServer_, CStream& Stream_)
	{
		SSmChangeNick Proto;
		Stream_ >> Proto;

		if (!_NetA.IsLinked(0))
		{
			_SendMsChangeNickFail(itServer_->Key, SMsChangeNickFail(EGameRet::AuthDisconnected, Proto.ClientKey, Proto.UID));
			return;
		}

		auto itChangeNickClient = _ChangeNickClients.emplace(Proto.UID, _SChangeNickClient(itServer_->Key, Proto.ClientKey));
		if (itChangeNickClient.second)
			_NetA.SendAll(SHeader(EProto::MaChangeNick), SMaChangeNick(SSmChangeNick(Proto.UID, Proto.Nick, Proto.ClientKey), itServer_->Key));
		else
			_SendMsChangeNickFail(itServer_->Key, SMsChangeNickFail(EGameRet::InvalidAccess, Proto.ClientKey, Proto.UID));
	}
	void CMaster::_RecvSmCheck(_TServersIt /*itServer_*/, CStream& Stream_)
	{
		SSmCheck Proto;
		Stream_ >> Proto;

		_NetA.Send(Proto.AuthKey, SHeader(EProto::MaCheck), SMaCheck(Proto.UID, Proto.UserKey, Proto.Stream));
	}
	void CMaster::_RecvSmAddFriend(_TServersIt itServer_, CStream& Stream_)
	{
		SSmAddFriend Proto;
		Stream_ >> Proto;

		_NetA.SendAll(SHeader(EProto::MaAddFriendGetUID), SMaAddFriendGetUID(Proto.ToNick, Proto.FromUID, Proto.FromNick, itServer_->Key));
	}
	void CMaster::_RecvSmAllowFriend(_TServersIt itServer_, CStream& Stream_)
	{
		SSmAllowFriend Proto;
		Stream_ >> Proto;

		try
		{
			auto itSession = _Sessions.find(Proto.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidUID;

			auto itFriend = itSession->second.Friends.find(Proto.FriendUID);
			if (itFriend == itSession->second.Friends.end())
				throw EGameRet::InvalidUID;

			if (itFriend->second != EFriendState::Request)
				throw EGameRet::InvalidPacket;

			_DBPush(itServer_->Key, SMasterAllowFriendInDb(Proto.UID, Proto.FriendUID, EFriendState::Normal), Proto.UserKey);
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(itServer_->Key.PeerNum, SHeader(EProto::MsAllowFriendFail), SMsAllowFriendFail(GameRet_, Proto.UserKey));
		}
	}
	void CMaster::_RecvSmDenyFriend(_TServersIt itServer_, CStream& Stream_)
	{
		SSmDenyFriend Proto;
		Stream_ >> Proto;

		_DBPush(itServer_->Key, SMasterDenyFriendInDb(Proto.UID, Proto.FriendUID, EFriendState::Blocked), Proto.UserKey);
	}
	void CMaster::_RecvSmBlockFriend(_TServersIt itServer_, CStream& Stream_)
	{
		SSmBlockFriend Proto;
		Stream_ >> Proto;

		_DBPush(itServer_->Key, SMasterBlockFriendInDb(Proto.UID, Proto.FriendUID, EFriendState::Blocked), Proto.UserKey);
	}
	void CMaster::_RecvSmUnBlockFriend(_TServersIt itServer_, CStream& Stream_)
	{
		SSmUnBlockFriend Proto;
		Stream_ >> Proto;

		_DBPush(itServer_->Key, SMasterUnBlockFriendInDb(Proto.UID, Proto.FriendUID, EFriendState::Normal), Proto.UserKey);
	}
	void CMaster::_RecvSmChangeState(_TServersIt itServer_, CStream& Stream_)
	{
		SSmChangeState Proto;
		Stream_ >> Proto;

		try
		{
			auto itSession = _Sessions.find(Proto.UID);
			if (itSession == _Sessions.end())
				throw EGameRet::InvalidSession;

			if (itSession->second.User.State == Proto.State)
				throw EGameRet::InvalidPacket;

			_DBPush(itServer_->Key, SMasterChangeStateInDb(Proto.UID, Proto.State), Proto.UserKey);
		}
		catch (const EGameRet GameRet_)
		{
			_NetS.Send(itServer_->Key, SHeader(EProto::MsChangeStateFail), SMsChangeStateFail(GameRet_, Proto.UserKey));
		}
	}
	void CMaster::_RecvSmSetOpened(const CKey& Key_, CStream& Stream_)
	{
		SSmSetOpened Proto;
		Stream_ >> Proto;

		_Servers[Key_.PeerNum].Opened = Proto.Opened;
	}
	void CMaster::_RecvSmToServer(_TServersIt /*itServer_*/, CStream& Stream_)
	{
		TUID UID = 0;
		Stream_ >> UID;

		CStream Stream;
		Stream_ >> Stream;

		if (_IsMyUID(UID))
		{
			SendToServerStream(UID, Stream);
			return;
		}

		auto itAuth = _Auths.cycle();
		if (!itAuth)
		{
			itAuth = _Auths.cycle();
			if (!itAuth)
				return;
		}

		_NetA.Send(*itAuth, SHeader(EProto::MaToServer), UID, Stream);
	}
	void CMaster::_RecvSmSessionEnd(_TServersIt /*itServer_*/, CStream& Stream_)
	{
		SSmSessionEnd Proto;
		Stream_ >> Proto;

		auto it = _Sessions.find(Proto.UID);
		if (it == _Sessions.end() || it->second.SessionCode != Proto.SessionCode)
			return;

		_Servers[it->second.ServerKey.PeerNum].Sessions.erase(it->first);
		_SessionEnd(it);
	}
	void CMaster::_RecvSmSetOpened(_TServersIt itServer_, CStream& Stream_)
	{
		SSmSetOpened Proto;
		Stream_ >> Proto;

		itServer_->Opened = Proto.Opened;
	}
	void CMaster::_LinkC(const CKey& Key_)
	{
		_LinkFuncC(Key_);
	}
	void CMaster::_UnLinkC(const CKey& Key_, ENetRet NetRet_)
	{
		_Clients.erase(Key_.PeerNum);
		_UnLinkFuncC(Key_, NetRet_);
	}
	void CMaster::_RecvC(const CKey& Key_, CStream& Stream_)
	{
		SHeader Header;
		Stream_ >> Header;

		auto itClient = _Clients.get(Key_.PeerNum);
		if (itClient)
		{
			return _NetC.Close(Key_.PeerNum);
		}
		else
		{
			switch (Header.Proto)
			{
			case EProto::CmLogin: return _RecvCmLogin(Key_, Stream_);
			default: return _NetC.Close(Key_.PeerNum);
			}
		}
	}
	void CMaster::_RecvCmLogin(const CKey& Key_, CStream& Stream_)
	{
		// catch 하지 못한 exception 이 있어도 Master가 Auth와 끊어지므로 상관없음.
		SCmLogin Proto;
		Stream_ >> Proto;

		try
		{
			if (Proto.UID <= 0)
				throw EGameRet::InvalidUID;

			if (!_IsMyUID(Proto.UID))
				throw EGameRet::InvalidUID;

			_Clients.emplace_at(Key_.PeerNum, Key_);
			_DBPush(Key_, SMasterLoginInDb(Proto.UID, Proto.ID), Proto.SubUID);
		}
		catch (const EGameRet GameRet_)
		{
			_McLoginFail(Key_.PeerNum, GameRet_);
		}
	}

	CMaster::CMaster(
		EAddressFamily AddressFamily_,
		TLinkFunc LinkFuncA_, TLinkFailFunc LinkFailFuncA_, TUnLinkFunc UnLinkFuncA_, TRecvFunc RecvFuncA_,
		TLinkFunc LinkFuncS_, TUnLinkFunc UnLinkFuncS_, TRecvFunc RecvFuncS_,
		TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_,
		TSessionBeginFunc SessionBeginFunc_, TSessionEndFunc SessionEndFunc_, TRecvPunishFunc RecvPunishFunc_, TDBCallback DBCallbackFunc_,
		const TRangeUID& RangeUID_, const SDBOption& DBOption_,
		const CNamePort& AuthNamePort_, const CNamePort& ServerBindNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_,
		TPort MasterBindPort_) :
		_LinkFuncA(LinkFuncA_), _LinkFailFuncA(LinkFailFuncA_), _UnLinkFuncA(UnLinkFuncA_), _RecvFuncA(RecvFuncA_),
		_LinkFuncS(LinkFuncS_), _UnLinkFuncS(UnLinkFuncS_), _RecvFuncS(RecvFuncS_),
		_LinkFuncC(LinkFuncC_), _UnLinkFuncC(UnLinkFuncC_),
		_RangeUID(RangeUID_), _AuthNamePort(AuthNamePort_), _DBOption(DBOption_), _DB(DBOption_, std::bind(&CMaster::_DBCallback, this, _1)),
		_ClientBindNamePortPub(ClientBindNamePortPub_),
		_NetA(
			std::bind(&CMaster::_LinkA, this, _1),
			std::bind(&CMaster::_LinkFailA, this, _1, _2),
			std::bind(&CMaster::_UnLinkA, this, _1, _2),
			std::bind(&CMaster::_RecvA, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 5,
			milliseconds(3000)),
		_NetM(
			EAddressFamily::INET, CNamePort(MasterBindPort_),
			std::bind(&CMaster::_LinkM, this, _1),
			std::bind(&CMaster::_UnLinkM, this, _1),
			std::bind(&CMaster::_RecvM, this, _1),
			true, 0, 65535),
		_NetS(
			AddressFamily_,
			std::bind(&CMaster::_LinkS, this, _1),
			std::bind(&CMaster::_UnLinkS, this, _1, _2),
			std::bind(&CMaster::_RecvS, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(), milliseconds(), 1, 0,
			ServerBindNamePort_, SOMAXCONN),
		_NetC(
			AddressFamily_,
			std::bind(&CMaster::_LinkC, this, _1),
			std::bind(&CMaster::_UnLinkC, this, _1, _2),
			std::bind(&CMaster::_RecvC, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(), milliseconds(), NetCThreadCnt_, 0,
			ClientBindNamePort_, SOMAXCONN),
		_SessionBeginFunc(SessionBeginFunc_), _SessionEndFunc(SessionEndFunc_), _RecvPunishFunc(RecvPunishFunc_), _DBCallbackFunc(DBCallbackFunc_)
	{
	}
	void CMaster::Start(void)
	{
		_NetA.Connect(_AuthNamePort);

		CMSSQL MSSQL(_DBOption);

		if (!MSSQL.Connect())
			THROWEX();

		const wstring UserTableName = L"sys_tbl_User";
		const wstring FriendTableName = L"sys_tbl_Friend";
		const wstring ClearSPName = L"sys_sp_Clear";
		const wstring CreateSPName = L"sys_sp_Create";
		const wstring LoginSPName = L"sys_sp_Login";
		const wstring ChangeNickSPName = L"sys_sp_ChangeNick";
		const wstring AddFriendBeginSPName = L"sys_sp_AddFriendBegin";
		const wstring AddFriendRequestSPName = L"sys_sp_AddFriendRequest";
		const wstring AddFriendEndSPName = L"sys_sp_AddFriendEnd";
		const wstring AddFriendFailSPName = L"sys_sp_AddFriendFail";
		const wstring AllowFriendSPName = L"sys_sp_AllowFriend";
		const wstring DenyFriendSPName = L"sys_sp_DenyFriend";
		const wstring BlockFriendSPName = L"sys_sp_BlockFriend";
		const wstring UnBlockFriendSPName = L"sys_sp_UnBlockFriend";
		const wstring ChangeStateSPName = L"sys_sp_ChangeState";
		const wstring CheckSPName = L"sys_sp_Check";
		const wstring PunishSPName = L"sys_sp_Punish";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CREATE User Table If not exists
		auto CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserTableName + LR"(]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[)" + UserTableName + LR"(](
	[UID] [bigint] NOT NULL,
	[ID] [varbinary](MAX) NOT NULL,
	[Nick] [nvarchar](MAX) NOT NULL,
	[State] [tinyint] NOT NULL,
	[PunishEndTime] [bigint] NOT NULL,
	[InsertedTime] [datetime] NOT NULL,
	CONSTRAINT [PK_)" + UserTableName + LR"(] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END)";
		if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
			THROWEX();

		// CREATE Table Constraint
		CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_)" + UserTableName + LR"(_PunishEndTime]') AND type = 'D')
BEGIN
	ALTER TABLE [dbo].[)" + UserTableName + LR"(] ADD  CONSTRAINT [DF_)" + UserTableName + LR"(_PunishEndTime]  DEFAULT ((0)) FOR [PunishEndTime]
END)";
		if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
			THROWEX();

		CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_)" + UserTableName + LR"(_InsertedTime]') AND type = 'D')
BEGIN
	ALTER TABLE [dbo].[)" + UserTableName + LR"(] ADD  CONSTRAINT [DF_)" + UserTableName + LR"(_InsertedTime]  DEFAULT (getdate()) FOR [InsertedTime]
END)";
		if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
			THROWEX();


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CREATE Friend Table If not exists
		CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + FriendTableName + LR"(]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[)" + FriendTableName + LR"(](
	[UID] [bigint] NOT NULL,
	[FriendUID] [bigint] NOT NULL,
	[FriendNick] [nvarchar](MAX) NOT NULL,
	[FriendState] [tinyint] NOT NULL,
	CONSTRAINT [PK_)" + FriendTableName + LR"(] PRIMARY KEY CLUSTERED 
(
	[UID] ASC,
	[FriendUID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END)";
		if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
			THROWEX();


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CREATE Clear SP If not exists
		auto ClearSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + ClearSPName + LR"(]
	@UID_ BIGINT
,	@ID_ NVARCHAR(MAX)
,	@Nick_ NVARCHAR(MAX)
,	@State_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	TRUNCATE TABLE dbo.)" + UserTableName + LR"(
	TRUNCATE TABLE dbo.)" + FriendTableName + LR"(

	RETURN 0

END)";

		MSSQL.CreateSP(ClearSPName, ClearSPQuery);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto CreateSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + CreateSPName + LR"(]
	@UID_ BIGINT
,	@ID_ NVARCHAR(MAX)
,	@Nick_ NVARCHAR(MAX)
,	@State_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	DECLARE @Ret INT = 0

	BEGIN TRY

		DECLARE @ID VARBINARY(MAX)
		DECLARE @Nick NVARCHAR(MAX)

		SELECT	@ID = ID, @Nick = Nick
		FROM	dbo.)" + UserTableName + LR"(
		WHERE	[UID] = @UID_

		IF @ID IS NOT NULL
		BEGIN
			UPDATE	dbo.)" + UserTableName + LR"(
			SET		ID = HASHBYTES('SHA2_256', @ID_)
			WHERE	[UID] = @UID_
		END
		ELSE
		BEGIN
			INSERT INTO dbo.)" + UserTableName + LR"( ([UID], ID, Nick, [State])
			VALUES (@UID_, HASHBYTES('SHA2_256', @ID_), @Nick_, @State_)
		END

		RETURN 0

	END TRY
	BEGIN CATCH

		IF
			ERROR_NUMBER() <> 50000 AND
			ERROR_NUMBER() <> 266
		SET @Ret = ERROR_NUMBER()

		RETURN @Ret

	END CATCH

END)";


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto LoginSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + LoginSPName + LR"(]
	@UID_ BIGINT
,	@ID_ NVARCHAR(MAX)
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON
	
	BEGIN TRY

		SELECT	Nick, [State], PunishEndTime
		FROM	dbo.)" + UserTableName + LR"(
		WHERE	[UID] = @UID_
			AND ID = HASHBYTES('SHA2_256', @ID_)

		SELECT	FriendUID, FriendNick, FriendState
		FROM	dbo.)" + FriendTableName + LR"(
		WHERE	[UID] = @UID_

		RETURN 0

	END TRY
	BEGIN CATCH

		RETURN ERROR_NUMBER()

	END CATCH

END)";


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto ChangeNickSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + ChangeNickSPName + LR"(]
	@UID_ BIGINT
,	@Nick_ NVARCHAR(MAX)
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + UserTableName + LR"(
	SET		Nick = @Nick_
	WHERE	[UID] = @UID_

	RETURN 0

END)";


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto AddFriendBeginSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + AddFriendBeginSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
,	@FriendNick_ NVARCHAR(MAX)
,	@FriendState_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	DECLARE @Ret INT = 0
	
	BEGIN TRY

		INSERT	dbo.)" + FriendTableName + LR"( ([UID], FriendUID, FriendNick, FriendState)
		VALUES (@UID_, @FriendUID_, @FriendNick_, @FriendState_)

		RETURN 0
		
	END TRY
	BEGIN CATCH
		IF
			ERROR_NUMBER() <> 50000 AND
			ERROR_NUMBER() <> 266
		SET @Ret = ERROR_NUMBER()
		
		RETURN @Ret
		
	END CATCH
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto AddFriendRequestSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + AddFriendRequestSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
,	@FriendNick_ NVARCHAR(MAX)
,	@FriendState_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	MERGE dbo.)" + FriendTableName + LR"( WITH(HOLDLOCK) AS tT
	USING (SELECT @UID_ [UID], @FriendUID_ FriendUID) AS tS
	ON (tT.[UID] = tS.[UID] AND tT.FriendUID = tS.FriendUID)
	WHEN NOT MATCHED BY TARGET THEN
		INSERT ([UID], FriendUID, FriendNick, FriendState)
		VALUES (@UID_, @FriendUID_, @FriendNick_, @FriendState_);

	SELECT @@ROWCOUNT

	RETURN 0
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto AddFriendEndSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + AddFriendEndSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
,	@FriendState_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + FriendTableName + LR"(
	SET		FriendState = @FriendState_
	WHERE	[UID] = @UID_
		AND	FriendUID = @FriendUID_

	RETURN 0
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto AddFriendFailSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + AddFriendFailSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	DELETE	dbo.)" + FriendTableName + LR"(
	WHERE	[UID] = @UID_
		AND	FriendUID = @FriendUID_

	RETURN 0
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto AllowFriendSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + AllowFriendSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
,	@FriendState_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + FriendTableName + LR"(
	SET		FriendState = @FriendState_
	WHERE	[UID] = @UID_
		AND	FriendUID = @FriendUID_

	RETURN 0
END)";


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto DenyFriendSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + DenyFriendSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
,	@FriendState_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + FriendTableName + LR"(
	SET		FriendState = @FriendState_
	WHERE	[UID] = @UID_
		AND	FriendUID = @FriendUID_

	RETURN 0
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto BlockFriendSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + BlockFriendSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
,	@FriendState_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + FriendTableName + LR"(
	SET		FriendState = @FriendState_
	WHERE	[UID] = @UID_
		AND	FriendUID = @FriendUID_

	RETURN 0
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto UnBlockFriendSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + UnBlockFriendSPName + LR"(]
	@UID_ BIGINT
,	@FriendUID_ BIGINT
,	@FriendState_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + FriendTableName + LR"(
	SET		FriendState = @FriendState_
	WHERE	[UID] = @UID_
		AND	FriendUID = @FriendUID_

	RETURN 0
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto ChangeStateSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + ChangeStateSPName + LR"(]
	@UID_ BIGINT
,	@State_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + UserTableName + LR"(
	SET		[State] = @State_
	WHERE	[UID] = @UID_

	RETURN 0
END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto CheckSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + CheckSPName + LR"(]
	@UID_ BIGINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON
	
	BEGIN TRY

		SELECT	PunishEndTime
		FROM	dbo.)" + UserTableName + LR"(
		WHERE	[UID] = @UID_

		RETURN 0

	END TRY
	BEGIN CATCH

		RETURN ERROR_NUMBER()

	END CATCH

END)";

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auto PunishSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + PunishSPName + LR"(]
	@UID_ BIGINT
,	@EndTime_ BIGINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	BEGIN TRY

		UPDATE	dbo.)" + UserTableName + LR"(
		SET		PunishEndTime = @EndTime_
		WHERE	[UID] = @UID_

		RETURN 0

	END TRY
	BEGIN CATCH

		RETURN ERROR_NUMBER()

	END CATCH

END)";

		_DBAddCmd<SMasterCreateInDb, SMasterCreateOutDb>(MSSQL, CreateSPName, CreateSPQuery, std::bind(&CMaster::_DBCreate, this, _1));
		_DBAddCmd<SMasterLoginInDb, SMasterLoginOutDb>(MSSQL, LoginSPName, LoginSPQuery, std::bind(&CMaster::_DBLogin, this, _1));
		_DBAddCmd<SMasterChangeNickInDb, SDummyOutDb>(MSSQL, ChangeNickSPName, ChangeNickSPQuery, std::bind(&CMaster::_DBChangeNick, this, _1));
		_DBAddCmd<SMasterAddFriendBeginInDb, SDummyOutDb>(MSSQL, AddFriendBeginSPName, AddFriendBeginSPQuery, std::bind(&CMaster::_DBAddFriendBegin, this, _1));
		_DBAddCmd<SMasterAddFriendRequestInDb, SMasterAddFriendRequestOutDb>(MSSQL, AddFriendRequestSPName, AddFriendRequestSPQuery, std::bind(&CMaster::_DBAddFriendRequest, this, _1));
		_DBAddCmd<SMasterAddFriendEndInDb, SDummyOutDb>(MSSQL, AddFriendEndSPName, AddFriendEndSPQuery, std::bind(&CMaster::_DBAddFriendEnd, this, _1));
		_DBAddCmd<SMasterAddFriendFailInDb, SDummyOutDb>(MSSQL, AddFriendFailSPName, AddFriendFailSPQuery, std::bind(&CMaster::_DBAddFriendFail, this, _1));
		_DBAddCmd<SMasterAllowFriendInDb, SDummyOutDb>(MSSQL, AllowFriendSPName, AllowFriendSPQuery, std::bind(&CMaster::_DBAllowFriend, this, _1));
		_DBAddCmd<SMasterDenyFriendInDb, SDummyOutDb>(MSSQL, DenyFriendSPName, DenyFriendSPQuery, std::bind(&CMaster::_DBDenyFriend, this, _1));
		_DBAddCmd<SMasterBlockFriendInDb, SDummyOutDb>(MSSQL, BlockFriendSPName, BlockFriendSPQuery, std::bind(&CMaster::_DBBlockFriend, this, _1));
		_DBAddCmd<SMasterUnBlockFriendInDb, SDummyOutDb>(MSSQL, UnBlockFriendSPName, UnBlockFriendSPQuery, std::bind(&CMaster::_DBUnBlockFriend, this, _1));
		_DBAddCmd<SMasterChangeStateInDb, SDummyOutDb>(MSSQL, ChangeStateSPName, ChangeStateSPQuery, std::bind(&CMaster::_DBChangeState, this, _1));
		_DBAddCmd<SMasterCheckInDb, SMasterCheckOutDb>(MSSQL, CheckSPName, CheckSPQuery, std::bind(&CMaster::_DBCheck, this, _1));
		_DBAddCmd<SMasterPunishInDb, SMasterPunishOutDb>(MSSQL, PunishSPName, PunishSPQuery, std::bind(&CMaster::_DBPunish, this, _1));

		_DB.Start(5);
	}
	bool CMaster::Punish(TUID UID_, const TTime& EndTime_)
	{
		_DBPush(CKey(), SMasterPunishInDb(UID_, EndTime_));
		_RecvPunishFunc(UID_, EndTime_); // include release from punish

		if (EndTime_ > steady_now())
		{
			auto itSession = _Sessions.find(UID_);
			if (itSession == _Sessions.end())
				return false;

			_NetS.Send(itSession->second.ServerKey.PeerNum, SHeader(EProto::MsPunish), SMsPunish(UID_, EndTime_));
		}

		return true;
	}
	void CMaster::Proc(void)
	{
		_DB.Proc();
		_NetA.Proc();
		_NetM.Proc();
		_NetS.Proc();
		_NetC.Proc();
	}
}
