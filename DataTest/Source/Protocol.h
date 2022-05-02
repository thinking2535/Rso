#pragma once


#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>


using namespace std;
using namespace rso;
using namespace core;
using namespace json;


struct _STest : public SProto
{
	__int64 Index = 0;
	wstring Name;
	_STest()
	{
	}
	_STest(const __int64& Index_, const wstring& Name_) : Index(Index_), Name(Name_)
	{
	}
	_STest(__int64&& Index_, wstring&& Name_) : Index(std::move(Index_)), Name(std::move(Name_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Index;
		Stream_ >> Name;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Index"] >> Index;
		Value_["Name"] >> Name;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Index;
		Stream_ << Name;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Index"] = Index;
		Value_["Name"] = Name;
	}
	static wstring StdName(void)
	{
		return L"int64,wstring";
	}
	static wstring MemberName(void)
	{
		return L"Index,Name";
	}
};
struct STest2 : public SProto
{
	list<_STest> Datas;
	STest2()
	{
	}
	STest2(const list<_STest>& Datas_) : Datas(Datas_)
	{
	}
	STest2(list<_STest>&& Datas_) : Datas(std::move(Datas_))
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
		return L"{int64,wstring}";
	}
	static wstring MemberName(void)
	{
		return L"Datas";
	}
};
