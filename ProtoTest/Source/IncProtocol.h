#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

namespace ns
{
	namespace inc
	{
		using namespace rso;
		using namespace core;
		enum class ETest
		{
			A,
			B,
			C,
		};
		struct STestBase : public SProto
		{
			ETest e = (ETest)0;
			int8 i8 = 0;
			uint8 ui8 = 0;
			int16 i16 = 0;
			uint16 ui16 = 0;
			int32 i32 = 0;
			uint32 ui32 = 0;
			int64 i64 = 0;
			uint64 ui64 = 0;
			float f = 0.0f;
			double d = 0.0;
			string s;
			wstring ws;
			CStream stm;
			SDateTime dt;
			system_clock::time_point tp;
			array<int32,3> a{};
			vector<CStream> v;
			list<int8> l;
			set<float> st;
			map<int32,SDateTime> mp;
			STestBase()
			{
			}
			STestBase(const ETest& e_, const int8& i8_, const uint8& ui8_, const int16& i16_, const uint16& ui16_, const int32& i32_, const uint32& ui32_, const int64& i64_, const uint64& ui64_, const float& f_, const double& d_, const string& s_, const wstring& ws_, const CStream& stm_, const SDateTime& dt_, const system_clock::time_point& tp_, const array<int32,3>& a_, const vector<CStream>& v_, const list<int8>& l_, const set<float>& st_, const map<int32,SDateTime>& mp_) : e(e_), i8(i8_), ui8(ui8_), i16(i16_), ui16(ui16_), i32(i32_), ui32(ui32_), i64(i64_), ui64(ui64_), f(f_), d(d_), s(s_), ws(ws_), stm(stm_), dt(dt_), tp(tp_), a(a_), v(v_), l(l_), st(st_), mp(mp_)
			{
			}
			STestBase(ETest&& e_, int8&& i8_, uint8&& ui8_, int16&& i16_, uint16&& ui16_, int32&& i32_, uint32&& ui32_, int64&& i64_, uint64&& ui64_, float&& f_, double&& d_, string&& s_, wstring&& ws_, CStream&& stm_, SDateTime&& dt_, system_clock::time_point&& tp_, array<int32,3>&& a_, vector<CStream>&& v_, list<int8>&& l_, set<float>&& st_, map<int32,SDateTime>&& mp_) : e(std::move(e_)), i8(std::move(i8_)), ui8(std::move(ui8_)), i16(std::move(i16_)), ui16(std::move(ui16_)), i32(std::move(i32_)), ui32(std::move(ui32_)), i64(std::move(i64_)), ui64(std::move(ui64_)), f(std::move(f_)), d(std::move(d_)), s(std::move(s_)), ws(std::move(ws_)), stm(std::move(stm_)), dt(std::move(dt_)), tp(std::move(tp_)), a(std::move(a_)), v(std::move(v_)), l(std::move(l_)), st(std::move(st_)), mp(std::move(mp_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> (int32&)e;
				Stream_ >> i8;
				Stream_ >> ui8;
				Stream_ >> i16;
				Stream_ >> ui16;
				Stream_ >> i32;
				Stream_ >> ui32;
				Stream_ >> i64;
				Stream_ >> ui64;
				Stream_ >> f;
				Stream_ >> d;
				Stream_ >> s;
				Stream_ >> ws;
				Stream_ >> stm;
				Stream_ >> dt;
				Stream_ >> tp;
				Stream_ >> a;
				Stream_ >> v;
				Stream_ >> l;
				Stream_ >> st;
				Stream_ >> mp;
			}
			void operator << (const Value& Value_) override
			{
				Value_["e"] >> (int32&)e;
				Value_["i8"] >> i8;
				Value_["ui8"] >> ui8;
				Value_["i16"] >> i16;
				Value_["ui16"] >> ui16;
				Value_["i32"] >> i32;
				Value_["ui32"] >> ui32;
				Value_["i64"] >> i64;
				Value_["ui64"] >> ui64;
				Value_["f"] >> f;
				Value_["d"] >> d;
				Value_["s"] >> s;
				Value_["ws"] >> ws;
				Value_["stm"] >> stm;
				Value_["dt"] >> dt;
				Value_["tp"] >> tp;
				Value_["a"] >> a;
				Value_["v"] >> v;
				Value_["l"] >> l;
				Value_["st"] >> st;
				Value_["mp"] >> mp;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << (int32)e;
				Stream_ << i8;
				Stream_ << ui8;
				Stream_ << i16;
				Stream_ << ui16;
				Stream_ << i32;
				Stream_ << ui32;
				Stream_ << i64;
				Stream_ << ui64;
				Stream_ << f;
				Stream_ << d;
				Stream_ << s;
				Stream_ << ws;
				Stream_ << stm;
				Stream_ << dt;
				Stream_ << tp;
				Stream_ << a;
				Stream_ << v;
				Stream_ << l;
				Stream_ << st;
				Stream_ << mp;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["e"] = (int32)e;
				Value_["i8"] = i8;
				Value_["ui8"] = ui8;
				Value_["i16"] = i16;
				Value_["ui16"] = ui16;
				Value_["i32"] = i32;
				Value_["ui32"] = ui32;
				Value_["i64"] = i64;
				Value_["ui64"] = ui64;
				Value_["f"] = f;
				Value_["d"] = d;
				Value_["s"] = s;
				Value_["ws"] = ws;
				Value_["stm"] = stm;
				Value_["dt"] = dt;
				Value_["tp"] = tp;
				Value_["a"] = a;
				Value_["v"] = v;
				Value_["l"] = l;
				Value_["st"] = st;
				Value_["mp"] = mp;
			}
			static wstring StdName(void)
			{
				return L"int32,int8,uint8,int16,uint16,int32,uint32,int64,uint64,float,double,string,wstring,stream,datetime,time_point,int32,int32,int32,{stream},{int8},{float},{int32,datetime}";
			}
			static wstring MemberName(void)
			{
				return L"e,i8,ui8,i16,ui16,i32,ui32,i64,ui64,f,d,s,ws,stm,dt,tp,a,v,l,st,mp";
			}
		};
	}
}
