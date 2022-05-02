#include "Core.h"
#include <ctime>

namespace rso::core
{
	system_clock::time_point steady_now(void)
	{
		static system_clock::time_point _Cache = system_clock::now();

		auto Now = system_clock::now();

		if (Now > _Cache)
			_Cache = Now;

		return _Cache;
	}

	bool IsLeapYear(int16 Year_)
	{
		if (Year_ % 4 == 0)
		{
			if (Year_ % 100 == 0)
			{
				if (Year_ % 400 == 0)
					return true;

				return false;
			}
			return true;
		}
		return false;
	}
	uint16 LastDayOfMonth(int16 Year_, uint16 Month_)
	{
		if (Month_ == 1 ||
			Month_ == 3 ||
			Month_ == 5 ||
			Month_ == 7 ||
			Month_ == 8 ||
			Month_ == 10 ||
			Month_ == 12)
			return 31;
		else if (Month_ == 4 ||
			Month_ == 6 ||
			Month_ == 9 ||
			Month_ == 11)
			return 30;
		else if (IsLeapYear(Year_))
			return 29;
		else
			return 28;
	}
	TCoreCheckSum GetCheckSum(const void* pData_, int64 TotSize_)
	{
		if (TotSize_ <= 0)
			return 0;

		auto* pData = (TCoreCheckSum*)pData_;

		TCoreCheckSum Sum = 0;
		int64 CheckCnt = TotSize_ / 8;
		int64 LeftByte = TotSize_ % 8;

		for (int64 Cnt = 0; Cnt < CheckCnt; ++Cnt)
		{
			Sum ^= pData[Cnt];
		}

		auto* pcData = (unsigned char*)&pData[CheckCnt];
		int64 LeftNum = 0;
		for (int64 Left = 0; Left < LeftByte; ++Left)
		{
			LeftNum |= ((uint64)pcData[Left]) << (Left << 3);
		}

		return Sum ^ LeftNum;
	}
	TCoreCheckSum GetFileCheckSum(ifstream& File_)
	{
		// Target File /////////////////////////////////

		File_.seekg(0, ios::beg);
		std::streampos begin = File_.tellg();
		File_.seekg(0, ios::end);
		std::streamoff Size = (File_.tellg() - begin);
		File_.seekg(0, ios::beg);

		auto* pTargetFileBuf = new char[(uint32)Size];
		if (pTargetFileBuf == 0)
			THROWEX();

		File_.read(pTargetFileBuf, Size);
		TCoreCheckSum TargetCheckSum = core::GetCheckSum(pTargetFileBuf, Size);
		delete[] pTargetFileBuf;
		pTargetFileBuf = 0;

		return TargetCheckSum;
	}
	TCoreCheckSum GetFileCheckSum(const wstring& FileName_)
	{
		ifstream f(FileName_, ios::binary);
		if (f.is_open() == false)
			THROWEX();

		return GetFileCheckSum(f);
	}

	//void ReplaceAll( string& Source_, char From_, const string& To_ )
	//{
	// for( auto FoundIndex = Source_.find(From_);
	// FoundIndex != string::npos;
	// FoundIndex = Source_.find(From_) )
	// {
	// Source_.replace( FoundIndex, 1, To_);
	// }
	//}

	//void ReplaceAll( wstring& Source_, wchar_t From_, const wstring& To_ )
	//{
	// for( auto FoundIndex = Source_.find(From_);
	// FoundIndex != wstring::npos;
	// FoundIndex = Source_.find(From_) )
	// {
	// Source_.replace( FoundIndex, 1, To_ );
	// }
	//}

	string WCSToMBS(const wstring& Src_, ELocale Locale_)
	{
		string OldLocale;
		if (Locale_ != ELocale::Null)
		{
			OldLocale.assign(setlocale(LC_ALL, nullptr));
			setlocale(LC_ALL, GetLocaleString(Locale_));
		}

		vector<char> vDest;
		vDest.resize(Src_.size() * 2 + 1);

		std::size_t Size = 0;
		wcstombs_s(&Size, vDest.data(), vDest.size(), Src_.c_str(), vDest.size());

		if (Locale_ != ELocale::Null)
			setlocale(LC_ALL, OldLocale.c_str());

		return vDest.data();
	}
	wstring MBSToWCS(const string& Src_, ELocale Locale_)
	{
		string OldLocale;
		if (Locale_ != ELocale::Null)
		{
			OldLocale.assign(setlocale(LC_ALL, nullptr));
			setlocale(LC_ALL, GetLocaleString(Locale_));
		}

		vector<wchar_t> vDest;
		vDest.resize(Src_.size() + 1);

		std::size_t Size = 0;
		mbstowcs_s(&Size, vDest.data(), vDest.size(), Src_.c_str(), vDest.size());

		if (Locale_ != ELocale::Null)
			setlocale(LC_ALL, OldLocale.c_str());

		return vDest.data();
	}
	wstring GetSlashPath(const wstring& Path_)
	{
		wstring Path(Path_);

		for (auto& c : Path)
		{
			if (c == L'\\')
				c = L'/';
		}

		return Path;
	}
	wstring GetBackSlashPath(const wstring& Path_)
	{
		wstring Path(Path_);

		for (auto& c : Path)
		{
			if (c == L'/')
				c = L'\\';
		}

		return Path;
	}
	int32 GetWeekOfDay(const TTime& Time_)
	{
		auto TimeT = system_clock::to_time_t(Time_);
		tm Tm;
		localtime_s(&Tm, &TimeT);
		return Tm.tm_wday;
	}
	int32 GetWeek(const TTime& Time_)
	{
		auto TimeT = system_clock::to_time_t(Time_);
		tm Tm;
		localtime_s(&Tm, &TimeT);
		return int32((Tm.tm_mday - 1 + Tm.tm_wday) / 7);
	}
	int32 GetFullWeek(const TTime& Time_)
	{
		auto TimeT = system_clock::to_time_t(Time_);
		tm Tm;
		localtime_s(&Tm, &TimeT);
		return ((Tm.tm_mday - 1) / 7);
	}
	int64 GetTimePart(const TTime& Time_)
	{
		return (Time_.time_since_epoch().count() % 864000000000);
	}
	int64 GetTimePart(int64 Hours_, int64 Minutes_, int64 Seconds_)
	{
		Hours_ *= 60;
		Hours_ += Minutes_;
		Hours_ *= 60;
		Hours_ += Seconds_;
		Hours_ *= 10000000;
		return Hours_;
	}
	std::wstring u16string_to_wstring(const std::u16string& Str_)
	{
		std::wstring_convert< std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> conv;

		return conv.from_bytes(
			reinterpret_cast<const char*> (&Str_[0]),
			reinterpret_cast<const char*> (&Str_[0] + Str_.size())
		);
	}
	bool compare_files(const std::string& file0_, const std::string& file1_)
	{
		std::ifstream fs0(file0_, std::ifstream::binary | std::ifstream::ate);
		std::ifstream fs1(file1_, std::ifstream::binary | std::ifstream::ate);

		if (fs0.fail() || fs1.fail())
			return false;

		if (fs0.tellg() != fs1.tellg())
			return false;

		fs0.seekg(0, std::ifstream::beg);
		fs1.seekg(0, std::ifstream::beg);

		return std::equal(std::istreambuf_iterator<char>(fs0.rdbuf()),
			std::istreambuf_iterator<char>(),
			std::istreambuf_iterator<char>(fs1.rdbuf()));
	}
}
