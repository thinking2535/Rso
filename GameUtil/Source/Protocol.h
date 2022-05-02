#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

namespace rso
{
	namespace gameutil
	{
		using namespace core;
		using namespace json;
		using TCode = int32;
		enum class EOS : uint8
		{
			Android,
			iOS,
			Max,
			Null,
		};
		struct SIndexCode : public SProto
		{
			int64 Index{};
			TCode Code{};
			SIndexCode()
			{
			}
			SIndexCode(const int64& Index_, const TCode& Code_) : Index(Index_), Code(Code_)
			{
			}
			SIndexCode(int64&& Index_, TCode&& Code_) : Index(std::move(Index_)), Code(std::move(Code_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Index;
				Stream_ >> Code;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Index"] >> Index;
				Value_["Code"] >> Code;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Index;
				Stream_ << Code;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Index"] = Index;
				Value_["Code"] = Code;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(TCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"Index") + L"," + 
					GetMemberName(TCode(), L"Code");
			}
		};
		struct SIndexCodeCnt : public SIndexCode
		{
			int32 Cnt{};
			SIndexCodeCnt()
			{
			}
			SIndexCodeCnt(const SIndexCode& Super_, const int32& Cnt_) : SIndexCode(Super_), Cnt(Cnt_)
			{
			}
			SIndexCodeCnt(SIndexCode&& Super_, int32&& Cnt_) : SIndexCode(std::move(Super_)), Cnt(std::move(Cnt_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SIndexCode::operator << (Stream_);
				Stream_ >> Cnt;
			}
			void operator << (const Value& Value_) override
			{
				SIndexCode::operator << (Value_);
				Value_["Cnt"] >> Cnt;
			}
			void operator >> (CStream& Stream_) const override
			{
				SIndexCode::operator >> (Stream_);
				Stream_ << Cnt;
			}
			void operator >> (Value& Value_) const override
			{
				SIndexCode::operator >> (Value_);
				Value_["Cnt"] = Cnt;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SIndexCode()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SIndexCode(), L"") + L"," + 
					GetMemberName(int32(), L"Cnt");
			}
		};
		struct STimeBoost : public SProto
		{
			system_clock::time_point EndTime{};
			double Speed{};
			STimeBoost()
			{
			}
			STimeBoost(const system_clock::time_point& EndTime_, const double& Speed_) : EndTime(EndTime_), Speed(Speed_)
			{
			}
			STimeBoost(system_clock::time_point&& EndTime_, double&& Speed_) : EndTime(std::move(EndTime_)), Speed(std::move(Speed_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> EndTime;
				Stream_ >> Speed;
			}
			void operator << (const Value& Value_) override
			{
				Value_["EndTime"] >> EndTime;
				Value_["Speed"] >> Speed;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << EndTime;
				Stream_ << Speed;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["EndTime"] = EndTime;
				Value_["Speed"] = Speed;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(system_clock::time_point()) + L"," + 
					GetStdName(double());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(system_clock::time_point(), L"EndTime") + L"," + 
					GetMemberName(double(), L"Speed");
			}
		};
	}
}
