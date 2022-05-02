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
	string IP{};
	TPort Port{};
	SOption()
	{
	}
	SOption(const string& IP_, const TPort& Port_) : IP(IP_), Port(Port_)
	{
	}
	SOption(string&& IP_, TPort&& Port_) : IP(std::move(IP_)), Port(std::move(Port_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> IP;
		Stream_ >> Port;
	}
	void operator << (const Value& Value_) override
	{
		Value_["IP"] >> IP;
		Value_["Port"] >> Port;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << IP;
		Stream_ << Port;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["IP"] = IP;
		Value_["Port"] = Port;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(string()) + L"," + 
			GetStdName(TPort());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(string(), L"IP") + L"," + 
			GetMemberName(TPort(), L"Port");
	}
};
