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
	namespace udp
	{
		using namespace core;
		using namespace json;
		using TCounter = int16;
		using TPacketCnt = int16;
		struct SHeader : public SProto
		{
			int64 Time{};
			TCounter Counter{};
			TPacketCnt PacketCnt{};
			TPacketCnt PacketNum{};
			int32 TotalSize{};
			int32 Index{};
			SHeader()
			{
			}
			SHeader(const int64& Time_, const TCounter& Counter_, const TPacketCnt& PacketCnt_, const TPacketCnt& PacketNum_, const int32& TotalSize_, const int32& Index_) : Time(Time_), Counter(Counter_), PacketCnt(PacketCnt_), PacketNum(PacketNum_), TotalSize(TotalSize_), Index(Index_)
			{
			}
			SHeader(int64&& Time_, TCounter&& Counter_, TPacketCnt&& PacketCnt_, TPacketCnt&& PacketNum_, int32&& TotalSize_, int32&& Index_) : Time(std::move(Time_)), Counter(std::move(Counter_)), PacketCnt(std::move(PacketCnt_)), PacketNum(std::move(PacketNum_)), TotalSize(std::move(TotalSize_)), Index(std::move(Index_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Time;
				Stream_ >> Counter;
				Stream_ >> PacketCnt;
				Stream_ >> PacketNum;
				Stream_ >> TotalSize;
				Stream_ >> Index;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Time"] >> Time;
				Value_["Counter"] >> Counter;
				Value_["PacketCnt"] >> PacketCnt;
				Value_["PacketNum"] >> PacketNum;
				Value_["TotalSize"] >> TotalSize;
				Value_["Index"] >> Index;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Time;
				Stream_ << Counter;
				Stream_ << PacketCnt;
				Stream_ << PacketNum;
				Stream_ << TotalSize;
				Stream_ << Index;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Time"] = Time;
				Value_["Counter"] = Counter;
				Value_["PacketCnt"] = PacketCnt;
				Value_["PacketNum"] = PacketNum;
				Value_["TotalSize"] = TotalSize;
				Value_["Index"] = Index;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(TCounter()) + L"," + 
					GetStdName(TPacketCnt()) + L"," + 
					GetStdName(TPacketCnt()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"Time") + L"," + 
					GetMemberName(TCounter(), L"Counter") + L"," + 
					GetMemberName(TPacketCnt(), L"PacketCnt") + L"," + 
					GetMemberName(TPacketCnt(), L"PacketNum") + L"," + 
					GetMemberName(int32(), L"TotalSize") + L"," + 
					GetMemberName(int32(), L"Index");
			}
		};
	}
}
