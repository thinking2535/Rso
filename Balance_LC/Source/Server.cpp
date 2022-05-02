#include "Server.h"

namespace rso
{
	namespace balance_lc
	{
		void CServer::_LinkA(const CKey& Key_)
		{
			_NetA.Send(Key_.PeerNum, EProto::SaServerOn, SSaServerOn(_ClientBindNamePortPub, _ClientCnt));
			_LinkFuncA(Key_);
		}

		void CServer::_LinkC(const CKey& Key_)
		{
			++_ClientCnt;
			_LinkFuncC(Key_);
			_NetC.Send(Key_.PeerNum, EProto::ScAllocated, SScAllocated());
		}
		void CServer::_UnLinkC(const CKey& Key_, ENetRet NetRet_)
		{
			--_ClientCnt;
			_UnLinkFuncC(Key_, NetRet_);
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
			_ClientBindNamePortPub(ClientBindNamePortPub_),
			_NetA(
				std::bind(&CServer::_LinkA, this, _1), LinkFailFuncA_, UnLinkFuncA_, RecvFuncA_,
				false, 1024000, 1024000,
				milliseconds(), milliseconds(), 1, 5,
				milliseconds(5000)),
			_NetC(
				AddressFamily_,
				std::bind(&CServer::_LinkC, this, _1),
				std::bind(&CServer::_UnLinkC, this, _1, _2),
				std::bind(&CServer::_RecvC, this, _1, _2),
				false, 10240000, 10240000,
				milliseconds(120000), milliseconds(), NetCThreadCnt_, 0,
				ClientBindNamePort_, SOMAXCONN),
			_LinkFuncA(LinkFuncA_),
			_LinkFuncC(LinkFuncC_), _UnLinkFuncC(UnLinkFuncC_), _RecvFuncC(RecvFuncC_)
		{
			_NetA.Connect(AllocatorNamePort_);
		}
		void CServer::Proc(void)
		{
			_NetA.Proc();
			_NetC.Proc();

			if (_SyncDelay.CheckAndNextLoose())
				_NetA.SendAll(EProto::SaClientCnt, SSaClientCnt(int32(_NetC.GetPeerCnt())));
		}
	}
}