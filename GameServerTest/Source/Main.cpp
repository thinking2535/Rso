#include "stdafx.h"

void DBCallback(TSessionsIt itSession_, TOutObject& OutObject_)
{
}
void Check(TSessionsIt itSession_, TSessionCode SessionCode_)
{
	LOG(StringFormat(L"Check UID : %d", itSession_->first));
}
void LinkM(const CKey& Key_)
{
	LOG(StringFormat(L"LinkM PeerNum:%d", Key_.PeerNum).c_str());
}
void LinkFailM(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	LOG(StringFormat(L"LinkFailM PeerNum:%d", PeerNum_).c_str());
}
void UnLinkM(const CKey& Key_, ENetRet NetRet_)
{
	LOG(StringFormat(L"UnLinkM PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvM(const CKey& Key_, CStream& Stream_)
{
	DLOG(StringFormat(L"RecvM PeerNum:%d", Key_.PeerNum).c_str());

	EProtoNetMs ProtoNum;
	Stream_ >> ProtoNum;

	auto itRecv = g_BinderRecvM.get(static_cast<size_t>(ProtoNum));
	if (!itRecv)
		THROWEX();

	(*itRecv)(Key_, Stream_);
}
void LinkC(TSessionsIt itSession_, const CKey& Key_, bool Create_, CStream& Stream_)
{
	g_Net->Certify(Key_.PeerNum);
	DLOG(StringFormat(L"Login PeerNum:%d UID:%d", Key_.PeerNum, itSession_->first).c_str());
}
void UnLinkC(TSessionsIt itSession_, const CKey& Key_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"Logout PeerNum:%d NetRet:%d", Key_.PeerNum, int32(NetRet_)).c_str());
}
void RecvC(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_)
{
	EProtoNetCs ProtoNum;
	Stream_ >> ProtoNum;

	auto itRecv = g_BinderRecvC.get(static_cast<size_t>(ProtoNum));
	if (!itRecv)
	{
		g_Net->Close(Key_);
		return;
	}

	(*itRecv)(Key_, Stream_);
}

void RecvSessionHold(TSessionsIt itSession_, CStream& Stream_)
{
	DLOG(StringFormat(L"RecvSessionHold UID:%d", itSession_->first).c_str());
}
void RecvSendToServer(TSessionsIt itSession_, CStream& Stream_)
{
	DLOG(StringFormat(L"RecvSendToServer UID:%d", itSession_->first).c_str());
}
void RecvPunish(TUID UID_, const TTime& EndTime_)
{
	DLOG(StringFormat(L"RecvPunish UID:%d", UID_).c_str());
}
void RecvChangeNick(const CKey& Key_, const TNick& Nick_)
{
	DLOG(StringFormat(L"RecvChangeNick").c_str());
}
void RecvChangeNickFail(const CKey& Key_, EGameRet GameRet_)
{
	DLOG(StringFormat(L"RecvChangeNick GameRet:%d", static_cast<int32>(GameRet_)).c_str());
}
void ChatNetCs(const CKey& Key_, CStream& Stream_)
{
	SChatNetCs Proto;
	Stream_ >> Proto;

	//wcout << Proto.b << endl;
	//wcout << Proto.i16 << endl;
	//wcout << Proto.i32 << endl;
	//cout << Proto.s << endl;
	//wcout << Proto.w << endl;
	//wcout << Proto.tp.time_since_epoch().count() << endl;

	cout << "CsProto" << endl;
}
void GameProtoNetMs(const CKey& Key_, CStream& Stream_)
{
	SGameProtoNetMs Proto;
	Stream_ >> Proto;

	//wcout << Proto.b << endl;
	//wcout << Proto.i16 << endl;
	//wcout << Proto.i32 << endl;
	//cout << Proto.s << endl;
	//wcout << Proto.w << endl;
	//wcout << Proto.tp.time_since_epoch().count() << endl;

	cout << "MsProto" << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		COptionJson<SServerOption> Option(L"Option.ini", true);
		COptionJson<SDBOption> DBOption(L"DBOption.ini", true);

		SVersion Version(1, 0);

		wstring ProcName(MBSToWCS(
			Option->Title +
			" NetVer:" + WCSToMBS(StringFormat(L"%d", int32(Version.Main))) +
			" ClientBindPort:" + to_string(Option->ClientBindPort)));
		SetConsoleTitle(ProcName.c_str());

		g_Net.reset(new TServer(
			EAddressFamily::INET,
			Check, LinkM, LinkFailM, UnLinkM, RecvM, LinkC, UnLinkC, RecvC, RecvSessionHold, RecvSendToServer, RecvPunish, RecvChangeNick, RecvChangeNickFail, DBCallback,
			Version, *DBOption,
			CNamePort(Option->MasterNamePort), Option->NetCThreadCnt, CNamePort(Option->ClientBindPort), CNamePort(Option->ClientBindNamePortPub), Option->MessengerPort,
			false));

		// Add Bind
		g_BinderRecvM.emplace_at((size_t)EProtoNetMs::GameProto, GameProtoNetMs);
		g_BinderRecvC.emplace_at((size_t)EProtoNetCs::Chat, ChatNetCs);

		g_Net->Start();

		while (true)
		{
			auto* InputKey = g_Input.Get();
			if (InputKey)
			{
				switch (InputKey->Event.KeyEvent.uChar.AsciiChar)
				{
				case 's':
					SendMAll(SGameProtoNetSm(true, 9, 8, 7, L"Large", SDateTime(), system_clock::now()));
					break;
				}
			}

//			Send(CKey(), SGameProtoNetSc());
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
