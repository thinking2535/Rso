#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

#include <Rso/Net/Protocol.h>
#include <Rso/MSSQL/Protocol.h>
#include <Rso/GameUtil/Protocol.h>
using namespace std;
using namespace rso;
using namespace core;
using namespace json;
using namespace net;
struct SServerOption : public SProto
{
	TPort MonitorBindPort{};
	SNamePort MasterNamePort{};
	int32 NetCThreadCnt{};
	TPort ClientBindPort{};
	SNamePort ClientBindNamePortPub{};
	TPort MessengerPort{};
	string Title{};
	int8 Debug{};
	bool EndlessGame{};
	SServerOption()
	{
	}
	SServerOption(const TPort& MonitorBindPort_, const SNamePort& MasterNamePort_, const int32& NetCThreadCnt_, const TPort& ClientBindPort_, const SNamePort& ClientBindNamePortPub_, const TPort& MessengerPort_, const string& Title_, const int8& Debug_, const bool& EndlessGame_) : MonitorBindPort(MonitorBindPort_), MasterNamePort(MasterNamePort_), NetCThreadCnt(NetCThreadCnt_), ClientBindPort(ClientBindPort_), ClientBindNamePortPub(ClientBindNamePortPub_), MessengerPort(MessengerPort_), Title(Title_), Debug(Debug_), EndlessGame(EndlessGame_)
	{
	}
	SServerOption(TPort&& MonitorBindPort_, SNamePort&& MasterNamePort_, int32&& NetCThreadCnt_, TPort&& ClientBindPort_, SNamePort&& ClientBindNamePortPub_, TPort&& MessengerPort_, string&& Title_, int8&& Debug_, bool&& EndlessGame_) : MonitorBindPort(std::move(MonitorBindPort_)), MasterNamePort(std::move(MasterNamePort_)), NetCThreadCnt(std::move(NetCThreadCnt_)), ClientBindPort(std::move(ClientBindPort_)), ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), MessengerPort(std::move(MessengerPort_)), Title(std::move(Title_)), Debug(std::move(Debug_)), EndlessGame(std::move(EndlessGame_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> MonitorBindPort;
		Stream_ >> MasterNamePort;
		Stream_ >> NetCThreadCnt;
		Stream_ >> ClientBindPort;
		Stream_ >> ClientBindNamePortPub;
		Stream_ >> MessengerPort;
		Stream_ >> Title;
		Stream_ >> Debug;
		Stream_ >> EndlessGame;
	}
	void operator << (const Value& Value_) override
	{
		Value_["MonitorBindPort"] >> MonitorBindPort;
		Value_["MasterNamePort"] >> MasterNamePort;
		Value_["NetCThreadCnt"] >> NetCThreadCnt;
		Value_["ClientBindPort"] >> ClientBindPort;
		Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
		Value_["MessengerPort"] >> MessengerPort;
		Value_["Title"] >> Title;
		Value_["Debug"] >> Debug;
		Value_["EndlessGame"] >> EndlessGame;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << MonitorBindPort;
		Stream_ << MasterNamePort;
		Stream_ << NetCThreadCnt;
		Stream_ << ClientBindPort;
		Stream_ << ClientBindNamePortPub;
		Stream_ << MessengerPort;
		Stream_ << Title;
		Stream_ << Debug;
		Stream_ << EndlessGame;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["MonitorBindPort"] = MonitorBindPort;
		Value_["MasterNamePort"] = MasterNamePort;
		Value_["NetCThreadCnt"] = NetCThreadCnt;
		Value_["ClientBindPort"] = ClientBindPort;
		Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
		Value_["MessengerPort"] = MessengerPort;
		Value_["Title"] = Title;
		Value_["Debug"] = Debug;
		Value_["EndlessGame"] = EndlessGame;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(TPort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(TPort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(TPort()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(int8()) + L"," + 
			GetStdName(bool());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(TPort(), L"MonitorBindPort") + L"," + 
			GetMemberName(SNamePort(), L"MasterNamePort") + L"," + 
			GetMemberName(int32(), L"NetCThreadCnt") + L"," + 
			GetMemberName(TPort(), L"ClientBindPort") + L"," + 
			GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
			GetMemberName(TPort(), L"MessengerPort") + L"," + 
			GetMemberName(string(), L"Title") + L"," + 
			GetMemberName(int8(), L"Debug") + L"," + 
			GetMemberName(bool(), L"EndlessGame");
	}
};
