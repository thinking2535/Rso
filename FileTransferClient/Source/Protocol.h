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
struct SOption : public SProto
{
	wstring Title{};
	SOption()
	{
	}
	SOption(const wstring& Title_) : Title(Title_)
	{
	}
	SOption(wstring&& Title_) : Title(std::move(Title_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Title;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Title"] >> Title;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Title;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Title"] = Title;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(wstring());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(wstring(), L"Title");
	}
};
