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
using namespace rso;
using namespace net;
namespace rso
{
	namespace monitor
	{
		struct SAccount : public SProto
		{
			wstring ID{};
			wstring PW{};
			SAccount()
			{
			}
			SAccount(const wstring& ID_, const wstring& PW_) : ID(ID_), PW(PW_)
			{
			}
			SAccount(wstring&& ID_, wstring&& PW_) : ID(std::move(ID_)), PW(std::move(PW_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ID;
				Stream_ >> PW;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ID"] >> ID;
				Value_["PW"] >> PW;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ID;
				Stream_ << PW;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ID"] = ID;
				Value_["PW"] = PW;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"ID") + L"," + 
					GetMemberName(wstring(), L"PW");
			}
		};
		struct SOption : public SProto
		{
			string Title{};
			vector<SAccount> Accounts{};
			SNamePort AgentBindNamePort{};
			SNamePort ClientBindNamePort{};
			SOption()
			{
			}
			SOption(const string& Title_, const vector<SAccount>& Accounts_, const SNamePort& AgentBindNamePort_, const SNamePort& ClientBindNamePort_) : Title(Title_), Accounts(Accounts_), AgentBindNamePort(AgentBindNamePort_), ClientBindNamePort(ClientBindNamePort_)
			{
			}
			SOption(string&& Title_, vector<SAccount>&& Accounts_, SNamePort&& AgentBindNamePort_, SNamePort&& ClientBindNamePort_) : Title(std::move(Title_)), Accounts(std::move(Accounts_)), AgentBindNamePort(std::move(AgentBindNamePort_)), ClientBindNamePort(std::move(ClientBindNamePort_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Title;
				Stream_ >> Accounts;
				Stream_ >> AgentBindNamePort;
				Stream_ >> ClientBindNamePort;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Title"] >> Title;
				Value_["Accounts"] >> Accounts;
				Value_["AgentBindNamePort"] >> AgentBindNamePort;
				Value_["ClientBindNamePort"] >> ClientBindNamePort;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Title;
				Stream_ << Accounts;
				Stream_ << AgentBindNamePort;
				Stream_ << ClientBindNamePort;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Title"] = Title;
				Value_["Accounts"] = Accounts;
				Value_["AgentBindNamePort"] = AgentBindNamePort;
				Value_["ClientBindNamePort"] = ClientBindNamePort;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(string()) + L"," + 
					GetStdName(vector<SAccount>()) + L"," + 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(string(), L"Title") + L"," + 
					GetMemberName(vector<SAccount>(), L"Accounts") + L"," + 
					GetMemberName(SNamePort(), L"AgentBindNamePort") + L"," + 
					GetMemberName(SNamePort(), L"ClientBindNamePort");
			}
		};
	}
}
