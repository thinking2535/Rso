#include "ClientKeepConnect.h"

namespace rso::net
{
	void CClientKeepConnect::_Disconnected(TPeerCnt PeerNum_)
	{
		if (_Nodes.get(PeerNum_))
			_NodesDisconnected.emplace_at(PeerNum_); // will not fail
	}
	void CClientKeepConnect::_LinkFailCallback(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_Disconnected(PeerNum_);
		_LinkFailFunc(PeerNum_, NetRet_);
	}
	void CClientKeepConnect::_UnLinkCallback(const CKey& Key_, ENetRet NetRet_)
	{
		_Disconnected(Key_.PeerNum);
		_UnLinkFunc(Key_, NetRet_);
	}

	CClientKeepConnect::CClientKeepConnect(
		TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
		bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
		milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, long ConnectTimeOutSec_,
		const milliseconds& ReconnectPeriod_) :
		CClient(
			LinkFunc_, std::bind(&CClientKeepConnect::_LinkFailCallback, this, _1, _2), std::bind(&CClientKeepConnect::_UnLinkCallback, this, _1, _2), RecvFunc_,
			NoDelay_, RecvBuffSize_, SendBuffSize_,
			HBRcvDelay_, HBSndDelay_, ThreadCnt_, ConnectTimeOutSec_),
		_ConnectPeriod(ReconnectPeriod_),
		_LinkFailFunc(LinkFailFunc_), _UnLinkFunc(UnLinkFunc_)
	{
	}
	CKey CClientKeepConnect::Connect(const CNamePort& NamePort_, TPeerCnt PeerNum_)
	{
		try
		{
			_NodesDisconnected.reserve(PeerNum_); // emplace 실패하지 않도록 미리 예약

			auto ib = _NodesMap.emplace(NamePort_, PeerNum_);
			if (!ib.second)
				THROWEX();

			try
			{
				auto it = _Nodes.emplace_at(size_t(PeerNum_), _SNode(ib.first, CKey()));
				it->Key = CClient::Connect(NamePort_, PeerNum_);
				if (!it->Key)
				{
					_Nodes.erase(it);
					THROWEX();
				}

				return it->Key;
			}
			catch (...)
			{
				_NodesMap.erase(ib.first);
				throw;
			}
		}
		catch (...)
		{
			return CKey();
		}
	}
	void CClientKeepConnect::Connect(const list<SNamePort>& NamePorts_)
	{
		CloseAll();

		for (auto& it : NamePorts_)
			Connect(CNamePort(it));
	}
	void CClientKeepConnect::_Close(_TNodesIt It_)
	{
		_NodesMap.erase(It_->MapIt);

		auto Index = It_.Index();
		_Nodes.erase(It_);
		_NodesDisconnected.erase(Index);
		CClient::Close(TPeerCnt(Index));
	}
	bool CClientKeepConnect::Close(const CKey& Key_)
	{
		auto It = _Nodes.get(Key_.PeerNum);
		if (!It)
			return false;

		if (It->Key.PeerCounter != Key_.PeerCounter)
			return false;

		_Close(It);

		return true;
	}
	void CClientKeepConnect::CloseAll(void)
	{
		for (auto it = _Nodes.begin(); it != _Nodes.end(); it = _Nodes.begin())
			_Close(it);

		_NodesDisconnected.clear();
	}
	void CClientKeepConnect::Close(const CNamePort& NamePort_)
	{
		auto it = _NodesMap.find(NamePort_);
		if (it == _NodesMap.end())
			return;

		auto PeerNum = TPeerCnt(it->second);

		_NodesMap.erase(it);
		_Nodes.erase(PeerNum);
		_NodesDisconnected.erase(PeerNum);
		CClient::Close(PeerNum);
	}
	void CClientKeepConnect::Proc(void)
	{
		CClient::Proc();

		if (_ConnectPeriod.CheckAndNextLoose())
		{
			for (auto it = _NodesDisconnected.begin(); it != _NodesDisconnected.end();)
			{
				auto itCheck = it;
				++it;

				try
				{
					if (!CClient::Connect(_Nodes[itCheck.Index()].MapIt->first, TPeerCnt(itCheck.Index())))
						break;

					_NodesDisconnected.erase(itCheck);
				}
				catch (...)
				{
					break;
				}
			}
		}
	}
}