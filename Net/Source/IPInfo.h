#pragma once

#include "Base.h"
#include <Rso/Core/Stream.h>

namespace rso::net
{
	using namespace std;

	class CIPInfo
	{
	public:
		struct SCountryCodeMinuteOffset
		{
			string CountryCode;
			minutes MinuteOffset{};

			SCountryCodeMinuteOffset()
			{
			}
			SCountryCodeMinuteOffset(const string& CountryCode_, const minutes& MinuteOffset_) :
				CountryCode(CountryCode_), MinuteOffset(MinuteOffset_)
			{
			}
			CDateTime GetLocalDateTime(const TTime& Now_) const
			{
				return CDateTime(Now_, MinuteOffset);
			}
		};

	private:
		using _TRangeIP = CRangeKey<TLongIP>;
		using _TIPs = map<_TRangeIP, SCountryCodeMinuteOffset>;
		using _TIPsIt = typename _TIPs::iterator;
		using _TIPsCIt = typename _TIPs::const_iterator;

		const SCountryCodeMinuteOffset _Null;
		_TIPs _IPs;

	public:
		CIPInfo(const wstring& IPFile_);
		const SCountryCodeMinuteOffset& Get(const string& IP_) const;
		const SCountryCodeMinuteOffset& Get(TLongIP LongIP_) const;
		inline _TIPsIt begin(void) noexcept { return _IPs.begin(); }
		inline _TIPsIt end(void) noexcept { return _IPs.end(); }
		inline _TIPsCIt begin(void) const noexcept { return _IPs.begin(); }
		inline _TIPsCIt end(void) const noexcept { return _IPs.end(); }
		static void CSVToBin_IP2LocationLite(const wstring& CSVFileName_, const wstring& BinFileName_); // IP2LocationLite
		static void CSVToBin_KRNIC(const wstring& CSVFileName_, const wstring& BinFileName_); // 한국인터넷정보센터 ipv4.csv
	};
}