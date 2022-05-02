#include "Server.h"

namespace rso::balance_rr
{
	void CServer::_LinkA(const CKey& Key_)
	{
		_NetA.Send(Key_.PeerNum, EProto::SaServerOn, SSaServerOn(_ClientBindNamePortPub));
		_LinkFuncA(Key_);
	}
	void CServer::_RecvA(const CKey& Key_, CStream& Stream_)
	{
		int32 Proto = 0;
		Stream_ >> Proto;

		switch (EProto(Proto))
		{
		case EProto::AsUserProto: return _RecvFuncA(Key_, Stream_);
		default: return _NetA.Close(Key_.PeerNum);
		}
	}

	void CServer::_LinkC(const CKey& Key_)
	{
		_LinkFuncC(Key_);
		_NetC.Send(Key_.PeerNum, EProto::ScAllocated, SScAllocated());
	}
	void CServer::_RecvC(const CKey& Key_, CStream& Stream_)
	{
		EProto ProtoNum;
		Stream_ >> ProtoNum;

		switch (ProtoNum)
		{
		case EProto::CsUserProto: return _RecvFuncC(Key_, Stream_);
		default: return _NetC.Close(Key_.PeerNum);
		}
	}
	CServer::CServer(
		EAddressFamily AddressFamily_,
		TLinkFunc LinkFuncA_, TLinkFailFunc LinkFailFuncA_, TUnLinkFunc UnLinkFuncA_, TRecvFunc RecvFuncA_,
		TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TRecvFunc RecvFuncC_,
		const CNamePort& AllocatorNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_) :
		_LinkFuncA(LinkFuncA_), _RecvFuncA(RecvFuncA_),
		_LinkFuncC(LinkFuncC_), _RecvFuncC(RecvFuncC_),
		_ClientBindNamePortPub(ClientBindNamePortPub_),
		_NetA(
			std::bind(&CServer::_LinkA, this, _1), LinkFailFuncA_, UnLinkFuncA_, std::bind(&CServer::_RecvA, this, _1, _2),
			false, 1024000, 1024000,
			milliseconds(), milliseconds(), 1, 5,
			milliseconds(5000)),
		_NetC(
			AddressFamily_,
			std::bind(&CServer::_LinkC, this, _1), UnLinkFuncC_, std::bind(&CServer::_RecvC, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(120000), milliseconds(), NetCThreadCnt_, 0,
			ClientBindNamePort_, SOMAXCONN)
	{
		_NetA.Connect(AllocatorNamePort_);
	}
	void CServer::Proc(void)
	{
		_NetA.Proc();
		_NetC.Proc();
	}
}