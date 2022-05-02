#include "Server.h"

namespace rso
{
	namespace patch
	{
		// Client 및 Agent 의 Version 에 맞는 데이터를 반환
		SPatchData CServer::_GetPatchData(const SVersion& Version_) const
		{
			const auto Version = _GetVersion();
			if (Version_ == Version)
			{
				return SPatchData(Version, TFiles());
			}
			else if (Version_.Main == Version.Main && Version_.Sub >= 0 && Version_.Sub < Version.Sub)
			{
				return SPatchData(Version, _Data.Patches.at(Version_.Sub).Files);
			}
			else
			{
				if (_Data.Patches.empty())
					return SPatchData(Version, TFiles());

				return SPatchData(Version, _Data.Patches.front().Files);
			}
		}
		void CServer::_Update(const SUpdateData& Data_)
		{
			if (Data_.IsReset)
			{
				++_Data.MainVersion;
				_Data.Patches.clear();
			}

			_Data.Patches.emplace_back(SFiles());

			// Patches 에 갱신
			for (auto& p : _Data.Patches)
			{
				for (auto& i : Data_.Files)
					p.Files.insert_or_assign(i.first, SFile(i.second, (TVersion)_Data.Patches.size()));
			}

			_NetB.SendAllD(SHeader(EProto::UdUpdate), Data_);
		}
		bool CServer::_IsValidData(void) const noexcept
		{
			return (_IsMaster || _IsReceivedData);
		}
		void CServer::_RecvU(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			try
			{
				switch (Header.Proto)
				{
				case EProto::UdData: return _RecvUdData(Key_, Stream_);
				case EProto::UdUpdate: return _RecvUdUpdate(Key_, Stream_);
				default: throw 0;
				}
			}
			catch (...)
			{
				_NetB.CloseU(Key_.PeerNum);
			}
		}
		void CServer::_RecvUdData(const CKey& /*Key_*/, CStream& Stream_)
		{
			Stream_ >> _Data;
			_IsReceivedData = true;

			_NetB.SendAllD(SHeader(EProto::UdData), _Data);
		}
		void CServer::_RecvUdUpdate(const CKey& /*Key_*/, CStream& Stream_)
		{
			SUpdateData Proto;
			Stream_ >> Proto;

			_Update(Proto);
		}
		void CServer::_LinkD(const CKey& Key_)
		{
			if (_IsValidData())
				_NetB.SendD(Key_.PeerNum, SHeader(EProto::UdData), _Data);
		}
		void CServer::_RecvC(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			try
			{
				if (_IsValidData())
				{
					switch (Header.Proto)
					{
					case EProto::CsLogin: return _RecvCsLogin(Key_, Stream_);
					default: throw 0;
					}
				}
				else
				{
					throw 0;
				}
			}
			catch (...)
			{
				_NetB.Close(Key_.PeerNum);
			}
		}
		void CServer::_RecvCsLogin(const CKey& Key_, CStream& Stream_)
		{
			SCsLogin Proto;
			Stream_ >> Proto;

			if (Proto.Version.Sub < 0)
				throw 0;

			_NetB.Send(Key_.PeerNum, SHeader(EProto::ScPatchData), _GetPatchData(Proto.Version));
			_NetB.WillClose(Key_.PeerNum, milliseconds(10000)); // 클라는 ScPatchData 를 받으면 끊음.
		}

		CServer::CServer(
			EAddressFamily AddressFamily_,
			const CNamePort& MasterNamePort_, TPeerCnt ChildCntMax_, TPeerCnt ErrorCnt_, const CNamePort& ChildBindNamePort_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_) :
			_IsMaster(!MasterNamePort_),
			_NetB(
				AddressFamily_,
				std::bind(&CServer::_LinkU, this, _1),
				std::bind(&CServer::_LinkFailU, this, _1, _2),
				std::bind(&CServer::_UnLinkU, this, _1, _2),
				std::bind(&CServer::_RecvU, this, _1, _2),
				std::bind(&CServer::_RecvUBroadCast, this, _1, _2),
				std::bind(&CServer::_LinkD, this, _1),
				std::bind(&CServer::_UnLinkD, this, _1, _2),
				std::bind(&CServer::_RecvD, this, _1, _2),
				std::bind(&CServer::_LinkC, this, _1),
				std::bind(&CServer::_UnLinkC, this, _1, _2),
				std::bind(&CServer::_RecvC, this, _1, _2),
				MasterNamePort_, ChildCntMax_, ErrorCnt_, 1, ChildBindNamePort_, ClientBindNamePort_, ClientBindNamePortPub_)
		{
		}
		void CServer::Proc(void)
		{
			_NetB.Proc();
		}
	}
}
