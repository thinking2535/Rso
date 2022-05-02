#pragma once

#include "Base.h"

namespace rso::net
{
	class CIPEndPointV4 : public IIPEndPoint
	{
		SOCKADDR_IN _SockAddrIn{};

	public:
		CIPEndPointV4();
		CIPEndPointV4(const SOCKADDR* SockAddr_);
		EAddressFamily GetAddressFamily(void) const;
		SOCKADDR* GetSockAddrPtr(void) const;
		int32 GetAddrLen(void) const;
		string GetName(void) const;
		TPort GetPort(void) const;
		CNamePort GetNamePort(void) const;
		TIPEndPoint MakeEmptyIPEndPoint(void) const;
	};
}
