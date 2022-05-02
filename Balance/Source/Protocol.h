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
	namespace balance
	{
		using namespace net;
		enum class EProto
		{
			UdParentOn,
			DuChildOn,
			UdNewChild,
			DuNewChild,
			DuNewChildFail,
			UdNewParent,
			UdCapacity,
			DuCapacity,
			CsConnect,
			ScNewParent,
			ScAllocated,
			UdBroadCast,
			UdUserProto,
			DuUserProto,
			ScUserProto,
			CsUserProto,
			Max,
			Null,
		};
		struct SHeader : public SProto
		{
			EProto Proto{};
			SHeader()
			{
			}
			SHeader(const EProto& Proto_) : Proto(Proto_)
			{
			}
			SHeader(EProto&& Proto_) : Proto(std::move(Proto_))
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
					GetStdName(EProto());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EProto(), L"Proto");
			}
		};
		struct SCapacity : public SProto
		{
			TPeerCnt ClientCnt{};
			TPeerCnt ServerCnt{};
			SCapacity()
			{
			}
			SCapacity(const TPeerCnt& ClientCnt_, const TPeerCnt& ServerCnt_) : ClientCnt(ClientCnt_), ServerCnt(ServerCnt_)
			{
			}
			SCapacity(TPeerCnt&& ClientCnt_, TPeerCnt&& ServerCnt_) : ClientCnt(std::move(ClientCnt_)), ServerCnt(std::move(ServerCnt_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientCnt;
				Stream_ >> ServerCnt;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientCnt"] >> ClientCnt;
				Value_["ServerCnt"] >> ServerCnt;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientCnt;
				Stream_ << ServerCnt;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientCnt"] = ClientCnt;
				Value_["ServerCnt"] = ServerCnt;
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
					GetMemberName(TPeerCnt(), L"ClientCnt") + L"," + 
					GetMemberName(TPeerCnt(), L"ServerCnt");
			}
		};
		struct SServer : public SProto
		{
			SNamePort ClientBindNamePortPub{};
			SCapacity Capacity{};
			SServer()
			{
			}
			SServer(const SNamePort& ClientBindNamePortPub_, const SCapacity& Capacity_) : ClientBindNamePortPub(ClientBindNamePortPub_), Capacity(Capacity_)
			{
			}
			SServer(SNamePort&& ClientBindNamePortPub_, SCapacity&& Capacity_) : ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), Capacity(std::move(Capacity_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientBindNamePortPub;
				Stream_ >> Capacity;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
				Value_["Capacity"] >> Capacity;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientBindNamePortPub;
				Stream_ << Capacity;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
				Value_["Capacity"] = Capacity;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(SCapacity());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
					GetMemberName(SCapacity(), L"Capacity");
			}
		};
		struct SUdParentOn : public SServer
		{
			TPeerCnt ChildCntMax{};
			TPeerCnt ErrorCnt{};
			SUdParentOn()
			{
			}
			SUdParentOn(const SServer& Super_, const TPeerCnt& ChildCntMax_, const TPeerCnt& ErrorCnt_) : SServer(Super_), ChildCntMax(ChildCntMax_), ErrorCnt(ErrorCnt_)
			{
			}
			SUdParentOn(SServer&& Super_, TPeerCnt&& ChildCntMax_, TPeerCnt&& ErrorCnt_) : SServer(std::move(Super_)), ChildCntMax(std::move(ChildCntMax_)), ErrorCnt(std::move(ErrorCnt_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SServer::operator << (Stream_);
				Stream_ >> ChildCntMax;
				Stream_ >> ErrorCnt;
			}
			void operator << (const Value& Value_) override
			{
				SServer::operator << (Value_);
				Value_["ChildCntMax"] >> ChildCntMax;
				Value_["ErrorCnt"] >> ErrorCnt;
			}
			void operator >> (CStream& Stream_) const override
			{
				SServer::operator >> (Stream_);
				Stream_ << ChildCntMax;
				Stream_ << ErrorCnt;
			}
			void operator >> (Value& Value_) const override
			{
				SServer::operator >> (Value_);
				Value_["ChildCntMax"] = ChildCntMax;
				Value_["ErrorCnt"] = ErrorCnt;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SServer()) + L"," + 
					GetStdName(TPeerCnt()) + L"," + 
					GetStdName(TPeerCnt());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SServer(), L"") + L"," + 
					GetMemberName(TPeerCnt(), L"ChildCntMax") + L"," + 
					GetMemberName(TPeerCnt(), L"ErrorCnt");
			}
		};
		struct SDuChildOn : public SServer
		{
			TSessionCode SessionCode{};
			SNamePort ChildBindNamePort{};
			SDuChildOn()
			{
			}
			SDuChildOn(const SServer& Super_, const TSessionCode& SessionCode_, const SNamePort& ChildBindNamePort_) : SServer(Super_), SessionCode(SessionCode_), ChildBindNamePort(ChildBindNamePort_)
			{
			}
			SDuChildOn(SServer&& Super_, TSessionCode&& SessionCode_, SNamePort&& ChildBindNamePort_) : SServer(std::move(Super_)), SessionCode(std::move(SessionCode_)), ChildBindNamePort(std::move(ChildBindNamePort_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SServer::operator << (Stream_);
				Stream_ >> SessionCode;
				Stream_ >> ChildBindNamePort;
			}
			void operator << (const Value& Value_) override
			{
				SServer::operator << (Value_);
				Value_["SessionCode"] >> SessionCode;
				Value_["ChildBindNamePort"] >> ChildBindNamePort;
			}
			void operator >> (CStream& Stream_) const override
			{
				SServer::operator >> (Stream_);
				Stream_ << SessionCode;
				Stream_ << ChildBindNamePort;
			}
			void operator >> (Value& Value_) const override
			{
				SServer::operator >> (Value_);
				Value_["SessionCode"] = SessionCode;
				Value_["ChildBindNamePort"] = ChildBindNamePort;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SServer()) + L"," + 
					GetStdName(TSessionCode()) + L"," + 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SServer(), L"") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode") + L"," + 
					GetMemberName(SNamePort(), L"ChildBindNamePort");
			}
		};
		struct SUdNewChild : public SProto
		{
			SNamePort ClientBindNamePortPub{};
			TSessionCode SessionCode{};
			SKey NewChildKey{};
			SUdNewChild()
			{
			}
			SUdNewChild(const SNamePort& ClientBindNamePortPub_, const TSessionCode& SessionCode_, const SKey& NewChildKey_) : ClientBindNamePortPub(ClientBindNamePortPub_), SessionCode(SessionCode_), NewChildKey(NewChildKey_)
			{
			}
			SUdNewChild(SNamePort&& ClientBindNamePortPub_, TSessionCode&& SessionCode_, SKey&& NewChildKey_) : ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), SessionCode(std::move(SessionCode_)), NewChildKey(std::move(NewChildKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientBindNamePortPub;
				Stream_ >> SessionCode;
				Stream_ >> NewChildKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
				Value_["SessionCode"] >> SessionCode;
				Value_["NewChildKey"] >> NewChildKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientBindNamePortPub;
				Stream_ << SessionCode;
				Stream_ << NewChildKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
				Value_["SessionCode"] = SessionCode;
				Value_["NewChildKey"] = NewChildKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(TSessionCode()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode") + L"," + 
					GetMemberName(SKey(), L"NewChildKey");
			}
		};
		struct SDuNewChild : public SProto
		{
			SKey NewChildKey{};
			TSessionCode SessionCode{};
			SDuNewChild()
			{
			}
			SDuNewChild(const SKey& NewChildKey_, const TSessionCode& SessionCode_) : NewChildKey(NewChildKey_), SessionCode(SessionCode_)
			{
			}
			SDuNewChild(SKey&& NewChildKey_, TSessionCode&& SessionCode_) : NewChildKey(std::move(NewChildKey_)), SessionCode(std::move(SessionCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> NewChildKey;
				Stream_ >> SessionCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["NewChildKey"] >> NewChildKey;
				Value_["SessionCode"] >> SessionCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << NewChildKey;
				Stream_ << SessionCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["NewChildKey"] = NewChildKey;
				Value_["SessionCode"] = SessionCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(TSessionCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"NewChildKey") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode");
			}
		};
		struct SDuNewChildFail : public SProto
		{
			SKey NewChildKey{};
			SDuNewChildFail()
			{
			}
			SDuNewChildFail(const SKey& NewChildKey_) : NewChildKey(NewChildKey_)
			{
			}
			SDuNewChildFail(SKey&& NewChildKey_) : NewChildKey(std::move(NewChildKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> NewChildKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["NewChildKey"] >> NewChildKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << NewChildKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["NewChildKey"] = NewChildKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"NewChildKey");
			}
		};
		struct SUdNewParent : public SProto
		{
			SNamePort ChildBindNamePort{};
			TSessionCode SessionCode{};
			SUdNewParent()
			{
			}
			SUdNewParent(const SNamePort& ChildBindNamePort_, const TSessionCode& SessionCode_) : ChildBindNamePort(ChildBindNamePort_), SessionCode(SessionCode_)
			{
			}
			SUdNewParent(SNamePort&& ChildBindNamePort_, TSessionCode&& SessionCode_) : ChildBindNamePort(std::move(ChildBindNamePort_)), SessionCode(std::move(SessionCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ChildBindNamePort;
				Stream_ >> SessionCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ChildBindNamePort"] >> ChildBindNamePort;
				Value_["SessionCode"] >> SessionCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ChildBindNamePort;
				Stream_ << SessionCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ChildBindNamePort"] = ChildBindNamePort;
				Value_["SessionCode"] = SessionCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(TSessionCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ChildBindNamePort") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode");
			}
		};
		struct SCsConnect : public SProto
		{
			SNamePort PrevClientBindNamePortPub{};
			SCsConnect()
			{
			}
			SCsConnect(const SNamePort& PrevClientBindNamePortPub_) : PrevClientBindNamePortPub(PrevClientBindNamePortPub_)
			{
			}
			SCsConnect(SNamePort&& PrevClientBindNamePortPub_) : PrevClientBindNamePortPub(std::move(PrevClientBindNamePortPub_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> PrevClientBindNamePortPub;
			}
			void operator << (const Value& Value_) override
			{
				Value_["PrevClientBindNamePortPub"] >> PrevClientBindNamePortPub;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << PrevClientBindNamePortPub;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["PrevClientBindNamePortPub"] = PrevClientBindNamePortPub;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"PrevClientBindNamePortPub");
			}
		};
		struct SScNewParent : public SProto
		{
			SNamePort ClientBindNamePortPub{};
			SScNewParent()
			{
			}
			SScNewParent(const SNamePort& ClientBindNamePortPub_) : ClientBindNamePortPub(ClientBindNamePortPub_)
			{
			}
			SScNewParent(SNamePort&& ClientBindNamePortPub_) : ClientBindNamePortPub(std::move(ClientBindNamePortPub_))
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
	}
}
