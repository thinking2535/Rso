#include "Base.h"
#include <ws2tcpip.h>

namespace rso::game
{
	EGameRet NetRetToGameRet(ENetRet NetRet_)
	{
		switch (NetRet_)
		{
		case ENetRet::Ok: return EGameRet::Ok;
		case ENetRet::UserClose: return EGameRet::UserClose;
		case ENetRet::HeartBeatFail: return EGameRet::HeartBeatFail;
		case ENetRet::KeepConnectTimeOut: return EGameRet::KeepConnectTimeOut;
		case ENetRet::ConnectFail: return EGameRet::ConnectFail;
		case ENetRet::CertifyFail: return EGameRet::CertifyFail;
		case ENetRet::SystemError: return EGameRet::SystemError;
		case ENetRet::SocketError: return EGameRet::SocketError;
		case ENetRet::InvalidPacket: return EGameRet::InvalidPacket;
		default: return EGameRet::NetError;
		}
	}
}