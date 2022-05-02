#include "Allocator.h"

namespace rso
{
	namespace balance_lc
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
				case EProto::SaClientCnt: return _RecvSaClientCnt(itServer, Stream_);
				case EProto::SaUserProto: return _RecvFuncS(Key_, Stream_);
				default: return _NetS.Close(Key_.PeerNum);
				}
			}
		}
		void CAllocator::_RecvSaServerOn(const CKey& Key_, CStream& Stream_)
		{
			SSaServerOn Proto;
			Stream_ >> Proto;

			_Servers.emplace_at(Key_.PeerNum, SServer(Proto, list<system_clock::time_point>()));
			_NetS.Certify(Key_.PeerNum);
			_LinkFuncS(Key_);
		}
		void CAllocator::_RecvSaClientCnt(_TServersIt itServer_, CStream& Stream_)
		{
			SSaClientCnt Proto;
			Stream_ >> Proto;

			itServer_->ClientCnt = Proto.ClientCnt;

			auto Now = system_clock::now();
			auto itTo = itServer_->EndTimes.begin();
			for (auto it = itServer_->EndTimes.begin(); it != itServer_->EndTimes.end(); ++it)
			{
				if ((*it) > Now)
					break;

				itTo = it;
			}

			itServer_->EndTimes.erase(itServer_->EndTimes.begin(), itTo);
		}

		void CAllocator::_LinkC(const CKey& Key_)
		{
			auto itServer = _Servers.begin();
			if (!itServer)
				return _NetC.Close(Key_.PeerNum);

			auto MinClientCnt = MaxValue<int32>();

			for (auto it = _Servers.begin(); it != _Servers.end(); ++it)
			{
				if (it->ClientCnt + static_cast<int32>(it->EndTimes.size()) < MinClientCnt)
				{
					itServer = it;
					MinClientCnt = decltype(MinClientCnt)(it->ClientCnt + it->EndTimes.size());
				}
			}

			itServer->EndTimes.emplace_back(system_clock::now() + c_EndTimeDuration);

			_NetC.Send(Key_.PeerNum, EProto::AcServerToConnect, SAcServerToConnect(itServer->ClientBindNamePortPub));
			_NetC.WillClose(Key_.PeerNum, milliseconds(3000));
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
}
