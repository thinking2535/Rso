#pragma once

#include "Base.h"

namespace rso::net
{
	class CIPEndPointV6 : public IIPEndPoint
	{
		SOCKADDR_IN6 _SockAddrIn{};

	public:
		CIPEndPointV6();
		CIPEndPointV6(const SOCKADDR* SockAddr_);
		EAddressFamily GetAddressFamily(void) const;
		SOCKADDR* GetSockAddrPtr(void) const;
		int32 GetAddrLen(void) const;
		string GetName(void) const;
		TPort GetPort(void) const;
		CNamePort GetNamePort(void) const;
		TIPEndPoint MakeEmptyIPEndPoint(void) const;
	};
}
