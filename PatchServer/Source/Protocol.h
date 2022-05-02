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
struct SOption : public SProto
{
	wstring Title{};
	TPeerCnt ChildCntMax{};
	SNamePort MasterNamePort{};
	SNamePort ChildBindNamePort{};
	SNamePort ClientBindNamePort{};
	SNamePort ClientBindNamePortPub{};
	SOption()
	{
	}
	SOption(const wstring& Title_, const TPeerCnt& ChildCntMax_, const SNamePort& MasterNamePort_, const SNamePort& ChildBindNamePort_, const SNamePort& ClientBindNamePort_, const SNamePort& ClientBindNamePortPub_) : Title(Title_), ChildCntMax(ChildCntMax_), MasterNamePort(MasterNamePort_), ChildBindNamePort(ChildBindNamePort_), ClientBindNamePort(ClientBindNamePort_), ClientBindNamePortPub(ClientBindNamePortPub_)
	{
	}
	SOption(wstring&& Title_, TPeerCnt&& ChildCntMax_, SNamePort&& MasterNamePort_, SNamePort&& ChildBindNamePort_, SNamePort&& ClientBindNamePort_, SNamePort&& ClientBindNamePortPub_) : Title(std::move(Title_)), ChildCntMax(std::move(ChildCntMax_)), MasterNamePort(std::move(MasterNamePort_)), ChildBindNamePort(std::move(ChildBindNamePort_)), ClientBindNamePort(std::move(ClientBindNamePort_)), ClientBindNamePortPub(std::move(ClientBindNamePortPub_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> Title;
		Stream_ >> ChildCntMax;
		Stream_ >> MasterNamePort;
		Stream_ >> ChildBindNamePort;
		Stream_ >> ClientBindNamePort;
		Stream_ >> ClientBindNamePortPub;
	}
	void operator << (const Value& Value_) override
	{
		Value_["Title"] >> Title;
		Value_["ChildCntMax"] >> ChildCntMax;
		Value_["MasterNamePort"] >> MasterNamePort;
		Value_["ChildBindNamePort"] >> ChildBindNamePort;
		Value_["ClientBindNamePort"] >> ClientBindNamePort;
		Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << Title;
		Stream_ << ChildCntMax;
		Stream_ << MasterNamePort;
		Stream_ << ChildBindNamePort;
		Stream_ << ClientBindNamePort;
		Stream_ << ClientBindNamePortPub;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["Title"] = Title;
		Value_["ChildCntMax"] = ChildCntMax;
		Value_["MasterNamePort"] = MasterNamePort;
		Value_["ChildBindNamePort"] = ChildBindNamePort;
		Value_["ClientBindNamePort"] = ClientBindNamePort;
		Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(wstring()) + L"," + 
			GetStdName(TPeerCnt()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(SNamePort());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(wstring(), L"Title") + L"," + 
			GetMemberName(TPeerCnt(), L"ChildCntMax") + L"," + 
			GetMemberName(SNamePort(), L"MasterNamePort") + L"," + 
			GetMemberName(SNamePort(), L"ChildBindNamePort") + L"," + 
			GetMemberName(SNamePort(), L"ClientBindNamePort") + L"," + 
			GetMemberName(SNamePort(), L"ClientBindNamePortPub");
	}
};
