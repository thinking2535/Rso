#include "Agent.h"

namespace rso::ranking
{
	bool CAgent::_IsValidInfo(void) const
	{
		return (_Info.Counter > 0);
	}
	void CAgent::_LinkS(const CKey& Key_)
	{
		_ServerKey = Key_;
		_LinkFuncS(Key_);
	}
	void CAgent::_LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_LinkFailFuncS(PeerNum_, NetRet_);
	}
	void CAgent::_UnLinkS(const CKey& Key_, ENetRet NetRet_)
	{
		new (&_ServerKey) CKey();
		new (&_Info) SInfoSa();
		_UnLinkFuncS(Key_, NetRet_);
	}
	void CAgent::_RecvS(const CKey& Key_, CStream& Stream_)
	{
		EProto ProtoNum;
		Stream_ >> ProtoNum;

		switch (ProtoNum)
		{
		case EProto::InfoSa: _RecvSaInfo(Key_, Stream_); break;
		case EProto::UserMinPointSa: _RecvSaUserMinPoint(Key_, Stream_); break;
		default: return _Net.Close(Key_.PeerNum);
		}
	}
	void CAgent::_RecvSaInfo(const CKey& /*Key_*/, CStream& Stream_)
	{
		Stream_ >> _Info;
	}
	void CAgent::_RecvSaUserMinPoint(const CKey& /*Key_*/, CStream& Stream_)
	{
		Stream_ >> _Info.UserMinPoint;
	}
	CAgent::CAgent(TLinkFunc LinkFuncS_, TLinkFailFunc LinkFailFuncS_, TUnLinkFunc UnLinkFuncS_) :
		_LinkFuncS(LinkFuncS_), _LinkFailFuncS(LinkFailFuncS_), _UnLinkFuncS(UnLinkFuncS_),
		_Net(
			std::bind(&CAgent::_LinkS, this, _1),
			std::bind(&CAgent::_LinkFailS, this, _1, _2),
			std::bind(&CAgent::_UnLinkS, this, _1, _2),
			std::bind(&CAgent::_RecvS, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 5,
			milliseconds(5000))
	{
	}
	bool CAgent::IsLinked(void) const
	{
		return _Net.IsLinked(0);
	}
	bool CAgent::Connect(const CNamePort& MasterNamePort_)
	{
		return (bool)_Net.Connect(MasterNamePort_, 0);
	}
	void CAgent::SendUpdate(TUsers Changed_)
	{
		if (!IsLinked())
			return;

		_Net.Send(_ServerKey.PeerNum, EProto::UpdateAs, Changed_);
	}
	int64 CAgent::GetRewardCounter(TUID UID_) // return 0 : fail
	{
		if (!_IsValidInfo())
			return 0;

		if (_Info.Rewards.find(UID_) == _Info.Rewards.end())
			return 0;

		return _Info.Counter;
	}
	void CAgent::Proc(void)
	{
		_Net.Proc();
	}
}
