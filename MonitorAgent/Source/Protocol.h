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
	wstring LogDir{};
	wstring ServiceName{};
	SNamePort ServerNamePort{};
	wstring WorkingRelativePath{};
	wstring AppFile{};
	SNamePort AppNamePort{};
	SOption()
	{
	}
	SOption(const wstring& LogDir_, const wstring& ServiceName_, const SNamePort& ServerNamePort_, const wstring& WorkingRelativePath_, const wstring& AppFile_, const SNamePort& AppNamePort_) : LogDir(LogDir_), ServiceName(ServiceName_), ServerNamePort(ServerNamePort_), WorkingRelativePath(WorkingRelativePath_), AppFile(AppFile_), AppNamePort(AppNamePort_)
	{
	}
	SOption(wstring&& LogDir_, wstring&& ServiceName_, SNamePort&& ServerNamePort_, wstring&& WorkingRelativePath_, wstring&& AppFile_, SNamePort&& AppNamePort_) : LogDir(std::move(LogDir_)), ServiceName(std::move(ServiceName_)), ServerNamePort(std::move(ServerNamePort_)), WorkingRelativePath(std::move(WorkingRelativePath_)), AppFile(std::move(AppFile_)), AppNamePort(std::move(AppNamePort_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> LogDir;
		Stream_ >> ServiceName;
		Stream_ >> ServerNamePort;
		Stream_ >> WorkingRelativePath;
		Stream_ >> AppFile;
		Stream_ >> AppNamePort;
	}
	void operator << (const Value& Value_) override
	{
		Value_["LogDir"] >> LogDir;
		Value_["ServiceName"] >> ServiceName;
		Value_["ServerNamePort"] >> ServerNamePort;
		Value_["WorkingRelativePath"] >> WorkingRelativePath;
		Value_["AppFile"] >> AppFile;
		Value_["AppNamePort"] >> AppNamePort;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << LogDir;
		Stream_ << ServiceName;
		Stream_ << ServerNamePort;
		Stream_ << WorkingRelativePath;
		Stream_ << AppFile;
		Stream_ << AppNamePort;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["LogDir"] = LogDir;
		Value_["ServiceName"] = ServiceName;
		Value_["ServerNamePort"] = ServerNamePort;
		Value_["WorkingRelativePath"] = WorkingRelativePath;
		Value_["AppFile"] = AppFile;
		Value_["AppNamePort"] = AppNamePort;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(wstring()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(SNamePort());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(wstring(), L"LogDir") + L"," + 
			GetMemberName(wstring(), L"ServiceName") + L"," + 
			GetMemberName(SNamePort(), L"ServerNamePort") + L"," + 
			GetMemberName(wstring(), L"WorkingRelativePath") + L"," + 
			GetMemberName(wstring(), L"AppFile") + L"," + 
			GetMemberName(SNamePort(), L"AppNamePort");
	}
};
