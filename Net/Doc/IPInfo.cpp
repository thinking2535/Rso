#include "IPInfo.h"

namespace rso
{
	namespace net
	{
		CIPInfo::CIPInfo(const wstring& IPFile_)
		{
			CStream Stream(IPFile_);

			__int32 Size = 0;
			TLongIP Start = 0;
			TLongIP End = 0;
			string CountryCode;

			Stream >> Size;

			for (__int32 i = 0; i < Size; ++i)
			{
				Stream >> Start;
				Stream >> End;
				Stream >> CountryCode;

				_IPs.emplace(_TRangeIP(Start, End + 1), CountryCode);
			}
		}
		const string& CIPInfo::GetCountryCode(const string& IP_) const
		{
			return GetCountryCode(inet_addr(IP_.c_str()));
		}
		const string& CIPInfo::GetCountryCode(TLongIP LongIP_) const
		{
			auto it = _IPs.find(_TRangeIP(_byteswap_ulong(LongIP_)));
			if (it == _IPs.end())
				return _NullCode;

			return it->second;
		}
		void CIPInfo::CSVToBin(const wstring& CSVFileName_, const wstring& BinFileName_)
		{
			ifstream IPv4File(CSVFileName_);

			string Line;
			IPv4File >> Line; // Title line 은 버림

			CStream Stream; // 기준일자, 국가코드, 시작IP, 끝IP, PREFIX, 할당일자

			__int32 RowCount = 0;
			Stream << RowCount;

			while (IPv4File >> Line)
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

				Stream << _byteswap_ulong(StringToLongIP(StartIP));
				Stream << _byteswap_ulong(StringToLongIP(EndIP));
				Stream << CountryCode;

				++RowCount;
			}
			IPv4File.close();

			*((__int32*)Stream.at(0)) = (__int32)RowCount;

			Stream.SaveFile(BinFileName_);
		}
	}
}