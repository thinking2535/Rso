#include "Agent.h"

namespace rso
{
	namespace patch
	{
		void CAgent::_LinkM(const CKey& Key_)
		{
			_NetM.Send(Key_.PeerNum, SHeader(EProto::AmLogin), SAmLogin(_ID, _PW, _Data->Version));
			_LinkFunc(Key_);
		}
		void CAgent::_LinkFailM(TPeerCnt PeerNum_, ENetRet NetRet_)
		{
			_LinkFailFunc(PeerNum_, NetRet_);
		}
		void CAgent::_UnLinkM(const CKey& Key_, ENetRet NetRet_)
		{
			_UnLinkFunc(Key_, NetRet_);
		}
		void CAgent::_RecvM(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			try
			{
				switch (Header.Proto)
				{
				case EProto::MaPatchData: return _RecvMaPatchData(Key_, Stream_);
				default: throw 0;
				}
			}
			catch (...)
			{
				_NetM.Close(Key_.PeerNum);
			}
		}
		void CAgent::_RecvMaPatchData(const CKey& /*Key_*/, CStream& Stream_)
		{
			SPatchData Proto;
			Stream_ >> Proto;

			_PatchCore(Proto);
			_DataFunc(Proto);
		}

		CAgent::CAgent(
			const wstring& FileName_, const string& DataPath_,
			const CNamePort& MasterNamePort_, TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_,
			TDataFunc DataFunc_) :
			CClientCore(FileName_, DataPath_),
			_MasterNamePort(MasterNamePort_),
			_LinkFunc(LinkFunc_), _LinkFailFunc(LinkFailFunc_), _UnLinkFunc(UnLinkFunc_), _DataFunc(DataFunc_),
			_NetM(
				std::bind(&CAgent::_LinkM, this, _1),
				std::bind(&CAgent::_LinkFailM, this, _1, _2),
				std::bind(&CAgent::_UnLinkM, this, _1, _2),
				std::bind(&CAgent::_RecvM, this, _1, _2),
				false, 1024000, 1024000,
				milliseconds(30000), milliseconds(20000), 1, 5)
		{
		}
		void CAgent::Update(bool IsReset_, const TUpdateFiles& Files_)
		{
			_NetM.SendAll(SHeader(EProto::AmUpdate), SUpdateData(IsReset_, Files_));
		}
	}
}
