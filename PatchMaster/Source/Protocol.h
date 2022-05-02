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
using namespace std;
using namespace rso;
using namespace net;
struct SIDPW : public SProto
{
	wstring ID{};
	wstring PW{};
	SIDPW()
	{
	}
	SIDPW(const wstring& ID_, const wstring& PW_) : ID(ID_), PW(PW_)
	{
	}
	SIDPW(wstring&& ID_, wstring&& PW_) : ID(std::move(ID_)), PW(std::move(PW_))
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
	wstring Title{};
	wstring DataFile{};
	TPeerCnt ChildCntMax{};
	SNamePort ChildBindNamePort{};
	SNamePort ClientBindNamePort{};
	SNamePort ClientBindNamePortPub{};
	SNamePort AgentBindNamePort{};
	list<SIDPW> Accounts{};
	SOption()
	{
	}
	SOption(const wstring& Title_, const wstring& DataFile_, const TPeerCnt& ChildCntMax_, const SNamePort& ChildBindNamePort_, const SNamePort& ClientBindNamePort_, const SNamePort& ClientBindNamePortPub_, const SNamePort& AgentBindNamePort_, const list<SIDPW>& Accounts_) : Title(Title_), DataFile(DataFile_), ChildCntMax(ChildCntMax_), ChildBindNamePort(ChildBindNamePort_), ClientBindNamePort(ClientBindNamePort_), ClientBindNamePortPub(ClientBindNamePortPub_), AgentBindNamePort(AgentBindNamePort_), Accounts(Accounts_)
	{
	}
	SOption(wstring&& Title_, wstring&& DataFile_, TPeerCnt&& ChildCntMax_, SNamePort&& ChildBindNamePort_, SNamePort&& ClientBindNamePort_, SNamePort&& ClientBindNamePortPub_, SNamePort&& AgentBindNamePort_, list<SIDPW>&& Accounts_) : Title(std::move(Title_)), DataFile(std::move(DataFile_)), ChildCntMax(std::move(ChildCntMax_)), ChildBindNamePort(std::move(ChildBindNamePort_)), ClientBindNamePort(std::move(ClientBindNamePort_)), ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), AgentBindNamePort(std::move(AgentBindNamePort_)), Accounts(std::move(Accounts_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Title;
		Stream_ >> DataFile;
		Stream_ >> ChildCntMax;
		Stream_ >> ChildBindNamePort;
		Stream_ >> ClientBindNamePort;
		Stream_ >> ClientBindNamePortPub;
		Stream_ >> AgentBindNamePort;
		Stream_ >> Accounts;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Title"] >> Title;
		Value_["DataFile"] >> DataFile;
		Value_["ChildCntMax"] >> ChildCntMax;
		Value_["ChildBindNamePort"] >> ChildBindNamePort;
		Value_["ClientBindNamePort"] >> ClientBindNamePort;
		Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
		Value_["AgentBindNamePort"] >> AgentBindNamePort;
		Value_["Accounts"] >> Accounts;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Title;
		Stream_ << DataFile;
		Stream_ << ChildCntMax;
		Stream_ << ChildBindNamePort;
		Stream_ << ClientBindNamePort;
		Stream_ << ClientBindNamePortPub;
		Stream_ << AgentBindNamePort;
		Stream_ << Accounts;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Title"] = Title;
		Value_["DataFile"] = DataFile;
		Value_["ChildCntMax"] = ChildCntMax;
		Value_["ChildBindNamePort"] = ChildBindNamePort;
		Value_["ClientBindNamePort"] = ClientBindNamePort;
		Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
		Value_["AgentBindNamePort"] = AgentBindNamePort;
		Value_["Accounts"] = Accounts;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(wstring()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(TPeerCnt()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(list<SIDPW>());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(wstring(), L"Title") + L"," + 
			GetMemberName(wstring(), L"DataFile") + L"," + 
			GetMemberName(TPeerCnt(), L"ChildCntMax") + L"," + 
			GetMemberName(SNamePort(), L"ChildBindNamePort") + L"," + 
			GetMemberName(SNamePort(), L"ClientBindNamePort") + L"," + 
			GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
			GetMemberName(SNamePort(), L"AgentBindNamePort") + L"," + 
			GetMemberName(list<SIDPW>(), L"Accounts");
	}
};
