#include "stdafx.h"


using TOutObject = game::CMaster::TOutObject;


void LinkA(const CKey& Key_)
{
	LOG(StringFormat(L"LinkA PeerNum:%d", Key_.PeerNum).c_str());
}
void LinkFailA(TPeerCnt PeerNum_, ENetRet NetRet_)
{
	LOG(StringFormat(L"LinkFailA PeerNum:%d", PeerNum_).c_str());
}
void UnLinkA(const CKey& Key_, ENetRet NetRet_)
{
	LOG(StringFormat(L"UnLinkA PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvA(const CKey& Key_, CStream& Stream_)
{
	LOG(StringFormat(L"RecvA PeerNum:%d", Key_.PeerNum).c_str());
}
void LinkS(const CKey& Key_)
{
	LOG(StringFormat(L"LinkS PeerNum:%d", Key_.PeerNum).c_str());
}
void UnLinkS(const CKey& Key_, ENetRet NetRet_)
{
	LOG(StringFormat(L"UnLinkS PeerNum:%d", Key_.PeerNum).c_str());
}
void RecvS(const CKey& Key_, CStream& Stream_)
{
	DLOG(StringFormat(L"RecvS PeerNum:%d", Key_.PeerNum).c_str());

	EProtoNetSm ProtoNum;
	Stream_ >> ProtoNum;

	auto itRecv = g_BinderRecvS.get(static_cast<size_t>(ProtoNum));
	if (!itRecv)
		THROWEX();

	(*itRecv)(Key_, Stream_);
}
void LinkC(const CKey& Key_)
{
	DLOG(StringFormat(L"LinkC PeerNum:%d", Key_.PeerNum).c_str());
}
void UnLinkC(const CKey& Key_, ENetRet NetRet_)
{
	DLOG(StringFormat(L"UnLinkC PeerNum:%d", Key_.PeerNum).c_str());
}
void SessionBegin(TPeerCnt ServerNum_, TUID UID_)
{
	DLOG(StringFormat(L"SessionBegin ServerNum:%d UID:%d", ServerNum_, UID_).c_str());
}
void SessionEnd(TPeerCnt ServerNum_, TUID UID_)
{
	DLOG(StringFormat(L"SessionEnd ServerNum:%d UID:%d", ServerNum_, UID_).c_str());
}
void RecvPunish(TUID UID_, const TTime& EndTime_)
{
	DLOG(StringFormat(L"RecvPunish UID:%d", UID_).c_str());
}


void GameProtoNetSm(const CKey& Key_, CStream& Stream_)
{
	SGameProtoNetSm Proto;
	Stream_ >> Proto;

	//wcout << Proto.b << endl;
	//wcout << Proto.i16 << endl;
	//wcout << Proto.i32 << endl;
	//cout << Proto.s << endl;
	//wcout << Proto.w << endl;
	//wcout << Proto.tp.time_since_epoch().count() << endl;

	cout << "SmProto" << endl;
}

void DBCallback(TOutObject& OutObject_)
{

}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		LOG(L"Initializing");

		COptionJson<SOption> Option(L"Option.ini", true);
		COptionJson<SDBOption> DBOption(L"DBOption.ini", true);

		wstring ProcName(MBSToWCS(
			Option->Title +
			to_string(Option->ClientBindPort)));
		SetConsoleTitle(ProcName.c_str());

		g_Net.reset(new game::CMaster(
			EAddressFamily::INET,
			LinkA, LinkFailA, UnLinkA, RecvA,
			LinkS, UnLinkS, RecvS,
			LinkC, UnLinkC,
			SessionBegin, SessionEnd, RecvPunish, DBCallback,
			255, 32, 32, Option->RangeUID, *DBOption,
			CNamePort(Option->AuthNamePort),
			CNamePort(Option->ServerBindPort),
			Option->NetCThreadCnt,
			CNamePort(Option->ClientBindPort),
			CNamePort(Option->ClientBindNamePortPub),
			Option->MasterBindPort));

		g_Net->Start();

		g_BinderRecvS.emplace_at((size_t)EProtoNetSm::GameProto, GameProtoNetSm);

		while (true)
		{
			g_Net->Proc();
			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}
	catch (...)
	{

	}

	return 0;
}
