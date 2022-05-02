#pragma once

#include "Base.h"
#include "Socket.h"
#include <functional>
#include <chrono>
#include <Rso/Base/Period.h>
#include <Rso/Base/Limiter.h>
#include <Rso/Crypto/Crypto.h>

namespace rso::net
{
	using namespace base;
	using namespace std;
	using namespace std::placeholders;
	using namespace chrono;
	using namespace crypto;

	class CPeer final : public CSocket
	{
		size_t _HeaderSize = 0;

		const SNetInfo* _NetInfo = nullptr;
		CLimiter<seconds, TSize> _Limiter;
		SOverLapInfo _RecvOverLapInfo;
		SOverLapInfo _SendOverLapInfo;
		CKey _Key;
		TIPEndPoint _IPEndPoint;
		bool _Certified = false;
		system_clock::time_point _CertifyEndTime;
		system_clock::time_point _CloseTime;
		CPeriod<milliseconds> _HBRcvDelay;
		CPeriod<milliseconds> _HBSndDelay;
		TData _aRecvBuf[40960];
		bool _IsValidHeader = false;
		SHeader _RecvHeader;
		CStream _StreamRcv;
		CStream _StreamSnd;
		CCrypto _Crypto;
		TPacketSeq _SendPacketSeq = 0;
		TPacketSeq _RecvPacketSeq = 0;
		TPacketSeq _PingPacketSeq = 0;
		TTime _PingTime;
		ENetRet _WillCloseNetRet = ENetRet::Null;
		milliseconds _Latency;

		void _SendBegin(EPacketType PacketType_ = EPacketType::User);
		void _SendEnd(void);
		bool _SendPing(void);
		bool _SendPong(void);

		// Buffer, Size 로 전송이 필요하면 주석해제
		//template<typename TType>
		//bool _Send(TType* Buf_, TSize Size_)
		//{
		//	return _StreamSnd.PushStream(Buf_, Size_);
		//}
		//template<typename TType, typename... TTypes>
		//bool _Send(TType* Buf_, TSize Size_, const TTypes&... Args_)
		//{
		//	return (_Send(Buf_, Size_) && _Send(Args_...));
		//}

	public:
		CPeer(CSocket&& Socket_, size_t HeaderSize_, const SNetInfo* NetInfo_, HANDLE Iocp_, const CKey& Key_, TIPEndPoint&& IPEndPoint_);
		inline const CKey& Key(void) const { return _Key; }
		inline CNamePort GetNamePort(void) const { return _IPEndPoint->GetNamePort(); }
		inline bool ValidKey(const CKey& Key_) { return (Key_ == _Key); }
		inline CStream& StreamRcv(void) { return _StreamRcv; }
		inline milliseconds Latency(void) const { return _Latency; }
		bool Recv(void);
		bool RecvedBeginPrepare(TSize Size_);
		ERecvState RecvedBegin(void);
		void RecvedEnd(void);
		void Sended(CKey Key_, TSize Size_);
		void WillClose(const milliseconds& WaitMilliseconds_, ENetRet NetRet_);
		inline bool DoesWillClose(void) const
		{
			return (_CloseTime.time_since_epoch().count() != 0);
		}
		inline void Certify(void)
		{
			_Certified = true;
		}
		inline bool IsCertified(void) const
		{
			return _Certified;
		}
		ENetRet Proc(void);

		template<typename... TTypes>
		void Send(const TTypes&... Args_)
		{
			if (DoesWillClose())
				return;

			try
			{
				_SendBegin();
				_StreamSnd.Send(Args_...);
				_SendEnd();
			}
			catch (...)
			{
				// GQCS, WSARecv 에서 에러 통보받지 않는 경우 아래로 들어오게되므로 WillClose 로 종료 예약
				WillClose(milliseconds(0), ENetRet::SystemError);
			}
		}
	};
}