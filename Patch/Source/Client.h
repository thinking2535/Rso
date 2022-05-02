#pragma once

#include "ClientCore.h"
#include <Rso/Balance/Client.h>

namespace rso
{
	namespace patch
	{
		using namespace balance;

		using TDownloadChangedFunc = function<void(const string& ObjectName_, DWORD Received_, DWORD Total_)>;
		using TDownloadCompletedFunc = function<void(EHttpRet Ret_, const string& ObjectName_)>;

		class CClient : public CClientCore
		{
			struct _SPatchDataCount
			{
				SPatchData PatchData;
				int LeftCount = 0;

				_SPatchDataCount(SPatchData PatchData_, int LeftCount_) :
					PatchData(PatchData_),
					LeftCount(LeftCount_)
				{
				}
			};

			CHttp<> _Http;
			string _ServerName;
			bool _VersionPath;
			queue<_SPatchDataCount> _PatchDatas;
			TLinkFunc _LinkFunc;
			TDownloadChangedFunc _DownloadChangedFunc;
			TDownloadCompletedFunc _DownloadCompletedFunc;
			balance::CClient _NetB;

			void _LinkS(const CKey& Key_);
			void _RecvS(const CKey& Key_, CStream& Stream_);
			void _RecvScPatchData(const CKey& Key_, CStream& Stream_);
			void _DownloadChanged(size_t SessionIndex_, const string& ObjectName_, DWORD Received_, DWORD Total_);
			void _DownloadCompleted(size_t SessionIndex_, EHttpRet Ret_, const string& ObjectName_, const TBuffer& Buffer_);
			void _Patch(const SPatchData& Data_);

		public:
			CClient(
				const wstring& FileName_, const string& ServerName_, bool VersionPath_, const string& DataPath_,
				TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_,
				TDownloadChangedFunc DownloadChangedFunc_, TDownloadCompletedFunc DownloadCompletedFunc_);
			inline CKey Connect(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& MasterNamePort_)
			{
				return _NetB.Connect(PeerNum_, DataPath_, MasterNamePort_);
			}
			inline CKey Connect(const wstring& DataPath_, const CNamePort& MasterNamePort_)
			{
				return _NetB.Connect(DataPath_, MasterNamePort_);
			}
			inline void Proc(void)
			{
				_Http.Proc();
				_NetB.Proc();
			}
		};
	}
}
