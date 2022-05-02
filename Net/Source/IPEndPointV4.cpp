#include "IPEndPointV4.h"

namespace rso::net
{
	CIPEndPointV4::CIPEndPointV4()
	{
	}
	CIPEndPointV4::CIPEndPointV4(const SOCKADDR* SockAddr_)
	{
		_SockAddrIn = *((SOCKADDR_IN*)SockAddr_);
	}
	EAddressFamily CIPEndPointV4::GetAddressFamily(void) const
	{
		return EAddressFamily::INET;
	}
	SOCKADDR* CIPEndPointV4::GetSockAddrPtr(void) const
	{
		return (SOCKADDR*)&_SockAddrIn;
	}
	int32 CIPEndPointV4::GetAddrLen(void) const
	{
		return sizeof(_SockAddrIn);
	}
	string CIPEndPointV4::GetName(void) const
	{
		char AddrStr[INET_ADDRSTRLEN] = {};
		inet_ntop((int32)GetAddressFamily(), (void*)&_SockAddrIn.sin_addr, AddrStr, sizeof(AddrStr));
		return AddrStr;
	}
	TPort CIPEndPointV4::GetPort(void) const
	{
		return ntohs(_SockAddrIn.sin_port);
	}
	CNamePort CIPEndPointV4::GetNamePort(void) const
	{
		return CNamePort(GetName(), GetPort());
	}
	TIPEndPoint CIPEndPointV4::MakeEmptyIPEndPoint(void) const
	{
		return std::make_unique<CIPEndPointV4>();
	}
}
