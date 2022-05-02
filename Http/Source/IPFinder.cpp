#include "IPFinder.h"

namespace rso::http
{
	void CIPFinder::_Change(size_t /*SessionIndex_*/, const string& /*ObjectName_*/, DWORD /*Received_*/, DWORD /*Total_*/)
	{
	}
	void CIPFinder::_Download(size_t /*SessionIndex_*/, EHttpRet Ret_, const string& /*ObjectName_*/, const TBuffer& Buffer_)
	{
		if (Ret_ == EHttpRet::Ok)
			_IP = string(Buffer_.begin(), Buffer_.end());

		_Returned = true;
	}

	string CIPFinder::GetMyIP(const string& ServerURL_)
	{
		_IP.clear();

		string UserHeader("Content-Type: application/x-www-form-urlencoded\r\n");
		_Http.Push(SInObj(UserHeader, ServerURL_, 80, "", "", EMethod::Get, "", "", false));

		while (!_Returned)
		{
			_Http.Proc();
			this_thread::sleep_for(milliseconds(100));
		}

		return _IP;
	}
}
