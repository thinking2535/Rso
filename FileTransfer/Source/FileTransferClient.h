#pragma once

#include "FileTransfer.h"
#include <Rso/Net/Client.h>

namespace rso
{
	namespace filetransfer
	{
		class CFileTransferClient : public CFileTransfer<CClient>
		{
			TLinkFunc _LinkFunc;
			TLinkFailFunc _LinkFailFunc;
			TUnLinkFunc _UnLinkFunc;

			void _Link(const CKey& Key_, const CIPPort& IPPort_)
			{
				여기서 로긴 프로토콜 날려?
				_LinkFunc(Key_, IPPort_);
			}
			void _LinkFail(TPeerCnt PeerNum_, ENetRet NetRet_)
			{
				_LinkFailFunc(PeerNum_, NetRet_);
			}
			void _UnLink(const CKey& Key_, ENetRet NetRet_)
			{
				_UnLinkFunc(Key_, NetRet_);
			}

		public:
			CFileTransferClient(
				TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TGetDoneFunc GetDoneFunc_, TPutDoneFunc PutDoneFunc_, TErrorFunc ErrorFunc_) :
				CFileTransfer(new CClient(1, milliseconds(), milliseconds(),
					std::bind(&CFileTransferClient::_Link, this, _1, _2),
					std::bind(&CFileTransferClient::_LinkFail, this, _1, _2),
					std::bind(&CFileTransferClient::_UnLink, this, _1, _2),
					std::bind(&CFileTransferClient::_Recv, this, _1, _2),
					false, 10240000, 10240000, 5),
					GetDoneFunc_, PutDoneFunc_, ErrorFunc_),
				_LinkFunc(LinkFunc_), _LinkFailFunc(LinkFailFunc_), _UnLinkFunc(UnLinkFunc_)
			{
			}
			inline CKey Connect(const CIPPort& IPPort_, TPeerCnt PeerNum_)
			{
				로그인 함수를 Connect 에 CStream 을 넣어서 합칠것.

				return _Net->Connect(IPPort_, PeerNum_);
			}
			inline CKey Connect(const CIPPort& IPPort_)
			{
				return _Net->Connect(IPPort_);
			}
		};
	}
}
