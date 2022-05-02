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
	namespace util
	{
		struct SData : public SProto
		{
			wstring Key{};
			wstring Data{};
			int32 Grade{};
			int32 RemindCount{};
			SData()
			{
			}
			SData(const wstring& Key_, const wstring& Data_, const int32& Grade_, const int32& RemindCount_) : Key(Key_), Data(Data_), Grade(Grade_), RemindCount(RemindCount_)
			{
			}
			SData(wstring&& Key_, wstring&& Data_, int32&& Grade_, int32&& RemindCount_) : Key(std::move(Key_)), Data(std::move(Data_)), Grade(std::move(Grade_)), RemindCount(std::move(RemindCount_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Key;
				Stream_ >> Data;
				Stream_ >> Grade;
				Stream_ >> RemindCount;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Key"] >> Key;
				Value_["Data"] >> Data;
				Value_["Grade"] >> Grade;
				Value_["RemindCount"] >> RemindCount;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Key;
				Stream_ << Data;
				Stream_ << Grade;
				Stream_ << RemindCount;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Key"] = Key;
				Value_["Data"] = Data;
				Value_["Grade"] = Grade;
				Value_["RemindCount"] = RemindCount;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Key") + L"," + 
					GetMemberName(wstring(), L"Data") + L"," + 
					GetMemberName(int32(), L"Grade") + L"," + 
					GetMemberName(int32(), L"RemindCount");
			}
		};
		struct SDataTime : public SData
		{
			int64 TimePoint{};
			SDataTime()
			{
			}
			SDataTime(const SData& Super_, const int64& TimePoint_) : SData(Super_), TimePoint(TimePoint_)
			{
			}
			SDataTime(SData&& Super_, int64&& TimePoint_) : SData(std::move(Super_)), TimePoint(std::move(TimePoint_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SData::operator << (Stream_);
				Stream_ >> TimePoint;
			}
			void operator << (const Value& Value_) override
			{
				SData::operator << (Value_);
				Value_["TimePoint"] >> TimePoint;
			}
			void operator >> (CStream& Stream_) const override
			{
				SData::operator >> (Stream_);
				Stream_ << TimePoint;
			}
			void operator >> (Value& Value_) const override
			{
				SData::operator >> (Value_);
				Value_["TimePoint"] = TimePoint;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SData()) + L"," + 
					GetStdName(int64());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SData(), L"") + L"," + 
					GetMemberName(int64(), L"TimePoint");
			}
		};
	}
}
