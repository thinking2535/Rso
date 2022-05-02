#include "Server.h"

namespace rso::net
{
	void CServer::_Acceptor(const volatile bool* Exit_)
	{
		while (*Exit_ == false)
		{
			auto pIPEndPoint = _Binder.MakeEmptyIPEndPoint();
			auto AddrLen = pIPEndPoint->GetAddrLen();

			SOCKET AcceptSocket = accept(_Binder.Socket(), pIPEndPoint->GetSockAddrPtr(), &AddrLen);
			if (AcceptSocket == INVALID_SOCKET)
			{
				if (WSAGetLastError() == WSAEINTR)
					break;

				this_thread::sleep_for(seconds(1));
				continue;
			}

			try
			{
				_Accepts.emplace(_SAccept(CSocket(AcceptSocket, EIPProto::TCP, _NoDelay, _RecvBuffSize, _SendBuffSize), std::move(pIPEndPoint)));
			}
			catch (...)
			{
				closesocket(AcceptSocket);
				this_thread::sleep_for(milliseconds(1));
				continue;
			}
		}
	}
	CServer::CServer(
		EAddressFamily AddressFamily_,
		TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
		bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
		milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, TSize MaxRcvBPS_,
		const CNamePort& BindInfo_, int BackLog_) :
		CNet(
			LinkFunc_, UnLinkFunc_, RecvFunc_,
			NoDelay_, RecvBuffSize_, SendBuffSize_,
			HBRcvDelay_, HBSndDelay_, ThreadCnt_, MaxRcvBPS_),
		_Binder(CBinder(CSocket(AddressFamily_, EIPProto::TCP, _NoDelay, _RecvBuffSize, _SendBuffSize), AddressFamily_, BindInfo_, true), BackLog_)
	{
		_AcceptorThread.reset(new CThread(std::bind(&CServer::_Acceptor, this, _1)));
	}
	bool CServer::Certify(const CKey& Key_)
	{
		auto itPeer = _Peers.get(Key_.PeerNum);
		if (!itPeer)
			return false;

		if (itPeer->Key().PeerCounter != Key_.PeerCounter)
			return false;

		itPeer->Certify();

		return true;
	}
	void CServer::Proc(void)
	{
		for (auto* pAccept = _Accepts.get();
			pAccept;
			pAccept = _Accepts.get())
		{
			try
			{
				_Link(std::move(pAccept->Socket), std::move(pAccept->IPEndPoint));
			}
			catch (const exception_extern&)
			{
				throw;
			}
			catch (...)
			{
			}

			_Accepts.pop();
		}

		CNet::Proc();
	}
}