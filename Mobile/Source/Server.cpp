#include "Server.h"

namespace rso::mobile
{
	void CServer::_Close(_TPeersExtIt itPeerExt_, ENetRet NetRet_)
	{
		auto ExtKey = itPeerExt_->Key;
		_PeersExt.erase(itPeerExt_);

		try
		{
			_UnLinkFunc(ExtKey, NetRet_);
		}
		catch (...)
		{
		}
	}
	void CServer::_Close(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_Close(_PeersExt.get(PeerNum_), NetRet_);
	}
	void CServer::_Close(_TPeersExtIt itPeerExt_)
	{
		if (itPeerExt_->NetKey)
		{
			auto itPeerNet = _PeersNet.get(itPeerExt_->NetKey.PeerNum);
			if (itPeerNet)
			{
				itPeerNet->ExtKey = CKey();
				_SendUnLink(itPeerExt_->NetKey.PeerNum); // _Close 이 호출되기 전에 _Close가 호출되어야 하지만 _SendUnLink() 아래에 _Close() 가 있는 이유는 itPeerExt_ 를 여기서 사용하기 위함이고
															   // 또한 _Net.Proc이 호출되기 전 까지 _Close 는 호출되지 않으므로 안전
			}
		}

		if (itPeerExt_->itPeerWillExpire)
		{
			_PeersWillExpire.erase(itPeerExt_->itPeerWillExpire);
			itPeerExt_->itPeerWillExpire = _PeersWillExpire.end();
		}

		_Close(itPeerExt_, ENetRet::UserClose);
	}
	void CServer::_Link(const CKey& Key_)
	{
		try
		{
			_PeersNet.emplace_at(Key_.PeerNum, _SPeerNet());
		}
		catch (...)
		{
			_Net.Close(Key_.PeerNum);
		}
	}
	void CServer::_UnLink(const CKey& Key_, ENetRet NetRet_)
	{
		auto itPeerNet = _PeersNet.get(Key_.PeerNum);
		if (!itPeerNet) // _Link 에서 _PeersNet.emplace 실패할 수 있으므로
			return;

		auto itPeerExt = _PeersExt.get(itPeerNet->ExtKey.PeerNum);

		_PeersNet.erase(itPeerNet);

		if (itPeerExt)
		{
			if (NetRet_ == ENetRet::CertifyFail ||
				NetRet_ == ENetRet::InvalidPacket ||
				NetRet_ == ENetRet::SystemError ||
				NetRet_ == ENetRet::UserClose)
			{
				_Close(itPeerExt, NetRet_);
			}
			else
			{
				try
				{
					if (itPeerExt->itPeerWillExpire == _PeersWillExpire.end())
						itPeerExt->itPeerWillExpire = _PeersWillExpire.emplace(_SPeerWillExpire(system_clock::now() + _KeepConnectDuration, itPeerExt->Key.PeerNum));

					itPeerExt->NetKey = CKey();
				}
				catch (...)
				{
					_Close(itPeerExt, NetRet_);
				}
			}
		}
	}
	void CServer::_Recv(const CKey& Key_, CStream& Stream_)
	{
		SHeaderCs Proto;
		Stream_ >> Proto;

		auto itPeerNet = _PeersNet.get(Key_.PeerNum);

		if (!itPeerNet->ExtKey) // Not Linked Peer
		{
			switch (Proto.Proto)
			{
			case EProtoCs::Link: return _RecvLink(Key_, itPeerNet, Stream_);
			case EProtoCs::ReLink: return _RecvReLink(Key_, itPeerNet, Stream_);
			default: return _Net.Close(Key_.PeerNum);
			}
		}
		else // Linked Peer
		{
			auto itPeerExt = _PeersExt.get(itPeerNet->ExtKey.PeerNum);

			switch (Proto.Proto)
			{
			case EProtoCs::UnLink: return _RecvUnLink(Key_, Stream_);
			case EProtoCs::Ack: return _RecvAck(itPeerExt);
			case EProtoCs::ReSend:
				Stream_.PopSize(sizeof(int32));
			case EProtoCs::UserProto: return _RecvUserProto(itPeerExt, Stream_);
			default:
				_Close(itPeerExt, ENetRet::InvalidPacket); // 인증된 클라이언트가 잘못된 프로토콜을 보내면 접속해제 절차 없이 삭제
				return _Net.Close(Key_.PeerNum);
			}
		}
	}
	void CServer::_RecvAck(_TPeersExtIt itPeerExt_)
	{
		itPeerExt_->RecvAck();
	}
	void CServer::_RecvLink(const CKey& NetKey_, _TPeersNetIt itPeerNet_, CStream& Stream_)
	{
		SLinkCs Proto;
		Stream_ >> Proto;

		auto itPeerExt = _PeersExt.emplace(_SPeerExt(CKey(TPeerCnt(_PeersExt.new_index()), _PeerCounter), NetKey_, _Net.GetNamePort((TPeerCnt)itPeerNet_.Index())));
		auto ExtKey = itPeerExt->Key;

		itPeerNet_->ExtKey = ExtKey;
		++_PeerCounter;

		itPeerExt = _PeersExt.get(ExtKey.PeerNum); // _LinkFunc 내부에서 throw or Close 할 수 있으므로 재확인
		if (itPeerExt && itPeerExt->NetKey)
			_Net.Send(itPeerExt->NetKey.PeerNum, SHeaderSc(EProtoSc::Link), SLinkSc(ExtKey));

		_LinkFunc(ExtKey);
	}
	void CServer::_RecvReLink(const CKey& NetKey_, _TPeersNetIt itPeerNet_, CStream& Stream_)
	{
		SReLinkCs Proto;
		Stream_ >> Proto;

		try
		{
			auto itPeerExt = _PeersExt.get(Proto.ServerExtKey.PeerNum);
			if (!itPeerExt)
				throw 0;

			if (itPeerExt->Key != Proto.ServerExtKey)
				throw 0;

			auto itPeerNet = _PeersNet.get(itPeerExt->NetKey.PeerNum);
			if (itPeerNet) // 이미 연결된 PeerNet이 있으면 연결 해제
			{
				itPeerNet->ExtKey = CKey();
				_Net.Close(itPeerExt->NetKey.PeerNum);
			}
			else
			{
				if (itPeerExt->itPeerWillExpire != _PeersWillExpire.end()) // 무조건 참이어야 하지만 로직이 바뀔수도 있으므로
				{
					_PeersWillExpire.erase(itPeerExt->itPeerWillExpire);
					itPeerExt->itPeerWillExpire = _PeersWillExpire.end();
				}
			}

			itPeerExt->NetKey = NetKey_;
			itPeerNet_->ExtKey = itPeerExt->Key;
			itPeerExt->NamePort = _Net.GetNamePort((TPeerCnt)itPeerNet_.Index());

			if (itPeerExt->Certified)
				_Net.Certify(NetKey_.PeerNum);

			TProtoSeq MustDeleteCnt = (Proto.ClientProtoSeqMustRecv - itPeerExt->ProtoSeqFirstForSendProtos); // 0 이상의 값이 나와야 함.(조작에의해 음수(큰 양수)가 나오더라도 상관없음
			for (auto it = itPeerExt->SendProtos.begin(); it != itPeerExt->SendProtos.end();)
			{
				if (MustDeleteCnt == 0)
					break;

				auto itCheck = it;
				++it;

				itCheck->clear();
				itPeerExt->SendProtos.erase(itCheck);
				--MustDeleteCnt;
			}
			itPeerExt->ProtoSeqFirstForSendProtos = Proto.ClientProtoSeqMustRecv;

			_Net.Send(NetKey_.PeerNum, SHeaderSc(EProtoSc::ReLink), SReLinkSc(itPeerExt->ProtoSeqMustRecv));

			for (auto& i : itPeerExt->SendProtos)
				_Net.Send(NetKey_.PeerNum, SHeaderSc(EProtoSc::ReSend), i);
		}
		catch (...)
		{
			_SendUnLink(NetKey_.PeerNum);
		}
	}
	void CServer::_RecvUnLink(const CKey& NetKey_, CStream& Stream_)
	{
		SUnLinkCs Proto;
		Stream_ >> Proto;

		_Net.Close(NetKey_.PeerNum);
	}
	void CServer::_RecvUserProto(_TPeersExtIt itPeerExt_, CStream& Stream_)
	{
		++itPeerExt_->ProtoSeqMustRecv;

		auto ExtKey = itPeerExt_->Key;
		_RecvFunc(ExtKey, Stream_);

		auto itPeerExt = _PeersExt.get(ExtKey.PeerNum); // _RecvFunc 내부에서 throw or Close 할 수 있으므로 재확인
		if (itPeerExt && itPeerExt->NetKey)
			_Net.Send(itPeerExt->NetKey.PeerNum, SHeaderSc(EProtoSc::Ack));
	}
	void CServer::_SendUnLink(TPeerCnt NetPeerNum_)
	{
		_Net.Send(NetPeerNum_, SHeaderSc(EProtoSc::UnLink), SUnLinkSc());
		_Net.WillClose(NetPeerNum_, milliseconds(3000));
	}

	CServer::CServer(
		EAddressFamily AddressFamily_,
		TLinkFunc LinkFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
		bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
		milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, TSize MaxRcvBPS_,
		const CNamePort& BindInfo_, int BackLog_, const milliseconds& KeepConnectDuration_) :
		CNet(LinkFunc_, UnLinkFunc_, RecvFunc_, KeepConnectDuration_),
		_Net(
			AddressFamily_,
			std::bind(&CServer::_Link, this, _1),
			std::bind(&CServer::_UnLink, this, _1, _2),
			std::bind(&CServer::_Recv, this, _1, _2),
			NoDelay_, RecvBuffSize_, SendBuffSize_,
			HBRcvDelay_, HBSndDelay_, ThreadCnt_, MaxRcvBPS_,
			BindInfo_, BackLog_)
	{
	}
	bool CServer::Close(const CKey& Key_)
	{
		auto itPeerExt = _PeersExt.get(Key_.PeerNum);
		if (!itPeerExt)
			return false;

		if (itPeerExt->Key.PeerCounter != Key_.PeerCounter)
			return false;

		_Close(itPeerExt);

		return true;
	}
	void CServer::CloseAll(void)
	{
		for (auto it = _PeersExt.begin(); it != _PeersExt.end(); )
		{
			auto itCheck = it;
			++it;

			_Close(itCheck);
		}
	}
	void CServer::WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
	{
		auto itPeerExt = _PeersExt.get(PeerNum_);

		if (itPeerExt->DoesWillClose())
			return;

		itPeerExt->CloseTime = system_clock::now() + WaitMilliseconds_;
	}
	bool CServer::WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
	{
		auto itPeerExt = _PeersExt.get(Key_.PeerNum);
		if (!itPeerExt)
			return false;

		if (itPeerExt->Key.PeerCounter != Key_.PeerCounter)
			return false;

		if (itPeerExt->DoesWillClose())
			return false;

		itPeerExt->CloseTime = system_clock::now() + WaitMilliseconds_;

		return true;
	}
	void CServer::Proc(void)
	{
		_Net.Proc();

		if (_PeriodUnLinked.CheckAndNextLoose())
		{
			auto Now = system_clock::now();

			for (auto it = _PeersExt.begin(); it; )
			{
				auto itCheck = it;
				++it;

				if (itCheck->DoesWillClose() && itCheck->CloseTime < Now)
					_Close(itCheck);
			}

			for (auto it = _PeersWillExpire.begin(); it != _PeersWillExpire.end(); )
			{
				auto itCheck = it;
				++it;

				if (itCheck->ExpireTime > Now)
					break;

				_Close(itCheck->PeerExtNum, ENetRet::KeepConnectTimeOut);
				_PeersWillExpire.erase(itCheck);
			}
		}
	}
	void CServer::Certify(TPeerCnt PeerNum_)
	{
		_PeersExt[PeerNum_].Certified = true;

		if (_PeersExt[PeerNum_].NetKey)
			_Net.Certify(_PeersExt[PeerNum_].NetKey.PeerNum);
	}
	bool CServer::Certify(const CKey& Key_)
	{
		auto itPeerExt = _PeersExt.get(Key_.PeerNum);
		if (!itPeerExt)
			return false;

		if (itPeerExt->Key.PeerCounter != Key_.PeerCounter)
			return false;

		itPeerExt->Certified = true;

		if (itPeerExt->NetKey)
			_Net.Certify(itPeerExt->NetKey.PeerNum);

		return true;
	}
}