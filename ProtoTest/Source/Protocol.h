#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

using namespace rso;
using namespace core;
struct STest : public SProto
{
	milliseconds a{};
	STest()
	{
	}
	STest(const milliseconds& a_) : a(a_)
	{
	}
	STest(milliseconds&& a_) : a(std::move(a_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> a;
	}
	void operator << (const Value& Value_) override
	{
		Value_["a"] >> a;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << a;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["a"] = a;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(milliseconds());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(milliseconds(), L"a");
	}
};
