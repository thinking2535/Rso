#include "IPInfo.h"

namespace rso::net
{
	CIPInfo::CIPInfo(const wstring& IPFile_)
	{
		CStream Stream(IPFile_);

		int32 Size = 0;
		TLongIP Start = 0;
		TLongIP End = 0;
		SCountryCodeMinuteCountOffset CountryCodeMinuteCountOffset;

		Stream >> Size;

		for (int32 i = 0; i < Size; ++i)
		{
			Stream >> Start;
			Stream >> End;
			Stream >> CountryCodeMinuteCountOffset;

			_IPs.emplace(_TRangeIP(Start, End + 1), SCountryCodeMinuteOffset(CountryCodeMinuteCountOffset.CountryCode, minutes(CountryCodeMinuteCountOffset.MinuteCountOffset)));
		}
	}
	const CIPInfo::SCountryCodeMinuteOffset& CIPInfo::Get(const string& IP_) const
	{
		return Get(inet_addr(IP_.c_str()));
	}
	const CIPInfo::SCountryCodeMinuteOffset& CIPInfo::Get(TLongIP LongIP_) const
	{
		auto it = _IPs.find(_TRangeIP(_byteswap_ulong(LongIP_)));
		if (it == _IPs.end())
			return _Null;

		return it->second;
	}
	void CIPInfo::CSVToBin_IP2LocationLite(const wstring& CSVFileName_, const wstring& BinFileName_)
	{
		ifstream CSVFile(CSVFileName_);

		string Line;
		std::getline(CSVFile, Line); // Title line 은 버림

		CStream Stream;

		int32 RowCount = 0;
		Stream << RowCount;

		while (std::getline(CSVFile, Line))
		{
			size_t IndexBegin = 0;
			size_t IndexEnd = 0;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // "
			IndexBegin = IndexEnd + 1;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 시작IP
			string BeginIP = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 끝IP
			string EndIP = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 국가코드
			string CountryCode = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;
			if (CountryCode == "-")
				continue;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 국가
			string Country = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 지역0
			string Location0 = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 지역1
			string Location1 = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 위도
			string Latitude = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // 경도
			string Longitude = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // ZipCode
			string ZipCode = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;

			IndexEnd = Line.find_first_of('\"', IndexBegin); // TimeZone
			string TimeZone = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 3;
			if (TimeZone == "-")
				continue;

			if (TimeZone.size() != 6)
			{
//				cout << "Invalid TimeZone : " << TimeZone << endl;
				continue;
			}

			auto Hours = stoi(TimeZone.substr(1, 2));
			auto Minutes = stoi(TimeZone.substr(4, 2));
			minutes MinuteOffset(Hours * 60 + Minutes);
			if (TimeZone[0] == '-')
				MinuteOffset *= -1;

			Stream << TLongIP(stoul(BeginIP));
			Stream << TLongIP(stoul(EndIP));

			//Stream << Country;
			//Stream << Location0;
			//Stream << Location1;
			//Stream << stof(Latitude);
			//Stream << stof(Longitude);
			//Stream << ZipCode;

			Stream << SCountryCodeMinuteCountOffset(CountryCode, MinuteOffset.count());

			++RowCount;
		}

		CSVFile.close();
		*((int32*)Stream.at(0)) = (int32)RowCount;
		Stream.SaveFile(BinFileName_);
	}
	void CIPInfo::CSVToBin_KRNIC(const wstring& CSVFileName_, const wstring& BinFileName_)
	{
		ifstream CSVFile(CSVFileName_);

		string Line;
		CSVFile >> Line; // Title line 은 버림

		CStream Stream; // 기준일자, 국가코드, 시작IP, 끝IP, PREFIX, 할당일자

		int32 RowCount = 0;
		Stream << RowCount;

		while (CSVFile >> Line)
		{
			size_t IndexBegin = 0;
			size_t IndexEnd = 0;

			IndexEnd = Line.find_first_of(',', IndexBegin); // 기준일자
			IndexBegin = IndexEnd + 1;

			IndexEnd = Line.find_first_of(',', IndexBegin); // 국가코드
			string CountryCode = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 1;

			IndexEnd = Line.find_first_of(',', IndexBegin); // 시작IP
			string StartIP = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 1;

			IndexEnd = Line.find_first_of(',', IndexBegin); // 끝IP
			string EndIP = Line.substr(IndexBegin, IndexEnd - IndexBegin);
			IndexBegin = IndexEnd + 1;

			Stream << _byteswap_ulong(IPv4ToLongIP(StartIP));
			Stream << _byteswap_ulong(IPv4ToLongIP(EndIP));
			Stream << CountryCode;

			++RowCount;
		}

		CSVFile.close();
		*((int32*)Stream.at(0)) = (int32)RowCount;
		Stream.SaveFile(BinFileName_);
	}
}