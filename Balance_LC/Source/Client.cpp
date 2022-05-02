#pragma once

#include "Client.h"

namespace rso
{
	namespace balance_lc
	{
		void CClient::_Link(const CKey& Key_)
		{
			_Servers[Key_.PeerNum].Key = Key_;
		}
		void CClient::_LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
		{
			auto itServer = _Servers.get(PeerNum_);
			itServer->NamePort.Clear();

			_Servers.erase(itServer);
			_LinkFailFunc(PeerNum_, NetRet_);
		}
		void CClient::_UnLink(const CKey& Key_, ENetRet NetRet_)
		{
			auto itServe = _Servers.get(Key_.PeerNum);

			switch (itServe->State)
			{
			case _EState::WillConnectServer:
				return;
			case _EState::Allocated:
				_UnLinkFunc(Key_, NetRet_);
				break;
			default:
				_LinkFailFunc(Key_.PeerNum, NetRet_);
				break;
			}

			_Servers.erase(itServe);
		}
		void CClient::_Recv(const CKey& Key_, CStream& Stream_)
		{
			EProto ProtoNum;
			Stream_ >> ProtoNum;

			switch (ProtoNum)
			{
			case EProto::AcServerToConnect: return _RecvAcServerToConnect(Key_, Stream_);
			case EProto::ScAllocated: return _RecvScAllocated(Key_, Stream_);
			case EProto::ScUserProto: return _RecvFunc(Key_, Stream_);
			default: return _Net.Close(Key_.PeerNum);
			}
		}
		void CClient::_RecvAcServerToConnect(const CKey& Key_, CStream& Stream_)
		{
			auto itServer = _Servers.get(Key_.PeerNum);

			SAcServerToConnect Proto;
			Stream_ >> Proto;

			itServer->State = _EState::WillConnectServer;
			_Net.Close(Key_.PeerNum);

			itServer->State = _EState::Null;
			itServer->NamePort = CNamePort(Proto.ClientBindNamePortPub);
			if (!_Net.Connect(CNamePort(Proto.ClientBindNamePortPub), Key_.PeerNum))
			{
				_Servers.erase(itServer);
				_LinkFailFunc(Key_.PeerNum, ENetRet::SystemError);
				return;
			}
		}
		void CClient::_RecvScAllocated(const CKey& Key_, CStream& /*Stream_*/)
		{
			auto itServer = _Servers.get(Key_.PeerNum);

			//SScAllocated Proto;
			//Stream_ >> Proto;

			itServer->State = _EState::Allocated;
			_LinkFunc(Key_);
		}
		CClient::CClient(
			TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_, const CNamePort& AllocatorNamePort_, const wstring& DataPath_) :
			_AllocatorNamePort(AllocatorNamePort_),
			_DataPath(DataPath_),
			_Net(
				std::bind(&CClient::_Link, this, _1),
				std::bind(&CClient::_LinkFail, this, _1, _2),
				std::bind(&CClient::_UnLink, this, _1, _2),
				std::bind(&CClient::_Recv, this, _1, _2),
				false, 1024000, 1024000,
				milliseconds(120000), milliseconds(60000), 1, 60),
			_LinkFunc(LinkFunc_), _LinkFailFunc(LinkFailFunc_), _UnLinkFunc(UnLinkFunc_), _RecvFunc(RecvFunc_)
		{
		}
		bool CClient::IsLinked(TPeerCnt PeerNum_) const
		{
			auto Server = _Servers.get(PeerNum_);
			if (!Server)
				return false;

			return (Server->State == _EState::Allocated);
		}
		bool CClient::Connect(TPeerCnt PeerNum_)
		{
			try
			{
				CNamePort AuthNamePort;
				auto DataFile = _DataPath + L"Allocator_" + to_wstring(PeerNum_) + L".dat";
				auto itServer = _Servers.emplace_at(PeerNum_, _SServer(DataFile));
				if (*itServer->NamePort)
					AuthNamePort = *itServer->NamePort;
				else
					AuthNamePort = _AllocatorNamePort;

				if (!_Net.Connect(AuthNamePort, PeerNum_))
				{
					_Servers.erase(PeerNum_);
					return false;
				}

				return true;
			}
			catch (...)
			{
				return false;
			}
		}
	}
}
