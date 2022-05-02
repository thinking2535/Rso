#pragma once

#include "Base.h"
#include <Rso/Core/Stream.h>

namespace rso
{
	namespace net
	{
		using namespace std;

		class CIPInfo
		{
			using _TRangeIP = SRangeKey<TLongIP>;

			const string _NullCode;
			map<_TRangeIP, string> _IPs;

		public:
			CIPInfo(const wstring& IPFile_);
			const string& GetCountryCode(const string& IP_) const;
			const string& GetCountryCode(TLongIP LongIP_) const;
			static void CSVToBin(const wstring& CSVFileName_, const wstring& BinFileName_); // 한국인터넷정보센터 ipv4.csv
		};
	}
}