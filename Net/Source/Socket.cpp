#include "Socket.h"

namespace rso::net
{
	TIPEndPoints GetIPEndPoints(const SNamePort& NamePort_, bool Passive_)
	{
		ADDRINFO Hints, * AddrInfo;
		memset(&Hints, 0, sizeof(Hints));
		Hints.ai_family = (int32)EAddressFamily::UNSPEC;
		Hints.ai_socktype = SOCK_STREAM;
		//Hints.ai_flags = AI_NUMERICHOST;

		if (Passive_)
			Hints.ai_flags |= AI_PASSIVE;

		auto Ret = getaddrinfo(NamePort_.Name.empty() ? NULL : u16string_to_u8string(NamePort_.Name).c_str(), to_string(NamePort_.Port).c_str(), &Hints, &AddrInfo);
		ASSERTIONA(Ret == 0 || AddrInfo, L"getaddrinfo fail [%d]", Ret);

		TIPEndPoints IPEndPoints;

		for (auto i = AddrInfo; i; i = i->ai_next)
		{
			switch ((EAddressFamily)i->ai_family)
			{
			case EAddressFamily::INET:
				IPEndPoints.emplace_back(std::make_unique<CIPEndPointV4>(i->ai_addr));
				break;

			case EAddressFamily::INET6:
				IPEndPoints.emplace_back(std::make_unique<CIPEndPointV6>(i->ai_addr));
				break;

			default:
				continue;
			}
		}

		freeaddrinfo(AddrInfo);

		if (IPEndPoints.empty())
			THROWEXA(L"GetIPEndPoints Fail");

		return IPEndPoints;
	}
	TIPEndPoint GetIPEndPoint(EAddressFamily AddressFamily_, const SNamePort& NamePort_, bool Passive_)
	{
		for (auto& i : GetIPEndPoints(NamePort_, Passive_))
		{
			if (i->GetAddressFamily() != AddressFamily_)
				continue;

			return std::move(i);
		}

		THROWEXA(L"GetIPEndPoint Fail");
	}

	void CSocket::_setsockopt(SOCKET Socket_, EIPProto IPProto_, bool NoDelay_, int RecvBuffSize_, int SendBuffSize_)
	{
		int OptVal = (NoDelay_ == true ? 1 : 0);
		if (setsockopt(Socket_, IPProto_, TCP_NODELAY, (char*)&OptVal, sizeof(OptVal)) == SOCKET_ERROR)
			THROWEX();

		LINGER	Linger;
		Linger.l_onoff = 1;
		Linger.l_linger = 0;
		// l_onoff = 0 : default (closesocket() : non block, send send_buffer)
		// l_onoff = 1, l_linger = 0 : (closesocket() : non block, del send_buffer)
		// l_onoff = 1, l_linger > 0 : (closesocket() : block l_linger time, try send for l_linger)

		if (IPProto_ == EIPProto::TCP)
		{
			if (setsockopt(Socket_, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(Linger)) == SOCKET_ERROR)
				THROWEX();
		}

		if (setsockopt(Socket_, SOL_SOCKET, SO_RCVBUF, (char*)&RecvBuffSize_, sizeof(RecvBuffSize_)) == SOCKET_ERROR)
			THROWEX();

		if (setsockopt(Socket_, SOL_SOCKET, SO_SNDBUF, (char*)&SendBuffSize_, sizeof(SendBuffSize_)) == SOCKET_ERROR)
			THROWEX();
	}
	CSocket::CSocket()
	{
	}
	CSocket::CSocket(SOCKET Socket_, EIPProto IPProto_, bool NoDelay_, int RecvBuffSize_, int SendBuffSize_) :
		_Socket(Socket_)
	{
		_setsockopt(Socket_, IPProto_, NoDelay_, RecvBuffSize_, SendBuffSize_);
	}
	CSocket::CSocket(EAddressFamily AddressFamily_, EIPProto IPProto_, bool NoDelay_, int RecvBuffSize_, int SendBuffSize_)
	{
		_Socket = socket((int32)AddressFamily_, (IPProto_ == EIPProto::TCP ? SOCK_STREAM : SOCK_DGRAM), 0);

		if (_Socket == INVALID_SOCKET)
			THROWEXA(L"socket() Fail");

		_setsockopt(_Socket, IPProto_, NoDelay_, RecvBuffSize_, SendBuffSize_);
	}
	CSocket::CSocket(CSocket&& Var_) :
		_Socket(Var_._Socket)
	{
		Var_._Socket = INVALID_SOCKET;
	}
	CSocket::~CSocket()
	{
		Clear();
	}
	CSocket& CSocket::operator = (CSocket&& Var_)
	{
		this->~CSocket();
		new (this) CSocket(std::move(Var_));
		return *this;
	}
	void CSocket::Clear(void)
	{
		if (_Socket != INVALID_SOCKET)
			closesocket(_Socket);
	}
}