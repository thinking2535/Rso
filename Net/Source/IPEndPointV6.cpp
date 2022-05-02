#include "IPEndPointV6.h"

namespace rso::net
{
	CIPEndPointV6::CIPEndPointV6()
	{
	}
	CIPEndPointV6::CIPEndPointV6(const SOCKADDR* SockAddr_)
	{
		_SockAddrIn = *((SOCKADDR_IN6*)SockAddr_);
	}
	EAddressFamily CIPEndPointV6::GetAddressFamily(void) const
	{
		return EAddressFamily::INET6;
	}
	SOCKADDR* CIPEndPointV6::GetSockAddrPtr(void) const
	{
		return (SOCKADDR*)&_SockAddrIn;
	}
	int32 CIPEndPointV6::GetAddrLen(void) const
	{
		return sizeof(_SockAddrIn);
	}
	string CIPEndPointV6::GetName(void) const
	{
		char AddrStr[INET6_ADDRSTRLEN] = {};
		inet_ntop((int32)GetAddressFamily(), (void*)&_SockAddrIn.sin6_addr, AddrStr, sizeof(AddrStr));
		return AddrStr;
	}
	TPort CIPEndPointV6::GetPort(void) const
	{
		return ntohs(_SockAddrIn.sin6_port);
	}
	CNamePort CIPEndPointV6::GetNamePort(void) const
	{
		return CNamePort(GetName(), GetPort());
	}
	TIPEndPoint CIPEndPointV6::MakeEmptyIPEndPoint(void) const
	{
		return std::make_unique<CIPEndPointV6>();
	}
}
