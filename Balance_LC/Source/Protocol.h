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
namespace rso
{
	namespace balance_lc
	{
		using namespace net;
		enum class EProto
		{
			SaServerOn,
			SaClientCnt,
			AcServerToConnect,
			ScAllocated,
			AsUserProto,
			SaUserProto,
			ScUserProto,
			CsUserProto,
			Max,
			Null,
		};
		struct SSaServerOn : public SProto
		{
			SNamePort ClientBindNamePortPub{};
			int32 ClientCnt{};
			SSaServerOn()
			{
			}
			SSaServerOn(const SNamePort& ClientBindNamePortPub_, const int32& ClientCnt_) : ClientBindNamePortPub(ClientBindNamePortPub_), ClientCnt(ClientCnt_)
			{
			}
			SSaServerOn(SNamePort&& ClientBindNamePortPub_, int32&& ClientCnt_) : ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), ClientCnt(std::move(ClientCnt_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientBindNamePortPub;
				Stream_ >> ClientCnt;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
				Value_["ClientCnt"] >> ClientCnt;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientBindNamePortPub;
				Stream_ << ClientCnt;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
				Value_["ClientCnt"] = ClientCnt;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
					GetMemberName(int32(), L"ClientCnt");
			}
		};
		struct SSaClientCnt : public SProto
		{
			int32 ClientCnt{};
			SSaClientCnt()
			{
			}
			SSaClientCnt(const int32& ClientCnt_) : ClientCnt(ClientCnt_)
			{
			}
			SSaClientCnt(int32&& ClientCnt_) : ClientCnt(std::move(ClientCnt_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientCnt;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientCnt"] >> ClientCnt;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientCnt;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientCnt"] = ClientCnt;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"ClientCnt");
			}
		};
		struct SAcServerToConnect : public SProto
		{
			SNamePort ClientBindNamePortPub{};
			SAcServerToConnect()
			{
			}
			SAcServerToConnect(const SNamePort& ClientBindNamePortPub_) : ClientBindNamePortPub(ClientBindNamePortPub_)
			{
			}
			SAcServerToConnect(SNamePort&& ClientBindNamePortPub_) : ClientBindNamePortPub(std::move(ClientBindNamePortPub_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientBindNamePortPub;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientBindNamePortPub;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ClientBindNamePortPub");
			}
		};
		struct SScAllocated : public SProto
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
		struct SServer : public SSaServerOn
		{
			list<system_clock::time_point> EndTimes{};
			SServer()
			{
			}
			SServer(const SSaServerOn& Super_, const list<system_clock::time_point>& EndTimes_) : SSaServerOn(Super_), EndTimes(EndTimes_)
			{
			}
			SServer(SSaServerOn&& Super_, list<system_clock::time_point>&& EndTimes_) : SSaServerOn(std::move(Super_)), EndTimes(std::move(EndTimes_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SSaServerOn::operator << (Stream_);
				Stream_ >> EndTimes;
			}
			void operator << (const Value& Value_) override
			{
				SSaServerOn::operator << (Value_);
				Value_["EndTimes"] >> EndTimes;
			}
			void operator >> (CStream& Stream_) const override
			{
				SSaServerOn::operator >> (Stream_);
				Stream_ << EndTimes;
			}
			void operator >> (Value& Value_) const override
			{
				SSaServerOn::operator >> (Value_);
				Value_["EndTimes"] = EndTimes;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SSaServerOn()) + L"," + 
					GetStdName(list<system_clock::time_point>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SSaServerOn(), L"") + L"," + 
					GetMemberName(list<system_clock::time_point>(), L"EndTimes");
			}
		};
	}
}
