#pragma once

#include <ws2tcpip.h>
#include <Rso/Core/Base.h>

#pragma comment( lib, "Ws2_32.lib" )
#pragma comment( lib, "Rso/Net_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Base/Base.h>
#include <functional>
#include "Protocol.h"
#include <Rso/Log/Log.h>

namespace rso::net
{
	static_assert(TPacketSeq(-1) > 0, "TPacketSeq must be unsigned");

	using namespace base;
	using namespace log;
	using namespace std;

	const TPeerCnt c_PeerCnt_Null = TPeerCnt(~0);

	enum class EDir
	{
		Send,
		Recv,
		Max,
		Null
	};
	enum EIPProto
	{
		TCP = IPPROTO_TCP,
		UDP = IPPROTO_UDP,
	};
	enum class EAddressFamily
	{
		UNSPEC = AF_UNSPEC,
		UNIX = AF_UNIX,
		INET = AF_INET,
		IMPLINK = AF_IMPLINK,
		PUP = AF_PUP,
		CHAOS = AF_CHAOS,
		NS = AF_NS,
		IPX = AF_IPX,
		ISO = AF_ISO,
		OSI = AF_OSI,
		ECMA = AF_ECMA,
		DATAKIT = AF_DATAKIT,
		CCITT = AF_CCITT,
		SNA = AF_SNA,
		DECnet = AF_DECnet,
		DLI = AF_DLI,
		LAT = AF_LAT,
		HYLINK = AF_HYLINK,
		APPLETALK = AF_APPLETALK,
		NETBIOS = AF_NETBIOS,
		VOICEVIEW = AF_VOICEVIEW,
		FIREFOX = AF_FIREFOX,
		UNKNOWN1 = AF_UNKNOWN1,
		BAN = AF_BAN,
		ATM = AF_ATM,
		INET6 = AF_INET6,
		CLUSTER = AF_CLUSTER,
		IEEE12844 = AF_12844,
		IRDA = AF_IRDA,
		NETDES = AF_NETDES,
		Max
	};

	inline int64 KeyToInt64(const SKey& Key_)
	{
		return (((int64)Key_.PeerCounter << 32) + Key_.PeerNum);
	}
	inline bool KeyToBool(const SKey& Key_)
	{
		return (Key_.PeerNum != c_PeerCnt_Null);
	}
	inline bool operator == (const SKey& lhs_, const SKey& rhs_)
	{
		return (lhs_.PeerNum == rhs_.PeerNum && lhs_.PeerCounter == rhs_.PeerCounter);
	}
	inline bool operator != (const SKey& lhs_, const SKey& rhs_)
	{
		return !(lhs_ == rhs_);
	}
	inline bool operator < (const SKey& lhs_, const SKey& rhs_)
	{
		return KeyToInt64(lhs_) < KeyToInt64(rhs_);
	}
	class CKey : public SKey
	{
	public:
		CKey() :
			SKey(c_PeerCnt_Null, 0)
		{
		}
		CKey(TPeerCnt PeerNum_, TPeerCnt PeerCounter_) :
			SKey(PeerNum_, PeerCounter_)
		{
		}
		CKey(const SKey& Key_) :
			SKey(Key_)
		{
		}
		explicit CKey(int64 Value_) :
			SKey(Value_ & 0x00000000FFFFFFFF, Value_ >> 32)
		{
		}
		inline explicit operator int64() const
		{
			return KeyToInt64(*this);
		}
		inline explicit operator bool() const
		{
			return KeyToBool(*this);
		}
	};

	struct SOverLapInfo
	{
		OVERLAPPED OverLapped;
		EDir Dir;

		SOverLapInfo(EDir Dir_) :
			Dir(Dir_)
		{
			ZeroMemory(&OverLapped, sizeof(OverLapped));
		}
	};

	inline TLongIP IPv4ToLongIP(const char* IP_)
	{
		return inet_addr(IP_);
	}
	inline TLongIP IPv4ToLongIP(const string& IP_)
	{
		return IPv4ToLongIP(IP_.c_str());
	}
	inline wstring GetWSAError(void)
	{
		wstring Msg;
		WCHAR* pErrMsg = NULL;
		DWORD Ret = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&pErrMsg, 0, NULL);

		if (pErrMsg != NULL && Ret != 0)
			Msg = wstring(pErrMsg);

		if (pErrMsg != NULL) LocalFree(pErrMsg);

		return std::move(Msg);
	}
	inline bool NamePortToBool(const SNamePort& NamePort_)
	{
		return (NamePort_.Port != 0);
	}
	inline bool operator == (const SNamePort& lhs_, const SNamePort& rhs_)
	{
		return (lhs_.Name == rhs_.Name && lhs_.Port == rhs_.Port);
	}
	inline bool operator != (const SNamePort& lhs_, const SNamePort& rhs_)
	{
		return !(lhs_ == rhs_);
	}
	inline bool operator < (const SNamePort& lhs_, const SNamePort& rhs_)
	{
		return (
			(lhs_.Name < rhs_.Name) ||
			(lhs_.Name == rhs_.Name && lhs_.Port < rhs_.Port));
	}

	class CNamePort : public SNamePort
	{
	public:
		CNamePort()
		{
		}
		CNamePort(const SNamePort& NamePort_) :
			SNamePort(NamePort_)
		{
		}
		CNamePort(const u16string& Name_, TPort Port_) :
			SNamePort(Name_, Port_)
		{
		}
		CNamePort(const string& Name_, TPort Port_) :
			SNamePort(u8string_to_u16string(Name_), Port_)
		{
		}
		CNamePort(TPort Port_) :
			SNamePort(u"0.0.0.0"s, Port_)
		{
		}
		inline explicit operator bool() const noexcept
		{
			return NamePortToBool(*this);
		}
		void Clear(void) noexcept
		{
			Name.clear();
			Port = 0;
		}
	};

	class exception_extern : public exception
	{
	public:
		exception_extern() noexcept
		{
		}
		explicit exception_extern(char const* const _Message, int i_) noexcept
			: exception(_Message, i_)
		{
		}
		explicit exception_extern(exception const& _Other) noexcept
			: exception(_Other)
		{
		}
	};

	template<typename TType>
	class CRangeKey : public SRangeKey<TType>
	{
	public:
		CRangeKey(const SRangeKey<TType>& RangeKey_) :
			SRangeKey(RangeKey_)
		{
		}
		CRangeKey(const TType& MinValue_, const TType& MaxValue_) :
			SRangeKey(MinValue_, MaxValue_)
		{
			ASSERTION(MinValue_ < MaxValue_);
		}
		explicit CRangeKey(const TType& Value_) :
			SRangeKey(Value_, Value_ + 1)
		{
		}
		bool operator < (const SRangeKey<TType>& rhs_) const
		{
			return (MaxValue <= rhs_.MinValue);
		}
		bool IsValid(TType Value_) const
		{
			return (Value_ >= MinValue && Value_ < MaxValue);
		}
	};

	using TLinkFunc = function<void(const CKey& Key_)>;
	using TLinkFailFunc = function<void(TPeerCnt PeerNum_, ENetRet NetRet_)>;
	using TUnLinkFunc = function<void(const CKey& Key_, ENetRet NetRet_)>;
	using TRecvFunc = function<void(const CKey& Key_, CStream& Stream_)>;
	using TWSAErrorLogFunc = function<void(const wstring& Msg_)>;
	using TRangeUID = CRangeKey<TUID>;

	__interface IIPEndPoint
	{
		EAddressFamily GetAddressFamily(void) const;
		CNamePort GetNamePort(void) const;
		SOCKADDR* GetSockAddrPtr(void) const;
		int32 GetAddrLen(void) const;
		string GetName(void) const;
		TPort GetPort(void) const;
		shared_ptr<IIPEndPoint> MakeEmptyIPEndPoint(void) const;
	};
	using TIPEndPoint = shared_ptr<IIPEndPoint>;
	using TIPEndPoints = list<TIPEndPoint>;
	struct SNetInfo
	{
		TSize MaxRcvBPS;
		milliseconds HBRcvDelay;
		milliseconds HBSndDelay;
		TWSAErrorLogFunc ErrorLogFunc;

		SNetInfo(TSize MaxRcvBPS_, milliseconds HBRcvDelay_, milliseconds HBSndDelay_, TWSAErrorLogFunc ErrorLogFunc_) :
			MaxRcvBPS(MaxRcvBPS_), HBRcvDelay(HBRcvDelay_), HBSndDelay(HBSndDelay_), ErrorLogFunc(ErrorLogFunc_)
		{
		}
	};

	__interface INet
	{
		bool IsLinked(TPeerCnt PeerNum_) const; // Certification completed and Fully Connected
		void Close(TPeerCnt PeerNum_);
		bool Close(const CKey& Key_);
		void CloseAll(void);
		void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_);
		bool WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_);
		TPeerCnt GetPeerCnt(void) const;
		milliseconds Latency(TPeerCnt PeerNum_) const;
		void Proc(void);
	};
	__interface IServer
	{
		void Certify(TPeerCnt PeerNum_);
		bool Certify(const CKey& Key_);
		bool IsCertified(TPeerCnt PeerNum_) const;
	};
	__interface IClient
	{
		bool IsConnecting(TPeerCnt PeerNum_) const; // IsLinked : Certification completed, IsConnecting : All Condition except Not Connected State (state of called Connect())
		CKey Connect(const CNamePort& NamePort_, TPeerCnt PeerNum_);
		CKey Connect(const CNamePort& NamePort_);
	};
}
