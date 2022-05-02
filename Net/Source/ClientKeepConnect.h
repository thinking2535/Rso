#pragma once

#include "Client.h"
#include <Rso/Base/ListVoid.h>

namespace rso::net
{
	class CClientKeepConnect : public CClient
	{
		using _TNodesMap = map<CNamePort, size_t>;
		using _TNodesMapIt = _TNodesMap::iterator;
		struct _SNode
		{
			_TNodesMapIt MapIt;
			CKey Key;

			_SNode(_TNodesMapIt MapIt_, const CKey& Key_) :
				MapIt(MapIt_), Key(Key_)
			{
			}
		};

		using _TNodes = CList<_SNode>;
		using _TNodesIt = _TNodes::iterator;

		CPeriod<milliseconds> _ConnectPeriod;
		_TNodesMap _NodesMap;
		_TNodes _Nodes;
		CListVoid _NodesDisconnected;
		TLinkFailFunc _LinkFailFunc;
		TUnLinkFunc _UnLinkFunc;

		void _Disconnected(TPeerCnt PeerNum_);
		void _LinkFailCallback(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkCallback(const CKey& Key_, ENetRet NetRet_);

	public:
		CClientKeepConnect(
			TLinkFunc LinkFunc_, TLinkFailFunc LinkFailFunc_, TUnLinkFunc UnLinkFunc_, TRecvFunc RecvFunc_,
			bool NoDelay_, int RecvBuffSize_, int SendBuffSize_,
			milliseconds HBRcvDelay_, milliseconds HBSndDelay_, size_t ThreadCnt_, long ConnectTimeOutSec_,
			const milliseconds& ReconnectPeriod_);
		CKey Connect(const CNamePort& NamePort_, TPeerCnt PeerNum_);
		inline CKey Connect(const CNamePort& NamePort_)
		{
			return Connect(NamePort_, TPeerCnt(_Nodes.new_index()));
		}
		void Connect(const list<SNamePort>& NamePorts_);
	private:
		void _Close(_TNodesIt It_);
	public:
		inline void Close(TPeerCnt PeerNum_)
		{
			_Close(_Nodes.get(PeerNum_));
		}
		bool Close(const CKey& Key_);
		void CloseAll(void);
		void Close(const CNamePort& NamePort_);
		void Proc(void);
	};
}