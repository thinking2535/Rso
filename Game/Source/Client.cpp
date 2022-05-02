#include "Client.h"

namespace rso::game
{
	EGameRet CClient::_NetRetToGameRet(ENetRet NetRet_)
	{
		switch (NetRet_)
		{
		case ENetRet::Ok: return EGameRet::Ok;
		case ENetRet::UserClose: return EGameRet::UserClose;
		case ENetRet::HeartBeatFail: return EGameRet::HeartBeatFail;
		case ENetRet::KeepConnectTimeOut: return EGameRet::KeepConnectTimeOut;
		case ENetRet::ConnectFail: return EGameRet::ConnectFail;
		case ENetRet::CertifyFail: return EGameRet::CertifyFail;
		case ENetRet::SystemError: return EGameRet::SystemError;
		case ENetRet::SocketError: return EGameRet::SocketError;
		case ENetRet::InvalidPacket: return EGameRet::InvalidPacket;
		default: return EGameRet::NetError;
		}
	}
	void CClient::_LoginClear(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		auto itClient = _Clients.get(PeerNum_);

		if (GameRet_ == EGameRet::InvalidID)
			itClient->Clear();

		_Clients.erase(itClient);
	}
	void CClient::_LoginFail(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		_LoginClear(PeerNum_, GameRet_);

		if (LinkFailFunc)
			LinkFailFunc(PeerNum_, GameRet_);
	}
	void CClient::_LoginFailAndCloseA(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		_LoginClear(PeerNum_, GameRet_);
		_NetA.Close(PeerNum_);

		if (LinkFailFunc)
			LinkFailFunc(PeerNum_, GameRet_);
	}
	void CClient::_LoginFailAndCloseM(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		_LoginClear(PeerNum_, GameRet_);
		_NetM.Close(PeerNum_);

		if (LinkFailFunc)
			LinkFailFunc(PeerNum_, GameRet_);
	}
	void CClient::_LoginFailAndCloseS(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		_LoginClear(PeerNum_, GameRet_);
		_NetS.Close(PeerNum_);

		if (LinkFailFunc)
			LinkFailFunc(PeerNum_, GameRet_);
	}
	void CClient::_CheckFail(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		_Clients.erase(PeerNum_);

		if (ErrorFunc)
			ErrorFunc(PeerNum_, GameRet_);
	}
	void CClient::_CheckFailAndClose(TPeerCnt PeerNum_, EGameRet GameRet_)
	{
		_Clients.erase(PeerNum_);
		_NetA.Close(PeerNum_);

		if (ErrorFunc)
			ErrorFunc(PeerNum_, GameRet_); // 외부에서 다시 연결 할 수 있으므로 콜백 전에 연결전 상태로
	}
	void CClient::_Connect(TPeerCnt PeerNum_, _ENode Node_, const CNamePort& NamePort_)
	{
		_Clients[PeerNum_].CurNode = Node_;

		switch (Node_)
		{
		case _ENode::Auth:
			if (!_NetA.Connect(_Clients[PeerNum_].AuthNamePort, PeerNum_))
				_LoginFail(PeerNum_, EGameRet::ConnectAuthFail);
			break;
		case _ENode::Master:
			if (!_NetM.Connect(NamePort_, PeerNum_))
				_LoginFail(PeerNum_, EGameRet::ConnectMasterFail);
			break;
		case _ENode::Server:
			if (!_NetS.Connect(NamePort_, PeerNum_))
				_LoginFail(PeerNum_, EGameRet::ConnectServerFail);
			break;
		default:
			break;
		}
	}
	void CClient::_ConnectToUpper(TPeerCnt PeerNum_, _ENode Node_)
	{
		auto& Client = _Clients[PeerNum_];

		switch (Node_)
		{
		default:
		case _ENode::Master:
			if (**Client.MasterNamePort)
				return _Connect(PeerNum_, _ENode::Master, **Client.MasterNamePort);

		case _ENode::Auth:
			return _Connect(PeerNum_, _ENode::Auth, CNamePort());
		}
	}
	bool CClient::_ConnectToLower(TPeerCnt PeerNum_, _ENode Node_, const CNamePort& NamePort_)
	{
		if (!_Clients[PeerNum_].ConnectHistory.Connect(NamePort_))
			return false;

		_Connect(PeerNum_, Node_, NamePort_);
		return true;
	}

	void CClient::_LinkA(const CKey& Key_)
	{
		switch (_Clients[Key_.PeerNum].CaProto)
		{
		case EProto::CaCreate:
			_NetA.Send(Key_.PeerNum, SHeader(_Clients[Key_.PeerNum].CaProto), _Clients[Key_.PeerNum].GetCaCreate());
			break;
		case EProto::CaLogin:
			_NetA.Send(Key_.PeerNum, SHeader(_Clients[Key_.PeerNum].CaProto), _Clients[Key_.PeerNum].GetCaLogin());
			break;
		case EProto::CaCheck:
			_NetA.Send(Key_.PeerNum, SHeader(_Clients[Key_.PeerNum].CaProto), _Clients[Key_.PeerNum].GetCaCheck());
			break;
		}
	}
	void CClient::_LinkFailA(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		if (_Clients[PeerNum_].IsCheck())
			_CheckFail(PeerNum_, _NetRetToGameRet(NetRet_));
		else
			_LoginFail(PeerNum_, _NetRetToGameRet(NetRet_));
	}
	void CClient::_UnLinkA(const CKey& Key_, ENetRet NetRet_)
	{
		auto Client = _Clients.get(Key_.PeerNum);
		if (!Client)
			return;

		if (_Clients[Key_.PeerNum].IsCheck())
		{
			_CheckFail(Key_.PeerNum, _NetRetToGameRet(NetRet_));
		}
		else
		{
			if (_Clients[Key_.PeerNum].CurNode != _ENode::Auth)
				return;

			_LoginFail(Key_.PeerNum, _NetRetToGameRet(NetRet_));
		}
	}
	void CClient::_RecvA(const CKey& Key_, CStream& Stream_)
	{
		SHeader Header;
		Stream_ >> Header;

		if (_Clients[Key_.PeerNum].IsCheck())
		{
			switch (Header.Proto)
			{
			case EProto::AcCheck: return _RecvAcCheck(Key_, Stream_);
			case EProto::AcCheckFail: return _RecvAcCheckFail(Key_, Stream_);
			default: return _CheckFailAndClose(Key_.PeerNum, EGameRet::InvalidPacket);
			}
		}
		else
		{
			switch (Header.Proto)
			{
			case EProto::AcLogin: return _RecvAcLogin(Key_, Stream_);
			case EProto::AcLoginFail: return _RecvAcLoginFail(Key_, Stream_);
			default: return _LoginFailAndCloseA(Key_.PeerNum, EGameRet::InvalidPacket);
			}
		}
	}
	void CClient::_RecvAcLogin(const CKey& Key_, CStream& Stream_)
	{
		SAcLogin Proto;
		Stream_ >> Proto;

		_Clients[Key_.PeerNum].UID = Proto.UID;

		if (_ConnectToLower(Key_.PeerNum, _ENode::Master, CNamePort(Proto.ClientBindNamePortPubToMaster)))
			_NetA.Close(Key_.PeerNum);
		else
			_LoginFailAndCloseA(Key_.PeerNum, EGameRet::ConnectMasterFail);
	}
	void CClient::_RecvAcLoginFail(const CKey& Key_, CStream& Stream_)
	{
		SAcLoginFail Proto;
		Stream_ >> Proto;

		_LoginFailAndCloseA(Key_.PeerNum, Proto.GameRet);
	}
	void CClient::_RecvAcCheck(const CKey& Key_, CStream& Stream_)
	{
		SAcCheck Proto;
		Stream_ >> Proto;

		_Clients.erase(Key_.PeerNum);
		_NetA.Close(Key_.PeerNum);

		if (CheckFunc)
			CheckFunc(Proto.UID, Proto.Stream);
	}
	void CClient::_RecvAcCheckFail(const CKey& Key_, CStream& Stream_)
	{
		SAcCheckFail Proto;
		Stream_ >> Proto;

		_CheckFailAndClose(Key_.PeerNum, Proto.GameRet);
	}

	void CClient::_LinkM(const CKey& Key_)
	{
		auto& Client = _Clients[Key_.PeerNum];
		**Client.MasterNamePort = _NetM.GetNamePort(Key_.PeerNum);
		_NetM.Send(Key_.PeerNum, SHeader(EProto::CmLogin), SCmLogin(Client.UID, Client.ID, Client.SubUID));
	}
	void CClient::_LinkFailM(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		auto Client = _Clients.get(PeerNum_);
		if (!Client)
			return;

		if (_Clients[PeerNum_].CurNode != _ENode::Master)
			return;

		if (!_Clients[PeerNum_].IsValidAccount())
			return _LoginFail(PeerNum_, _NetRetToGameRet(NetRet_));

		_ConnectToUpper(PeerNum_, _ENode::Auth);
	}
	void CClient::_UnLinkM(const CKey& Key_, ENetRet NetRet_)
	{
		auto Client = _Clients.get(Key_.PeerNum);
		if (!Client)
			return;

		if (_Clients[Key_.PeerNum].CurNode != _ENode::Master)
			return;

		if (!_Clients[Key_.PeerNum].IsValidAccount())
			return _LoginFail(Key_.PeerNum, _NetRetToGameRet(NetRet_));

		_ConnectToUpper(Key_.PeerNum, _ENode::Auth);
	}
	void CClient::_RecvM(const CKey& Key_, CStream& Stream_)
	{
		SHeader Header;
		Stream_ >> Header;

		switch (Header.Proto)
		{
		case EProto::McLogin: return _RecvMcLogin(Key_, Stream_);
		case EProto::McLoginFail: return _RecvMcLoginFail(Key_, Stream_);
		default: return _NetM.Close(Key_.PeerNum);
		}
	}
	void CClient::_RecvMcLogin(const CKey& Key_, CStream& Stream_)
	{
		SMcLogin Proto;
		Stream_ >> Proto;

		_Clients[Key_.PeerNum].SessionCode = Proto.SessionCode;

		if (_ConnectToLower(Key_.PeerNum, _ENode::Server, CNamePort(Proto.ClientBindNamePortPubToServer)))
			_NetM.Close(Key_.PeerNum);
		else
			_LoginFailAndCloseM(Key_.PeerNum, EGameRet::ConnectServerFail);
	}
	void CClient::_RecvMcLoginFail(const CKey& Key_, CStream& Stream_)
	{
		SMcLoginFail Proto;
		Stream_ >> Proto;

		_LoginFailAndCloseM(Key_.PeerNum, Proto.GameRet);
	}

	void CClient::_LinkS(const CKey& Key_)
	{
		auto& Client = _Clients[Key_.PeerNum];
		_NetS.Send(Key_.PeerNum, SHeader(EProto::CsLogin), SCsLogin(Client.UID, Client.ID, Client.SubUID, Client.SessionCode, _Version, Client.IsCreate(), Client.Stream));
	}
	void CClient::_LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		auto Client = _Clients.get(PeerNum_);
		if (!Client)
			return;

		if (_Clients[PeerNum_].CurNode != _ENode::Server)
			return;

		if (!_Clients[PeerNum_].IsValidAccount())
			return _LoginFail(PeerNum_, _NetRetToGameRet(NetRet_));

		_ConnectToUpper(PeerNum_, _ENode::Master);
	}
	void CClient::_UnLinkS(const CKey& Key_, ENetRet NetRet_)
	{
		auto Client = _Clients.get(Key_.PeerNum);
		if (!Client)
			return;

		if (_Clients[Key_.PeerNum].CurNode != _ENode::Server)
			return;

		if (_Clients[Key_.PeerNum].Logon)
		{
			_Clients.erase(Key_.PeerNum);

			if (UnLinkFunc)
				UnLinkFunc(Key_, _NetRetToGameRet(NetRet_));

			return;
		}

		// 현재 클라가 로그인 시에 최초 GameServer로 접속하는 경우는 없고 최소 Master를 거쳐야 하기 때문에 최초 게임서버로 접속하여 호출된 _UnLinkS 
		_LoginFail(Key_.PeerNum, _NetRetToGameRet(NetRet_));
	}
	void CClient::_RecvS(const CKey& Key_, CStream& Stream_)
	{
		SHeader Header;
		Stream_ >> Header;

		switch (Header.Proto)
		{
		case EProto::ScLogin: return _RecvScLogin(Key_, Stream_);
		case EProto::ScLoginFail: return _RecvScLoginFail(Key_, Stream_);
		case EProto::ScError: return _RecvScError(Key_, Stream_);
		case EProto::ScAddFriendRequest: return _RecvScAddFriendRequest(Key_, Stream_);
		case EProto::ScAddFriend: return _RecvScAddFriend(Key_, Stream_);
		case EProto::ScAllowFriend: return _RecvScAllowFriend(Key_, Stream_);
		case EProto::ScDenyFriend: return _RecvScDenyFriend(Key_, Stream_);
		case EProto::ScBlockFriend: return _RecvScBlockFriend(Key_, Stream_);
		case EProto::ScUnBlockFriend: return _RecvScUnBlockFriend(Key_, Stream_);
		case EProto::ScChangeState: return _RecvScChangeState(Key_, Stream_);
		case EProto::ScFriendStateChanged: return _RecvScFriendStateChanged(Key_, Stream_);
		case EProto::ScFriendStateChangedOffline: return _RecvScFriendStateChangedOffline(Key_, Stream_);
		case EProto::ScMessageReceived: return _RecvScMessageReceived(Key_, Stream_);
		case EProto::ScUserProto: return _RecvScUserProto(Key_, Stream_);
		default: return _NetS.Close(Key_.PeerNum);
		}
	}
	void CClient::_RecvScLogin(const CKey& Key_, CStream& Stream_)
	{
		SScLogin Proto;
		Stream_ >> Proto;

		_Clients[Key_.PeerNum].Login(Proto.Nick);

		if (LinkFunc)
			LinkFunc(Key_, _Clients[Key_.PeerNum].UID, _Clients[Key_.PeerNum].Nick, Proto.Friends);
	}
	void CClient::_RecvScLoginFail(const CKey& Key_, CStream& Stream_)
	{
		SScLoginFail Proto;
		Stream_ >> Proto;

		_LoginFailAndCloseS(Key_.PeerNum, Proto.GameRet);
	}
	void CClient::_RecvScError(const CKey& Key_, CStream& Stream_)
	{
		SScError Proto;
		Stream_ >> Proto;

		if (ErrorFunc)
			ErrorFunc(Key_.PeerNum, Proto.GameRet);
	}
	void CClient::_RecvScAddFriendRequest(const CKey& Key_, CStream& Stream_)
	{
		SScAddFriendRequest Proto;
		Stream_ >> Proto;

		if (FriendRequestedFunc)
			FriendRequestedFunc(Key_.PeerNum, Proto.FromUID, Proto.FromNick);
	}
	void CClient::_RecvScAddFriend(const CKey& Key_, CStream& Stream_)
	{
		SScAddFriend Proto;
		Stream_ >> Proto;

		if (FriendAddedFunc)
			FriendAddedFunc(Key_.PeerNum, Proto.ToUID, Proto.Friend);
	}
	void CClient::_RecvScAllowFriend(const CKey& Key_, CStream& Stream_)
	{
		SScAllowFriend Proto;
		Stream_ >> Proto;

		if (FriendAllowedFunc)
			FriendAllowedFunc(Key_.PeerNum, Proto.FriendUID);
	}
	void CClient::_RecvScDenyFriend(const CKey& Key_, CStream& Stream_)
	{
		SScDenyFriend Proto;
		Stream_ >> Proto;

		if (FriendDenyedFunc)
			FriendDenyedFunc(Key_.PeerNum, Proto.FriendUID);
	}
	void CClient::_RecvScBlockFriend(const CKey& Key_, CStream& Stream_)
	{
		SScBlockFriend Proto;
		Stream_ >> Proto;

		if (FriendBlockedFunc)
			FriendBlockedFunc(Key_.PeerNum, Proto.FriendUID);
	}
	void CClient::_RecvScUnBlockFriend(const CKey& Key_, CStream& Stream_)
	{
		SScUnBlockFriend Proto;
		Stream_ >> Proto;

		if (FriendUnBlockedFunc)
			FriendUnBlockedFunc(Key_.PeerNum, Proto.FriendUID);
	}
	void CClient::_RecvScChangeState(const CKey& Key_, CStream& Stream_)
	{
		SScChangeState Proto;
		Stream_ >> Proto;

		if (StateChangedFunc)
			StateChangedFunc(Key_.PeerNum, Proto.State);
	}
	void CClient::_RecvScFriendStateChanged(const CKey& Key_, CStream& Stream_)
	{
		SScFriendStateChanged Proto;
		Stream_ >> Proto;

		if (FriendStateChangedFunc)
			FriendStateChangedFunc(Key_.PeerNum, Proto.FriendUID, Proto.FriendState);
	}
	void CClient::_RecvScFriendStateChangedOffline(const CKey& Key_, CStream& Stream_)
	{
		SScFriendStateChangedOffline Proto;
		Stream_ >> Proto;

		if (FriendStateChangedFunc)
		{
			for (auto& i : Proto.Friends)
				FriendStateChangedFunc(Key_.PeerNum, i, c_Default_State);
		}
	}
	void CClient::_RecvScMessageReceived(const CKey& Key_, CStream& Stream_)
	{
		SScMessageReceived Proto;
		Stream_ >> Proto;

		if (MessageReceivedFunc)
			MessageReceivedFunc(Key_.PeerNum, Proto.FromUID, Proto.Message);
	}
	void CClient::_RecvScUserProto(const CKey& Key_, CStream& Stream_)
	{
		if (RecvFunc)
			RecvFunc(Key_, Stream_);
	}

	CClient::CClient(const SVersion& Version_) :
		_Version(Version_),
		_NetA(
			std::bind(&CClient::_LinkA, this, _1),
			std::bind(&CClient::_LinkFailA, this, _1, _2),
			std::bind(&CClient::_UnLinkA, this, _1, _2),
			std::bind(&CClient::_RecvA, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 60),
		_NetM(
			std::bind(&CClient::_LinkM, this, _1),
			std::bind(&CClient::_LinkFailM, this, _1, _2),
			std::bind(&CClient::_UnLinkM, this, _1, _2),
			std::bind(&CClient::_RecvM, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 60),
		_NetS(
			std::bind(&CClient::_LinkS, this, _1),
			std::bind(&CClient::_LinkFailS, this, _1, _2),
			std::bind(&CClient::_UnLinkS, this, _1, _2),
			std::bind(&CClient::_RecvS, this, _1, _2),
			true, 1024000, 1024000,
			milliseconds(120000), milliseconds(5000), 1, 60)
	{
	}
	bool CClient::IsLinked(TPeerCnt PeerNum_) const
	{
		auto itClient = _Clients.get(PeerNum_);
		if (!itClient)
			return false;

		return itClient->Logon;
	}
	void CClient::Proc(void)
	{
		_NetA.Proc();
		_NetM.Proc();
		_NetS.Proc();
	}
	void CClient::Create(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& HubNamePort_, const TID& ID_, const TNick& Nick_, TUID SubUID_, TState State_, const CStream& Stream_)
	{
		_Clients.emplace_at(PeerNum_, _SClient(PeerNum_, DataPath_, HubNamePort_, ID_, Nick_, SubUID_, State_, Stream_));
		_ConnectToUpper(PeerNum_, _ENode::Auth);
	}
	bool CClient::Login(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& HubNamePort_, const TID& ID_, TUID SubUID_, const CStream& Stream_)
	{
		auto itClient = _Clients.emplace_at(PeerNum_, _SClient(PeerNum_, DataPath_, HubNamePort_, SubUID_, Stream_));
		auto NodeToConnect = itClient->GetNodeToConnect(ID_);
		if (NodeToConnect == _ENode::Null)
		{
			_Clients.erase(itClient);
			return false;
		}

		_ConnectToUpper(PeerNum_, NodeToConnect);

		return true;
	}
	void CClient::_Logout(_TClients::iterator Client_)
	{
		Client_->SetNullAccount();

		if (_NetA.IsConnecting((TPeerCnt)Client_.Index()))
			_NetA.Close((TPeerCnt)Client_.Index());

		if (_NetM.IsConnecting((TPeerCnt)Client_.Index()))
			_NetM.Close((TPeerCnt)Client_.Index());

		if (_NetS.IsConnecting((TPeerCnt)Client_.Index()))
			_NetS.Close((TPeerCnt)Client_.Index());
	}
	void CClient::Logout(TPeerCnt PeerNum_)
	{
		_Logout(_Clients.get(PeerNum_));
	}
	void CClient::Logout(void)
	{
		for (auto it = _Clients.begin(); it;)
		{
			auto itCheck = it;
			++it;

			_Logout(itCheck);
		}
	}
	void CClient::Check(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& AuthNamePort_, const TID& ID_, TUID SubUID_)
	{
		_Clients.emplace_at(PeerNum_, _SClient(DataPath_, AuthNamePort_, ID_, SubUID_));

		if (!_NetA.Connect(AuthNamePort_, PeerNum_))
			_CheckFail(PeerNum_, EGameRet::ConnectAuthFail);
	}
	void CClient::ReleaseAccount(TPeerCnt PeerNum_)
	{
		auto itClient = _Clients.get(PeerNum_);
		itClient->ReleaseAccount();
		_Logout(itClient);
	}
	void CClient::AddFriend(TPeerCnt PeerNum_, const TNick& Nick_)
	{
		_NetS.Send(PeerNum_, SHeader(EProto::CsAddFriend), SCsAddFriend(Nick_));
	}
	void CClient::AllowFriend(TPeerCnt PeerNum_, TUID UID_)
	{
		_NetS.Send(PeerNum_, SHeader(EProto::CsAllowFriend), SCsAllowFriend(UID_));
	}
	void CClient::DenyFriend(TPeerCnt PeerNum_, TUID UID_)
	{
		_NetS.Send(PeerNum_, SHeader(EProto::CsDenyFriend), SCsDenyFriend(UID_));
	}
	void CClient::BlockFriend(TPeerCnt PeerNum_, TUID UID_)
	{
		_NetS.Send(PeerNum_, SHeader(EProto::CsBlockFriend), SCsBlockFriend(UID_));
	}
	void CClient::UnBlockFriend(TPeerCnt PeerNum_, TUID UID_)
	{
		_NetS.Send(PeerNum_, SHeader(EProto::CsUnBlockFriend), SCsUnBlockFriend(UID_));
	}
	void CClient::ChangeState(TPeerCnt PeerNum_, TState State_)
	{
		_NetS.Send(PeerNum_, SHeader(EProto::CsChangeState), SCsChangeState(State_));
	}
	void CClient::MessageSend(TPeerCnt PeerNum_, TUID ToUID_, const TMessage& Message_)
	{
		_NetS.Send(PeerNum_, SHeader(EProto::CsMessageSend), SCsMessageSend(ToUID_, Message_));
	}
}
