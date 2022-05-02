#include "Client.h"

namespace rso
{
	namespace patch
	{
		void CClient::_LinkS(const CKey& Key_)
		{
			_NetB.Send(Key_.PeerNum, SHeader(EProto::CsLogin), SCsLogin(_Data->Version));
			_LinkFunc(Key_);
		}
		void CClient::_RecvS(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			try
			{
				switch (Header.Proto)
				{
				case EProto::ScPatchData: return _RecvScPatchData(Key_, Stream_);
				default: throw 0;
				}
			}
			catch (...)
			{
				_NetB.Close(Key_.PeerNum);
			}
		}
		void CClient::_RecvScPatchData(const CKey& /*Key_*/, CStream& Stream_)
		{
			SPatchData Proto;
			Stream_ >> Proto;

			_Patch(Proto);
			_NetB.CloseAll();
		}
		void CClient::_DownloadChanged(size_t /*SessionIndex_*/, const string& ObjectName_, DWORD Received_, DWORD Total_)
		{
			_DownloadChangedFunc(ObjectName_, Received_, Total_);
		}
		void CClient::_DownloadCompleted(size_t /*SessionIndex_*/, EHttpRet Ret_, const string& ObjectName_, const TBuffer& Buffer_)
		{
			if (Ret_ != EHttpRet::Ok)
				THROWEX();

			auto FullPath = _DataPathFull;
			FullPath += ObjectName_;
			create_directories(FullPath.parent_path());
			ofstream f(FullPath, ios::binary);
			if (!f.is_open())
				THROWEX();

			f.write(Buffer_.data(), Buffer_.size());

			_DownloadCompletedFunc(Ret_, ObjectName_);

			--_PatchDatas.front().LeftCount;

			if (_PatchDatas.front().LeftCount == 0)
			{
				_PatchCore(_PatchDatas.front().PatchData);
				_PatchDatas.pop();
			}
		}
		void CClient::_Patch(const SPatchData& Data_)
		{
			if (_Data->Version.Main != Data_.Version.Main)
			{
				for (auto& p : directory_iterator(_DataPathFull))
					remove_all(p);
			}

			int32 LeftCount = 0;

			for (auto& i : Data_.Files)
			{
				auto Key = WCSToMBS(i.first);

				if (i.second.IsAdded)
				{
					auto ObjectName = (_VersionPath ? (to_string(i.second.SubVersion) + "/") : string()) += Key;
					replace(ObjectName.begin(), ObjectName.end(), '\\', '/');
					_Http.Push(SInObj("", _ServerName, 80, "", "", EMethod::Get, ObjectName, "", false));
					++LeftCount;
				}
				else
				{
					remove(_DataPathFull /= Key);
				}
			}

			if (LeftCount > 0)
				_PatchDatas.emplace(_SPatchDataCount(Data_, LeftCount));
		}

		CClient::CClient(
			const wstring& FileName_, const string& ServerName_, bool VersionPath_, const string& DataPath_,
			TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_,
			TDownloadChangedFunc DownloadChangedFunc_, TDownloadCompletedFunc DownloadCompletedFunc_) :
			CClientCore(FileName_, DataPath_),
			_Http(1, std::bind(&CClient::_DownloadChanged, this, _1, _2, _3, _4), std::bind(&CClient::_DownloadCompleted, this, _1, _2, _3, _4)),
			_ServerName(ServerName_),
			_VersionPath(VersionPath_),
			_LinkFunc(LinkFunc_),
			_DownloadChangedFunc(DownloadChangedFunc_),
			_DownloadCompletedFunc(DownloadCompletedFunc_),
			_NetB(std::bind(&CClient::_LinkS, this, _1), LinkFailFunc_, UnLinkFunc_, std::bind(&CClient::_RecvS, this, _1, _2))
		{
		}
	}
}
