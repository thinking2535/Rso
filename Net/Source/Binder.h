#pragma once

#include "Socket.h"

namespace rso::net
{
	class CBinder : public CSocket
	{
		TIPEndPoint _pIPEndPoint;

	public:
		CBinder(CSocket&& Socket_, EAddressFamily AddressFamily_, const SNamePort& NamePort_, bool Passive_);
		inline int32 Bind(void)
		{
			return ::bind(Socket(), _pIPEndPoint->GetSockAddrPtr(), _pIPEndPoint->GetAddrLen());
		}
		inline TIPEndPoint MakeEmptyIPEndPoint(void) const
		{
			return _pIPEndPoint->MakeEmptyIPEndPoint();
		}
		inline EAddressFamily GetAddressFamily(void) const
		{
			return _pIPEndPoint->GetAddressFamily();
		}
		inline string GetName(void) const
		{
			return _pIPEndPoint->GetName();
		}
		inline TPort GetPort(void) const
		{
			return _pIPEndPoint->GetPort();
		}
		inline CNamePort GetNamePort(void) const
		{
			return _pIPEndPoint->GetNamePort();
		}
		inline SOCKADDR* GetSockAddrPtr(void) const
		{
			return _pIPEndPoint->GetSockAddrPtr();
		}
		inline int32 GetAddrLen(void) const
		{
			return _pIPEndPoint->GetAddrLen();
		}
	};
}