#include "Client.h"

namespace rso::net
{
	void CClient::_LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		try
		{
			_LinkFailFunc(PeerNum_, NetRet_);
		}
		catch (...)
		{
		}
	}
	void CClient::_Link(const CKey& Key_)
	{
		_Peers[Key_.PeerNum].Certify();
		_LinkFunc(Key_);
	}
	void CClient::_UnLink(const CKey& Key_, ENetRet NetRet_)
	{
		_PeersAndConnectings.erase(Key_.PeerNum);
		_UnLinkFunc(Key_, NetRet_);
	}
	void CClient::_Connector(const volatile bool* Exit_)
	{
		while (*Exit_ == false)
		{
			for (auto LFIOCP = _ConnectingInfos.get();
				LFIOCP;
				LFIOCP = _ConnectingInfos.get())
			{
				auto LFConnectGet = _Connect(*LFIOCP);

				while (true)
				{
					try
					{
						_Connects.emplace(std::move(LFConnectGet));
						break;
					}
					catch (...)
					{
						this_thread::sleep_for(seconds(1));
					}
				}

				_ConnectingInfos.pop();
			}

			this_thread::sleep_for(milliseconds(10));
		}
	}
	CClient::_SConnect CClient::_Connect(const _SConnectingInfo& ConnectInfo_)
	{
		try
		{
			unique_ptr<CSocket> pSocket;

			array<EAddressFamily, 2> AddressFamilies = { EAddressFamily::INET, EAddressFamily::INET6 };

			for (auto& i : GetIPEndPoints(ConnectInfo_.NamePort, false))
			{
				if (i->GetAddressFamily() != EAddressFamily::INET)
					continue;

				for (auto& a : AddressFamilies)
				{
					pSocket.reset(new CSocket(a, EIPProto::TCP, _NoDelay, _RecvBuffSize, _SendBuffSize));

					try
					{
						unsigned long Mode = 1;
						if (ioctlsocket(pSocket->Socket(), FIONBIO, &Mode) == SOCKET_ERROR)
							throw ENetRet::SocketError;

						if (connect(pSocket->Socket(), i->GetSockAddrPtr(), i->GetAddrLen()) == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)
							throw ENetRet::ConnectFail;

						Mode = 0;
						if (ioctlsocket(pSocket->Socket(), FIONBIO, &Mode) == SOCKET_ERROR)
							throw ENetRet::ConnectFail;

						fd_set Write, Err;
						FD_ZERO(&Write);
						FD_ZERO(&Err);
						FD_SET(pSocket->Socket(), &Write);
						FD_SET(pSocket->Socket(), &Err);

						TIMEVAL Timeout;
						Timeout.tv_sec = _ConnectTimeOutSec;
						Timeout.tv_usec = 0;
						select(0, NULL, &Write, &Err, &Timeout);
						if (!FD_ISSET(pSocket->Socket(), &Write))
							throw ENetRet::ConnectFail;
					}
					catch (...)
					{
						pSocket.reset();
						continue;
					}

					return _SConnect(ENetRet::Ok, ConnectInfo_.Key, std::move(*pSocket), std::move(i));
				}
			}

			throw ENetRet::SocketError;
		}
		catch (const ENetRet& NetRet_)
		{
			return _SConnect(NetRet_, ConnectInfo_.Key);
		}
		catch (...)
		{
			return _SConnect(ENetRet::SocketError, ConnectInfo_.Key);
		}
	}
	CClient::CClient(
		TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
		bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
		milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, long ConnectTimeOutSec_) :
		CNet(
			std::bind(&CClient::_Link, this, _1), std::bind(&CClient::_UnLink, this, _1, _2), RecvFunc_,
			NoDelay_, RecvBuffSize_, SendBuffSize_,
			HBRcvDelay_, HBSndDelay_, ThreadCnt_, 0),
		_ConnectTimeOutSec(ConnectTimeOutSec_),
		_LinkFunc(LinkFunc_),
		_LinkFailFunc(LinkFailFunc_),
		_UnLinkFunc(UnLinkFunc_),
		_ConnectorThread(std::bind(&CClient::_Connector, this, _1))
	{
	}
	CKey CClient::Connect(const CNamePort& NamePort_, TPeerCnt PeerNum_)
	{
		try
		{
			auto itPeerConnecting = _PeersAndConnectings.emplace_at(PeerNum_, _NewKey(TPeerCnt(PeerNum_)));
			try
			{
				_ConnectingInfos.emplace(_SConnectingInfo{ *itPeerConnecting, NamePort_ });
			}
			catch (...)
			{
				_PeersAndConnectings.erase(itPeerConnecting);
				throw;
			}

			return *itPeerConnecting;
		}
		catch (...)
		{
			return CKey();
		}
	}
	void CClient::Close(TPeerCnt PeerNum_)
	{
		auto Peer = _Peers.get(PeerNum_);
		if (Peer)
		{
			_Close(Peer, ENetRet::UserClose);
		}
		else
		{
			if (_PeersAndConnectings.erase(PeerNum_)) // 여기서 지워야 LockFree 큐에서 들어오는 Connect 정보가 처리되지 않음.
				_LinkFail(PeerNum_, ENetRet::UserClose);
		}
	}
	bool CClient::Close(const CKey& Key_)
	{
		auto Peer = _Peers.get(Key_.PeerNum);
		if (Peer)
		{
			if (Peer->Key().PeerCounter != Key_.PeerCounter)
				return false;

			_Close(Peer, ENetRet::UserClose);

			return true;
		}
		else
		{
			auto PeerConnecting = _PeersAndConnectings.get(Key_.PeerNum);
			if (!PeerConnecting)
				return false;

			if (PeerConnecting->PeerCounter != Key_.PeerCounter)
				return false;

			_PeersAndConnectings.erase(PeerConnecting); // 여기서 먼저 지워야 LockFree 큐에서 들어오는 Connect 정보가 처리되지 않음.
			_LinkFail(Key_.PeerNum, ENetRet::UserClose);

			return true;
		}
	}
	void CClient::CloseAll(void)
	{
		for (auto it = _PeersAndConnectings.begin(); it != _PeersAndConnectings.end(); it = _PeersAndConnectings.begin())
		{
			auto Peer = _Peers.get(it.Index());
			if (Peer)
			{
				CNet::Close(*it);
			}
			else
			{
				auto PeerNum = it->PeerNum;
				_PeersAndConnectings.erase(it);
				_LinkFail(PeerNum, ENetRet::UserClose);
			}
		}
	}
	void CClient::Proc(void)
	{
		CNet::Proc();

		for (auto pLFConnect = _Connects.get();
			pLFConnect;
			pLFConnect = _Connects.get())
		{
			auto itPeer = _PeersAndConnectings.get(pLFConnect->Key.PeerNum);
			if (itPeer && *itPeer == pLFConnect->Key)
			{
				if (pLFConnect->NetRet == ENetRet::Ok)
				{
					try
					{
						CNet::_Link(pLFConnect->Key, std::move(pLFConnect->Socket), std::move(pLFConnect->IPEndPoint));
					}
					catch (const exception_extern&)
					{
						throw;
					}
					catch (...)
					{
						pLFConnect->NetRet = ENetRet::SystemError;
					}
				}

				if (pLFConnect->NetRet != ENetRet::Ok)
				{
					_PeersAndConnectings.erase(itPeer);
					_LinkFail(pLFConnect->Key.PeerNum, pLFConnect->NetRet);
				}
			}

			_Connects.pop();
		}
	}
}
