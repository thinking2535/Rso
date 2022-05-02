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
struct SDBTestIn : public SProto
{
	int8 i8{};
	int32 i32{};
	CStream s{};
	system_clock::time_point tp{};
	system_clock::time_point tp_io{};
	list<int32> i32s{};
	list<system_clock::time_point> tps{};
	SDateTime dt{};
	SDateTime dt_io{};
	string str{};
	wstring wstr{};
	SDBTestIn()
	{
	}
	SDBTestIn(const int8& i8_, const int32& i32_, const CStream& s_, const system_clock::time_point& tp_, const system_clock::time_point& tp_io_, const list<int32>& i32s_, const list<system_clock::time_point>& tps_, const SDateTime& dt_, const SDateTime& dt_io_, const string& str_, const wstring& wstr_) : i8(i8_), i32(i32_), s(s_), tp(tp_), tp_io(tp_io_), i32s(i32s_), tps(tps_), dt(dt_), dt_io(dt_io_), str(str_), wstr(wstr_)
	{
	}
	SDBTestIn(int8&& i8_, int32&& i32_, CStream&& s_, system_clock::time_point&& tp_, system_clock::time_point&& tp_io_, list<int32>&& i32s_, list<system_clock::time_point>&& tps_, SDateTime&& dt_, SDateTime&& dt_io_, string&& str_, wstring&& wstr_) : i8(std::move(i8_)), i32(std::move(i32_)), s(std::move(s_)), tp(std::move(tp_)), tp_io(std::move(tp_io_)), i32s(std::move(i32s_)), tps(std::move(tps_)), dt(std::move(dt_)), dt_io(std::move(dt_io_)), str(std::move(str_)), wstr(std::move(wstr_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> i8;
		Stream_ >> i32;
		Stream_ >> s;
		Stream_ >> tp;
		Stream_ >> tp_io;
		Stream_ >> i32s;
		Stream_ >> tps;
		Stream_ >> dt;
		Stream_ >> dt_io;
		Stream_ >> str;
		Stream_ >> wstr;
	}
	void operator << (const Value& Value_) override
	{
		Value_["i8"] >> i8;
		Value_["i32"] >> i32;
		Value_["s"] >> s;
		Value_["tp"] >> tp;
		Value_["tp_io"] >> tp_io;
		Value_["i32s"] >> i32s;
		Value_["tps"] >> tps;
		Value_["dt"] >> dt;
		Value_["dt_io"] >> dt_io;
		Value_["str"] >> str;
		Value_["wstr"] >> wstr;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << i8;
		Stream_ << i32;
		Stream_ << s;
		Stream_ << tp;
		Stream_ << tp_io;
		Stream_ << i32s;
		Stream_ << tps;
		Stream_ << dt;
		Stream_ << dt_io;
		Stream_ << str;
		Stream_ << wstr;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["i8"] = i8;
		Value_["i32"] = i32;
		Value_["s"] = s;
		Value_["tp"] = tp;
		Value_["tp_io"] = tp_io;
		Value_["i32s"] = i32s;
		Value_["tps"] = tps;
		Value_["dt"] = dt;
		Value_["dt_io"] = dt_io;
		Value_["str"] = str;
		Value_["wstr"] = wstr;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(int8()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(CStream()) + L"," + 
			GetStdName(system_clock::time_point()) + L"," + 
			GetStdName(system_clock::time_point()) + L"," + 
			GetStdName(list<int32>()) + L"," + 
			GetStdName(list<system_clock::time_point>()) + L"," + 
			GetStdName(SDateTime()) + L"," + 
			GetStdName(SDateTime()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(wstring());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(int8(), L"i8") + L"," + 
			GetMemberName(int32(), L"i32") + L"," + 
			GetMemberName(CStream(), L"s") + L"," + 
			GetMemberName(system_clock::time_point(), L"tp") + L"," + 
			GetMemberName(system_clock::time_point(), L"tp_io") + L"," + 
			GetMemberName(list<int32>(), L"i32s") + L"," + 
			GetMemberName(list<system_clock::time_point>(), L"tps") + L"," + 
			GetMemberName(SDateTime(), L"dt") + L"," + 
			GetMemberName(SDateTime(), L"dt_io") + L"," + 
			GetMemberName(string(), L"str") + L"," + 
			GetMemberName(wstring(), L"wstr");
	}
};
struct SDBTestOut : public SProto
{
	list<int8> int8s{};
	list<int32> int32s{};
	list<CStream> streams{};
	list<system_clock::time_point> tps{};
	list<SDateTime> dts{};
	list<string> strs{};
	list<wstring> wstrs{};
	int32 out_i32{};
	system_clock::time_point tp_io{};
	SDateTime dt_io{};
	SDBTestOut()
	{
	}
	SDBTestOut(const list<int8>& int8s_, const list<int32>& int32s_, const list<CStream>& streams_, const list<system_clock::time_point>& tps_, const list<SDateTime>& dts_, const list<string>& strs_, const list<wstring>& wstrs_, const int32& out_i32_, const system_clock::time_point& tp_io_, const SDateTime& dt_io_) : int8s(int8s_), int32s(int32s_), streams(streams_), tps(tps_), dts(dts_), strs(strs_), wstrs(wstrs_), out_i32(out_i32_), tp_io(tp_io_), dt_io(dt_io_)
	{
	}
	SDBTestOut(list<int8>&& int8s_, list<int32>&& int32s_, list<CStream>&& streams_, list<system_clock::time_point>&& tps_, list<SDateTime>&& dts_, list<string>&& strs_, list<wstring>&& wstrs_, int32&& out_i32_, system_clock::time_point&& tp_io_, SDateTime&& dt_io_) : int8s(std::move(int8s_)), int32s(std::move(int32s_)), streams(std::move(streams_)), tps(std::move(tps_)), dts(std::move(dts_)), strs(std::move(strs_)), wstrs(std::move(wstrs_)), out_i32(std::move(out_i32_)), tp_io(std::move(tp_io_)), dt_io(std::move(dt_io_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> int8s;
		Stream_ >> int32s;
		Stream_ >> streams;
		Stream_ >> tps;
		Stream_ >> dts;
		Stream_ >> strs;
		Stream_ >> wstrs;
		Stream_ >> out_i32;
		Stream_ >> tp_io;
		Stream_ >> dt_io;
	}
	void operator << (const Value& Value_) override
	{
		Value_["int8s"] >> int8s;
		Value_["int32s"] >> int32s;
		Value_["streams"] >> streams;
		Value_["tps"] >> tps;
		Value_["dts"] >> dts;
		Value_["strs"] >> strs;
		Value_["wstrs"] >> wstrs;
		Value_["out_i32"] >> out_i32;
		Value_["tp_io"] >> tp_io;
		Value_["dt_io"] >> dt_io;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << int8s;
		Stream_ << int32s;
		Stream_ << streams;
		Stream_ << tps;
		Stream_ << dts;
		Stream_ << strs;
		Stream_ << wstrs;
		Stream_ << out_i32;
		Stream_ << tp_io;
		Stream_ << dt_io;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["int8s"] = int8s;
		Value_["int32s"] = int32s;
		Value_["streams"] = streams;
		Value_["tps"] = tps;
		Value_["dts"] = dts;
		Value_["strs"] = strs;
		Value_["wstrs"] = wstrs;
		Value_["out_i32"] = out_i32;
		Value_["tp_io"] = tp_io;
		Value_["dt_io"] = dt_io;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(list<int8>()) + L"," + 
			GetStdName(list<int32>()) + L"," + 
			GetStdName(list<CStream>()) + L"," + 
			GetStdName(list<system_clock::time_point>()) + L"," + 
			GetStdName(list<SDateTime>()) + L"," + 
			GetStdName(list<string>()) + L"," + 
			GetStdName(list<wstring>()) + L"," + 
			GetStdName(int32()) + L"," + 
			GetStdName(system_clock::time_point()) + L"," + 
			GetStdName(SDateTime());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(list<int8>(), L"int8s") + L"," + 
			GetMemberName(list<int32>(), L"int32s") + L"," + 
			GetMemberName(list<CStream>(), L"streams") + L"," + 
			GetMemberName(list<system_clock::time_point>(), L"tps") + L"," + 
			GetMemberName(list<SDateTime>(), L"dts") + L"," + 
			GetMemberName(list<string>(), L"strs") + L"," + 
			GetMemberName(list<wstring>(), L"wstrs") + L"," + 
			GetMemberName(int32(), L"out_i32") + L"," + 
			GetMemberName(system_clock::time_point(), L"tp_io") + L"," + 
			GetMemberName(SDateTime(), L"dt_io");
	}
};
struct SStringWStringInt : public SProto
{
	string a{};
	wstring b{};
	int32 c{};
	SStringWStringInt()
	{
	}
	SStringWStringInt(const string& a_, const wstring& b_, const int32& c_) : a(a_), b(b_), c(c_)
	{
	}
	SStringWStringInt(string&& a_, wstring&& b_, int32&& c_) : a(std::move(a_)), b(std::move(b_)), c(std::move(c_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> a;
		Stream_ >> b;
		Stream_ >> c;
	}
	void operator << (const Value& Value_) override
	{
		Value_["a"] >> a;
		Value_["b"] >> b;
		Value_["c"] >> c;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << a;
		Stream_ << b;
		Stream_ << c;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["a"] = a;
		Value_["b"] = b;
		Value_["c"] = c;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(string()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(int32());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(string(), L"a") + L"," + 
			GetMemberName(wstring(), L"b") + L"," + 
			GetMemberName(int32(), L"c");
	}
};
struct STestDBIn : public SProto
{
	list<CStream> s{};
	list<string> a{};
	list<wstring> b{};
	list<SStringWStringInt> c{};
	string d{};
	CStream e{};
	STestDBIn()
	{
	}
	STestDBIn(const list<CStream>& s_, const list<string>& a_, const list<wstring>& b_, const list<SStringWStringInt>& c_, const string& d_, const CStream& e_) : s(s_), a(a_), b(b_), c(c_), d(d_), e(e_)
	{
	}
	STestDBIn(list<CStream>&& s_, list<string>&& a_, list<wstring>&& b_, list<SStringWStringInt>&& c_, string&& d_, CStream&& e_) : s(std::move(s_)), a(std::move(a_)), b(std::move(b_)), c(std::move(c_)), d(std::move(d_)), e(std::move(e_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> s;
		Stream_ >> a;
		Stream_ >> b;
		Stream_ >> c;
		Stream_ >> d;
		Stream_ >> e;
	}
	void operator << (const Value& Value_) override
	{
		Value_["s"] >> s;
		Value_["a"] >> a;
		Value_["b"] >> b;
		Value_["c"] >> c;
		Value_["d"] >> d;
		Value_["e"] >> e;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << s;
		Stream_ << a;
		Stream_ << b;
		Stream_ << c;
		Stream_ << d;
		Stream_ << e;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["s"] = s;
		Value_["a"] = a;
		Value_["b"] = b;
		Value_["c"] = c;
		Value_["d"] = d;
		Value_["e"] = e;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(list<CStream>()) + L"," + 
			GetStdName(list<string>()) + L"," + 
			GetStdName(list<wstring>()) + L"," + 
			GetStdName(list<SStringWStringInt>()) + L"," + 
			GetStdName(string()) + L"," + 
			GetStdName(CStream());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(list<CStream>(), L"s") + L"," + 
			GetMemberName(list<string>(), L"a") + L"," + 
			GetMemberName(list<wstring>(), L"b") + L"," + 
			GetMemberName(list<SStringWStringInt>(), L"c") + L"," + 
			GetMemberName(string(), L"d") + L"," + 
			GetMemberName(CStream(), L"e");
	}
};
struct STestDBOut : public SProto
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
struct STest2DBIn : public SProto
{
	list<string> a{};
	STest2DBIn()
	{
	}
	STest2DBIn(const list<string>& a_) : a(a_)
	{
	}
	STest2DBIn(list<string>&& a_) : a(std::move(a_))
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
			GetStdName(list<string>());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(list<string>(), L"a");
	}
};
struct STest3DBIn : public SProto
{
	string a{};
	STest3DBIn()
	{
	}
	STest3DBIn(const string& a_) : a(a_)
	{
	}
	STest3DBIn(string&& a_) : a(std::move(a_))
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
			GetStdName(string());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(string(), L"a");
	}
};
struct SBulkCopyDBIn : public SProto
{
	int64 a{};
	SBulkCopyDBIn()
	{
	}
	SBulkCopyDBIn(const int64& a_) : a(a_)
	{
	}
	SBulkCopyDBIn(int64&& a_) : a(std::move(a_))
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
			GetStdName(int64());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(int64(), L"a");
	}
};
struct SUTFTestDBIn : public SProto
{
	string Char{};
	u16string WChar{};
	SUTFTestDBIn()
	{
	}
	SUTFTestDBIn(const string& Char_, const u16string& WChar_) : Char(Char_), WChar(WChar_)
	{
	}
	SUTFTestDBIn(string&& Char_, u16string&& WChar_) : Char(std::move(Char_)), WChar(std::move(WChar_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Char;
		Stream_ >> WChar;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Char"] >> Char;
		Value_["WChar"] >> WChar;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Char;
		Stream_ << WChar;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Char"] = Char;
		Value_["WChar"] = WChar;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(string()) + L"," + 
			GetStdName(u16string());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(string(), L"Char") + L"," + 
			GetMemberName(u16string(), L"WChar");
	}
};
struct SUTFDummyDBOut : public SProto
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
struct SDummyOutDb : public SProto
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
struct SAuthLoginInDb : public SProto
{
	u16string ID{};
	SAuthLoginInDb()
	{
	}
	SAuthLoginInDb(const u16string& ID_) : ID(ID_)
	{
	}
	SAuthLoginInDb(u16string&& ID_) : ID(std::move(ID_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> ID;
	}
	void operator << (const Value& Value_) override
	{
		Value_["ID"] >> ID;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << ID;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["ID"] = ID;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(u16string());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(u16string(), L"ID");
	}
};
struct SAuthUser : public SProto
{
	int64 UID{};
	u16string Nick{};
	SAuthUser()
	{
	}
	SAuthUser(const int64& UID_, const u16string& Nick_) : UID(UID_), Nick(Nick_)
	{
	}
	SAuthUser(int64&& UID_, u16string&& Nick_) : UID(std::move(UID_)), Nick(std::move(Nick_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> UID;
		Stream_ >> Nick;
	}
	void operator << (const Value& Value_) override
	{
		Value_["UID"] >> UID;
		Value_["Nick"] >> Nick;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << UID;
		Stream_ << Nick;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["UID"] = UID;
		Value_["Nick"] = Nick;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(int64()) + L"," + 
			GetStdName(u16string());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(int64(), L"UID") + L"," + 
			GetMemberName(u16string(), L"Nick");
	}
};
struct SAuthLoginOutDb : public SProto
{
	list<SAuthUser> Users{};
	SAuthLoginOutDb()
	{
	}
	SAuthLoginOutDb(const list<SAuthUser>& Users_) : Users(Users_)
	{
	}
	SAuthLoginOutDb(list<SAuthUser>&& Users_) : Users(std::move(Users_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Users;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Users"] >> Users;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Users;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Users"] = Users;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(list<SAuthUser>());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(list<SAuthUser>(), L"Users");
	}
};
