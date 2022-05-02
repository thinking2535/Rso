#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

struct SBase : public SProto
{
	string Name{};
	int32 Age{};
	SBase()
	{
	}
	SBase(const string& Name_, const int32& Age_) : Name(Name_), Age(Age_)
	{
	}
	SBase(string&& Name_, int32&& Age_) : Name(std::move(Name_)), Age(std::move(Age_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Name;
		Stream_ >> Age;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Name"] >> Name;
		Value_["Age"] >> Age;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Name;
		Stream_ << Age;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Name"] = Name;
		Value_["Age"] = Age;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(string()) + L"," + 
			GetStdName(int32());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(string(), L"Name") + L"," + 
			GetMemberName(int32(), L"Age");
	}
};
template<typename TType>
struct STest : public SProto
{
	TType ty{};
	SBase base{};
	STest()
	{
	}
	STest(const TType& ty_, const SBase& base_) : ty(ty_), base(base_)
	{
	}
	STest(TType&& ty_, SBase&& base_) : ty(std::move(ty_)), base(std::move(base_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> ty;
		Stream_ >> base;
	}
	void operator << (const Value& Value_) override
	{
		Value_["ty"] >> ty;
		Value_["base"] >> base;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << ty;
		Stream_ << base;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["ty"] = ty;
		Value_["base"] = base;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(TType()) + L"," + 
			GetStdName(SBase());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(TType(), L"ty") + L"," + 
			GetMemberName(SBase(), L"base");
	}
};
