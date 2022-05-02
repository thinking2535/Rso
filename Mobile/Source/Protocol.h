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
	using namespace net;
	namespace mobile
	{
		using TProtoSeq = uint64;
		enum class EProtoCs
		{
			Link,
			ReLink,
			UnLink,
			Ack,
			ReSend,
			UserProto,
			Max,
			Null,
		};
		enum class EProtoSc
		{
			Link,
			ReLink,
			UnLink,
			Ack,
			ReSend,
			UserProto,
			Max,
			Null,
		};
		struct SHeaderCs : public SProto
		{
			EProtoCs Proto{};
			SHeaderCs()
			{
			}
			SHeaderCs(const EProtoCs& Proto_) : Proto(Proto_)
			{
			}
			SHeaderCs(EProtoCs&& Proto_) : Proto(std::move(Proto_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Proto;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Proto"] >> Proto;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Proto;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Proto"] = Proto;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EProtoCs());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EProtoCs(), L"Proto");
			}
		};
		struct SHeaderSc : public SProto
		{
			EProtoSc Proto{};
			SHeaderSc()
			{
			}
			SHeaderSc(const EProtoSc& Proto_) : Proto(Proto_)
			{
			}
			SHeaderSc(EProtoSc&& Proto_) : Proto(std::move(Proto_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Proto;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Proto"] >> Proto;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Proto;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Proto"] = Proto;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EProtoSc());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EProtoSc(), L"Proto");
			}
		};
		struct SLinkCs : public SProto
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
		struct SLinkSc : public SProto
		{
			SKey ServerExtKey{};
			SLinkSc()
			{
			}
			SLinkSc(const SKey& ServerExtKey_) : ServerExtKey(ServerExtKey_)
			{
			}
			SLinkSc(SKey&& ServerExtKey_) : ServerExtKey(std::move(ServerExtKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerExtKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerExtKey"] >> ServerExtKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerExtKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerExtKey"] = ServerExtKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"ServerExtKey");
			}
		};
		struct SReLinkCs : public SProto
		{
			SKey ServerExtKey{};
			TProtoSeq ClientProtoSeqMustRecv{};
			SReLinkCs()
			{
			}
			SReLinkCs(const SKey& ServerExtKey_, const TProtoSeq& ClientProtoSeqMustRecv_) : ServerExtKey(ServerExtKey_), ClientProtoSeqMustRecv(ClientProtoSeqMustRecv_)
			{
			}
			SReLinkCs(SKey&& ServerExtKey_, TProtoSeq&& ClientProtoSeqMustRecv_) : ServerExtKey(std::move(ServerExtKey_)), ClientProtoSeqMustRecv(std::move(ClientProtoSeqMustRecv_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerExtKey;
				Stream_ >> ClientProtoSeqMustRecv;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerExtKey"] >> ServerExtKey;
				Value_["ClientProtoSeqMustRecv"] >> ClientProtoSeqMustRecv;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerExtKey;
				Stream_ << ClientProtoSeqMustRecv;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerExtKey"] = ServerExtKey;
				Value_["ClientProtoSeqMustRecv"] = ClientProtoSeqMustRecv;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(TProtoSeq());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"ServerExtKey") + L"," + 
					GetMemberName(TProtoSeq(), L"ClientProtoSeqMustRecv");
			}
		};
		struct SReLinkSc : public SProto
		{
			TProtoSeq ServerProtoSeqMustRecv{};
			SReLinkSc()
			{
			}
			SReLinkSc(const TProtoSeq& ServerProtoSeqMustRecv_) : ServerProtoSeqMustRecv(ServerProtoSeqMustRecv_)
			{
			}
			SReLinkSc(TProtoSeq&& ServerProtoSeqMustRecv_) : ServerProtoSeqMustRecv(std::move(ServerProtoSeqMustRecv_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerProtoSeqMustRecv;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerProtoSeqMustRecv"] >> ServerProtoSeqMustRecv;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerProtoSeqMustRecv;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerProtoSeqMustRecv"] = ServerProtoSeqMustRecv;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TProtoSeq());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TProtoSeq(), L"ServerProtoSeqMustRecv");
			}
		};
		struct SUnLinkCs : public SProto
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
		struct SUnLinkSc : public SProto
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
	}
}
