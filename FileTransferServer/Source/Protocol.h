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
using namespace rso;
using namespace net;
struct SOption : public SProto
{
	wstring Title;
	__int32 NetCThreadCnt = 0;
	SStrIPPort IPPort;
	SOption()
	{
	}
	SOption(const wstring& Title_, const __int32& NetCThreadCnt_, const SStrIPPort& IPPort_) : Title(Title_), NetCThreadCnt(NetCThreadCnt_), IPPort(IPPort_)
	{
	}
	SOption(wstring&& Title_, __int32&& NetCThreadCnt_, SStrIPPort&& IPPort_) : Title(std::move(Title_)), NetCThreadCnt(std::move(NetCThreadCnt_)), IPPort(std::move(IPPort_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Title;
		Stream_ >> NetCThreadCnt;
		Stream_ >> IPPort;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Title"] >> Title;
		Value_["NetCThreadCnt"] >> NetCThreadCnt;
		IPPort << Value_["IPPort"];
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Title;
		Stream_ << NetCThreadCnt;
		Stream_ << IPPort;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Title"] = Title;
		Value_["NetCThreadCnt"] = NetCThreadCnt;
		Value_["IPPort"] = IPPort;
	}
	static wstring StdName(void)
	{
		return L"wstring,int32,string,uint16";
	}
	static wstring MemberName(void)
	{
		return L"Title,NetCThreadCnt,IP,Port";
	}
};
