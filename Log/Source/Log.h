#pragma once

#include <Rso/Core/Base.h>

#pragma comment( lib, "Rso/Log_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <filesystem>
#include <Rso/Win/Lock.h>
#include <Rso/Core/DateTime.h>
#include <fstream>

namespace rso
{
	namespace log
	{
		using namespace std;
		using namespace filesystem;
		using namespace core;
		using namespace win;

		enum class EPeriod
		{
			Second,
			Minute,
			Hour,
			Day,
			Month,
			Year,
			Permanent,	// 한 파일에 영구적으로
			EveryRun,	// 매번 실행시마다
			Max
		};

		enum ETarget	// 1, 2, 4, 8
		{
			Target_Console = 1,
			Target_File = 2
		};

		class CLog
		{
		private:
			using _TTarget = int32;

			_TTarget _Target = 0;
			wstring _Path;
			wstring _FileName;
			wstring _TimeFormat;
			SDateTime _FileTime;		// 파일 생성 할지 확인 하기 위함
			EPeriod _Period;
			wfstream _FS;
			CLock _Lock;

			bool _NeedToChangeFile(const SDateTime& Time_) const;

		public:
			CLog(_TTarget Target_, EPeriod Period_, const wstring& LogPath_, const wstring& LogFileName_, ELocale Locale_ = ELocale::English);
			void Push(const wstring& Msg_);
			void Push(const string& Msg_);
		};
	}
}
