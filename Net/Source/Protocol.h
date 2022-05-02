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
	namespace net
	{
		using namespace std;
		using namespace core;
		using namespace json;
		using TSize = int32;
		using TCheckSum = uint64;
		using TUID = int64;
		using TPeerCnt = uint32;
		using TLongIP = uint32;
		using TPort = uint16;
		using TPacketSeq = uint64;
		using TSessionCode = int64;
		enum class ENetRet
		{
			Ok,
			UserClose,
			HeartBeatFail,
			KeepConnectTimeOut,
			RecvError,
			ConnectFail,
			CertifyFail,
			SystemError,
			SocketError,
			InvalidPacket,
			Max,
			Null,
		};
		enum class EPacketType : int8
		{
			Ping,
			Pong,
			User,
			Max,
			Null,
		};
		enum class ERecvState
		{
			NoData,
			PingPong,
			UserData,
			Error,
			Max,
			Null,
		};
		struct SKey : public SProto
		{
			TPeerCnt PeerNum{};
			TPeerCnt PeerCounter{};
			SKey()
			{
			}
			SKey(const TPeerCnt& PeerNum_, const TPeerCnt& PeerCounter_) : PeerNum(PeerNum_), PeerCounter(PeerCounter_)
			{
			}
			SKey(TPeerCnt&& PeerNum_, TPeerCnt&& PeerCounter_) : PeerNum(std::move(PeerNum_)), PeerCounter(std::move(PeerCounter_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> PeerNum;
				Stream_ >> PeerCounter;
			}
			void operator << (const Value& Value_) override
			{
				Value_["PeerNum"] >> PeerNum;
				Value_["PeerCounter"] >> PeerCounter;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << PeerNum;
				Stream_ << PeerCounter;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["PeerNum"] = PeerNum;
				Value_["PeerCounter"] = PeerCounter;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TPeerCnt()) + L"," + 
					GetStdName(TPeerCnt());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TPeerCnt(), L"PeerNum") + L"," + 
					GetMemberName(TPeerCnt(), L"PeerCounter");
			}
		};
		struct SHeader : public SProto
		{
			TSize Size{};
			TCheckSum CheckSum{};
			TPacketSeq SendPacketSeq{};
			SHeader()
			{
			}
			SHeader(const TSize& Size_, const TCheckSum& CheckSum_, const TPacketSeq& SendPacketSeq_) : Size(Size_), CheckSum(CheckSum_), SendPacketSeq(SendPacketSeq_)
			{
			}
			SHeader(TSize&& Size_, TCheckSum&& CheckSum_, TPacketSeq&& SendPacketSeq_) : Size(std::move(Size_)), CheckSum(std::move(CheckSum_)), SendPacketSeq(std::move(SendPacketSeq_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Size;
				Stream_ >> CheckSum;
				Stream_ >> SendPacketSeq;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Size"] >> Size;
				Value_["CheckSum"] >> CheckSum;
				Value_["SendPacketSeq"] >> SendPacketSeq;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Size;
				Stream_ << CheckSum;
				Stream_ << SendPacketSeq;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Size"] = Size;
				Value_["CheckSum"] = CheckSum;
				Value_["SendPacketSeq"] = SendPacketSeq;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TSize()) + L"," + 
					GetStdName(TCheckSum()) + L"," + 
					GetStdName(TPacketSeq());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TSize(), L"Size") + L"," + 
					GetMemberName(TCheckSum(), L"CheckSum") + L"," + 
					GetMemberName(TPacketSeq(), L"SendPacketSeq");
			}
		};
		struct SHeader2 : public SProto
		{
			EPacketType PacketType{};
			TPacketSeq RecvPacketSeq{};
			SHeader2()
			{
			}
			SHeader2(const EPacketType& PacketType_, const TPacketSeq& RecvPacketSeq_) : PacketType(PacketType_), RecvPacketSeq(RecvPacketSeq_)
			{
			}
			SHeader2(EPacketType&& PacketType_, TPacketSeq&& RecvPacketSeq_) : PacketType(std::move(PacketType_)), RecvPacketSeq(std::move(RecvPacketSeq_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> PacketType;
				Stream_ >> RecvPacketSeq;
			}
			void operator << (const Value& Value_) override
			{
				Value_["PacketType"] >> PacketType;
				Value_["RecvPacketSeq"] >> RecvPacketSeq;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << PacketType;
				Stream_ << RecvPacketSeq;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["PacketType"] = PacketType;
				Value_["RecvPacketSeq"] = RecvPacketSeq;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EPacketType()) + L"," + 
					GetStdName(TPacketSeq());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EPacketType(), L"PacketType") + L"," + 
					GetMemberName(TPacketSeq(), L"RecvPacketSeq");
			}
		};
		struct SNamePort : public SProto
		{
			u16string Name{};
			TPort Port{};
			SNamePort()
			{
			}
			SNamePort(const u16string& Name_, const TPort& Port_) : Name(Name_), Port(Port_)
			{
			}
			SNamePort(u16string&& Name_, TPort&& Port_) : Name(std::move(Name_)), Port(std::move(Port_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Name;
				Stream_ >> Port;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Name"] >> Name;
				Value_["Port"] >> Port;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Name;
				Stream_ << Port;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Name"] = Name;
				Value_["Port"] = Port;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(u16string()) + L"," + 
					GetStdName(TPort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(u16string(), L"Name") + L"," + 
					GetMemberName(TPort(), L"Port");
			}
		};
		struct SCountryCodeMinuteCountOffset : public SProto
		{
			string CountryCode{};
			int32 MinuteCountOffset{};
			SCountryCodeMinuteCountOffset()
			{
			}
			SCountryCodeMinuteCountOffset(const string& CountryCode_, const int32& MinuteCountOffset_) : CountryCode(CountryCode_), MinuteCountOffset(MinuteCountOffset_)
			{
			}
			SCountryCodeMinuteCountOffset(string&& CountryCode_, int32&& MinuteCountOffset_) : CountryCode(std::move(CountryCode_)), MinuteCountOffset(std::move(MinuteCountOffset_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> CountryCode;
				Stream_ >> MinuteCountOffset;
			}
			void operator << (const Value& Value_) override
			{
				Value_["CountryCode"] >> CountryCode;
				Value_["MinuteCountOffset"] >> MinuteCountOffset;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << CountryCode;
				Stream_ << MinuteCountOffset;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["CountryCode"] = CountryCode;
				Value_["MinuteCountOffset"] = MinuteCountOffset;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(string()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(string(), L"CountryCode") + L"," + 
					GetMemberName(int32(), L"MinuteCountOffset");
			}
		};
		template<typename TType>
		struct SRangeKey : public SProto
		{
			TType MinValue{};
			TType MaxValue{};
			SRangeKey()
			{
			}
			SRangeKey(const TType& MinValue_, const TType& MaxValue_) : MinValue(MinValue_), MaxValue(MaxValue_)
			{
			}
			SRangeKey(TType&& MinValue_, TType&& MaxValue_) : MinValue(std::move(MinValue_)), MaxValue(std::move(MaxValue_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MinValue;
				Stream_ >> MaxValue;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MinValue"] >> MinValue;
				Value_["MaxValue"] >> MaxValue;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MinValue;
				Stream_ << MaxValue;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MinValue"] = MinValue;
				Value_["MaxValue"] = MaxValue;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TType()) + L"," + 
					GetStdName(TType());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TType(), L"MinValue") + L"," + 
					GetMemberName(TType(), L"MaxValue");
			}
		};
		using SRangeUID = SRangeKey<TUID>;
	}
}
