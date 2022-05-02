#pragma once

#include <Rso/Win/Base.h>
#include <Wininet.h>

#pragma comment( lib, "Wininet.lib" )

namespace rso::http
{
	using namespace win;

	class CInternetHandle
	{
	private:
		HINTERNET _Handle = NULL;

	public:
		CInternetHandle(const string& Agent_);
		CInternetHandle(CInternetHandle&& Var_);
		virtual ~CInternetHandle();
		CInternetHandle& operator = (CInternetHandle&& Var_);
		HINTERNET GetHandle(void) const { return _Handle; }
	};
}