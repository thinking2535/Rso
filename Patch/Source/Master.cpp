#include "Master.h"

namespace rso
{
	namespace patch
	{
		void CMaster::_Save(void)
		{
			(CStream() << _Data).SaveFile(_FileName);
		}
		void CMaster::_UnLinkA(const CKey& Key_, ENetRet NetRet_)
		{
			_Agents.erase(Key_.PeerNum);
			_UnLinkFuncA(Key_, NetRet_);
		}
		void CMaster::_RecvA(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			try
			{
				auto itAgent = _Agents.get(Key_.PeerNum);
				if (!itAgent)
				{
					switch (Header.Proto)
					{
					case EProto::AmLogin: return _RecvAmLogin(Key_, Stream_);
					default: throw 0;
					}
				}
				else
				{
					switch (Header.Proto)
					{
					case EProto::AmUpdate: return _RecvAmUpdate(Key_, Stream_);
					default: throw 0;
					}
				}
			}
			catch (...)
			{
				_NetA.Close(Key_.PeerNum);
			}
		}
		void CMaster::_RecvAmLogin(const CKey& Key_, CStream& Stream_)
		{
			SAmLogin Proto;
			Stream_ >> Proto;

			_LoginFuncA(Key_, Proto);
		}
		void CMaster::_RecvAmUpdate(const CKey& /*Key_*/, CStream& Stream_)
		{
			SUpdateData Proto; // CS 버전과 통일 위해 TFiles 를 그냥 쓰지 않고 SFiles 로 한번 감싸서 사용
			Stream_ >> Proto;

			if (Proto.Files.empty())
				return;

			_Update(Proto);
			_Save();
			_NetA.SendAll(SHeader(EProto::MaPatchData), SPatchData(_GetVersion(), _Data.Patches.back().Files));
		}
		CMaster::CMaster(
			EAddressFamily AddressFamily_,
			const wstring& FileName_,
			TPeerCnt ChildCntMax_, TPeerCnt ErrorCnt_, const CNamePort& ChildBindNamePort_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_,
			TLinkFunc LinkFuncA_, TUnLinkFunc UnLinkFuncA_, TLoginFunc LoginFuncA_, const CNamePort& AgentBindNamePort_) :
			_FileName(FileName_),
			CServer(AddressFamily_, CNamePort(), ChildCntMax_, ErrorCnt_, ChildBindNamePort_, ClientBindNamePort_, ClientBindNamePortPub_),
			_UnLinkFuncA(UnLinkFuncA_), _LoginFuncA(LoginFuncA_),
			_NetA(
				AddressFamily_,
				LinkFuncA_, std::bind(&CMaster::_UnLinkA, this, _1, _2), std::bind(&CMaster::_RecvA, this, _1, _2),
				true, 10240000, 10240000,
				milliseconds(30000), milliseconds(20000), 1, 0,
				AgentBindNamePort_, SOMAXCONN)
		{
			CStream Stream;

			try
			{
				Stream.LoadFile(FileName_);
			}
			catch (...) // 읽기 실패시 파일 초기화
			{
				_Save();
				return;
			}

			Stream >> _Data;
		}
		void CMaster::_Certify(TPeerCnt PeerNum_, const SVersion& Version_)
		{
			_Agents.emplace_at(PeerNum_);
			_NetA.Send(PeerNum_, SHeader(EProto::MaPatchData), _GetPatchData(Version_));
		}
		void CMaster::Certify(TPeerCnt PeerNum_, const SVersion& Version_)
		{
			_NetA.Certify(PeerNum_);
			_Certify(PeerNum_, Version_);
		}
		bool CMaster::Certify(const CKey& Key_, const SVersion& Version_)
		{
			if (!_NetA.Certify(Key_))
				return false;

			_Certify(Key_.PeerNum, Version_);

			return true;
		}
		void CMaster::Proc(void)
		{
			CServer::Proc();
			_NetA.Proc();
		}
	}
}
