#pragma once

#include "Base.h"
#include <Rso/Balance/Client.h>

namespace rso::ranking
{
	using namespace balance;
	using namespace std;

	class CClient
	{
	public:
		using FCallback = function<void(const SRanking&)>;

	private:
		FCallback _fCallback;
		TLinkFunc _LinkFuncS;
		TLinkFailFunc _LinkFailFuncS;
		TUnLinkFunc _UnLinkFuncS;
		balance::CClient _NetB;

		void _LinkS(const CKey& Key_);
		void _LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkS(const CKey& Key_, ENetRet NetRet_);
		void _RecvS(const CKey& Key_, CStream& Stream_);
		void _RecvSRequest(const CKey& Key_, CStream& Stream_);

	public:
		CClient(FCallback fCallback_, TLinkFunc LinkFuncS_, TLinkFailFunc LinkFailFuncS_, TUnLinkFunc UnLinkFuncS_);
		bool IsLinked(void) const;
		bool Connect(const wstring& DataPath_, const CNamePort& MasterNamePort_);
		void Close(void);
		void Request(void);
		void Proc(void);
	};
}