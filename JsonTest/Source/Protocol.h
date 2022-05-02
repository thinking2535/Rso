#pragma once

#include <string>
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
enum ETest : uint64
{
	a,
};
struct STest : public SProto
{
	bool b{};
	int8 i8{};
	int32 i32{};
	int64 i64{};
	uint64 ui64{};
	string str{};
	wstring wstr{};
	system_clock::time_point tp{};
	SDateTime dt{};
	ETest e{};
	CStream s{};
	STest()
	{
	}
	STest(const bool& b_, const int8& i8_, const int32& i32_, const int64& i64_, const uint64& ui64_, const string& str_, const wstring& wstr_, const system_clock::time_point& tp_, const SDateTime& dt_, const ETest& e_, const CStream& s_) : b(b_), i8(i8_), i32(i32_), i64(i64_), ui64(ui64_), str(str_), wstr(wstr_), tp(tp_), dt(dt_), e(e_), s(s_)
	{
	}
	STest(bool&& b_, int8&& i8_, int32&& i32_, int64&& i64_, uint64&& ui64_, string&& str_, wstring&& wstr_, system_clock::time_point&& tp_, SDateTime&& dt_, ETest&& e_, CStream&& s_) : b(std::move(b_)), i8(std::move(i8_)), i32(std::move(i32_)), i64(std::move(i64_)), ui64(std::move(ui64_)), str(std::move(str_)), wstr(std::move(wstr_)), tp(std::move(tp_)), dt(std::move(dt_)), e(std::move(e_)), s(std::move(s_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> b;
		Stream_ >> i8;
		Stream_ >> i32;
		Stream_ >> i64;
		Stream_ >> ui64;
		Stream_ >> str;
		Stream_ >> wstr;
		Stream_ >> tp;
		Stream_ >> dt;
		Stream_ >> e;
		Stream_ >> s;
	}
	void operator << (const Value& Value_) override
	{
		Value_["b"] >> b;
		Value_["i8"] >> i8;
		Value_["i32"] >> i32;
		Value_["i64"] >> i64;
		Value_["ui64"] >> ui64;
		Value_["str"] >> str;
		Value_["wstr"] >> wstr;
		Value_["tp"] >> tp;
		Value_["dt"] >> dt;
		Value_["e"] >> e;
		Value_["s"] >> s;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << b;
		Stream_ << i8;
		Stream_ << i32;
		Stream_ << i64;
		Stream_ << ui64;
		Stream_ << str;
		Stream_ << wstr;
		Stream_ << tp;
		Stream_ << dt;
		Stream_ << e;
		Stream_ << s;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["b"] = b;
		Value_["i8"] = i8;
		Value_["i32"] = i32;
		Value_["i64"] = i64;
		Value_["ui64"] = ui64;
		Value_["str"] = str;
		Value_["wstr"] = wstr;
		Value_["tp"] = tp;
		Value_["dt"] = dt;
		Value_["e"] = e;
		Value_["s"] = s;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(bool()) + L"," + 
			GetStdName(int8()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(int64()) + L"," + 
			GetStdName(uint64()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(system_clock::time_point()) + L"," + 
			GetStdName(SDateTime()) + L"," + 
			GetStdName(ETest()) + L"," + 
			GetStdName(CStream());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(bool(), L"b") + L"," + 
			GetMemberName(int8(), L"i8") + L"," + 
			GetMemberName(int32(), L"i32") + L"," + 
			GetMemberName(int64(), L"i64") + L"," + 
			GetMemberName(uint64(), L"ui64") + L"," + 
			GetMemberName(string(), L"str") + L"," + 
			GetMemberName(wstring(), L"wstr") + L"," + 
			GetMemberName(system_clock::time_point(), L"tp") + L"," + 
			GetMemberName(SDateTime(), L"dt") + L"," + 
			GetMemberName(ETest(), L"e") + L"," + 
			GetMemberName(CStream(), L"s");
	}
};
struct STest2 : public STest
{
	string str2{};
	STest test{};
	STest2()
	{
	}
	STest2(const STest& Super_, const string& str2_, const STest& test_) : STest(Super_), str2(str2_), test(test_)
	{
	}
	STest2(STest&& Super_, string&& str2_, STest&& test_) : STest(std::move(Super_)), str2(std::move(str2_)), test(std::move(test_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		STest::operator << (Stream_);
		Stream_ >> str2;
		Stream_ >> test;
	}
	void operator << (const Value& Value_) override
	{
		STest::operator << (Value_);
		Value_["str2"] >> str2;
		Value_["test"] >> test;
	}
	void operator >> (CStream& Stream_) const override
	{
		STest::operator >> (Stream_);
		Stream_ << str2;
		Stream_ << test;
	}
	void operator >> (Value& Value_) const override
	{
		STest::operator >> (Value_);
		Value_["str2"] = str2;
		Value_["test"] = test;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(STest()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(STest());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(STest(), L"") + L"," + 
			GetMemberName(string(), L"str2") + L"," + 
			GetMemberName(STest(), L"test");
	}
};
