#include "Allocator.h"

namespace rso::balance_rr
{
	void CAllocator::_LinkS(const CKey& /*Key_*/)
	{
	}
	void CAllocator::_UnLinkS(const CKey& Key_, ENetRet NetRet_)
	{
		if (_Servers.erase(Key_.PeerNum))
			_UnLinkFuncS(Key_, NetRet_);
	}
	void CAllocator::_RecvS(const CKey& Key_, CStream& Stream_)
	{
		EProto ProtoNum;
		Stream_ >> ProtoNum;

		auto itServer = _Servers.get(Key_.PeerNum);
		if (!itServer)
		{
			switch (ProtoNum)
			{
			case EProto::SaServerOn: return _RecvSaServerOn(Key_, Stream_);
			default: return _NetS.Close(Key_.PeerNum);
			}
		}
		else
		{
			switch (ProtoNum)
			{
			case EProto::SaUserProto: return _RecvFuncS(Key_, Stream_);
			default: return _NetS.Close(Key_.PeerNum);
			}
		}
	}
	void CAllocator::_RecvSaServerOn(const CKey& Key_, CStream& Stream_)
	{
		SSaServerOn Proto;
		Stream_ >> Proto;

		_Servers.emplace_at(Key_.PeerNum, SServer(Proto));
		_NetS.Certify(Key_.PeerNum);
		_LinkFuncS(Key_);
	}

	void CAllocator::_LinkC(const CKey& Key_)
	{
		auto itServer = _Servers.cycle();
		if (!itServer)
		{
			itServer = _Servers.cycle();
			if (!itServer)
				return _NetC.Close(Key_.PeerNum);
		}


		_NetC.Send(Key_.PeerNum, EProto::AcServerToConnect, SAcServerToConnect(itServer->ClientBindNamePortPub));
		_NetC.WillClose(Key_.PeerNum, milliseconds(1000));
	}
	void CAllocator::_UnLinkC(const CKey& /*Key_*/, ENetRet /*NetRet_*/)
	{
	}
	void CAllocator::_RecvC(const CKey& Key_, CStream& /*Stream_*/)
	{
		return _NetC.Close(Key_.PeerNum);
	}
	CAllocator::CAllocator(
		EAddressFamily AddressFamily_,
		TLinkFunc LinkFuncS_, TUnLinkFunc UnLinkFuncS_, TRecvFunc RecvFuncS_,
		const CNamePort& ServerBindNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_) :
		_NetS(
			AddressFamily_,
			std::bind(&CAllocator::_LinkS, this, _1),
			std::bind(&CAllocator::_UnLinkS, this, _1, _2),
			std::bind(&CAllocator::_RecvS, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(), milliseconds(), 1, 0,
			ServerBindNamePort_, SOMAXCONN),
		_NetC(
			AddressFamily_,
			std::bind(&CAllocator::_LinkC, this, _1),
			std::bind(&CAllocator::_UnLinkC, this, _1, _2),
			std::bind(&CAllocator::_RecvC, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(), milliseconds(), NetCThreadCnt_, 0,
			ClientBindNamePort_, SOMAXCONN),
		_LinkFuncS(LinkFuncS_), _UnLinkFuncS(UnLinkFuncS_), _RecvFuncS(RecvFuncS_)
	{
	}
	void CAllocator::Proc(void)
	{
		_NetS.Proc();
		_NetC.Proc();
	}
}
