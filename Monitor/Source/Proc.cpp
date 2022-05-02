#include "Proc.h"

namespace rso::monitor
{
	void CProc::_LinkA(const CKey& Key_)
	{
		Certify(Key_.PeerNum);
		SendAll(SPaHeader(EPaProto::ProcOn), SPaProcOn(_Proc.GetSProc()));
	}
	void CProc::_UnLinkA(const CKey& /*Key_*/, ENetRet /*NetRet_*/)
	{
	}
	void CProc::_RecvA(const CKey& /*Key_*/, CStream& Stream_)
	{
		CStream Stream;
		Stream_ >> Stream;

		SKey ClientKey;
		Stream_ >> ClientKey;

		SApHeader Header;
		Stream >> Header;

		switch (Header.Proto)
		{
		case EApProto::Stop: return _RecvApStop(ClientKey, Stream);
		case EApProto::Message: return _RecvApMessage(ClientKey, Stream);
		case EApProto::UserProto: return _CallbackUserProto(ClientKey, Stream);
		default: THROWEX();
		}
	}
	void CProc::_RecvApStop(const CKey& /*ClientKey_*/, CStream& Stream_)
	{
		SApStop Proto;
		Stream_ >> Proto;

		_CallbackStop(Proto.Message, seconds(Proto.SecondLeft));
	}
	void CProc::_RecvApMessage(const CKey& /*ClientKey_*/, CStream& Stream_)
	{
		SApMessage Proto;
		Stream_ >> Proto;

		_CallbackMessage(Proto.Message);
	}
	CProc::CProc(
		EAddressFamily AddressFamily_,
		TCallbackUserProto CallbackUserProto_, TCallbackStop CallbackStop_, TCallbackMessage CallbackMessage_,
		const wstring& ProcName_, const CNamePort& BindNamePort_, const wstring& Stat_) :
		CServer(
			AddressFamily_,
			std::bind(&CProc::_LinkA, this, _1),
			std::bind(&CProc::_UnLinkA, this, _1, _2),
			std::bind(&CProc::_RecvA, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(20000), milliseconds(9000), 1, 0,
			BindNamePort_, SOMAXCONN),
		_CallbackUserProto(CallbackUserProto_),
		_CallbackStop(CallbackStop_),
		_CallbackMessage(CallbackMessage_),
		_Proc(SProc(ProcName_, Stat_))
	{
	}
	void CProc::SetStat(const SKeyData& KeyData_)
	{
		_Proc.Stat[KeyData_.Key] = KeyData_.Data;

		SendAll(SPaHeader(EPaProto::Stat), SPaStat(KeyData_));
	}
	void CProc::SendNotifyToClient(const CKey& Key_, const wstring& Msg_)
	{
		SendAll(SPaHeader(EPaProto::NotifyToClient), SNotifyToClient(Key_, Msg_));
	}
}
