#include "stdafx.h"

CKey g_Key;
void LoginFunc(const CKey& Key_, TUID UID_, const TNick& Nick_, const TFriends& Friends_)
{
	g_Key = Key_;
	wcout << L"Login UID:" << UID_ << L" Friends size:" << Friends_.size() << endl;

	for (auto& f : Friends_)
		cout << "friendUID:" << f.first << " friendState:" << static_cast<int32>(f.second.FriendState) << " state:" << static_cast<int32>(f.second.Info.State) << endl;
}
void LoginFailFunc(TPeerCnt PeerNum_, EGameRet GameRet_)
{
	wcout << L"LoginFail" << PeerNum_ << endl;
}
void LogoutFunc(const CKey& Key_, EGameRet GameRet_)
{
	g_Key = CKey();
	wcout << L"Logout" << endl;
}
void RecvFunc(const CKey& Key_, CStream& Stream_)
{
	wcout << L"RecvFunc" << endl;

	EProtoNetSc ProtoNum;
	Stream_ >> ProtoNum;

	auto itRecv = g_BinderRecvS.get(static_cast<size_t>(ProtoNum));
	if (!itRecv)
		THROWEX();

	(*itRecv)(Key_, Stream_);
}
void LinkFuncSoft(const CKey& Key_)
{
	wcout << L"LinkFuncSoft" << endl;
}
void UnLinkFuncSoft(const CKey& Key_, EGameRet GameRet_)
{
	wcout << L"UnLinkFuncSoft" << endl;
}
void ErrorFunc(TPeerCnt PeerNum_, EGameRet GameRet_)
{
	wcout << L"Error" << PeerNum_ << endl;
}
void CheckFunc(TUID UID_, CStream& Stream_)
{
	wcout << L"Check UID:" << UID_ << endl;
}
void FriendAddedFunc(TPeerCnt PeerNum_, TUID UID_, const SFriend& Friend_)
{
	wcout << L"FriendAddedFunc UID:" << UID_ << endl;
}
void FriendRequestedFunc(TPeerCnt PeerNum_, TUID UID_, const TNick& Nick_)
{
	wcout << L"FriendRequestedFunc" << PeerNum_ << endl;
}
void FriendAllowedFunc(TPeerCnt PeerNum_, TUID FriendUID_)
{
	wcout << L"FriendAllowedFunc" << PeerNum_ << endl;
}
void FriendDenyedFunc(TPeerCnt PeerNum_, TUID FriendUID_)
{
	wcout << L"FriendDenyedFunc" << PeerNum_ << endl;
}
void FriendBlockedFunc(TPeerCnt PeerNum_, TUID FriendUID_)
{
	wcout << L"FriendBlockedFunc" << PeerNum_ << endl;
}
void FriendUnBlockedFunc(TPeerCnt PeerNum_, TUID FriendUID_)
{
	wcout << L"FriendUnBlockedFunc" << PeerNum_ << endl;
}
void StateChangedFunc(TPeerCnt PeerNum_, TState State_)
{
	wcout << L"StateChangedFunc" << PeerNum_ << endl;
}
void FriendStateChangedFunc(TPeerCnt PeerNum_, TUID FriendUID_, TState FriendState_)
{
	wcout << L"FriendStateChangedFunc" << PeerNum_ << " FriendUID:" << FriendUID_ << " FriendState:" << static_cast<int32>(FriendState_) << endl;
}
void MessageReceivedFunc(TPeerCnt PeerNum_, TUID FriendUID_, const TMessage& Message_)
{
	wcout << L"MessageReceivedFunc" << PeerNum_ << " FriendUID:" << FriendUID_ << endl;
}

void RetNetSc(const CKey& Key_, CStream& Stream_)
{
	SRetNetSc Proto;
	Stream_ >> Proto;
}
void MsgNetSc(const CKey& Key_, CStream& Stream_)
{
	SMsgNetSc Proto;
	Stream_ >> Proto;
}
void LoginNetSc(const CKey& Key_, CStream& Stream_)
{
	SLoginNetSc Proto;
	Stream_ >> Proto;
}
void LobbyNetSc(const CKey& Key_, CStream& Stream_)
{
	SLobbyNetSc Proto;
	Stream_ >> Proto;
}
void ChatNetSc(const CKey& Key_, CStream& Stream_)
{
	SChatNetSc Proto;
	Stream_ >> Proto;
}
void PurchaseNetSc(const CKey& Key_, CStream& Stream_)
{
	SPurchaseNetSc Proto;
	Stream_ >> Proto;
}
void ChangeNickNetSc(const CKey& Key_, CStream& Stream_)
{
	SChangeNickNetSc Proto;
	Stream_ >> Proto;
}
void ChangeNickFailNetSc(const CKey& Key_, CStream& Stream_)
{
	SChangeNickFailNetSc Proto;
	Stream_ >> Proto;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		CJson OptionValue(CStream(L"Option.ini"));

		SVersion Version(1, 0);

		wstring ProcName(MBSToWCS(
			OptionValue["Title"].asString() +
			" NetVer:" + WCSToMBS(StringFormat(L"%d", int32(Version.Main)))));
		SetConsoleTitle(ProcName.c_str());

		g_Net.reset(new TClient(Version));
		g_Net->LinkFunc = LoginFunc;
		g_Net->LinkFailFunc = LoginFailFunc;
		g_Net->UnLinkFunc = LogoutFunc;
		g_Net->RecvFunc = RecvFunc;
		//g_Net->LinkFuncSoft = LinkFuncSoft;
		//g_Net->UnLinkFuncSoft = UnLinkFuncSoft;
		g_Net->ErrorFunc = ErrorFunc;
		g_Net->CheckFunc = CheckFunc;
		g_Net->FriendAddedFunc = FriendAddedFunc;
		g_Net->FriendRequestedFunc = FriendRequestedFunc;
		g_Net->FriendAllowedFunc = FriendAllowedFunc;
		g_Net->FriendDenyedFunc = FriendDenyedFunc;
		g_Net->FriendBlockedFunc = FriendBlockedFunc;
		g_Net->FriendUnBlockedFunc = FriendUnBlockedFunc;
		g_Net->StateChangedFunc = StateChangedFunc;
		g_Net->FriendStateChangedFunc = FriendStateChangedFunc;
		g_Net->MessageReceivedFunc = MessageReceivedFunc;

		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::Ret, RetNetSc);
		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::Msg, MsgNetSc);
		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::Login, LoginNetSc);
		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::Lobby, LobbyNetSc);
		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::Chat, ChatNetSc);
		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::Purchase, PurchaseNetSc);
		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::ChangeNick, ChangeNickNetSc);
		g_BinderRecvS.emplace_at((size_t)EProtoNetSc::ChangeNickFail, ChangeNickFailNetSc);

		int mode = 0;
		while (true)
		{
			//switch (rand() % 4)
			//{
			//case 0:
			//	if (!g_Net->Login(MBSToWCS(OptionValue["DataPath"].asString()), CNamePort(OptionValue["HubIP"].asString(), OptionValue["HubPort"].asInt()), SAccount(L"a", L"a")))
			//	THROWEX();
			//	break;
			//case 1:
			//	if (!g_Net->Login(MBSToWCS(OptionValue["DataPath"].asString()), CNamePort(OptionValue["HubIP"].asString(), OptionValue["HubPort"].asInt()), SAccount(L"b", L"b")))
			//	THROWEX();
			//	break;
			//case 2:
			//	Send(g_Key, SGameProtoNetCs(true, 9, 8, 7, L"Large", CDateTime(), system_clock::now()));
			//	break;
			//case 3:
			//	g_Net->Logout();
			//	break;
			//}

			auto* InputKey = g_Input.Get();
			if (InputKey)
			{
				switch (InputKey->Event.KeyEvent.uChar.AsciiChar)
				{
				case 'l':
				{
					CStream stm;
					stm << SUserLoginOption(EOS::Android);
					//auto Ret = g_Net->Login(0, L"LoginInfo", CNamePort("121.137.228.80", 30130), u"IDID", 0, stm);
					auto Ret = g_Net->Login(0, L"LoginInfo", CNamePort("127.0.0.1", 30130), u"IDID", 0, stm);
					if (!Ret)
					{
						cout << "Login Fail" << endl;
						break;
					}
					break;
				}

				case 'L':
				{
					CStream stm;
					stm << SUserLoginOption(EOS::Android);
					//auto Ret = g_Net->Login(0, L"LoginInfo", CNamePort("121.137.228.80", 30130), u"IDID", 0, stm);
					auto Ret = g_Net->Login(0, L"LoginInfo", CNamePort("127.0.0.1", 30130), u"IDID2", 0, stm);
					if (!Ret)
					{
						cout << "Login Fail" << endl;
						break;
					}
					break;
				}

				case 'c':
				{
					CStream stm;
					stm << SUserCreateOption(SUserLoginOption(EOS::Android));
					//g_Net->Create(L"LoginInfo", CNamePort("121.137.228.80", 30130), u"IDID", u"NickNick", 0, stm);
					g_Net->Create(0, L"LoginInfo", CNamePort("127.0.0.1", 30130), u"IDID", u"NickNick", 0, 1, stm);
					break;
				}

				case 'C':
				{
					CStream stm;
					stm << SUserCreateOption(SUserLoginOption(EOS::Android));
					//g_Net->Create(L"LoginInfo", CNamePort("121.137.228.80", 30130), u"IDID", u"NickNick", 0, stm);
					g_Net->Create(0, L"LoginInfo", CNamePort("127.0.0.1", 30130), u"IDID2", u"NickNick2", 0, 1, stm);
					break;
				}

				case 'v':
				{
					CStream stm;
					stm << SUserCreateOption(SUserLoginOption(EOS::Android));
					//g_Net->Create(L"LoginInfo", CNamePort("121.137.228.80", 30130), u"IDID", u"NickNick", 0, stm);
					g_Net->Create(0, L"LoginInfo", CNamePort("127.0.0.1", 30130), u"IDID3", u"NickNick3", 0, 1, stm);
					break;
				}

				case 'V':
				{
					CStream stm;
					stm << SUserCreateOption(SUserLoginOption(EOS::Android));
					//g_Net->Create(L"LoginInfo", CNamePort("121.137.228.80", 30130), u"IDID", u"NickNick", 0, stm);
					g_Net->Create(0, L"LoginInfo", CNamePort("127.0.0.1", 30130), u"IDID4", u"NickNick4", 0, 1, stm);
					break;
				}

				case 'o':
					g_Net->Logout();
					break;

				case 't':
					Send(g_Key, SChatNetCs(u"hi~"));
					break;

				case 'f':
					g_Net->AddFriend(g_Key.PeerNum, u"NickNick"s);
					break;

				case 'F':
					g_Net->AddFriend(g_Key.PeerNum, u"NickNick2"s);
					break;

				case 'g':
					g_Net->AddFriend(g_Key.PeerNum, u"NickNick3"s);
					break;

				case 'G':
					g_Net->AddFriend(g_Key.PeerNum, u"NickNick4"s);
					break;

				case 'a':
					g_Net->AllowFriend(g_Key.PeerNum, 1);
					break;

				case 'A':
					g_Net->AllowFriend(g_Key.PeerNum, 2);
					break;

				case 'q':
					g_Net->AllowFriend(g_Key.PeerNum, 3);
					break;

				case 'Q':
					g_Net->AllowFriend(g_Key.PeerNum, 4);
					break;

				case 'd':
					g_Net->DenyFriend(g_Key.PeerNum, 1);
					break;

				case 'D':
					g_Net->DenyFriend(g_Key.PeerNum, 2);
					break;

				case 'e':
					g_Net->DenyFriend(g_Key.PeerNum, 3);
					break;

				case 'E':
					g_Net->DenyFriend(g_Key.PeerNum, 4);
					break;

				case 'b':
					g_Net->BlockFriend(g_Key.PeerNum, 1);
					break;

				case 'B':
					g_Net->BlockFriend(g_Key.PeerNum, 2);
					break;

				case 'u':
					g_Net->UnBlockFriend(g_Key.PeerNum, 1);
					break;

				case 'U':
					g_Net->UnBlockFriend(g_Key.PeerNum, 2);
					break;

				case 'm':
					g_Net->MessageSend(g_Key.PeerNum, 1, u"hihi");
					break;

				case 'M':
					g_Net->MessageSend(g_Key.PeerNum, 2, u"hihi");
					break;

				case 's':
					g_Net->ChangeState(g_Key.PeerNum, 1);
					break;

				case 'S':
					g_Net->ChangeState(g_Key.PeerNum, 2);
					break;
				}
			}

			g_Net->Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}

	return 0;
}
