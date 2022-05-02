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
using namespace json;
struct SBulkSelectTickDBIn : public SProto
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
struct SDataDB : public SProto
{
	system_clock::time_point TimePoint{};
	int32 Close{};
	int32 Volume{};
	SDataDB()
	{
	}
	SDataDB(const system_clock::time_point& TimePoint_, const int32& Close_, const int32& Volume_) : TimePoint(TimePoint_), Close(Close_), Volume(Volume_)
	{
	}
	SDataDB(system_clock::time_point&& TimePoint_, int32&& Close_, int32&& Volume_) : TimePoint(std::move(TimePoint_)), Close(std::move(Close_)), Volume(std::move(Volume_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> TimePoint;
		Stream_ >> Close;
		Stream_ >> Volume;
	}
	void operator << (const Value& Value_) override
	{
		Value_["TimePoint"] >> TimePoint;
		Value_["Close"] >> Close;
		Value_["Volume"] >> Volume;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << TimePoint;
		Stream_ << Close;
		Stream_ << Volume;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["TimePoint"] = TimePoint;
		Value_["Close"] = Close;
		Value_["Volume"] = Volume;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(system_clock::time_point()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(int32());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(system_clock::time_point(), L"TimePoint") + L"," + 
			GetMemberName(int32(), L"Close") + L"," + 
			GetMemberName(int32(), L"Volume");
	}
};
struct SBulkSelectTickDBOut : public SProto
{
	list<SDataDB> Datas{};
	SBulkSelectTickDBOut()
	{
	}
	SBulkSelectTickDBOut(const list<SDataDB>& Datas_) : Datas(Datas_)
	{
	}
	SBulkSelectTickDBOut(list<SDataDB>&& Datas_) : Datas(std::move(Datas_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Datas;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Datas"] >> Datas;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Datas;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Datas"] = Datas;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(list<SDataDB>());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(list<SDataDB>(), L"Datas");
	}
};
