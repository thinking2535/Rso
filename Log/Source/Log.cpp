#include "Log.h"
#include <string>
#include <iostream>

namespace rso
{
	namespace log
	{
		bool CLog::_NeedToChangeFile(const SDateTime& Time_) const
		{
			switch (_Period)
			{
			case EPeriod::Second:
				if (Time_.second != _FileTime.second) return true;
			case EPeriod::Minute:
				if (Time_.minute != _FileTime.minute) return true;
			case EPeriod::Hour:
				if (Time_.hour != _FileTime.hour) return true;
			case EPeriod::Day:
				if (Time_.day != _FileTime.day) return true;
			case EPeriod::Month:
				if (Time_.month != _FileTime.month) return true;
			case EPeriod::Year:
				if (Time_.year != _FileTime.year) return true;
				return false;
			default:
				return false;
			}
		}
		CLog::CLog(_TTarget Target_, EPeriod Period_, const wstring& LogPath_, const wstring& LogFileName_, ELocale Locale_) :
			_Target(Target_),
			_Path(absolute(LogPath_).wstring()),
			_FileName(LogFileName_)
		{
			switch (Period_)
			{
			case EPeriod::Second:
				_TimeFormat = L"%Y%m%d_%H%M%S";
				break;
			case EPeriod::Minute:
				_TimeFormat = L"%Y%m%d_%H%M";
				break;
			case EPeriod::Hour:
				_TimeFormat = L"%Y%m%d_%H";
				break;
			case EPeriod::Day:
				_TimeFormat = L"%Y%m%d";
				break;
			case EPeriod::Month:
				_TimeFormat = L"%Y%m";
				break;
			case EPeriod::Year:
				_TimeFormat = L"%Y";
				break;
			case EPeriod::Permanent:
				_TimeFormat = L"";
				break;
			case EPeriod::EveryRun:
				_TimeFormat = L"%Y%m%d_%H%M%S";
				break;
			default:
				_TimeFormat = L"%Y%m%d_%H%M%S";
				break;
			}

			create_directories(_Path);

			wcout.imbue(std::locale(GetLocaleString(Locale_)));
			_FS.imbue(std::locale("korean"));

			_Period = Period_;
		}
		void CLog::Push(const wstring& Msg_)
		{
			CDateTime Now(system_clock::now());
			wstring Msg(Now.ToWString(L"%Y%m%d_%H%M%S"));
			Msg += L" ";
			Msg += Msg_;
			Msg += L"\n";

			_Lock.Lock();

			if (_Target & Target_Console)
			{
				wcout << Msg << flush;
			}
			if (_Target & Target_File)
			{
				if (_NeedToChangeFile(Now))
				{
					_FileTime = Now;
					_FS.close();
				}

				if (!_FS.is_open())
				{
					path FilePath(_Path);

					FilePath.append(_FileName);
					if (_Period != EPeriod::Permanent)
					{
						FilePath += L"_";
						FilePath += Now.ToWString(_TimeFormat.c_str());
					}
					FilePath += L".Log";

					_FS.open(FilePath, ios::out | ios::app);
				}

				if (_FS.is_open())
				{
					_FS << Msg;
					_FS.flush();
				}
			}

			_Lock.UnLock();
		}
		void CLog::Push(const string& Msg_)
		{
			Push(MBSToWCS(Msg_));
		}
	}
}
