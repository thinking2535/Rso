#pragma once

#include "Base.h"
#include <Rso/Base/MultiIndexMap.h>
#include <Rso/Balance/Server.h>

namespace rso::ranking
{
	using namespace balance;

	class CServer
	{
	protected:
		using _TRanking = CMultiIndexMap<SUserCore, unique_index<TUID>, non_unique_index<int64, greater<int64>>>; // UID, Point

		TLinkFunc _LinkFuncU;
		TLinkFailFunc _LinkFailFuncU;
		TUnLinkFunc _UnLinkFuncU;
		TLinkFunc _LinkFuncD;
		TUnLinkFunc _UnLinkFuncD;
		SOption _Option;
		balance::CServer _NetB;
		bool _RewardLoaded = false;
		_TRanking _Ranking;
		TRewards _Rewards;

		void _BroadCast(void);
		void _LinkU(const CKey& Key_);
		void _LinkFailU(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkU(const CKey& Key_, ENetRet NetRet_);
		void _RecvU(const CKey& Key_, CStream& Stream_);
		void _RecvUBroadCast(const CKey& Key_, CStream& Stream_);
		void _LinkD(const CKey& Key_);
		void _UnLinkD(const CKey& Key_, ENetRet NetRet_);
		void _RecvD(const CKey& Key_, CStream& Stream_);
		void _LinkC(const CKey& Key_);
		void _UnLinkC(const CKey& Key_, ENetRet NetRet_);
		void _RecvC(const CKey& Key_, CStream& Stream_);
		void _RecvCRequest(const CKey& Key_, CStream& Stream_);

	public:
		CServer(
			TLinkFunc LinkFuncU_, TLinkFailFunc LinkFailFuncU_, TUnLinkFunc UnLinkFuncU_,
			TLinkFunc LinkFuncD_, TUnLinkFunc UnLinkFuncD_,
			const SOption& Option_);
		void Proc(void);
	};
}