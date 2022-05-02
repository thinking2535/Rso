#include "Peer.h"

namespace rso::net
{
	// Recv 나 Send, DelPeer에서 closesocket 을 할 것이 아니라 _UnLink 을 호출하여  closesocket 과 UnLink 절차를 동시에 처리하여 이후 Accept가 빠르게 처리되도록 함.
	// 이후 GetQueuedCompletionStatus 에서 통보하는 Key 는 유효하지 않은 Key이기 때문에 새로 New된 Key가 지워질 경우는 없음
	// 만일 closesocket 만 한다면 GetQueuedCompletionStatus 에서 통보받을 때 까지 그 PeerNum에는 PeerMgr.New 가 호출 될 수 없기 때문에 
	void CPeer::_SendBegin(EPacketType PacketType_)
	{
		_StreamSnd.clear();
		_StreamSnd << SHeader(0, 0, _SendPacketSeq + 1);
		_StreamSnd << SHeader2(PacketType_, _RecvPacketSeq);
	}
	void CPeer::_SendEnd(void)
	{
		++_SendPacketSeq;

		auto* pBodySize = (TSize*)_StreamSnd.at(0);
		(*pBodySize) = static_cast<TSize>(_StreamSnd.size() - _HeaderSize);

		auto* pCheckSum = (TCheckSum*)_StreamSnd.at(sizeof(SHeader::Size));
		if (_StreamSnd.size() > _HeaderSize)
		{
			*pCheckSum = GetCheckSum(_StreamSnd.at(_HeaderSize), *pBodySize);

			_Crypto.Encode((unsigned char*)_StreamSnd.at(_HeaderSize), *pBodySize, (0x1f3a49b72c8d5ef6 ^ *pBodySize ^ *pCheckSum ^ _SendPacketSeq));
		}

		WSABUF	WsaBuf;
		WsaBuf.buf = (CHAR*)_StreamSnd.buff();
		WsaBuf.len = (u_long)(_StreamSnd.size());

		auto Ret = WSASend(
			Socket(),
			&WsaBuf,
			1,
			NULL,
			0,
			&_SendOverLapInfo.OverLapped,
			NULL);

		if (Ret == SOCKET_ERROR)
		{
			Ret = WSAGetLastError();
			switch (Ret)
			{
			case WSA_IO_PENDING:
				break;

			default:
				// GQCS, WSARecv 에서 통보 받았기 때문에 여기서 throw 하지 않음.
				_NetInfo->ErrorLogFunc(GET_DEBUG_FORMAT(L"%s", GetWSAError()));
			}
		}
	}
	bool CPeer::_SendPing(void)
	{
		if (_PingTime != TTime())
			return true;

		try
		{
			_SendBegin(EPacketType::Ping);
			_PingPacketSeq = _SendPacketSeq + 1;
			_PingTime = system_clock::now();
			_SendEnd();
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	bool CPeer::_SendPong(void)
	{
		try
		{
			_SendBegin(EPacketType::Pong);
			_SendEnd();
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
	CPeer::CPeer(CSocket&& Socket_, size_t HeaderSize_, const SNetInfo* NetInfo_, HANDLE Iocp_, const CKey& Key_, TIPEndPoint&& IPEndPoint_) :
		CSocket(std::move(Socket_)),
		_HeaderSize(HeaderSize_),
		_NetInfo(NetInfo_),
		_Limiter(seconds(1), _NetInfo->MaxRcvBPS),
		_RecvOverLapInfo(EDir::Recv),
		_SendOverLapInfo(EDir::Send),
		_Key(Key_),
		_IPEndPoint(std::move(IPEndPoint_)),
		_CertifyEndTime(system_clock::now() + milliseconds(60000)),
		_HBRcvDelay((NetInfo_->HBRcvDelay > milliseconds() ? NetInfo_->HBRcvDelay + milliseconds(3000) : milliseconds())),
		_HBSndDelay(NetInfo_->HBSndDelay)
	{
		if (CreateIoCompletionPort((HANDLE)Socket(), Iocp_, (ULONG_PTR)((int64)Key_), 0) == NULL)
			THROWEXA(L"%s", GetWSAError());
	}
	bool CPeer::Recv(void)
	{
		DWORD	dwFlag = 0;

		WSABUF RecvWSABUF;
		RecvWSABUF.buf = _aRecvBuf;
		RecvWSABUF.len = sizeof(_aRecvBuf);

		auto iRet = WSARecv(
			Socket(),
			&RecvWSABUF,
			1,
			NULL,
			&dwFlag,
			&_RecvOverLapInfo.OverLapped,
			NULL);

		if (iRet == SOCKET_ERROR)
		{
			iRet = WSAGetLastError();
			switch (iRet)
			{
			case WSA_IO_PENDING:
				break;

			default:
				_NetInfo->ErrorLogFunc(GET_DEBUG_FORMAT(L"%s", GetWSAError()));
				return false;
			}
		}

		return true;
	}
	bool CPeer::RecvedBeginPrepare(TSize Size_)
	{
		if (_NetInfo->MaxRcvBPS > 0)
		{
			if (!_Limiter.Set(Size_))
				return false;
		}

		if (_HBRcvDelay)
			_HBRcvDelay.NextLoose();

		_StreamRcv.Push(_aRecvBuf, Size_);

		return true;
	}
	ERecvState CPeer::RecvedBegin(void)
	{
		// For Header /////////////////////
		if (!_IsValidHeader)
		{
			if (_StreamRcv.size() < _HeaderSize)
				return ERecvState::NoData;

			_StreamRcv >> _RecvHeader;
			_IsValidHeader = true;
		}

		// For Body /////////////////////
		if (_StreamRcv.size() < size_t(_RecvHeader.Size))
			return ERecvState::NoData;

		if (_RecvHeader.SendPacketSeq != (_RecvPacketSeq + 1))
			return ERecvState::Error;

		_RecvPacketSeq = _RecvHeader.SendPacketSeq;

		auto* pData = (TData*)_StreamRcv.buff();
		_Crypto.Decode((unsigned char*)pData, _RecvHeader.Size, (0x1f3a49b72c8d5ef6 ^ _RecvHeader.Size ^ _RecvHeader.CheckSum ^ _RecvHeader.SendPacketSeq));

		if (GetCheckSum(pData, _RecvHeader.Size) != _RecvHeader.CheckSum)
			return ERecvState::Error;

		auto OldStreamSize = _StreamRcv.size();
		SHeader2 Header2;
		_StreamRcv >> Header2; // 위에서 size검사 했으므로 예외없다고 간주

		_RecvHeader.Size -= TSize(OldStreamSize - _StreamRcv.size());
		_StreamRcv.SaveState();

		switch (Header2.PacketType)
		{
		case EPacketType::Ping:
			if (!_SendPong())
				return ERecvState::Error;

			return ERecvState::PingPong;

		case EPacketType::Pong:
			if (Header2.RecvPacketSeq == _PingPacketSeq)
			{
				_Latency = duration_cast<milliseconds>(system_clock::now() - _PingTime);
				_PingTime = TTime();
			}
			return ERecvState::PingPong;

		case EPacketType::User:
			break;

		default:
			return ERecvState::Error;
		}

		return ERecvState::UserData;
	}
	void CPeer::RecvedEnd(void)
	{
		_StreamRcv.LoadState();
		_StreamRcv.PopSize(_RecvHeader.Size);
		_IsValidHeader = false;
	}
	void CPeer::Sended(CKey Key_, TSize /*Size_*/)
	{
		if (_Key != Key_)
			return;
	}
	void CPeer::WillClose(const milliseconds& WaitMilliseconds_, ENetRet NetRet_)
	{
		if (DoesWillClose())
			return;

		_WillCloseNetRet = NetRet_;
		_CloseTime = (system_clock::now() + WaitMilliseconds_);
	}
	ENetRet CPeer::Proc(void)
	{
		if (_HBRcvDelay && _HBRcvDelay.CheckAndNextLoose())
		{
			_NetInfo->ErrorLogFunc(GET_DEBUG_FORMAT(L"HeartBeat Fail Key[%d %d]", _Key.PeerCounter, _Key.PeerNum));
			return ENetRet::HeartBeatFail;
		}

		if (_HBSndDelay && _HBSndDelay.CheckAndNextLoose())
		{
			if (!_SendPing())
				return ENetRet::SystemError;
		}

		if (DoesWillClose() && _CloseTime < system_clock::now())
			return _WillCloseNetRet;

		if (!_Certified && _CertifyEndTime < system_clock::now())
		{
			_NetInfo->ErrorLogFunc(GET_DEBUG_FORMAT(L"Certify Fail Key[%d %d]", _Key.PeerCounter, _Key.PeerNum));
			return ENetRet::CertifyFail;
		}

		return ENetRet::Ok;
	}
}