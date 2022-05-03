#pragma once

#include "Base.h"
#include <Rso/Net/Server.h>

namespace rso::monitor
{
	using namespace net;

	class CProc : public CServer
	{
		using TCallbackStop = function<void(const wstring& Message_, const seconds& SecondLeft_)>;
		using TCallbackMessage = function<void(const wstring& Message_)>;

	private:
		TLinkFunc _LinkAFunc;
		TUnLinkFunc _UnLinkAFunc;
		TCallbackUserProto _CallbackUserProto;
		TCallbackStop _CallbackStop;
		TCallbackMessage _CallbackMessage;
		SProcValue _Proc;

		void _LinkA(const CKey& Key_);
		void _UnLinkA(const CKey& Key_, ENetRet NetRet_);
		void _RecvA(const CKey& Key_, CStream& Stream_);
		void _RecvApStop(const CKey& ClientKey_, CStream& Stream_);
		void _RecvApMessage(const CKey& ClientKey_, CStream& Stream_);

	public:
		CProc(
			EAddressFamily AddressFamily_,
			TLinkFunc LinkAFunc_, TUnLinkFunc UnLinkAFunc_,
			TCallbackUserProto CallbackUserProto_, TCallbackStop CallbackStop_, TCallbackMessage CallbackMessage_,
			const wstring& ProcName_, const CNamePort& BindNamePort_, const wstring& Stat_);

		void SetStat(const SKeyData& KeyData_);
		void SendNotifyToClient(const CKey& Key_, const wstring& Msg_);
	};
}
