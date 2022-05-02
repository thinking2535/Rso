#include "Net.h"

namespace rso::net
{
	void CNet::_Worker(const volatile bool* pExit_, size_t ThreadNum_)
	{
		BOOL bRet = FALSE;
		int64 KeyValue = 0;
		CKey Key;
		DWORD TranByte = 0;
		SOverLapInfo* pOverLapInfo = 0;

		while (*pExit_ == false)
		{
			bRet = GetQueuedCompletionStatus(_IOCP.IOCP(), &TranByte, (PULONG_PTR)&KeyValue, (LPOVERLAPPED*)&pOverLapInfo, INFINITE);
 			new (&Key) CKey(KeyValue);

			// Exit Signal
			if (pOverLapInfo == 0)
				return;

			if (bRet == FALSE || TranByte == 0)
			{
				try
				{
					_LFIOCPs[ThreadNum_].emplace(Key, TSize(0));
				}
				catch (...) // Proc 에서 _Close 처리됨
				{
				}
			}
			else
			{
				switch (pOverLapInfo->Dir)
				{
				case EDir::Recv:
				{
					try
					{
						_LFIOCPs[ThreadNum_].emplace(Key, TSize(TranByte));
					}
					catch (...) // Proc 에서 _Close 처리됨
					{
					}
				}
				break;

				case EDir::Send:
				{
					// _LFIOCPs 사용할것. _SendDone(Key, (TSize)TranByte);
				}
				break;
				}
			}
		}
	}
	void CNet::_ErrorLog(const wstring& Msg_)
	{
		_Log.Push(Msg_);
	}
	void CNet::_SendDone(CKey Key_, TSize Size_)
	{
		auto itPeer = _Peers.get(Key_.PeerNum);
		if (!itPeer)
			return;

		itPeer->Sended(Key_, Size_);
	}
	CKey CNet::_NewKey(TPeerCnt PeerNum_)
	{
		return CKey(PeerNum_, _Counter++);
	}
	void CNet::_Link(const CKey& Key_, CSocket&& Socket_, TIPEndPoint&& IPEndPoint_)
	{
		auto it = _Peers.emplace_at(Key_.PeerNum, CPeer(std::move(Socket_), _HeaderSize, &_NetInfo, _IOCP.IOCP(), Key_, std::move(IPEndPoint_)));

		try
		{
			_LinkFunc(Key_);
		}
		catch (const exception& e_)
		{
			throw exception_extern(e_);
		}
		catch (...)
		{
			throw exception_extern("external exception", 0);
		}

		it = _Peers.get(Key_.PeerNum); // _LinkFunc 내부에서 Close() 호출했을 경우 가상함수 _Close 가 호출되어 온전하게 유저는 삭제됨.
		if (it)
		{
			if (!it->Recv())
				_Close(it, ENetRet::SocketError);
		}
	}
	void CNet::_Link(CSocket&& Socket_, TIPEndPoint&& IPEndPoint_)
	{
		_Link(_NewKey(static_cast<TPeerCnt>(_Peers.new_index())), std::move(Socket_), std::move(IPEndPoint_));
	}
	void CNet::_Close(_TPeersIt itPeer_, ENetRet NetRet_)
	{
		auto Key = itPeer_->Key();
		_Peers.erase(itPeer_);
		_UnLinkFunc(Key, NetRet_);
	}
	void CNet::_Recved(_TPeersIt itPeer_, TSize TranBytes_)
	{
		auto Key = itPeer_->Key();

		try
		{
			if (!itPeer_->RecvedBeginPrepare(TranBytes_))
				throw ENetRet::SystemError;

			for (;;)
			{
				auto RecvState = itPeer_->RecvedBegin();
				if (RecvState == ERecvState::UserData)
				{
					if (!itPeer_->DoesWillClose())
					{
						if (!itPeer_->DoesWillClose())
						{
							_RecvFunc(Key, itPeer_->StreamRcv());
							if (!_Peers.get(Key.PeerNum))
								return;
						}

						// server must not throw
						//try
						//{
						//	_RecvFunc(Key, itPeer_->StreamRcv());
						//}
						//catch (const exception& e_)
						//{
						//	throw exception_extern(e_);
						//}
						//catch (...)
						//{
						//	throw exception_extern("external exception", 0);
						//}

						//if (!_Peers.get(Key.PeerNum))
						//	return;
					}
				}
				else
				{
					if (RecvState == ERecvState::NoData)
						break;

					if (RecvState == ERecvState::Error)
						_Close(itPeer_, ENetRet::RecvError);
				}

				itPeer_->RecvedEnd();
			}

			if (!itPeer_->Recv())
				throw ENetRet::SocketError;
		}
		catch (const ENetRet& NetRet_)
		{
			_Close(itPeer_, NetRet_);
		}
		catch (const exception_extern&)
		{
			throw;
		}
		catch (...)
		{
			_Close(itPeer_, ENetRet::SystemError);
		}
	}
	CNet::CNet(
		TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
		bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
		milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, TSize MaxRcvBPS_) :
		_LinkFunc(LinkFunc_), _UnLinkFunc(UnLinkFunc_), _RecvFunc(RecvFunc_),
		_NetInfo(MaxRcvBPS_, HBRcvDelay_, HBSndDelay_, std::bind(&CNet::_ErrorLog, this, _1)),
		_RecvBuffSize(RecvBuffSize_),
		_SendBuffSize(SendBuffSize_),
		_LFIOCPs(ThreadCnt_ > 0 ? ThreadCnt_ : THROWEXA(L"Invalid ThreadCnt[%d]", ThreadCnt_)), _NoDelay(NoDelay_),
		_IOCP(ThreadCnt_)
	{
		for (size_t i = 0; i < ThreadCnt_; ++i)
			_WorkerThreads.emplace_back(new CThread(std::bind(&CNet::_Worker, this, _1, _2), i));

		CStream StreamForHeaderSizeChecking;
		StreamForHeaderSizeChecking << SHeader();
		_HeaderSize = StreamForHeaderSizeChecking.size();
	}
	bool CNet::Close(const CKey& Key_)
	{
		auto itPeer = _Peers.get(Key_.PeerNum);
		if (!itPeer)
			return false;

		if (itPeer->Key().PeerCounter != Key_.PeerCounter)
			return false;

		_Close(itPeer, ENetRet::UserClose);

		return true;
	}
	void CNet::CloseAll(void)
	{
		for (auto itPeer = _Peers.begin(); itPeer != _Peers.end(); itPeer = _Peers.begin())
			_Close(itPeer, ENetRet::UserClose);
	}
	bool CNet::WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
	{
		auto itPeer = _Peers.get(Key_.PeerNum);
		if (!itPeer)
			return false;

		if (itPeer->Key().PeerCounter != Key_.PeerCounter)
			return false;

		itPeer->WillClose(WaitMilliseconds_, ENetRet::UserClose);

		return true;
	}
	void CNet::Proc(void)
	{
		// Recv, Close 처리
		for (auto& IOCP : _LFIOCPs)
		{
			for (auto pLFIOCP = IOCP.get();
				pLFIOCP;
				pLFIOCP = IOCP.get())
			{
				auto itPeer = _Peers.get(pLFIOCP->Key.PeerNum);
				if (itPeer && itPeer->ValidKey(pLFIOCP->Key))
				{
					if (pLFIOCP->TranBytes == 0)
						_Close(itPeer, ENetRet::SocketError);
					else
						_Recved(itPeer, pLFIOCP->TranBytes); // 내부에서 itPeer 를 삭제할 수 있음.
				}

				IOCP.pop();
			}
		}

		if (_Period.CheckAndNextLoose())
		{
			// HeartBeat 송수신 체크
			for (auto itPeer = _Peers.begin(); itPeer != _Peers.end();)
			{
				auto itCheck = itPeer;
				++itPeer;

				auto NetRet = itCheck->Proc();
				if (NetRet != ENetRet::Ok)
					_Close(itCheck, NetRet);
			}
		}
	}
}