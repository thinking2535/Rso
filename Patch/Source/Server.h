#pragma once

#include "Base.h"
#include <Rso/Balance/Server.h>

namespace rso
{
	namespace patch
	{
		using namespace balance;

		// SServerData 데이터 구조
		// TVersion MainVersion = 0;
		// vector<SFiles> Patches; index : Sub Version,  data : 받을 파일명.  0 Index 의 파일들이 최신 MainVersion 의 모든파일목록
		class CServer
		{
		protected:
			bool _IsMaster = false;
			bool _IsReceivedData = false;
			SServerData _Data;
			inline SVersion _GetVersion(void) const
			{
				return SVersion(_Data.MainVersion, TVersion(_Data.Patches.size()));
			}
			SPatchData _GetPatchData(const SVersion& Version_) const;
			void _Update(const SUpdateData& Data_);

		private:
			balance::CServer _NetB;

			bool _IsValidData(void) const noexcept;
			void _LinkU(const CKey& /*Key_*/) {}
			void _LinkFailU(TPeerCnt /*PeerNum_*/, ENetRet /*NetRet_*/) {}
			void _UnLinkU(const CKey& /*Key_*/, ENetRet /*NetRet_*/) {}
			void _RecvU(const CKey& Key_, CStream& Stream_);
			void _RecvUdData(const CKey& Key_, CStream& Stream_);
			void _RecvUdUpdate(const CKey& Key_, CStream& Stream_);
			void _RecvUBroadCast(const CKey& /*Key_*/, CStream& /*Stream_*/) {}
			void _LinkD(const CKey& Key_);
			void _UnLinkD(const CKey& /*Key_*/, ENetRet /*NetRet_*/) {}
			void _RecvD(const CKey& /*Key_*/, CStream& /*Stream_*/) {}
			void _LinkC(const CKey& /*Key_*/) {}
			void _UnLinkC(const CKey& /*Key_*/, ENetRet /*NetRet_*/) {}
			void _RecvC(const CKey& /*Key_*/, CStream& /*Stream_*/);
			void _RecvCsLogin(const CKey& /*Key_*/, CStream& /*Stream_*/);

		public:
			CServer(
				EAddressFamily AddressFamily_,
				const CNamePort& MasterNamePort_, TPeerCnt ChildCntMax_, TPeerCnt ErrorCnt_, const CNamePort& ChildBindNamePort_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_);
			void Proc(void);
		};
	}
}
