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
	namespace balance_rr
	{
		using namespace net;
		enum class EProto
		{
			SaServerOn,
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
			SSaServerOn()
			{
			}
			SSaServerOn(const SNamePort& ClientBindNamePortPub_) : ClientBindNamePortPub(ClientBindNamePortPub_)
			{
			}
			SSaServerOn(SNamePort&& ClientBindNamePortPub_) : ClientBindNamePortPub(std::move(ClientBindNamePortPub_))
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
			SServer()
			{
			}
			SServer(const SSaServerOn& Super_) : SSaServerOn(Super_)
			{
			}
			SServer(SSaServerOn&& Super_) : SSaServerOn(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SSaServerOn::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SSaServerOn::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SSaServerOn::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SSaServerOn::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SSaServerOn());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SSaServerOn(), L"");
			}
		};
	}
}
