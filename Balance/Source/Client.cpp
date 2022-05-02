#include "Client.h"

namespace rso
{
	namespace balance
	{
		bool CClient::_Connect(_TServersIt Server_)
		{
			const auto ConnectNamePort = Server_->PopConnectNamePort();
			if (ConnectNamePort)
			{
				if (_NetS.Connect(ConnectNamePort, (TPeerCnt)Server_.Index()))
					return true;
			}

			const auto PeerNum = (TPeerCnt)Server_.Index();
			_Servers.erase(Server_);
			_LinkFailFuncS(PeerNum, ENetRet::ConnectFail);

			return false;
		}
		void CClient::_LinkS(const CKey& Key_)
		{
			_NetS.Send(Key_.PeerNum, SHeader(EProto::CsConnect), SCsConnect(_Servers[Key_.PeerNum].PrevConnectNamePort));
			_Servers[Key_.PeerNum].PrevConnectNamePort = _NetS.GetNamePort(Key_.PeerNum);
		}
		void CClient::_LinkFailS(TPeerCnt PeerNum_, ENetRet /*NetRet_*/)
		{
			_Connect(PeerNum_);
		}
		void CClient::_UnLinkS(const CKey& Key_, ENetRet NetRet_)
		{
			auto It = _Servers.get(Key_.PeerNum);
			if (It->Logon)
			{
				_Servers.erase(Key_.PeerNum);
				_UnLinkFuncS(Key_, NetRet_);
			}
			else
			{
				_Connect(Key_.PeerNum);
			}
		}
		void CClient::_RecvS(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			if (!_Servers[Key_.PeerNum].Logon)
			{
				switch (Header.Proto)
				{
				case EProto::ScNewParent: return _RecvScNewParent(Key_, Stream_);
				case EProto::ScAllocated: return _RecvScAllocated(Key_, Stream_);
				default: return _NetS.Close(Key_.PeerNum);
				}
			}
			else
			{
				switch (Header.Proto)
				{
				case EProto::ScUserProto: return _RecvFuncS(Key_, Stream_);
				default: return _NetS.Close(Key_.PeerNum);
				}
			}
		}
		void CClient::_RecvScNewParent(const CKey& Key_, CStream& Stream_)
		{
			SScNewParent Proto;
			Stream_ >> Proto;

			_Servers[Key_.PeerNum].ConnectNamePorts.emplace(Proto.ClientBindNamePortPub);
			_NetS.Close(Key_.PeerNum);
		}
		void CClient::_RecvScAllocated(const CKey& Key_, CStream& Stream_)
		{
			SScAllocated Proto;
			Stream_ >> Proto;

			_Servers[Key_.PeerNum].Login();
			_LinkFuncS(Key_);
		}
		CClient::CClient(
			TLinkFunc LinkFuncS_, TLinkFailFunc LinkFailFuncS_, TUnLinkFunc UnLinkFuncS_, TRecvFunc RecvFuncS_) :
			_LinkFuncS(LinkFuncS_), _LinkFailFuncS(LinkFailFuncS_), _UnLinkFuncS(UnLinkFuncS_), _RecvFuncS(RecvFuncS_),
			_NetS(
				std::bind(&CClient::_LinkS, this, _1),
				std::bind(&CClient::_LinkFailS, this, _1, _2),
				std::bind(&CClient::_UnLinkS, this, _1, _2),
				std::bind(&CClient::_RecvS, this, _1, _2),
				false, 1024000, 1024000,
				milliseconds(120000), milliseconds(60000), 1, 60)
		{
		}
		bool CClient::IsLinked(TPeerCnt PeerNum_) const
		{
			const auto Server = _Servers.get(PeerNum_);
			if (!Server)
				return false;

			return Server->Logon;
		}
		CKey CClient::Connect(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& MasterNamePort_)
		{
			auto FullPath = absolute(DataPath_);

			auto FileName = FullPath.wstring() + L"\\Data_" + to_wstring(PeerNum_) + L".bin";

			_Servers.emplace_at(PeerNum_, _SServer(_PeerCounter, MasterNamePort_, FileName));
			if (!_Connect(PeerNum_))
				return CKey();

			return CKey(PeerNum_, _PeerCounter++);
		}
	}
}