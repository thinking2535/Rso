#include "WSA.h"

namespace rso::net
{
	CWSA::CWSA()
	{
		WSADATA WsaData;

		if (WSAStartup(WINSOCK_VERSION, &WsaData) != 0)
			THROWEX();

		if (LOBYTE(WsaData.wVersion) != 2 || HIBYTE(WsaData.wVersion) != 2)
		{
			WSACleanup();
			THROWEX();
		}

		_Initialized = true;
	}
	CWSA::CWSA(CWSA&& Var_) :
		_Initialized(Var_._Initialized)
	{
		Var_._Initialized = false;
	}
	CWSA::~CWSA()
	{
		if (_Initialized)
			WSACleanup();
	}
	CWSA& CWSA::operator = (CWSA&& Var_)
	{
		this->~CWSA();
		new (this) CWSA(std::move(Var_));
		return *this;
	}
}
