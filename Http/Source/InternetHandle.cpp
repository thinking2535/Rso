#include "InternetHandle.h"

namespace rso::http
{
	CInternetHandle::CInternetHandle(const string& Agent_)
	{
		_Handle = InternetOpenA(Agent_.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (_Handle == NULL)
			THROWEX();
	}
	CInternetHandle::CInternetHandle(CInternetHandle&& Var_) :
		_Handle(Var_._Handle)
	{
		Var_._Handle = NULL;
	}
	CInternetHandle::~CInternetHandle()
	{
		if (_Handle != NULL)
			InternetCloseHandle(_Handle);
	}
	CInternetHandle& CInternetHandle::operator = (CInternetHandle&& Var_)
	{
		this->~CInternetHandle();
		new (this) CInternetHandle(std::move(Var_));
		return *this;
	}
}
