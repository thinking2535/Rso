#pragma once

#include "Base.h"
#include "Http.h"

namespace rso::http
{
	using namespace std;

	class CIPFinder
	{
		bool _Returned = false;
		string _IP;
		CHttp<> _Http{ 1, std::bind(&CIPFinder::_Change, this, _1, _2, _3, _4), std::bind(&CIPFinder::_Download, this, _1, _2, _3, _4) };

		void _Change(size_t SessionIndex_, const string& ObjectName_, DWORD Received_, DWORD Total_);
		void _Download(size_t SessionIndex_, EHttpRet Ret_, const string& ObjectName_, const TBuffer& Buffer_);

	public:
		string GetMyIP(const string& ServerURL_);
	};
}
