#pragma once

#include "FileTransfer.h"
#include <Rso/Net/Server.h>
#include <Rso/Base/ListVoid.h>

namespace rso
{
	namespace filetransfer
	{
		class CFileTransferServer : public CFileTransfer<CServer>
		{
		public:
			using TLoginFunc = function<void(const CKey& Key_, CStream& Stream_)>;

		private:
			TLinkFunc _LinkFunc;
			TUnLinkFunc _UnLinkFunc;
			TLoginFunc _LoginFunc;
			CListVoid _Clients;

			void _Link(const CKey& Key_, const CIPPort& IPPort_)
			{
				_LinkFunc(Key_, IPPort_);
			}
			void _UnLink(const CKey& Key_, ENetRet NetRet_)
			{
				_UnLinkFunc(Key_, NetRet_);
			}

			�θ��� _Recv���� Ŭ���� Login ���������� ó���ؾ� �ϴµ� ��� ��Ӱ��踦 �ѱ�?


		public:
			CFileTransferServer(
				TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TLoginFunc LoginFunc_, TGetDoneFunc GetDoneFunc_, TPutDoneFunc PutDoneFunc_, TErrorFunc ErrorFunc_, const CIPPort& BindInfo_) :
				CFileTransfer(new CServer(1, 0, milliseconds(), milliseconds(),
					std::bind(&CFileTransferServer::_Link, this, _1, _2),
					std::bind(&CFileTransferServer::_UnLink, this, _1, _2),
					std::bind(&CFileTransferServer::_Recv, this, _1, _2),
					BindInfo_, false, 10240000, 10240000, SOMAXCONN),
					GetDoneFunc_, PutDoneFunc_, ErrorFunc_),
				_LinkFunc(LinkFunc_), _UnLinkFunc(UnLinkFunc_), _LoginFunc(LoginFunc_)
			{
			}
			void Certify(TPeerCnt PeerNum_)
			{
				_Clients.emplace_at(PeerNum_);
				_Net->Certify(PeerNum_);
			}
		};
	}
}
