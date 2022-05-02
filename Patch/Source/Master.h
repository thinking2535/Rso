#pragma once

#include "Server.h"
#include <Rso/Base/ListVoid.h>

namespace rso
{
	namespace patch
	{
		class CMaster : public CServer
		{
			wstring _FileName;
			TUnLinkFunc _UnLinkFuncA;
			TLoginFunc _LoginFuncA;
			CListVoid _Agents;
			net::CServer _NetA;

			void _Save(void);
			void _UnLinkA(const CKey& Key_, ENetRet NetRet_);
			void _RecvA(const CKey& Key_, CStream& Stream_);
			void _RecvAmLogin(const CKey& Key_, CStream& Stream_);
			void _RecvAmUpdate(const CKey& Key_, CStream& Stream_);

		public:
			CMaster(
				EAddressFamily AddressFamily_,
				const wstring& FileName_,
				TPeerCnt ChildCntMax_, TPeerCnt ErrorCnt_, const CNamePort& ChildBindNamePort_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_,
				TLinkFunc LinkFuncA_, TUnLinkFunc UnLinkFuncA_, TLoginFunc LoginFuncA_, const CNamePort& AgentBindNamePort_);
		private:
			void _Certify(TPeerCnt PeerNum_, const SVersion& Version_);
		public:
			void Certify(TPeerCnt PeerNum_, const SVersion& Version_);
			bool Certify(const CKey& Key_, const SVersion& Version_);
			void Proc(void);
			inline void CloseAgent(TPeerCnt PeerNum_) { _NetA.Close(PeerNum_); }
		};
	}
}
