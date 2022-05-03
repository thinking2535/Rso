#pragma once

#include "Base.h"
#include "IPEndPointV4.h"
#include "IPEndPointV6.h"

namespace rso::net
{
	TIPEndPoints GetIPEndPoints(const SNamePort& NamePort_, bool Passive_);
	TIPEndPoint GetIPEndPoint(EAddressFamily AddressFamily_, const SNamePort& NamePort_, bool Passive_);

	class CSocket
	{
		SOCKET _Socket = INVALID_SOCKET;

		void _setsockopt(SOCKET Socket_, EIPProto IPProto_, bool NoDelay_, int RecvBuffSize_, int SendBuffSize_);

	public:
		CSocket();
		CSocket(SOCKET Socket_, EIPProto IPProto_, bool NoDelay_, int RecvBuffSize_, int SendBuffSize_);
		CSocket(EAddressFamily AddressFamily_, EIPProto IPProto_, bool NoDelay_, int RecvBuffSize_, int SendBuffSize_);
		CSocket(const CSocket& Var_) = delete;
		CSocket(CSocket&& Var_);
		virtual ~CSocket();
		CSocket operator = (const CSocket& Var_) = delete;
		CSocket& operator = (CSocket&& Var_);
		SOCKET Socket(void) const { return _Socket; }
		void Clear(void);
	};
}