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
using namespace std;
using namespace rso;
using namespace core;
using namespace json;
using namespace net;
struct SOption : public SProto
{
	TPort MonitorBindPort{};
	TPort MasterBindPort{};
	int32 NetCThreadCnt{};
	TPort ClientBindPort{};
	string Title{};
	string FCMServerKey{};
	int32 ChangeNickFreeCount{};
	SOption()
	{
	}
	SOption(const TPort& MonitorBindPort_, const TPort& MasterBindPort_, const int32& NetCThreadCnt_, const TPort& ClientBindPort_, const string& Title_, const string& FCMServerKey_, const int32& ChangeNickFreeCount_) : MonitorBindPort(MonitorBindPort_), MasterBindPort(MasterBindPort_), NetCThreadCnt(NetCThreadCnt_), ClientBindPort(ClientBindPort_), Title(Title_), FCMServerKey(FCMServerKey_), ChangeNickFreeCount(ChangeNickFreeCount_)
	{
	}
	SOption(TPort&& MonitorBindPort_, TPort&& MasterBindPort_, int32&& NetCThreadCnt_, TPort&& ClientBindPort_, string&& Title_, string&& FCMServerKey_, int32&& ChangeNickFreeCount_) : MonitorBindPort(std::move(MonitorBindPort_)), MasterBindPort(std::move(MasterBindPort_)), NetCThreadCnt(std::move(NetCThreadCnt_)), ClientBindPort(std::move(ClientBindPort_)), Title(std::move(Title_)), FCMServerKey(std::move(FCMServerKey_)), ChangeNickFreeCount(std::move(ChangeNickFreeCount_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> MonitorBindPort;
		Stream_ >> MasterBindPort;
		Stream_ >> NetCThreadCnt;
		Stream_ >> ClientBindPort;
		Stream_ >> Title;
		Stream_ >> FCMServerKey;
		Stream_ >> ChangeNickFreeCount;
	}
	void operator << (const Value& Value_) override
	{
		Value_["MonitorBindPort"] >> MonitorBindPort;
		Value_["MasterBindPort"] >> MasterBindPort;
		Value_["NetCThreadCnt"] >> NetCThreadCnt;
		Value_["ClientBindPort"] >> ClientBindPort;
		Value_["Title"] >> Title;
		Value_["FCMServerKey"] >> FCMServerKey;
		Value_["ChangeNickFreeCount"] >> ChangeNickFreeCount;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << MonitorBindPort;
		Stream_ << MasterBindPort;
		Stream_ << NetCThreadCnt;
		Stream_ << ClientBindPort;
		Stream_ << Title;
		Stream_ << FCMServerKey;
		Stream_ << ChangeNickFreeCount;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["MonitorBindPort"] = MonitorBindPort;
		Value_["MasterBindPort"] = MasterBindPort;
		Value_["NetCThreadCnt"] = NetCThreadCnt;
		Value_["ClientBindPort"] = ClientBindPort;
		Value_["Title"] = Title;
		Value_["FCMServerKey"] = FCMServerKey;
		Value_["ChangeNickFreeCount"] = ChangeNickFreeCount;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(TPort()) + L"," + 
			GetStdName(TPort()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(TPort()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(int32());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(TPort(), L"MonitorBindPort") + L"," + 
			GetMemberName(TPort(), L"MasterBindPort") + L"," + 
			GetMemberName(int32(), L"NetCThreadCnt") + L"," + 
			GetMemberName(TPort(), L"ClientBindPort") + L"," + 
			GetMemberName(string(), L"Title") + L"," + 
			GetMemberName(string(), L"FCMServerKey") + L"," + 
			GetMemberName(int32(), L"ChangeNickFreeCount");
	}
};
