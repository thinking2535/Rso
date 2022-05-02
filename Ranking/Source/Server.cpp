#include "Server.h"

namespace rso::ranking
{
	void CServer::_BroadCast(void)
	{
		TUsers RankingUsers;
		for (auto& i : _Ranking)
			RankingUsers.emplace_back(SUser(i.second, std::get<0>(i.first)->first, std::get<1>(i.first)->first));

		_NetB.BroadCast(RankingUsers, _Rewards);
	}

	void CServer::_LinkU(const CKey& Key_)
	{
		_LinkFuncU(Key_);
	}
	void CServer::_LinkFailU(TPeerCnt PeerNum_, ENetRet NetRet_)
	{
		_LinkFailFuncU(PeerNum_, NetRet_);
	}
	void CServer::_UnLinkU(const CKey& Key_, ENetRet NetRet_)
	{
		_UnLinkFuncU(Key_, NetRet_);
	}
	void CServer::_RecvU(const CKey& /*Key_*/, CStream& /*Stream_*/)
	{
	}
	void CServer::_RecvUBroadCast(const CKey& /*Key_*/, CStream& Stream_)
	{
		TUsers RankingUsers;
		Stream_ >> RankingUsers;

		_Ranking.clear();

		for (auto& i : RankingUsers)
			_Ranking.emplace(i, i.UID, i.Point);

		_Rewards.clear();
		Stream_ >> _Rewards;
		_BroadCast();
	}
	void CServer::_LinkD(const CKey& Key_)
	{
		_BroadCast();
		_LinkFuncD(Key_);
	}
	void CServer::_UnLinkD(const CKey& Key_, ENetRet NetRet_)
	{
		_UnLinkFuncD(Key_, NetRet_);
	}
	void CServer::_RecvD(const CKey& /*Key_*/, CStream& /*Stream_*/)
	{
	}
	void CServer::_LinkC(const CKey& /*Key_*/)
	{
	}
	void CServer::_UnLinkC(const CKey& /*Key_*/, ENetRet /*NetRet_*/)
	{
	}
	void CServer::_RecvC(const CKey& Key_, CStream& Stream_)
	{
		EProto ProtoNum;
		Stream_ >> ProtoNum;

		switch (ProtoNum)
		{
		case EProto::RequestCs: _RecvCRequest(Key_, Stream_); break;
		default: return _NetB.Close(Key_.PeerNum);
		}
	}
	void CServer::_RecvCRequest(const CKey& Key_, CStream& Stream_)
	{
		SRequestCs Proto;
		Stream_ >> Proto;

		SRanking Ranking;

		for (auto& i : _Ranking.operator() < 1 > ())
		{
			auto itRanking = _Ranking.get(i.second);
			Ranking.Users.emplace_back(SUser(
				itRanking->second,
				std::get<0>(itRanking->first)->first, // UID
				std::get<1>(itRanking->first)->first)); // Point
		}

		Ranking.RewardedRanking = -1;
		auto itReward = _Rewards.find(Proto.UID);
		if (itReward != _Rewards.end())
			Ranking.RewardedRanking = itReward->second;

		_NetB.Send(Key_.PeerNum, EProto::RequestSc, Ranking);
	}
	CServer::CServer(
		TLinkFunc LinkFuncU_, TLinkFailFunc LinkFailFuncU_, TUnLinkFunc UnLinkFuncU_,
		TLinkFunc LinkFuncD_, TUnLinkFunc UnLinkFuncD_,
		const SOption& Option_) :
		_LinkFuncU(LinkFuncU_), _LinkFailFuncU(LinkFailFuncU_), _UnLinkFuncU(UnLinkFuncU_),
		_LinkFuncD(LinkFuncD_), _UnLinkFuncD(UnLinkFuncD_),
		_Option(Option_),
		_NetB(
			EAddressFamily::INET,
			std::bind(&CServer::_LinkU, this, _1),
			std::bind(&CServer::_LinkFailU, this, _1, _2),
			std::bind(&CServer::_UnLinkU, this, _1, _2),
			std::bind(&CServer::_RecvU, this, _1, _2),
			std::bind(&CServer::_RecvUBroadCast, this, _1, _2),
			std::bind(&CServer::_LinkD, this, _1),
			std::bind(&CServer::_UnLinkD, this, _1, _2),
			std::bind(&CServer::_RecvD, this, _1, _2),
			std::bind(&CServer::_LinkC, this, _1),
			std::bind(&CServer::_UnLinkC, this, _1, _2),
			std::bind(&CServer::_RecvC, this, _1, _2),
			CNamePort(Option_.MasterNamePort),
			5, 100, 4,
			CNamePort(Option_.ChildBindPort),
			CNamePort(Option_.ClientBindPort),
			CNamePort(Option_.ClientBindNamePortPub))
	{
	}
	void CServer::Proc(void)
	{
		_NetB.Proc();
	}
}