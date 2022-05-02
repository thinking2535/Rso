#include "Client.h"

namespace rso::mobile
{
	void CClient::_Close(_TPeersExtIt itPeerExt_, ENetRet NetRet_)
	{
		if (itPeerExt_->itPeerWillExpire != _PeersWillExpire.end())
		{
			_PeersWillExpire.erase(itPeerExt_->itPeerWillExpire);
			itPeerExt_->itPeerWillExpire = _PeersWillExpire.end();
		}

		auto ExtKey = itPeerExt_->Key;
		auto HaveBeenLinked = itPeerExt_->HaveBeenLinked;
		_PeersExt.erase(itPeerExt_);

		try
		{
			if (HaveBeenLinked)
				_UnLinkFunc(ExtKey, NetRet_);
			else
				_LinkFailFunc(ExtKey.PeerNum, NetRet_);
		}
		catch (...)
		{
		}
	}
	void CClient::_Close(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_Close(_PeersExt.get(PeerNum_), NetRet_);
	}
	void CClient::_Close(_TPeersExtIt itPeerExt_)
	{
		if (itPeerExt_->NetKey) // C# ������ �ڵ� ���߱� ����
		{
			auto itPeerNet = _PeersNet.get(itPeerExt_->NetKey.PeerNum);
			if (itPeerNet)
			{
				itPeerNet->ExtKey = CKey();

				// _Close �� ȣ��Ǳ� ���� _Close�� ȣ��Ǿ�� ������ _SendUnLink() �Ʒ��� _Close() �� �ִ� ������ itPeerExt_ �� ���⼭ ����ϱ� �����̰�
				// ���� _Net.Proc�� ȣ��Ǳ� �� ���� _Close �� ȣ����� �����Ƿ� ����
				if (_Net.IsLinked(itPeerExt_->NetKey.PeerNum))
					_SendUnLink(itPeerExt_->NetKey.PeerNum);
				else // Net�� ���� �� Connect �ϰ� LinkCallback �ޱ� �� �̶�� Send�� �� �����Ƿ� _Net.Close() �� ȣ��
					_Net.Close(itPeerExt_->NetKey.PeerNum);
			}
		}

		if (itPeerExt_->itPeerWillExpire != _PeersWillExpire.end())
		{
			_PeersWillExpire.erase(itPeerExt_->itPeerWillExpire);
			itPeerExt_->itPeerWillExpire = _PeersWillExpire.end();
		}

		_Close(itPeerExt_, ENetRet::UserClose);
	}
	void CClient::_Link(const CKey& Key_)
	{
		auto itPeerNet = _PeersNet.get(Key_.PeerNum); // _Net.Connect() ȣ�������̸� ������ _PeersNet �� �߰��� �����̹Ƿ� itPeerNet üũ ���ʿ�

		if (!itPeerNet->ExtKey) // C# ������ �ڵ� ���߱� ����
			return;

		auto itPeerExt = _PeersExt.get(itPeerNet->ExtKey.PeerNum);
		if (!itPeerExt)
			return;

		if (itPeerExt->HaveBeenLinked)
			_Net.Send(Key_.PeerNum, SHeaderCs(EProtoCs::ReLink), SReLinkCs(itPeerExt->ServerExtKey, itPeerExt->ProtoSeqMustRecv));
		else
			_Net.Send(Key_.PeerNum, SHeaderCs(EProtoCs::Link), SLinkCs());
	}
	void CClient::_LinkFail(TPeerCnt PeerNum_, ENetRet /*NetRet_*/)
	{
		auto itPeerNet = _PeersNet.get(PeerNum_);
		if (!itPeerNet)
			return;

		auto ExtKey = itPeerNet->ExtKey;
		_PeersNet.erase(itPeerNet);

		if (ExtKey) // C# ������ �ڵ� ���߱� ����.
		{
			auto itPeerExt = _PeersExt.get(ExtKey.PeerNum);
			if (itPeerExt)
			{
				itPeerExt->NetKey = CKey();
				itPeerExt->itPeerWillExpire->NeedToConnect = true;
			}
		}
	}
	void CClient::_UnLink(const CKey& Key_, ENetRet NetRet_)
	{
		auto itPeerNet = _PeersNet.get(Key_.PeerNum);
		if (!itPeerNet)
			return;

		auto ExtKey = itPeerNet->ExtKey; // C# ������ �ڵ� ���߱� ����
		_PeersNet.erase(itPeerNet);

		if (ExtKey)
		{
			auto itPeerExt = _PeersExt.get(ExtKey.PeerNum);
			if (itPeerExt)
			{
				if (NetRet_ == ENetRet::CertifyFail ||
					NetRet_ == ENetRet::InvalidPacket ||
					NetRet_ == ENetRet::SystemError ||
					NetRet_ == ENetRet::UserClose ||
					NetRet_ == ENetRet::KeepConnectTimeOut)
				{
					_Close(itPeerExt, NetRet_);
				}
				else
				{
					try
					{
						if (itPeerExt->itPeerWillExpire == _PeersWillExpire.end()) // LinkCallback ���� LinkSc, ReLinkSc �� ���� ���ϸ� �� �ڵ尡 false �� ��.
						{
							itPeerExt->itPeerWillExpire = _PeersWillExpire.emplace(_SPeerWillExpire(system_clock::now() + _KeepConnectDuration, itPeerExt->Key.PeerNum));
							_UnLinkFuncSoft(ExtKey, NetRet_);
						}

						itPeerExt->itPeerWillExpire->NeedToConnect = true;
						itPeerExt->NetKey = CKey();
					}
					catch (...)
					{
						_Close(itPeerExt, NetRet_);
					}
				}
			}
		}
	}
	void CClient::_Recv(const CKey& Key_, CStream& Stream_)
	{
		SHeaderSc Proto;
		Stream_ >> Proto;

		auto itPeerNet = _PeersNet.get(Key_.PeerNum);

		if (!itPeerNet->ExtKey)
			return;

		auto itPeerExt = _PeersExt.get(itPeerNet->ExtKey.PeerNum);
		if (!itPeerExt)
			return;

		if (itPeerExt->itPeerWillExpire != _PeersWillExpire.end()) // Not Linked Peer
		{
			switch (Proto.Proto)
			{
			case EProtoSc::Link: return _RecvLink(itPeerNet, itPeerExt, Stream_);
			case EProtoSc::ReLink: return _RecvReLink(Key_, itPeerNet, itPeerExt, Stream_);
			case EProtoSc::UnLink: return _RecvUnLink(Key_, Stream_);
			default: return _Net.Close(Key_.PeerNum);
			}
		}
		else // Linked Peer
		{
			switch (Proto.Proto)
			{
			case EProtoSc::UnLink: return _RecvUnLink(Key_, Stream_);
			case EProtoSc::Ack: return _RecvAck(itPeerExt);
			case EProtoSc::ReSend:
				Stream_.PopSize(sizeof(int32)); // no return
			case EProtoSc::UserProto: return _RecvUserProto(itPeerExt, Stream_);
			default:
				_Close(itPeerExt, ENetRet::InvalidPacket); // ������ Ŭ���̾�Ʈ�� �߸��� ���������� ������ �������� ���� ���� ����
				return _Net.Close(Key_.PeerNum);
			}
		}
	}
	void CClient::_RecvAck(_TPeersExtIt itPeerExt_)
	{
		itPeerExt_->RecvAck();
	}
	void CClient::_RecvLink(_TPeersNetIt /*itPeerNet_*/, _TPeersExtIt itPeerExt_, CStream& Stream_)
	{
		SLinkSc Proto;
		Stream_ >> Proto;

		itPeerExt_->ServerExtKey = Proto.ServerExtKey;
		_PeersWillExpire.erase(itPeerExt_->itPeerWillExpire);
		itPeerExt_->itPeerWillExpire = _PeersWillExpire.end();

		if (!itPeerExt_->HaveBeenLinked)
		{
			itPeerExt_->HaveBeenLinked = true;
			_LinkFunc(itPeerExt_->Key);
		}
	}
	void CClient::_RecvReLink(const CKey& NetKey_, _TPeersNetIt /*itPeerNet_*/, _TPeersExtIt itPeerExt_, CStream& Stream_)
	{
		SReLinkSc Proto;
		Stream_ >> Proto;

		_PeersWillExpire.erase(itPeerExt_->itPeerWillExpire);
		itPeerExt_->itPeerWillExpire = _PeersWillExpire.end();

		TProtoSeq MustDeleteCnt = (Proto.ServerProtoSeqMustRecv - itPeerExt_->ProtoSeqFirstForSendProtos); // 0 �̻��� ���� ���;� ��.(���ۿ����� ����(ū ���)�� �������� �������
		for (auto it = itPeerExt_->SendProtos.begin(); it != itPeerExt_->SendProtos.end();)
		{
			if (MustDeleteCnt == 0)
				break;

			auto itCheck = it;
			++it;

			itCheck->clear();
			itPeerExt_->SendProtos.erase(itCheck);
			--MustDeleteCnt;
		}
		itPeerExt_->ProtoSeqFirstForSendProtos = Proto.ServerProtoSeqMustRecv;

		for (auto& i : itPeerExt_->SendProtos)
			_Net.Send(NetKey_.PeerNum, SHeaderCs(EProtoCs::ReSend), i);

		_LinkFuncSoft(itPeerExt_->Key);
	}
	void CClient::_RecvUnLink(const CKey& NetKey_, CStream& Stream_)
	{
		SUnLinkSc Proto;
		Stream_ >> Proto;

		_Net.Close(NetKey_.PeerNum);
	}
	void CClient::_RecvUserProto(_TPeersExtIt itPeerExt_, CStream& Stream_)
	{
		++itPeerExt_->ProtoSeqMustRecv;

		auto ExtKey = itPeerExt_->Key;
		_RecvFunc(ExtKey, Stream_);

		auto itPeerExt = _PeersExt.get(ExtKey.PeerNum); // _RecvFunc ���ο��� throw or Close �� �� �����Ƿ� ��Ȯ��
		if (itPeerExt && itPeerExt->NetKey)
			_Net.Send(itPeerExt->NetKey.PeerNum, SHeaderCs(EProtoCs::Ack));
	}
	void CClient::_SendUnLink(TPeerCnt NetPeerNum_)
	{
		_Net.Send(NetPeerNum_, SHeaderCs(EProtoCs::UnLink), SUnLinkCs());
		_Net.WillClose(NetPeerNum_, milliseconds(3000));
	}

	CClient::CClient(
		TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
		bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
		milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, long ConnectTimeOutSec_,
		TLinkFunc LinkFuncSoft_, TUnLinkFunc UnLinkFuncSoft_, const milliseconds& KeepConnectDuration_) :
		CNet(LinkFunc_, UnLinkFunc_, RecvFunc_, KeepConnectDuration_),
		_LinkFailFunc(LinkFailFunc_), _LinkFuncSoft(LinkFuncSoft_), _UnLinkFuncSoft(UnLinkFuncSoft_),
		_Net(
			std::bind(&CClient::_Link, this, _1),
			std::bind(&CClient::_LinkFail, this, _1, _2),
			std::bind(&CClient::_UnLink, this, _1, _2),
			std::bind(&CClient::_Recv, this, _1, _2),
			NoDelay_, RecvBuffSize_, SendBuffSize_,
			HBRcvDelay_, HBSndDelay_, ThreadCnt_, ConnectTimeOutSec_)
	{
	}
	bool CClient::IsLinked(TPeerCnt PeerNum_) const
	{
		auto Peer = _PeersExt.get(PeerNum_);
		if (!Peer)
			return false;

		return Peer->HaveBeenLinked;
	}
	bool CClient::Close(const CKey& Key_)
	{
		auto itPeerExt = _PeersExt.get(Key_.PeerNum);
		if (!itPeerExt)
			return false;

		if (itPeerExt->Key.PeerCounter != Key_.PeerCounter)
			return false;

		_Close(itPeerExt);

		return true;
	}
	void CClient::CloseAll(void)
	{
		for (auto it = _PeersExt.begin(); it != _PeersExt.end(); )
		{
			auto itCheck = it;
			++it;

			_Close(itCheck);
		}
	}
	void CClient::WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
	{
		auto itPeerExt = _PeersExt.get(PeerNum_);

		if (itPeerExt->DoesWillClose())
			return;

		itPeerExt->WillClose(WaitMilliseconds_);
	}
	bool CClient::WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
	{
		auto itPeerExt = _PeersExt.get(Key_.PeerNum);
		if (!itPeerExt)
			return false;

		if (itPeerExt->Key.PeerCounter != Key_.PeerCounter)
			return false;

		if (itPeerExt->DoesWillClose())
			return false;

		itPeerExt->WillClose(WaitMilliseconds_);

		return true;
	}
	void CClient::Proc(void)
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

			for (auto it : _PeersWillExpire)
			{
				if (!it.NeedToConnect)
					continue;

				auto itPeerExt = _PeersExt.get(it.PeerExtNum);
				if (!itPeerExt)
					continue;

				_Connect(itPeerExt);
			}
		}
	}
	bool CClient::_Connect(_TPeersExtIt itPeerExt_)
	{
		try
		{
			auto itPeerNet = _PeersNet.emplace(_SPeerNet(itPeerExt_->Key));

			try
			{
				auto NetKey = _Net.Connect(itPeerExt_->NamePort, TPeerCnt(itPeerNet.Index()));
				if (!NetKey)
					THROWEX();

				itPeerExt_->NetKey = NetKey;
				itPeerExt_->itPeerWillExpire->NeedToConnect = false;
				return true;
			}
			catch (...)
			{
				_PeersNet.erase(itPeerNet);
				throw;
			}
		}
		catch (...)
		{
			return false;
		}
	}
	CKey CClient::Connect(const CNamePort& NamePort_, TPeerCnt PeerNum_)
	{
		try
		{
			auto itPeerExt = _PeersExt.emplace_at(PeerNum_, _SPeerExt(CKey(PeerNum_, _PeerCounter), NamePort_));

			try
			{
				itPeerExt->itPeerWillExpire = _PeersWillExpire.emplace(_SPeerWillExpire(system_clock::now() + _KeepConnectDuration, itPeerExt->Key.PeerNum));

				if (!_Connect(itPeerExt))
				{
					_PeersWillExpire.erase(itPeerExt->itPeerWillExpire);
					THROWEX();
				}

				++_PeerCounter;

				return itPeerExt->Key;
			}
			catch (...)
			{
				_PeersExt.erase(itPeerExt);
				throw;
			}
		}
		catch (...)
		{
			return CKey();
		}
	}
	CKey CClient::Connect(const CNamePort& NamePort_)
	{
		return Connect(NamePort_, TPeerCnt(_PeersExt.new_index()));
	}
}
