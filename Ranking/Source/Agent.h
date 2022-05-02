#pragma once

#include "Base.h"
#include <Rso/Net/ClientKeepConnect.h>

namespace rso::ranking
{
	using namespace net;

	class CAgent
	{
		TLinkFunc _LinkFuncS;
		TLinkFailFunc _LinkFailFuncS;
		TUnLinkFunc _UnLinkFuncS;
		CKey _ServerKey;
		CClientKeepConnect _Net;
		SInfoSa _Info;

		bool _IsValidInfo(void) const;
		void _LinkS(const CKey& Key_);
		void _LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkS(const CKey& Key_, ENetRet NetRet_);
		void _RecvS(const CKey& Key_, CStream& Stream_);
		void _RecvSaInfo(const CKey& Key_, CStream& Stream_);
		void _RecvSaUserMinPoint(const CKey& Key_, CStream& Stream_);
	public:
		CAgent(TLinkFunc LinkFuncS_, TLinkFailFunc LinkFailFuncS_, TUnLinkFunc UnLinkFuncS_);
		bool IsLinked(void) const;
		bool Connect(const CNamePort& MasterNamePort_);
		void SendUpdate(TUsers Changed_);
		int64 GetRewardCounter(TUID UID_);
		void Proc(void);
	};
}
