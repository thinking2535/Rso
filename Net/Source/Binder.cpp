#include "Binder.h"

namespace rso::net
{
	CBinder::CBinder(CSocket&& Socket_, EAddressFamily AddressFamily_, const SNamePort& NamePort_, bool Passive_) :
		CSocket(std::move(Socket_)),
		_pIPEndPoint(GetIPEndPoint(AddressFamily_, NamePort_, Passive_))
	{
		if (Bind() == SOCKET_ERROR)
			THROWEXA_MBS("bind Fail Name[%s] Port[%d] LastError[%d]", NamePort_.Name, NamePort_.Port, WSAGetLastError());
	}
}
