#include "UDP.h"

namespace rso::net
{
	void CUDP::_Receiver(const volatile bool* Exit_)
	{
		while (*Exit_ == false)
		{
			try
			{
				auto* pRecvData = _Queue.new_buf();

				pRecvData->Stream.clear();
				if (pRecvData->Stream.Capacity() < _BlockSize)
				{
					try
					{
						pRecvData->IPEndPoint = GetIPEndPoint(GetAddressFamily(), _BindNamePort, false);
						pRecvData->Stream.Reserve(_BlockSize);
					}
					catch (...)
					{
						this_thread::sleep_for(TSpan(100));	// Not Enough Memory or etc.
						continue;
					}
				}

				auto FromLen = pRecvData->IPEndPoint->GetAddrLen();
				auto Return = recvfrom(Socket(), pRecvData->Stream.data(), (int32)pRecvData->Stream.Capacity(), 0, pRecvData->IPEndPoint->GetSockAddrPtr(), &FromLen);
				if (Return == SOCKET_ERROR)
				{
					this_thread::sleep_for(TSpan(30));
					continue;
				}

				pRecvData->Stream.SetHead(0);
				pRecvData->Stream.SetTail(Return);
				_Queue.push();
			}
			catch (...)
			{
				this_thread::sleep_for(TSpan(1000));	// Not Enough Memory or etc.
			}
		}
	}
	CUDP::CUDP(
		EAddressFamily AddressFamily_, const CNamePort& BindNamePort_,
		TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
		bool NoDelay_, TSize SocketBuffSize_, TSize BlockSize_) :
		CBinder(CSocket(AddressFamily_, EIPProto::UDP, NoDelay_, SocketBuffSize_, SocketBuffSize_), AddressFamily_, BindNamePort_, false),
		_LinkFunc(LinkFunc_), _UnLinkFunc(UnLinkFunc_), _RecvFunc(RecvFunc_),
		_BlockSize(BlockSize_ > 0 ? BlockSize_ : 900),
		_ReceivThread(std::bind(&CUDP::_Receiver, this, _1))
	{
	}
	CUDP::~CUDP()
	{
		CSocket::Clear();
	}
	void CUDP::InsertPeer(const CNamePort& NamePort_)
	{
		auto ibPeer = _Peers.emplace(NamePort_, _SPeer(GetIPEndPoint(GetAddressFamily(), NamePort_, false)));
		if (!ibPeer.second)
			THROWEX();

		_Send(ibPeer.first->second.IPEndPoint, EPacketType::Ping);
	}
	void CUDP::ErasePeer(const CNamePort& NamePort_)
	{
		auto itPeer = _Peers.find(NamePort_);
		if (itPeer == _Peers.end())
			return;

		if (itPeer->second.IsLinked)
			_UnLinkFunc(itPeer->second.IPEndPoint);

		_Peers.erase(itPeer);
	}
	void CUDP::ClearPeer(void)
	{
		for (auto& p : _Peers)
		{
			if (p.second.IsLinked)
				_UnLinkFunc(p.second.IPEndPoint);
		}

		_Peers.clear();
	}
	void CUDP::Proc(void)
	{
		auto Now = system_clock::now();

		for (auto* pRecvData = _Queue.get();
			pRecvData;
			pRecvData = _Queue.get())
		{
			EPacketType PacketType = EPacketType::Null;
			pRecvData->Stream >> PacketType;

			auto itPeer = _Peers.find(pRecvData->IPEndPoint->GetNamePort());
			if (itPeer != _Peers.end())
			{
				itPeer->second.LastReceivedTime = Now;

				if (!itPeer->second.IsLinked)
				{
					itPeer->second.IsLinked = true;
					_LinkFunc(pRecvData->IPEndPoint);
				}
			}

			if (PacketType == EPacketType::User)
				_RecvFunc(*pRecvData);

			_Queue.pop();
		}

		if (_PeriodPingCheck.CheckAndNextLoose())
		{
			for (auto& i : _Peers)
			{
				if (i.second.IsLinked &&
					(Now - i.second.LastReceivedTime) >= c_UnLinkTimeOutDuration)
				{
					i.second.IsLinked = false;
					_UnLinkFunc(i.second.IPEndPoint);
				}

				_Send(i.second.IPEndPoint, EPacketType::Ping);
			}
		}
	}
}