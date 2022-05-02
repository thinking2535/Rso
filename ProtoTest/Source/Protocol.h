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
using TUID = int32;
template<typename T>
struct STest : public SProto
{
	void operator << (CStream&) override
	{
	}
	void operator << (const Value&) override
	{
	}
	void operator >> (CStream&) const override
	{
	}
	void operator >> (Value&) const override
	{
	}
	static wstring StdName(void)
	{
		return L"";
	}
	static wstring MemberName(void)
	{
		return L"";
	}
};
using TTT2 = list<TUID>;
using TTT3 = STest<TUID>;
