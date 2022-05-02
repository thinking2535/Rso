#pragma once

#include "ClientCore.h"
#include <Rso/Net/Client.h>

namespace rso
{
	namespace patch
	{
		using TDataFunc = function<void(const SPatchData & PatchData_)>;

		class CAgent : public CClientCore // Agent는 분산된 데이터가 아닌 오리지날 데이터를 받아야 하기 때문에 CClient를 상속받지 않음.
		{
			CNamePort _MasterNamePort;
			wstring _ID;
			wstring _PW;
			TLinkFunc _LinkFunc;
			TLinkFailFunc _LinkFailFunc;
			TUnLinkFunc _UnLinkFunc;
			TDataFunc _DataFunc;
			net::CClient _NetM;

			void _LinkM(const CKey& Key_);
			void _LinkFailM(TPeerCnt PeerNum_, ENetRet NetRet_);
			void _UnLinkM(const CKey& Key_, ENetRet NetRet_);
			void _RecvM(const CKey& Key_, CStream& Stream_);
			void _RecvMaPatchData(const CKey& Key_, CStream& Stream_);

		public:
			CAgent(
				const wstring& FileName_, const string& DataPath_,
				const CNamePort& MasterNamePort_, TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_,
				TDataFunc DataFunc_);
			inline void _SetAccount(const wstring& ID_, const wstring& PW_)
			{
				_ID = ID_;
				_PW = PW_;
			}
			inline CKey Connect(const wstring& ID_, const wstring& PW_, TPeerCnt PeerNum_)
			{
				_SetAccount(ID_, PW_);
				return _NetM.Connect(_MasterNamePort, PeerNum_);
			}
			inline CKey Connect(const wstring& ID_, const wstring& PW_)
			{
				_SetAccount(ID_, PW_);
				return _NetM.Connect(_MasterNamePort);
			}
			inline void Proc(void)
			{
				_NetM.Proc();
			}
			inline void Close(void)
			{
				_NetM.CloseAll();
			}
			void Update(bool IsReset_, const TUpdateFiles& Files_);
		};
	}
}
