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
	namespace stock
	{
		using namespace core;
		using namespace json;
		struct SDataDb : public SProto
		{
			system_clock::time_point TimePoint{};
			int32 Open{};
			int32 Close{};
			int32 High{};
			int32 Low{};
			int32 Volume{};
			int32 Val{};
			int32 OpenInterest{};
			SDataDb()
			{
			}
			SDataDb(const system_clock::time_point& TimePoint_, const int32& Open_, const int32& Close_, const int32& High_, const int32& Low_, const int32& Volume_, const int32& Val_, const int32& OpenInterest_) : TimePoint(TimePoint_), Open(Open_), Close(Close_), High(High_), Low(Low_), Volume(Volume_), Val(Val_), OpenInterest(OpenInterest_)
			{
			}
			SDataDb(system_clock::time_point&& TimePoint_, int32&& Open_, int32&& Close_, int32&& High_, int32&& Low_, int32&& Volume_, int32&& Val_, int32&& OpenInterest_) : TimePoint(std::move(TimePoint_)), Open(std::move(Open_)), Close(std::move(Close_)), High(std::move(High_)), Low(std::move(Low_)), Volume(std::move(Volume_)), Val(std::move(Val_)), OpenInterest(std::move(OpenInterest_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> TimePoint;
				Stream_ >> Open;
				Stream_ >> Close;
				Stream_ >> High;
				Stream_ >> Low;
				Stream_ >> Volume;
				Stream_ >> Val;
				Stream_ >> OpenInterest;
			}
			void operator << (const Value& Value_) override
			{
				Value_["TimePoint"] >> TimePoint;
				Value_["Open"] >> Open;
				Value_["Close"] >> Close;
				Value_["High"] >> High;
				Value_["Low"] >> Low;
				Value_["Volume"] >> Volume;
				Value_["Val"] >> Val;
				Value_["OpenInterest"] >> OpenInterest;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << TimePoint;
				Stream_ << Open;
				Stream_ << Close;
				Stream_ << High;
				Stream_ << Low;
				Stream_ << Volume;
				Stream_ << Val;
				Stream_ << OpenInterest;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["TimePoint"] = TimePoint;
				Value_["Open"] = Open;
				Value_["Close"] = Close;
				Value_["High"] = High;
				Value_["Low"] = Low;
				Value_["Volume"] = Volume;
				Value_["Val"] = Val;
				Value_["OpenInterest"] = OpenInterest;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(system_clock::time_point()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(system_clock::time_point(), L"TimePoint") + L"," + 
					GetMemberName(int32(), L"Open") + L"," + 
					GetMemberName(int32(), L"Close") + L"," + 
					GetMemberName(int32(), L"High") + L"," + 
					GetMemberName(int32(), L"Low") + L"," + 
					GetMemberName(int32(), L"Volume") + L"," + 
					GetMemberName(int32(), L"Val") + L"," + 
					GetMemberName(int32(), L"OpenInterest");
			}
		};
		struct _STick : public SDataDb
		{
			int32 Offer0{};
			int32 Bid0{};
			int32 Trade{};
			SDateTime InsertedTime{};
			_STick()
			{
			}
			_STick(const SDataDb& Super_, const int32& Offer0_, const int32& Bid0_, const int32& Trade_, const SDateTime& InsertedTime_) : SDataDb(Super_), Offer0(Offer0_), Bid0(Bid0_), Trade(Trade_), InsertedTime(InsertedTime_)
			{
			}
			_STick(SDataDb&& Super_, int32&& Offer0_, int32&& Bid0_, int32&& Trade_, SDateTime&& InsertedTime_) : SDataDb(std::move(Super_)), Offer0(std::move(Offer0_)), Bid0(std::move(Bid0_)), Trade(std::move(Trade_)), InsertedTime(std::move(InsertedTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SDataDb::operator << (Stream_);
				Stream_ >> Offer0;
				Stream_ >> Bid0;
				Stream_ >> Trade;
				Stream_ >> InsertedTime;
			}
			void operator << (const Value& Value_) override
			{
				SDataDb::operator << (Value_);
				Value_["Offer0"] >> Offer0;
				Value_["Bid0"] >> Bid0;
				Value_["Trade"] >> Trade;
				Value_["InsertedTime"] >> InsertedTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				SDataDb::operator >> (Stream_);
				Stream_ << Offer0;
				Stream_ << Bid0;
				Stream_ << Trade;
				Stream_ << InsertedTime;
			}
			void operator >> (Value& Value_) const override
			{
				SDataDb::operator >> (Value_);
				Value_["Offer0"] = Offer0;
				Value_["Bid0"] = Bid0;
				Value_["Trade"] = Trade;
				Value_["InsertedTime"] = InsertedTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SDataDb()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(SDateTime());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SDataDb(), L"") + L"," + 
					GetMemberName(int32(), L"Offer0") + L"," + 
					GetMemberName(int32(), L"Bid0") + L"," + 
					GetMemberName(int32(), L"Trade") + L"," + 
					GetMemberName(SDateTime(), L"InsertedTime");
			}
		};
		struct _SRemain : public SProto
		{
			int64 DateTime{};
			int32 OfferHo1{};
			int32 BidHo1{};
			int32 OfferRem1{};
			int32 BidRem1{};
			int32 OfferCnt1{};
			int32 BidCnt1{};
			int32 OfferHo2{};
			int32 BidHo2{};
			int32 OfferRem2{};
			int32 BidRem2{};
			int32 OfferCnt2{};
			int32 BidCnt2{};
			int32 OfferHo3{};
			int32 BidHo3{};
			int32 OfferRem3{};
			int32 BidRem3{};
			int32 OfferCnt3{};
			int32 BidCnt3{};
			int32 OfferHo4{};
			int32 BidHo4{};
			int32 OfferRem4{};
			int32 BidRem4{};
			int32 OfferCnt4{};
			int32 BidCnt4{};
			int32 OfferHo5{};
			int32 BidHo5{};
			int32 OfferRem5{};
			int32 BidRem5{};
			int32 OfferCnt5{};
			int32 BidCnt5{};
			int32 TotalOfferRem{};
			int32 TotalBidRem{};
			int32 TotalOfferCnt{};
			int32 TotalBidCnt{};
			SDateTime InsertedTime{};
			_SRemain()
			{
			}
			_SRemain(const int64& DateTime_, const int32& OfferHo1_, const int32& BidHo1_, const int32& OfferRem1_, const int32& BidRem1_, const int32& OfferCnt1_, const int32& BidCnt1_, const int32& OfferHo2_, const int32& BidHo2_, const int32& OfferRem2_, const int32& BidRem2_, const int32& OfferCnt2_, const int32& BidCnt2_, const int32& OfferHo3_, const int32& BidHo3_, const int32& OfferRem3_, const int32& BidRem3_, const int32& OfferCnt3_, const int32& BidCnt3_, const int32& OfferHo4_, const int32& BidHo4_, const int32& OfferRem4_, const int32& BidRem4_, const int32& OfferCnt4_, const int32& BidCnt4_, const int32& OfferHo5_, const int32& BidHo5_, const int32& OfferRem5_, const int32& BidRem5_, const int32& OfferCnt5_, const int32& BidCnt5_, const int32& TotalOfferRem_, const int32& TotalBidRem_, const int32& TotalOfferCnt_, const int32& TotalBidCnt_, const SDateTime& InsertedTime_) : DateTime(DateTime_), OfferHo1(OfferHo1_), BidHo1(BidHo1_), OfferRem1(OfferRem1_), BidRem1(BidRem1_), OfferCnt1(OfferCnt1_), BidCnt1(BidCnt1_), OfferHo2(OfferHo2_), BidHo2(BidHo2_), OfferRem2(OfferRem2_), BidRem2(BidRem2_), OfferCnt2(OfferCnt2_), BidCnt2(BidCnt2_), OfferHo3(OfferHo3_), BidHo3(BidHo3_), OfferRem3(OfferRem3_), BidRem3(BidRem3_), OfferCnt3(OfferCnt3_), BidCnt3(BidCnt3_), OfferHo4(OfferHo4_), BidHo4(BidHo4_), OfferRem4(OfferRem4_), BidRem4(BidRem4_), OfferCnt4(OfferCnt4_), BidCnt4(BidCnt4_), OfferHo5(OfferHo5_), BidHo5(BidHo5_), OfferRem5(OfferRem5_), BidRem5(BidRem5_), OfferCnt5(OfferCnt5_), BidCnt5(BidCnt5_), TotalOfferRem(TotalOfferRem_), TotalBidRem(TotalBidRem_), TotalOfferCnt(TotalOfferCnt_), TotalBidCnt(TotalBidCnt_), InsertedTime(InsertedTime_)
			{
			}
			_SRemain(int64&& DateTime_, int32&& OfferHo1_, int32&& BidHo1_, int32&& OfferRem1_, int32&& BidRem1_, int32&& OfferCnt1_, int32&& BidCnt1_, int32&& OfferHo2_, int32&& BidHo2_, int32&& OfferRem2_, int32&& BidRem2_, int32&& OfferCnt2_, int32&& BidCnt2_, int32&& OfferHo3_, int32&& BidHo3_, int32&& OfferRem3_, int32&& BidRem3_, int32&& OfferCnt3_, int32&& BidCnt3_, int32&& OfferHo4_, int32&& BidHo4_, int32&& OfferRem4_, int32&& BidRem4_, int32&& OfferCnt4_, int32&& BidCnt4_, int32&& OfferHo5_, int32&& BidHo5_, int32&& OfferRem5_, int32&& BidRem5_, int32&& OfferCnt5_, int32&& BidCnt5_, int32&& TotalOfferRem_, int32&& TotalBidRem_, int32&& TotalOfferCnt_, int32&& TotalBidCnt_, SDateTime&& InsertedTime_) : DateTime(std::move(DateTime_)), OfferHo1(std::move(OfferHo1_)), BidHo1(std::move(BidHo1_)), OfferRem1(std::move(OfferRem1_)), BidRem1(std::move(BidRem1_)), OfferCnt1(std::move(OfferCnt1_)), BidCnt1(std::move(BidCnt1_)), OfferHo2(std::move(OfferHo2_)), BidHo2(std::move(BidHo2_)), OfferRem2(std::move(OfferRem2_)), BidRem2(std::move(BidRem2_)), OfferCnt2(std::move(OfferCnt2_)), BidCnt2(std::move(BidCnt2_)), OfferHo3(std::move(OfferHo3_)), BidHo3(std::move(BidHo3_)), OfferRem3(std::move(OfferRem3_)), BidRem3(std::move(BidRem3_)), OfferCnt3(std::move(OfferCnt3_)), BidCnt3(std::move(BidCnt3_)), OfferHo4(std::move(OfferHo4_)), BidHo4(std::move(BidHo4_)), OfferRem4(std::move(OfferRem4_)), BidRem4(std::move(BidRem4_)), OfferCnt4(std::move(OfferCnt4_)), BidCnt4(std::move(BidCnt4_)), OfferHo5(std::move(OfferHo5_)), BidHo5(std::move(BidHo5_)), OfferRem5(std::move(OfferRem5_)), BidRem5(std::move(BidRem5_)), OfferCnt5(std::move(OfferCnt5_)), BidCnt5(std::move(BidCnt5_)), TotalOfferRem(std::move(TotalOfferRem_)), TotalBidRem(std::move(TotalBidRem_)), TotalOfferCnt(std::move(TotalOfferCnt_)), TotalBidCnt(std::move(TotalBidCnt_)), InsertedTime(std::move(InsertedTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> DateTime;
				Stream_ >> OfferHo1;
				Stream_ >> BidHo1;
				Stream_ >> OfferRem1;
				Stream_ >> BidRem1;
				Stream_ >> OfferCnt1;
				Stream_ >> BidCnt1;
				Stream_ >> OfferHo2;
				Stream_ >> BidHo2;
				Stream_ >> OfferRem2;
				Stream_ >> BidRem2;
				Stream_ >> OfferCnt2;
				Stream_ >> BidCnt2;
				Stream_ >> OfferHo3;
				Stream_ >> BidHo3;
				Stream_ >> OfferRem3;
				Stream_ >> BidRem3;
				Stream_ >> OfferCnt3;
				Stream_ >> BidCnt3;
				Stream_ >> OfferHo4;
				Stream_ >> BidHo4;
				Stream_ >> OfferRem4;
				Stream_ >> BidRem4;
				Stream_ >> OfferCnt4;
				Stream_ >> BidCnt4;
				Stream_ >> OfferHo5;
				Stream_ >> BidHo5;
				Stream_ >> OfferRem5;
				Stream_ >> BidRem5;
				Stream_ >> OfferCnt5;
				Stream_ >> BidCnt5;
				Stream_ >> TotalOfferRem;
				Stream_ >> TotalBidRem;
				Stream_ >> TotalOfferCnt;
				Stream_ >> TotalBidCnt;
				Stream_ >> InsertedTime;
			}
			void operator << (const Value& Value_) override
			{
				Value_["DateTime"] >> DateTime;
				Value_["OfferHo1"] >> OfferHo1;
				Value_["BidHo1"] >> BidHo1;
				Value_["OfferRem1"] >> OfferRem1;
				Value_["BidRem1"] >> BidRem1;
				Value_["OfferCnt1"] >> OfferCnt1;
				Value_["BidCnt1"] >> BidCnt1;
				Value_["OfferHo2"] >> OfferHo2;
				Value_["BidHo2"] >> BidHo2;
				Value_["OfferRem2"] >> OfferRem2;
				Value_["BidRem2"] >> BidRem2;
				Value_["OfferCnt2"] >> OfferCnt2;
				Value_["BidCnt2"] >> BidCnt2;
				Value_["OfferHo3"] >> OfferHo3;
				Value_["BidHo3"] >> BidHo3;
				Value_["OfferRem3"] >> OfferRem3;
				Value_["BidRem3"] >> BidRem3;
				Value_["OfferCnt3"] >> OfferCnt3;
				Value_["BidCnt3"] >> BidCnt3;
				Value_["OfferHo4"] >> OfferHo4;
				Value_["BidHo4"] >> BidHo4;
				Value_["OfferRem4"] >> OfferRem4;
				Value_["BidRem4"] >> BidRem4;
				Value_["OfferCnt4"] >> OfferCnt4;
				Value_["BidCnt4"] >> BidCnt4;
				Value_["OfferHo5"] >> OfferHo5;
				Value_["BidHo5"] >> BidHo5;
				Value_["OfferRem5"] >> OfferRem5;
				Value_["BidRem5"] >> BidRem5;
				Value_["OfferCnt5"] >> OfferCnt5;
				Value_["BidCnt5"] >> BidCnt5;
				Value_["TotalOfferRem"] >> TotalOfferRem;
				Value_["TotalBidRem"] >> TotalBidRem;
				Value_["TotalOfferCnt"] >> TotalOfferCnt;
				Value_["TotalBidCnt"] >> TotalBidCnt;
				Value_["InsertedTime"] >> InsertedTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << DateTime;
				Stream_ << OfferHo1;
				Stream_ << BidHo1;
				Stream_ << OfferRem1;
				Stream_ << BidRem1;
				Stream_ << OfferCnt1;
				Stream_ << BidCnt1;
				Stream_ << OfferHo2;
				Stream_ << BidHo2;
				Stream_ << OfferRem2;
				Stream_ << BidRem2;
				Stream_ << OfferCnt2;
				Stream_ << BidCnt2;
				Stream_ << OfferHo3;
				Stream_ << BidHo3;
				Stream_ << OfferRem3;
				Stream_ << BidRem3;
				Stream_ << OfferCnt3;
				Stream_ << BidCnt3;
				Stream_ << OfferHo4;
				Stream_ << BidHo4;
				Stream_ << OfferRem4;
				Stream_ << BidRem4;
				Stream_ << OfferCnt4;
				Stream_ << BidCnt4;
				Stream_ << OfferHo5;
				Stream_ << BidHo5;
				Stream_ << OfferRem5;
				Stream_ << BidRem5;
				Stream_ << OfferCnt5;
				Stream_ << BidCnt5;
				Stream_ << TotalOfferRem;
				Stream_ << TotalBidRem;
				Stream_ << TotalOfferCnt;
				Stream_ << TotalBidCnt;
				Stream_ << InsertedTime;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["DateTime"] = DateTime;
				Value_["OfferHo1"] = OfferHo1;
				Value_["BidHo1"] = BidHo1;
				Value_["OfferRem1"] = OfferRem1;
				Value_["BidRem1"] = BidRem1;
				Value_["OfferCnt1"] = OfferCnt1;
				Value_["BidCnt1"] = BidCnt1;
				Value_["OfferHo2"] = OfferHo2;
				Value_["BidHo2"] = BidHo2;
				Value_["OfferRem2"] = OfferRem2;
				Value_["BidRem2"] = BidRem2;
				Value_["OfferCnt2"] = OfferCnt2;
				Value_["BidCnt2"] = BidCnt2;
				Value_["OfferHo3"] = OfferHo3;
				Value_["BidHo3"] = BidHo3;
				Value_["OfferRem3"] = OfferRem3;
				Value_["BidRem3"] = BidRem3;
				Value_["OfferCnt3"] = OfferCnt3;
				Value_["BidCnt3"] = BidCnt3;
				Value_["OfferHo4"] = OfferHo4;
				Value_["BidHo4"] = BidHo4;
				Value_["OfferRem4"] = OfferRem4;
				Value_["BidRem4"] = BidRem4;
				Value_["OfferCnt4"] = OfferCnt4;
				Value_["BidCnt4"] = BidCnt4;
				Value_["OfferHo5"] = OfferHo5;
				Value_["BidHo5"] = BidHo5;
				Value_["OfferRem5"] = OfferRem5;
				Value_["BidRem5"] = BidRem5;
				Value_["OfferCnt5"] = OfferCnt5;
				Value_["BidCnt5"] = BidCnt5;
				Value_["TotalOfferRem"] = TotalOfferRem;
				Value_["TotalBidRem"] = TotalBidRem;
				Value_["TotalOfferCnt"] = TotalOfferCnt;
				Value_["TotalBidCnt"] = TotalBidCnt;
				Value_["InsertedTime"] = InsertedTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(SDateTime());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"DateTime") + L"," + 
					GetMemberName(int32(), L"OfferHo1") + L"," + 
					GetMemberName(int32(), L"BidHo1") + L"," + 
					GetMemberName(int32(), L"OfferRem1") + L"," + 
					GetMemberName(int32(), L"BidRem1") + L"," + 
					GetMemberName(int32(), L"OfferCnt1") + L"," + 
					GetMemberName(int32(), L"BidCnt1") + L"," + 
					GetMemberName(int32(), L"OfferHo2") + L"," + 
					GetMemberName(int32(), L"BidHo2") + L"," + 
					GetMemberName(int32(), L"OfferRem2") + L"," + 
					GetMemberName(int32(), L"BidRem2") + L"," + 
					GetMemberName(int32(), L"OfferCnt2") + L"," + 
					GetMemberName(int32(), L"BidCnt2") + L"," + 
					GetMemberName(int32(), L"OfferHo3") + L"," + 
					GetMemberName(int32(), L"BidHo3") + L"," + 
					GetMemberName(int32(), L"OfferRem3") + L"," + 
					GetMemberName(int32(), L"BidRem3") + L"," + 
					GetMemberName(int32(), L"OfferCnt3") + L"," + 
					GetMemberName(int32(), L"BidCnt3") + L"," + 
					GetMemberName(int32(), L"OfferHo4") + L"," + 
					GetMemberName(int32(), L"BidHo4") + L"," + 
					GetMemberName(int32(), L"OfferRem4") + L"," + 
					GetMemberName(int32(), L"BidRem4") + L"," + 
					GetMemberName(int32(), L"OfferCnt4") + L"," + 
					GetMemberName(int32(), L"BidCnt4") + L"," + 
					GetMemberName(int32(), L"OfferHo5") + L"," + 
					GetMemberName(int32(), L"BidHo5") + L"," + 
					GetMemberName(int32(), L"OfferRem5") + L"," + 
					GetMemberName(int32(), L"BidRem5") + L"," + 
					GetMemberName(int32(), L"OfferCnt5") + L"," + 
					GetMemberName(int32(), L"BidCnt5") + L"," + 
					GetMemberName(int32(), L"TotalOfferRem") + L"," + 
					GetMemberName(int32(), L"TotalBidRem") + L"," + 
					GetMemberName(int32(), L"TotalOfferCnt") + L"," + 
					GetMemberName(int32(), L"TotalBidCnt") + L"," + 
					GetMemberName(SDateTime(), L"InsertedTime");
			}
		};
		struct STick : public SProto
		{
			int32 Price{};
			int32 Volume{};
			int32 Offer0{};
			int32 Bid0{};
			int32 Trade{};
			SDateTime InsertedTime{};
			STick()
			{
			}
			STick(const int32& Price_, const int32& Volume_, const int32& Offer0_, const int32& Bid0_, const int32& Trade_, const SDateTime& InsertedTime_) : Price(Price_), Volume(Volume_), Offer0(Offer0_), Bid0(Bid0_), Trade(Trade_), InsertedTime(InsertedTime_)
			{
			}
			STick(int32&& Price_, int32&& Volume_, int32&& Offer0_, int32&& Bid0_, int32&& Trade_, SDateTime&& InsertedTime_) : Price(std::move(Price_)), Volume(std::move(Volume_)), Offer0(std::move(Offer0_)), Bid0(std::move(Bid0_)), Trade(std::move(Trade_)), InsertedTime(std::move(InsertedTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Price;
				Stream_ >> Volume;
				Stream_ >> Offer0;
				Stream_ >> Bid0;
				Stream_ >> Trade;
				Stream_ >> InsertedTime;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Price"] >> Price;
				Value_["Volume"] >> Volume;
				Value_["Offer0"] >> Offer0;
				Value_["Bid0"] >> Bid0;
				Value_["Trade"] >> Trade;
				Value_["InsertedTime"] >> InsertedTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Price;
				Stream_ << Volume;
				Stream_ << Offer0;
				Stream_ << Bid0;
				Stream_ << Trade;
				Stream_ << InsertedTime;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Price"] = Price;
				Value_["Volume"] = Volume;
				Value_["Offer0"] = Offer0;
				Value_["Bid0"] = Bid0;
				Value_["Trade"] = Trade;
				Value_["InsertedTime"] = InsertedTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(SDateTime());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"Price") + L"," + 
					GetMemberName(int32(), L"Volume") + L"," + 
					GetMemberName(int32(), L"Offer0") + L"," + 
					GetMemberName(int32(), L"Bid0") + L"," + 
					GetMemberName(int32(), L"Trade") + L"," + 
					GetMemberName(SDateTime(), L"InsertedTime");
			}
		};
		struct SRemain : public SProto
		{
			int32 OfferHo1{};
			int32 BidHo1{};
			int32 OfferRem1{};
			int32 BidRem1{};
			int32 OfferCnt1{};
			int32 BidCnt1{};
			int32 OfferHo2{};
			int32 BidHo2{};
			int32 OfferRem2{};
			int32 BidRem2{};
			int32 OfferCnt2{};
			int32 BidCnt2{};
			int32 OfferHo3{};
			int32 BidHo3{};
			int32 OfferRem3{};
			int32 BidRem3{};
			int32 OfferCnt3{};
			int32 BidCnt3{};
			int32 OfferHo4{};
			int32 BidHo4{};
			int32 OfferRem4{};
			int32 BidRem4{};
			int32 OfferCnt4{};
			int32 BidCnt4{};
			int32 OfferHo5{};
			int32 BidHo5{};
			int32 OfferRem5{};
			int32 BidRem5{};
			int32 OfferCnt5{};
			int32 BidCnt5{};
			int32 TotalOfferRem{};
			int32 TotalBidRem{};
			int32 TotalOfferCnt{};
			int32 TotalBidCnt{};
			SDateTime InsertedTime{};
			SRemain()
			{
			}
			SRemain(const int32& OfferHo1_, const int32& BidHo1_, const int32& OfferRem1_, const int32& BidRem1_, const int32& OfferCnt1_, const int32& BidCnt1_, const int32& OfferHo2_, const int32& BidHo2_, const int32& OfferRem2_, const int32& BidRem2_, const int32& OfferCnt2_, const int32& BidCnt2_, const int32& OfferHo3_, const int32& BidHo3_, const int32& OfferRem3_, const int32& BidRem3_, const int32& OfferCnt3_, const int32& BidCnt3_, const int32& OfferHo4_, const int32& BidHo4_, const int32& OfferRem4_, const int32& BidRem4_, const int32& OfferCnt4_, const int32& BidCnt4_, const int32& OfferHo5_, const int32& BidHo5_, const int32& OfferRem5_, const int32& BidRem5_, const int32& OfferCnt5_, const int32& BidCnt5_, const int32& TotalOfferRem_, const int32& TotalBidRem_, const int32& TotalOfferCnt_, const int32& TotalBidCnt_, const SDateTime& InsertedTime_) : OfferHo1(OfferHo1_), BidHo1(BidHo1_), OfferRem1(OfferRem1_), BidRem1(BidRem1_), OfferCnt1(OfferCnt1_), BidCnt1(BidCnt1_), OfferHo2(OfferHo2_), BidHo2(BidHo2_), OfferRem2(OfferRem2_), BidRem2(BidRem2_), OfferCnt2(OfferCnt2_), BidCnt2(BidCnt2_), OfferHo3(OfferHo3_), BidHo3(BidHo3_), OfferRem3(OfferRem3_), BidRem3(BidRem3_), OfferCnt3(OfferCnt3_), BidCnt3(BidCnt3_), OfferHo4(OfferHo4_), BidHo4(BidHo4_), OfferRem4(OfferRem4_), BidRem4(BidRem4_), OfferCnt4(OfferCnt4_), BidCnt4(BidCnt4_), OfferHo5(OfferHo5_), BidHo5(BidHo5_), OfferRem5(OfferRem5_), BidRem5(BidRem5_), OfferCnt5(OfferCnt5_), BidCnt5(BidCnt5_), TotalOfferRem(TotalOfferRem_), TotalBidRem(TotalBidRem_), TotalOfferCnt(TotalOfferCnt_), TotalBidCnt(TotalBidCnt_), InsertedTime(InsertedTime_)
			{
			}
			SRemain(int32&& OfferHo1_, int32&& BidHo1_, int32&& OfferRem1_, int32&& BidRem1_, int32&& OfferCnt1_, int32&& BidCnt1_, int32&& OfferHo2_, int32&& BidHo2_, int32&& OfferRem2_, int32&& BidRem2_, int32&& OfferCnt2_, int32&& BidCnt2_, int32&& OfferHo3_, int32&& BidHo3_, int32&& OfferRem3_, int32&& BidRem3_, int32&& OfferCnt3_, int32&& BidCnt3_, int32&& OfferHo4_, int32&& BidHo4_, int32&& OfferRem4_, int32&& BidRem4_, int32&& OfferCnt4_, int32&& BidCnt4_, int32&& OfferHo5_, int32&& BidHo5_, int32&& OfferRem5_, int32&& BidRem5_, int32&& OfferCnt5_, int32&& BidCnt5_, int32&& TotalOfferRem_, int32&& TotalBidRem_, int32&& TotalOfferCnt_, int32&& TotalBidCnt_, SDateTime&& InsertedTime_) : OfferHo1(std::move(OfferHo1_)), BidHo1(std::move(BidHo1_)), OfferRem1(std::move(OfferRem1_)), BidRem1(std::move(BidRem1_)), OfferCnt1(std::move(OfferCnt1_)), BidCnt1(std::move(BidCnt1_)), OfferHo2(std::move(OfferHo2_)), BidHo2(std::move(BidHo2_)), OfferRem2(std::move(OfferRem2_)), BidRem2(std::move(BidRem2_)), OfferCnt2(std::move(OfferCnt2_)), BidCnt2(std::move(BidCnt2_)), OfferHo3(std::move(OfferHo3_)), BidHo3(std::move(BidHo3_)), OfferRem3(std::move(OfferRem3_)), BidRem3(std::move(BidRem3_)), OfferCnt3(std::move(OfferCnt3_)), BidCnt3(std::move(BidCnt3_)), OfferHo4(std::move(OfferHo4_)), BidHo4(std::move(BidHo4_)), OfferRem4(std::move(OfferRem4_)), BidRem4(std::move(BidRem4_)), OfferCnt4(std::move(OfferCnt4_)), BidCnt4(std::move(BidCnt4_)), OfferHo5(std::move(OfferHo5_)), BidHo5(std::move(BidHo5_)), OfferRem5(std::move(OfferRem5_)), BidRem5(std::move(BidRem5_)), OfferCnt5(std::move(OfferCnt5_)), BidCnt5(std::move(BidCnt5_)), TotalOfferRem(std::move(TotalOfferRem_)), TotalBidRem(std::move(TotalBidRem_)), TotalOfferCnt(std::move(TotalOfferCnt_)), TotalBidCnt(std::move(TotalBidCnt_)), InsertedTime(std::move(InsertedTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> OfferHo1;
				Stream_ >> BidHo1;
				Stream_ >> OfferRem1;
				Stream_ >> BidRem1;
				Stream_ >> OfferCnt1;
				Stream_ >> BidCnt1;
				Stream_ >> OfferHo2;
				Stream_ >> BidHo2;
				Stream_ >> OfferRem2;
				Stream_ >> BidRem2;
				Stream_ >> OfferCnt2;
				Stream_ >> BidCnt2;
				Stream_ >> OfferHo3;
				Stream_ >> BidHo3;
				Stream_ >> OfferRem3;
				Stream_ >> BidRem3;
				Stream_ >> OfferCnt3;
				Stream_ >> BidCnt3;
				Stream_ >> OfferHo4;
				Stream_ >> BidHo4;
				Stream_ >> OfferRem4;
				Stream_ >> BidRem4;
				Stream_ >> OfferCnt4;
				Stream_ >> BidCnt4;
				Stream_ >> OfferHo5;
				Stream_ >> BidHo5;
				Stream_ >> OfferRem5;
				Stream_ >> BidRem5;
				Stream_ >> OfferCnt5;
				Stream_ >> BidCnt5;
				Stream_ >> TotalOfferRem;
				Stream_ >> TotalBidRem;
				Stream_ >> TotalOfferCnt;
				Stream_ >> TotalBidCnt;
				Stream_ >> InsertedTime;
			}
			void operator << (const Value& Value_) override
			{
				Value_["OfferHo1"] >> OfferHo1;
				Value_["BidHo1"] >> BidHo1;
				Value_["OfferRem1"] >> OfferRem1;
				Value_["BidRem1"] >> BidRem1;
				Value_["OfferCnt1"] >> OfferCnt1;
				Value_["BidCnt1"] >> BidCnt1;
				Value_["OfferHo2"] >> OfferHo2;
				Value_["BidHo2"] >> BidHo2;
				Value_["OfferRem2"] >> OfferRem2;
				Value_["BidRem2"] >> BidRem2;
				Value_["OfferCnt2"] >> OfferCnt2;
				Value_["BidCnt2"] >> BidCnt2;
				Value_["OfferHo3"] >> OfferHo3;
				Value_["BidHo3"] >> BidHo3;
				Value_["OfferRem3"] >> OfferRem3;
				Value_["BidRem3"] >> BidRem3;
				Value_["OfferCnt3"] >> OfferCnt3;
				Value_["BidCnt3"] >> BidCnt3;
				Value_["OfferHo4"] >> OfferHo4;
				Value_["BidHo4"] >> BidHo4;
				Value_["OfferRem4"] >> OfferRem4;
				Value_["BidRem4"] >> BidRem4;
				Value_["OfferCnt4"] >> OfferCnt4;
				Value_["BidCnt4"] >> BidCnt4;
				Value_["OfferHo5"] >> OfferHo5;
				Value_["BidHo5"] >> BidHo5;
				Value_["OfferRem5"] >> OfferRem5;
				Value_["BidRem5"] >> BidRem5;
				Value_["OfferCnt5"] >> OfferCnt5;
				Value_["BidCnt5"] >> BidCnt5;
				Value_["TotalOfferRem"] >> TotalOfferRem;
				Value_["TotalBidRem"] >> TotalBidRem;
				Value_["TotalOfferCnt"] >> TotalOfferCnt;
				Value_["TotalBidCnt"] >> TotalBidCnt;
				Value_["InsertedTime"] >> InsertedTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << OfferHo1;
				Stream_ << BidHo1;
				Stream_ << OfferRem1;
				Stream_ << BidRem1;
				Stream_ << OfferCnt1;
				Stream_ << BidCnt1;
				Stream_ << OfferHo2;
				Stream_ << BidHo2;
				Stream_ << OfferRem2;
				Stream_ << BidRem2;
				Stream_ << OfferCnt2;
				Stream_ << BidCnt2;
				Stream_ << OfferHo3;
				Stream_ << BidHo3;
				Stream_ << OfferRem3;
				Stream_ << BidRem3;
				Stream_ << OfferCnt3;
				Stream_ << BidCnt3;
				Stream_ << OfferHo4;
				Stream_ << BidHo4;
				Stream_ << OfferRem4;
				Stream_ << BidRem4;
				Stream_ << OfferCnt4;
				Stream_ << BidCnt4;
				Stream_ << OfferHo5;
				Stream_ << BidHo5;
				Stream_ << OfferRem5;
				Stream_ << BidRem5;
				Stream_ << OfferCnt5;
				Stream_ << BidCnt5;
				Stream_ << TotalOfferRem;
				Stream_ << TotalBidRem;
				Stream_ << TotalOfferCnt;
				Stream_ << TotalBidCnt;
				Stream_ << InsertedTime;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["OfferHo1"] = OfferHo1;
				Value_["BidHo1"] = BidHo1;
				Value_["OfferRem1"] = OfferRem1;
				Value_["BidRem1"] = BidRem1;
				Value_["OfferCnt1"] = OfferCnt1;
				Value_["BidCnt1"] = BidCnt1;
				Value_["OfferHo2"] = OfferHo2;
				Value_["BidHo2"] = BidHo2;
				Value_["OfferRem2"] = OfferRem2;
				Value_["BidRem2"] = BidRem2;
				Value_["OfferCnt2"] = OfferCnt2;
				Value_["BidCnt2"] = BidCnt2;
				Value_["OfferHo3"] = OfferHo3;
				Value_["BidHo3"] = BidHo3;
				Value_["OfferRem3"] = OfferRem3;
				Value_["BidRem3"] = BidRem3;
				Value_["OfferCnt3"] = OfferCnt3;
				Value_["BidCnt3"] = BidCnt3;
				Value_["OfferHo4"] = OfferHo4;
				Value_["BidHo4"] = BidHo4;
				Value_["OfferRem4"] = OfferRem4;
				Value_["BidRem4"] = BidRem4;
				Value_["OfferCnt4"] = OfferCnt4;
				Value_["BidCnt4"] = BidCnt4;
				Value_["OfferHo5"] = OfferHo5;
				Value_["BidHo5"] = BidHo5;
				Value_["OfferRem5"] = OfferRem5;
				Value_["BidRem5"] = BidRem5;
				Value_["OfferCnt5"] = OfferCnt5;
				Value_["BidCnt5"] = BidCnt5;
				Value_["TotalOfferRem"] = TotalOfferRem;
				Value_["TotalBidRem"] = TotalBidRem;
				Value_["TotalOfferCnt"] = TotalOfferCnt;
				Value_["TotalBidCnt"] = TotalBidCnt;
				Value_["InsertedTime"] = InsertedTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(SDateTime());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"OfferHo1") + L"," + 
					GetMemberName(int32(), L"BidHo1") + L"," + 
					GetMemberName(int32(), L"OfferRem1") + L"," + 
					GetMemberName(int32(), L"BidRem1") + L"," + 
					GetMemberName(int32(), L"OfferCnt1") + L"," + 
					GetMemberName(int32(), L"BidCnt1") + L"," + 
					GetMemberName(int32(), L"OfferHo2") + L"," + 
					GetMemberName(int32(), L"BidHo2") + L"," + 
					GetMemberName(int32(), L"OfferRem2") + L"," + 
					GetMemberName(int32(), L"BidRem2") + L"," + 
					GetMemberName(int32(), L"OfferCnt2") + L"," + 
					GetMemberName(int32(), L"BidCnt2") + L"," + 
					GetMemberName(int32(), L"OfferHo3") + L"," + 
					GetMemberName(int32(), L"BidHo3") + L"," + 
					GetMemberName(int32(), L"OfferRem3") + L"," + 
					GetMemberName(int32(), L"BidRem3") + L"," + 
					GetMemberName(int32(), L"OfferCnt3") + L"," + 
					GetMemberName(int32(), L"BidCnt3") + L"," + 
					GetMemberName(int32(), L"OfferHo4") + L"," + 
					GetMemberName(int32(), L"BidHo4") + L"," + 
					GetMemberName(int32(), L"OfferRem4") + L"," + 
					GetMemberName(int32(), L"BidRem4") + L"," + 
					GetMemberName(int32(), L"OfferCnt4") + L"," + 
					GetMemberName(int32(), L"BidCnt4") + L"," + 
					GetMemberName(int32(), L"OfferHo5") + L"," + 
					GetMemberName(int32(), L"BidHo5") + L"," + 
					GetMemberName(int32(), L"OfferRem5") + L"," + 
					GetMemberName(int32(), L"BidRem5") + L"," + 
					GetMemberName(int32(), L"OfferCnt5") + L"," + 
					GetMemberName(int32(), L"BidCnt5") + L"," + 
					GetMemberName(int32(), L"TotalOfferRem") + L"," + 
					GetMemberName(int32(), L"TotalBidRem") + L"," + 
					GetMemberName(int32(), L"TotalOfferCnt") + L"," + 
					GetMemberName(int32(), L"TotalBidCnt") + L"," + 
					GetMemberName(SDateTime(), L"InsertedTime");
			}
		};
		struct SRealTick : public SProto
		{
			int64 Index{};
			int64 DateTime{};
			int32 Price{};
			int32 CVolume{};
			int32 Volume{};
			int32 Val{};
			int32 SellVolume{};
			int32 SellCnt{};
			int32 BuyVolume{};
			int32 BuyCnt{};
			int32 Pwr{};
			int32 OfferHo1{};
			int32 BidHo1{};
			int32 OpenYak{};
			int32 TheoryPrice{};
			int32 Kasis{};
			int32 SBasis{};
			int32 IBasis{};
			int32 OpenYakCha{};
			SRealTick()
			{
			}
			SRealTick(const int64& Index_, const int64& DateTime_, const int32& Price_, const int32& CVolume_, const int32& Volume_, const int32& Val_, const int32& SellVolume_, const int32& SellCnt_, const int32& BuyVolume_, const int32& BuyCnt_, const int32& Pwr_, const int32& OfferHo1_, const int32& BidHo1_, const int32& OpenYak_, const int32& TheoryPrice_, const int32& Kasis_, const int32& SBasis_, const int32& IBasis_, const int32& OpenYakCha_) : Index(Index_), DateTime(DateTime_), Price(Price_), CVolume(CVolume_), Volume(Volume_), Val(Val_), SellVolume(SellVolume_), SellCnt(SellCnt_), BuyVolume(BuyVolume_), BuyCnt(BuyCnt_), Pwr(Pwr_), OfferHo1(OfferHo1_), BidHo1(BidHo1_), OpenYak(OpenYak_), TheoryPrice(TheoryPrice_), Kasis(Kasis_), SBasis(SBasis_), IBasis(IBasis_), OpenYakCha(OpenYakCha_)
			{
			}
			SRealTick(int64&& Index_, int64&& DateTime_, int32&& Price_, int32&& CVolume_, int32&& Volume_, int32&& Val_, int32&& SellVolume_, int32&& SellCnt_, int32&& BuyVolume_, int32&& BuyCnt_, int32&& Pwr_, int32&& OfferHo1_, int32&& BidHo1_, int32&& OpenYak_, int32&& TheoryPrice_, int32&& Kasis_, int32&& SBasis_, int32&& IBasis_, int32&& OpenYakCha_) : Index(std::move(Index_)), DateTime(std::move(DateTime_)), Price(std::move(Price_)), CVolume(std::move(CVolume_)), Volume(std::move(Volume_)), Val(std::move(Val_)), SellVolume(std::move(SellVolume_)), SellCnt(std::move(SellCnt_)), BuyVolume(std::move(BuyVolume_)), BuyCnt(std::move(BuyCnt_)), Pwr(std::move(Pwr_)), OfferHo1(std::move(OfferHo1_)), BidHo1(std::move(BidHo1_)), OpenYak(std::move(OpenYak_)), TheoryPrice(std::move(TheoryPrice_)), Kasis(std::move(Kasis_)), SBasis(std::move(SBasis_)), IBasis(std::move(IBasis_)), OpenYakCha(std::move(OpenYakCha_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Index;
				Stream_ >> DateTime;
				Stream_ >> Price;
				Stream_ >> CVolume;
				Stream_ >> Volume;
				Stream_ >> Val;
				Stream_ >> SellVolume;
				Stream_ >> SellCnt;
				Stream_ >> BuyVolume;
				Stream_ >> BuyCnt;
				Stream_ >> Pwr;
				Stream_ >> OfferHo1;
				Stream_ >> BidHo1;
				Stream_ >> OpenYak;
				Stream_ >> TheoryPrice;
				Stream_ >> Kasis;
				Stream_ >> SBasis;
				Stream_ >> IBasis;
				Stream_ >> OpenYakCha;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Index"] >> Index;
				Value_["DateTime"] >> DateTime;
				Value_["Price"] >> Price;
				Value_["CVolume"] >> CVolume;
				Value_["Volume"] >> Volume;
				Value_["Val"] >> Val;
				Value_["SellVolume"] >> SellVolume;
				Value_["SellCnt"] >> SellCnt;
				Value_["BuyVolume"] >> BuyVolume;
				Value_["BuyCnt"] >> BuyCnt;
				Value_["Pwr"] >> Pwr;
				Value_["OfferHo1"] >> OfferHo1;
				Value_["BidHo1"] >> BidHo1;
				Value_["OpenYak"] >> OpenYak;
				Value_["TheoryPrice"] >> TheoryPrice;
				Value_["Kasis"] >> Kasis;
				Value_["SBasis"] >> SBasis;
				Value_["IBasis"] >> IBasis;
				Value_["OpenYakCha"] >> OpenYakCha;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Index;
				Stream_ << DateTime;
				Stream_ << Price;
				Stream_ << CVolume;
				Stream_ << Volume;
				Stream_ << Val;
				Stream_ << SellVolume;
				Stream_ << SellCnt;
				Stream_ << BuyVolume;
				Stream_ << BuyCnt;
				Stream_ << Pwr;
				Stream_ << OfferHo1;
				Stream_ << BidHo1;
				Stream_ << OpenYak;
				Stream_ << TheoryPrice;
				Stream_ << Kasis;
				Stream_ << SBasis;
				Stream_ << IBasis;
				Stream_ << OpenYakCha;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Index"] = Index;
				Value_["DateTime"] = DateTime;
				Value_["Price"] = Price;
				Value_["CVolume"] = CVolume;
				Value_["Volume"] = Volume;
				Value_["Val"] = Val;
				Value_["SellVolume"] = SellVolume;
				Value_["SellCnt"] = SellCnt;
				Value_["BuyVolume"] = BuyVolume;
				Value_["BuyCnt"] = BuyCnt;
				Value_["Pwr"] = Pwr;
				Value_["OfferHo1"] = OfferHo1;
				Value_["BidHo1"] = BidHo1;
				Value_["OpenYak"] = OpenYak;
				Value_["TheoryPrice"] = TheoryPrice;
				Value_["Kasis"] = Kasis;
				Value_["SBasis"] = SBasis;
				Value_["IBasis"] = IBasis;
				Value_["OpenYakCha"] = OpenYakCha;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(int64()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"Index") + L"," + 
					GetMemberName(int64(), L"DateTime") + L"," + 
					GetMemberName(int32(), L"Price") + L"," + 
					GetMemberName(int32(), L"CVolume") + L"," + 
					GetMemberName(int32(), L"Volume") + L"," + 
					GetMemberName(int32(), L"Val") + L"," + 
					GetMemberName(int32(), L"SellVolume") + L"," + 
					GetMemberName(int32(), L"SellCnt") + L"," + 
					GetMemberName(int32(), L"BuyVolume") + L"," + 
					GetMemberName(int32(), L"BuyCnt") + L"," + 
					GetMemberName(int32(), L"Pwr") + L"," + 
					GetMemberName(int32(), L"OfferHo1") + L"," + 
					GetMemberName(int32(), L"BidHo1") + L"," + 
					GetMemberName(int32(), L"OpenYak") + L"," + 
					GetMemberName(int32(), L"TheoryPrice") + L"," + 
					GetMemberName(int32(), L"Kasis") + L"," + 
					GetMemberName(int32(), L"SBasis") + L"," + 
					GetMemberName(int32(), L"IBasis") + L"," + 
					GetMemberName(int32(), L"OpenYakCha");
			}
		};
		struct SRemainUnit : public SProto
		{
			int32 Price{};
			int32 Volume{};
			int32 Change{};
			int32 Count{};
			SRemainUnit()
			{
			}
			SRemainUnit(const int32& Price_, const int32& Volume_, const int32& Change_, const int32& Count_) : Price(Price_), Volume(Volume_), Change(Change_), Count(Count_)
			{
			}
			SRemainUnit(int32&& Price_, int32&& Volume_, int32&& Change_, int32&& Count_) : Price(std::move(Price_)), Volume(std::move(Volume_)), Change(std::move(Change_)), Count(std::move(Count_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Price;
				Stream_ >> Volume;
				Stream_ >> Change;
				Stream_ >> Count;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Price"] >> Price;
				Value_["Volume"] >> Volume;
				Value_["Change"] >> Change;
				Value_["Count"] >> Count;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Price;
				Stream_ << Volume;
				Stream_ << Change;
				Stream_ << Count;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Price"] = Price;
				Value_["Volume"] = Volume;
				Value_["Change"] = Change;
				Value_["Count"] = Count;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"Price") + L"," + 
					GetMemberName(int32(), L"Volume") + L"," + 
					GetMemberName(int32(), L"Change") + L"," + 
					GetMemberName(int32(), L"Count");
			}
		};
		struct SRemainDB : public SProto
		{
			system_clock::time_point TimePoint{};
			array<SRemainUnit,10> Datas{};
			SRemainDB()
			{
			}
			SRemainDB(const system_clock::time_point& TimePoint_, const array<SRemainUnit,10>& Datas_) : TimePoint(TimePoint_), Datas(Datas_)
			{
			}
			SRemainDB(system_clock::time_point&& TimePoint_, array<SRemainUnit,10>&& Datas_) : TimePoint(std::move(TimePoint_)), Datas(std::move(Datas_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> TimePoint;
				Stream_ >> Datas;
			}
			void operator << (const Value& Value_) override
			{
				Value_["TimePoint"] >> TimePoint;
				Value_["Datas"] >> Datas;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << TimePoint;
				Stream_ << Datas;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["TimePoint"] = TimePoint;
				Value_["Datas"] = Datas;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(system_clock::time_point()) + L"," + 
					GetStdName(array<SRemainUnit,10>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(system_clock::time_point(), L"TimePoint") + L"," + 
					GetMemberName(array<SRemainUnit,10>(), L"Datas");
			}
		};
		struct SChart : public SProto
		{
			int32 Price{};
			int32 Volume{};
			SChart()
			{
			}
			SChart(const int32& Price_, const int32& Volume_) : Price(Price_), Volume(Volume_)
			{
			}
			SChart(int32&& Price_, int32&& Volume_) : Price(std::move(Price_)), Volume(std::move(Volume_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Price;
				Stream_ >> Volume;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Price"] >> Price;
				Value_["Volume"] >> Volume;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Price;
				Stream_ << Volume;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Price"] = Price;
				Value_["Volume"] = Volume;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"Price") + L"," + 
					GetMemberName(int32(), L"Volume");
			}
		};
		struct SChartTime : public SChart
		{
			system_clock::time_point TimePoint{};
			SChartTime()
			{
			}
			SChartTime(const SChart& Super_, const system_clock::time_point& TimePoint_) : SChart(Super_), TimePoint(TimePoint_)
			{
			}
			SChartTime(SChart&& Super_, system_clock::time_point&& TimePoint_) : SChart(std::move(Super_)), TimePoint(std::move(TimePoint_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SChart::operator << (Stream_);
				Stream_ >> TimePoint;
			}
			void operator << (const Value& Value_) override
			{
				SChart::operator << (Value_);
				Value_["TimePoint"] >> TimePoint;
			}
			void operator >> (CStream& Stream_) const override
			{
				SChart::operator >> (Stream_);
				Stream_ << TimePoint;
			}
			void operator >> (Value& Value_) const override
			{
				SChart::operator >> (Value_);
				Value_["TimePoint"] = TimePoint;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SChart()) + L"," + 
					GetStdName(system_clock::time_point());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SChart(), L"") + L"," + 
					GetMemberName(system_clock::time_point(), L"TimePoint");
			}
		};
		struct SOrder : public SProto
		{
			int32 Price{};
			int32 Volume{};
			SOrder()
			{
			}
			SOrder(const int32& Price_, const int32& Volume_) : Price(Price_), Volume(Volume_)
			{
			}
			SOrder(int32&& Price_, int32&& Volume_) : Price(std::move(Price_)), Volume(std::move(Volume_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Price;
				Stream_ >> Volume;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Price"] >> Price;
				Value_["Volume"] >> Volume;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Price;
				Stream_ << Volume;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Price"] = Price;
				Value_["Volume"] = Volume;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"Price") + L"," + 
					GetMemberName(int32(), L"Volume");
			}
		};
		struct SCandle : public SProto
		{
			int32 Open{};
			int32 High{};
			int32 Low{};
			int32 Close{};
			SCandle()
			{
			}
			SCandle(const int32& Open_, const int32& High_, const int32& Low_, const int32& Close_) : Open(Open_), High(High_), Low(Low_), Close(Close_)
			{
			}
			SCandle(int32&& Open_, int32&& High_, int32&& Low_, int32&& Close_) : Open(std::move(Open_)), High(std::move(High_)), Low(std::move(Low_)), Close(std::move(Close_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Open;
				Stream_ >> High;
				Stream_ >> Low;
				Stream_ >> Close;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Open"] >> Open;
				Value_["High"] >> High;
				Value_["Low"] >> Low;
				Value_["Close"] >> Close;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Open;
				Stream_ << High;
				Stream_ << Low;
				Stream_ << Close;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Open"] = Open;
				Value_["High"] = High;
				Value_["Low"] = Low;
				Value_["Close"] = Close;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"Open") + L"," + 
					GetMemberName(int32(), L"High") + L"," + 
					GetMemberName(int32(), L"Low") + L"," + 
					GetMemberName(int32(), L"Close");
			}
		};
	}
}
