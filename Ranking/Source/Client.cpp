#include "Client.h"

namespace rso::ranking
{
	void CClient::_LinkS(const CKey& Key_)
	{
		_LinkFuncS(Key_);
	}
	void CClient::_LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_LinkFailFuncS(PeerNum_, NetRet_);
	}
	void CClient::_UnLinkS(const CKey& Key_, ENetRet NetRet_)
	{
		_UnLinkFuncS(Key_, NetRet_);
	}
	void CClient::_RecvS(const CKey& Key_, CStream& Stream_)
	{
		EProto ProtoNum;
		Stream_ >> ProtoNum;

		switch (ProtoNum)
		{
		case EProto::RequestSc: _RecvSRequest(Key_, Stream_); break;
		default: return _NetB.Close(Key_.PeerNum);
		}
	}
	void CClient::_RecvSRequest(const CKey& /*Key_*/, CStream& Stream_)
	{
		SRanking Ranking;
		Stream_ >> Ranking;
		_fCallback(Ranking);
	}
	CClient::CClient(FCallback fCallback_, TLinkFunc LinkFuncS_, TLinkFailFunc LinkFailFuncS_, TUnLinkFunc UnLinkFuncS_) :
		_fCallback(fCallback_),
		_LinkFuncS(LinkFuncS_),
		_LinkFailFuncS(LinkFailFuncS_),
		_UnLinkFuncS(UnLinkFuncS_),
		_NetB(
			std::bind(&CClient::_LinkS, this, _1),
			std::bind(&CClient::_LinkFailS, this, _1, _2),
			std::bind(&CClient::_UnLinkS, this, _1, _2),
			std::bind(&CClient::_RecvS, this, _1, _2))
	{
	}
	bool CClient::IsLinked(void) const
	{
		return _NetB.IsLinked(0);
	}
	bool CClient::Connect(const wstring& DataPath_, const CNamePort& MasterNamePort_)
	{
		return (bool)_NetB.Connect(0, DataPath_, MasterNamePort_);
	}
	void CClient::Close(void)
	{
		_NetB.CloseAll();
	}
	void CClient::Request(void)
	{
		_NetB.Send(0, EProto::RequestCs, SRequestCs());
	}
	void CClient::Proc(void)
	{
		_NetB.Proc();
	}
}
