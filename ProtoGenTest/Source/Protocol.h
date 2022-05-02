#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

struct NamePort : public SProto
{
	string name{};
	int64 port{};
	NamePort()
	{
	}
	NamePort(const string& name_, const int64& port_) : name(name_), port(port_)
	{
	}
	NamePort(string&& name_, int64&& port_) : name(std::move(name_)), port(std::move(port_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> name;
		Stream_ >> port;
	}
	void operator << (const Value& Value_) override
	{
		Value_["name"] >> name;
		Value_["port"] >> port;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << name;
		Stream_ << port;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["name"] = name;
		Value_["port"] = port;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(string()) + L"," + 
			GetStdName(int64());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(string(), L"name") + L"," + 
			GetMemberName(int64(), L"port");
	}
};
