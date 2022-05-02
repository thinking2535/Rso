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
#include <Rso/GameUtil/Protocol.h>
namespace rso
{
	namespace gamebill
	{
		using namespace net;
		using namespace gameutil;
		struct SPurchaseIn : public SProto
		{
			TUID UID{};
			string OrderID{};
			EOS OS{};
			string CountryCode{};
			int64 PurchaseTime{};
			int32 PurchaseType{};
			string ProductID{};
			CStream Stream{};
			SPurchaseIn()
			{
			}
			SPurchaseIn(const TUID& UID_, const string& OrderID_, const EOS& OS_, const string& CountryCode_, const int64& PurchaseTime_, const int32& PurchaseType_, const string& ProductID_, const CStream& Stream_) : UID(UID_), OrderID(OrderID_), OS(OS_), CountryCode(CountryCode_), PurchaseTime(PurchaseTime_), PurchaseType(PurchaseType_), ProductID(ProductID_), Stream(Stream_)
			{
			}
			SPurchaseIn(TUID&& UID_, string&& OrderID_, EOS&& OS_, string&& CountryCode_, int64&& PurchaseTime_, int32&& PurchaseType_, string&& ProductID_, CStream&& Stream_) : UID(std::move(UID_)), OrderID(std::move(OrderID_)), OS(std::move(OS_)), CountryCode(std::move(CountryCode_)), PurchaseTime(std::move(PurchaseTime_)), PurchaseType(std::move(PurchaseType_)), ProductID(std::move(ProductID_)), Stream(std::move(Stream_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> OrderID;
				Stream_ >> OS;
				Stream_ >> CountryCode;
				Stream_ >> PurchaseTime;
				Stream_ >> PurchaseType;
				Stream_ >> ProductID;
				Stream_ >> Stream;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["OrderID"] >> OrderID;
				Value_["OS"] >> OS;
				Value_["CountryCode"] >> CountryCode;
				Value_["PurchaseTime"] >> PurchaseTime;
				Value_["PurchaseType"] >> PurchaseType;
				Value_["ProductID"] >> ProductID;
				Value_["Stream"] >> Stream;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << OrderID;
				Stream_ << OS;
				Stream_ << CountryCode;
				Stream_ << PurchaseTime;
				Stream_ << PurchaseType;
				Stream_ << ProductID;
				Stream_ << Stream;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["OrderID"] = OrderID;
				Value_["OS"] = OS;
				Value_["CountryCode"] = CountryCode;
				Value_["PurchaseTime"] = PurchaseTime;
				Value_["PurchaseType"] = PurchaseType;
				Value_["ProductID"] = ProductID;
				Value_["Stream"] = Stream;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(EOS()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(int64()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(CStream());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(string(), L"OrderID") + L"," + 
					GetMemberName(EOS(), L"OS") + L"," + 
					GetMemberName(string(), L"CountryCode") + L"," + 
					GetMemberName(int64(), L"PurchaseTime") + L"," + 
					GetMemberName(int32(), L"PurchaseType") + L"," + 
					GetMemberName(string(), L"ProductID") + L"," + 
					GetMemberName(CStream(), L"Stream");
			}
		};
		struct SPurchaseOut : public SProto
		{
			list<CStream> Streams{};
			SPurchaseOut()
			{
			}
			SPurchaseOut(const list<CStream>& Streams_) : Streams(Streams_)
			{
			}
			SPurchaseOut(list<CStream>&& Streams_) : Streams(std::move(Streams_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Streams;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Streams"] >> Streams;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Streams;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Streams"] = Streams;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<CStream>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<CStream>(), L"Streams");
			}
		};
	}
}
