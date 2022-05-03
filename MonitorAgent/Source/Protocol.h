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
	SNamePort MonitorServerNamePort{};
	wstring WorkingRelativePath{};
	wstring MonitorProcFile{};
	TPort MonitorProcPort{};
	SOption()
	{
	}
	SOption(const wstring& LogDir_, const wstring& ServiceName_, const SNamePort& MonitorServerNamePort_, const wstring& WorkingRelativePath_, const wstring& MonitorProcFile_, const TPort& MonitorProcPort_) : LogDir(LogDir_), ServiceName(ServiceName_), MonitorServerNamePort(MonitorServerNamePort_), WorkingRelativePath(WorkingRelativePath_), MonitorProcFile(MonitorProcFile_), MonitorProcPort(MonitorProcPort_)
	{
	}
	SOption(wstring&& LogDir_, wstring&& ServiceName_, SNamePort&& MonitorServerNamePort_, wstring&& WorkingRelativePath_, wstring&& MonitorProcFile_, TPort&& MonitorProcPort_) : LogDir(std::move(LogDir_)), ServiceName(std::move(ServiceName_)), MonitorServerNamePort(std::move(MonitorServerNamePort_)), WorkingRelativePath(std::move(WorkingRelativePath_)), MonitorProcFile(std::move(MonitorProcFile_)), MonitorProcPort(std::move(MonitorProcPort_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> LogDir;
		Stream_ >> ServiceName;
		Stream_ >> MonitorServerNamePort;
		Stream_ >> WorkingRelativePath;
		Stream_ >> MonitorProcFile;
		Stream_ >> MonitorProcPort;
	}
	void operator << (const Value& Value_) override
	{
		Value_["LogDir"] >> LogDir;
		Value_["ServiceName"] >> ServiceName;
		Value_["MonitorServerNamePort"] >> MonitorServerNamePort;
		Value_["WorkingRelativePath"] >> WorkingRelativePath;
		Value_["MonitorProcFile"] >> MonitorProcFile;
		Value_["MonitorProcPort"] >> MonitorProcPort;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << LogDir;
		Stream_ << ServiceName;
		Stream_ << MonitorServerNamePort;
		Stream_ << WorkingRelativePath;
		Stream_ << MonitorProcFile;
		Stream_ << MonitorProcPort;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["LogDir"] = LogDir;
		Value_["ServiceName"] = ServiceName;
		Value_["MonitorServerNamePort"] = MonitorServerNamePort;
		Value_["WorkingRelativePath"] = WorkingRelativePath;
		Value_["MonitorProcFile"] = MonitorProcFile;
		Value_["MonitorProcPort"] = MonitorProcPort;
	}
	static wstring StdName(void)
	{
		return 
			GetStdName(wstring()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(SNamePort()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(wstring()) + L"," + 
			GetStdName(TPort());
	}
	static wstring MemberName(void)
	{
		return 
			GetMemberName(wstring(), L"LogDir") + L"," + 
			GetMemberName(wstring(), L"ServiceName") + L"," + 
			GetMemberName(SNamePort(), L"MonitorServerNamePort") + L"," + 
			GetMemberName(wstring(), L"WorkingRelativePath") + L"," + 
			GetMemberName(wstring(), L"MonitorProcFile") + L"," + 
			GetMemberName(TPort(), L"MonitorProcPort");
	}
};
