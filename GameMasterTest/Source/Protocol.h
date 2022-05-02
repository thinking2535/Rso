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
	SRangeUID RangeUID{};
	SNamePort AuthNamePort{};
	TPort ServerBindPort{};
	int32 NetCThreadCnt{};
	TPort ClientBindPort{};
	SNamePort ClientBindNamePortPub{};
	TPort MasterBindPort{};
	string Title{};
	bool EnableReceipt{};
	SOption()
	{
	}
	SOption(const TPort& MonitorBindPort_, const SRangeUID& RangeUID_, const SNamePort& AuthNamePort_, const TPort& ServerBindPort_, const int32& NetCThreadCnt_, const TPort& ClientBindPort_, const SNamePort& ClientBindNamePortPub_, const TPort& MasterBindPort_, const string& Title_, const bool& EnableReceipt_) : MonitorBindPort(MonitorBindPort_), RangeUID(RangeUID_), AuthNamePort(AuthNamePort_), ServerBindPort(ServerBindPort_), NetCThreadCnt(NetCThreadCnt_), ClientBindPort(ClientBindPort_), ClientBindNamePortPub(ClientBindNamePortPub_), MasterBindPort(MasterBindPort_), Title(Title_), EnableReceipt(EnableReceipt_)
	{
	}
	SOption(TPort&& MonitorBindPort_, SRangeUID&& RangeUID_, SNamePort&& AuthNamePort_, TPort&& ServerBindPort_, int32&& NetCThreadCnt_, TPort&& ClientBindPort_, SNamePort&& ClientBindNamePortPub_, TPort&& MasterBindPort_, string&& Title_, bool&& EnableReceipt_) : MonitorBindPort(std::move(MonitorBindPort_)), RangeUID(std::move(RangeUID_)), AuthNamePort(std::move(AuthNamePort_)), ServerBindPort(std::move(ServerBindPort_)), NetCThreadCnt(std::move(NetCThreadCnt_)), ClientBindPort(std::move(ClientBindPort_)), ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), MasterBindPort(std::move(MasterBindPort_)), Title(std::move(Title_)), EnableReceipt(std::move(EnableReceipt_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> MonitorBindPort;
		Stream_ >> RangeUID;
		Stream_ >> AuthNamePort;
		Stream_ >> ServerBindPort;
		Stream_ >> NetCThreadCnt;
		Stream_ >> ClientBindPort;
		Stream_ >> ClientBindNamePortPub;
		Stream_ >> MasterBindPort;
		Stream_ >> Title;
		Stream_ >> EnableReceipt;
	}
	void operator << (const Value& Value_) override
	{
		Value_["MonitorBindPort"] >> MonitorBindPort;
		Value_["RangeUID"] >> RangeUID;
		Value_["AuthNamePort"] >> AuthNamePort;
		Value_["ServerBindPort"] >> ServerBindPort;
		Value_["NetCThreadCnt"] >> NetCThreadCnt;
		Value_["ClientBindPort"] >> ClientBindPort;
		Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
		Value_["MasterBindPort"] >> MasterBindPort;
		Value_["Title"] >> Title;
		Value_["EnableReceipt"] >> EnableReceipt;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << MonitorBindPort;
		Stream_ << RangeUID;
		Stream_ << AuthNamePort;
		Stream_ << ServerBindPort;
		Stream_ << NetCThreadCnt;
		Stream_ << ClientBindPort;
		Stream_ << ClientBindNamePortPub;
		Stream_ << MasterBindPort;
		Stream_ << Title;
		Stream_ << EnableReceipt;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["MonitorBindPort"] = MonitorBindPort;
		Value_["RangeUID"] = RangeUID;
		Value_["AuthNamePort"] = AuthNamePort;
		Value_["ServerBindPort"] = ServerBindPort;
		Value_["NetCThreadCnt"] = NetCThreadCnt;
		Value_["ClientBindPort"] = ClientBindPort;
		Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
		Value_["MasterBindPort"] = MasterBindPort;
		Value_["Title"] = Title;
		Value_["EnableReceipt"] = EnableReceipt;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(TPort()) + L"," + 
			GetStdName(SRangeUID()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(TPort()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(TPort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(TPort()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(bool());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(TPort(), L"MonitorBindPort") + L"," + 
			GetMemberName(SRangeUID(), L"RangeUID") + L"," + 
			GetMemberName(SNamePort(), L"AuthNamePort") + L"," + 
			GetMemberName(TPort(), L"ServerBindPort") + L"," + 
			GetMemberName(int32(), L"NetCThreadCnt") + L"," + 
			GetMemberName(TPort(), L"ClientBindPort") + L"," + 
			GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
			GetMemberName(TPort(), L"MasterBindPort") + L"," + 
			GetMemberName(string(), L"Title") + L"," + 
			GetMemberName(bool(), L"EnableReceipt");
	}
};
enum class EProtoNetSm
{
	GameProto,
	Max,
};
enum class EProtoNetMs
{
	GameProto,
	Max,
};
struct SGameProtoNetSm : public SProto
{
	bool b{};
	int16 i16{};
	int32 i32{};
	int64 i64{};
	wstring w{};
	SDateTime dt{};
	system_clock::time_point tp{};
	SGameProtoNetSm()
	{
	}
	SGameProtoNetSm(const bool& b_, const int16& i16_, const int32& i32_, const int64& i64_, const wstring& w_, const SDateTime& dt_, const system_clock::time_point& tp_) : b(b_), i16(i16_), i32(i32_), i64(i64_), w(w_), dt(dt_), tp(tp_)
	{
	}
	SGameProtoNetSm(bool&& b_, int16&& i16_, int32&& i32_, int64&& i64_, wstring&& w_, SDateTime&& dt_, system_clock::time_point&& tp_) : b(std::move(b_)), i16(std::move(i16_)), i32(std::move(i32_)), i64(std::move(i64_)), w(std::move(w_)), dt(std::move(dt_)), tp(std::move(tp_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> b;
		Stream_ >> i16;
		Stream_ >> i32;
		Stream_ >> i64;
		Stream_ >> w;
		Stream_ >> dt;
		Stream_ >> tp;
	}
	void operator << (const Value& Value_) override
	{
		Value_["b"] >> b;
		Value_["i16"] >> i16;
		Value_["i32"] >> i32;
		Value_["i64"] >> i64;
		Value_["w"] >> w;
		Value_["dt"] >> dt;
		Value_["tp"] >> tp;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << b;
		Stream_ << i16;
		Stream_ << i32;
		Stream_ << i64;
		Stream_ << w;
		Stream_ << dt;
		Stream_ << tp;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["b"] = b;
		Value_["i16"] = i16;
		Value_["i32"] = i32;
		Value_["i64"] = i64;
		Value_["w"] = w;
		Value_["dt"] = dt;
		Value_["tp"] = tp;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(bool()) + L"," + 
			GetStdName(int16()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(int64()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(SDateTime()) + L"," + 
			GetStdName(system_clock::time_point());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(bool(), L"b") + L"," + 
			GetMemberName(int16(), L"i16") + L"," + 
			GetMemberName(int32(), L"i32") + L"," + 
			GetMemberName(int64(), L"i64") + L"," + 
			GetMemberName(wstring(), L"w") + L"," + 
			GetMemberName(SDateTime(), L"dt") + L"," + 
			GetMemberName(system_clock::time_point(), L"tp");
	}
};
struct SGameProtoNetMs : public SProto
{
	bool b{};
	int16 i16{};
	int32 i32{};
	int64 i64{};
	wstring w{};
	SDateTime dt{};
	system_clock::time_point tp{};
	SGameProtoNetMs()
	{
	}
	SGameProtoNetMs(const bool& b_, const int16& i16_, const int32& i32_, const int64& i64_, const wstring& w_, const SDateTime& dt_, const system_clock::time_point& tp_) : b(b_), i16(i16_), i32(i32_), i64(i64_), w(w_), dt(dt_), tp(tp_)
	{
	}
	SGameProtoNetMs(bool&& b_, int16&& i16_, int32&& i32_, int64&& i64_, wstring&& w_, SDateTime&& dt_, system_clock::time_point&& tp_) : b(std::move(b_)), i16(std::move(i16_)), i32(std::move(i32_)), i64(std::move(i64_)), w(std::move(w_)), dt(std::move(dt_)), tp(std::move(tp_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> b;
		Stream_ >> i16;
		Stream_ >> i32;
		Stream_ >> i64;
		Stream_ >> w;
		Stream_ >> dt;
		Stream_ >> tp;
	}
	void operator << (const Value& Value_) override
	{
		Value_["b"] >> b;
		Value_["i16"] >> i16;
		Value_["i32"] >> i32;
		Value_["i64"] >> i64;
		Value_["w"] >> w;
		Value_["dt"] >> dt;
		Value_["tp"] >> tp;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << b;
		Stream_ << i16;
		Stream_ << i32;
		Stream_ << i64;
		Stream_ << w;
		Stream_ << dt;
		Stream_ << tp;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["b"] = b;
		Value_["i16"] = i16;
		Value_["i32"] = i32;
		Value_["i64"] = i64;
		Value_["w"] = w;
		Value_["dt"] = dt;
		Value_["tp"] = tp;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(bool()) + L"," + 
			GetStdName(int16()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(int64()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(SDateTime()) + L"," + 
			GetStdName(system_clock::time_point());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(bool(), L"b") + L"," + 
			GetMemberName(int16(), L"i16") + L"," + 
			GetMemberName(int32(), L"i32") + L"," + 
			GetMemberName(int64(), L"i64") + L"," + 
			GetMemberName(wstring(), L"w") + L"," + 
			GetMemberName(SDateTime(), L"dt") + L"," + 
			GetMemberName(system_clock::time_point(), L"tp");
	}
};
