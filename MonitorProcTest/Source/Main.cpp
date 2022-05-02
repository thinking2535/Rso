#include "stdafx.h"

#include <Rso/Monitor/Proc.h>
#include <Rso/Json/Json.h>
#include <Rso/Log/Log.h>
#include <iostream>
#include <string>
#include <map>
#include <chrono>

using namespace std;
using namespace rso;
using namespace monitor;
using namespace json;
using namespace log;
using namespace chrono;

unique_ptr<CProc> g_Proc;
CLog g_Log(Target_Console | Target_File, EPeriod::Day, L"Log/", L"Log", ELocale::Korean);

#define LOG(...) g_Log.Push(StringFormat(__VA_ARGS__))

void CallbackUserProto(const CKey& ClientKey_, CStream& Stream_)
{
}
void CallbackClose(const wstring& Message_, const milliseconds& DurationLeft_)
{
}
void CallbackMessage(const wstring& Message_)
{
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		wcout.imbue(std::locale("korean"));

		CJson Setup(CStream(L"Setup.ini"));

		g_Proc.reset(new CProc(
			EAddressFamily::INET,
			CallbackUserProto, CallbackClose, CallbackMessage,
			MBSToWCS(Setup["Name"].asString()), CNamePort(Setup["BindIP"].asString(), Setup["BindPort"].asInt()),wstring()));

		LOG(L"Started");

		while (true)
		{
			g_Proc->Proc();

			this_thread::sleep_for(milliseconds(1));
		}
	}
	catch (const exception& Exception_)
	{
		LOG(MBSToWCS(Exception_.what()).c_str());
	}

	return 0;
}
