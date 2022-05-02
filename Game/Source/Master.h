// Master 서버의 DB 임무는 Create, Login, Check(계정 존재 유무 확인)

// Session 관리 알고리즘
// 세션은 UID (MainUID) 당 1개만 가짐
// Master에서는 Session을 생성하고, Server에 전파하고, 삭제는 반대로 Server에서 Master로 전파됨

// Master에서 생성된 Session된 이후 추가로 해당 세션으로 다른 명령 또는 SubUID 가 로그인 했으나 Server에 전파되기 전 Server에서 SessionEnd 를 호출하여 Master의 Session이 삭제되는 것을 방지하기 위하여
// Master에서는 Session 생성또는 기존 세션에 추가로 SubUID 등이 로그인 하는 경우 SessionCode를 무조건 증가시킴

// Client 가 짧은시간에 동일한 UID로 (SubUID는 다르더라도) Server로 생성 전파된 경우 첫번째 Client가 SessionCode가 달라서 Server로 로그인 하지 못하는 문제가 생길 수 있으므로
// Client 가 Server로 로그인 하기위한 SessionCode검사는 개별로 처리함
// SessionCode검사는 개별로 처리하기 위해 Server에는 [SubUID]를 Key로, [CKey, 개별 SessionCode] 를 Value로 하는 SubSessions 를 가짐

// Server의 SessionTime는 Session 을 Server로 생성 전파시킨 이후 Client 가 Server로 접속하지 않을 경우 생성된 Session을 Server, Master로 삭제 전파하기 위함.
// Master에서 Server로 생성 전파시 Server의 SubSessions에는 CKey만 Null 인 값을 채우고, 로그인 성공시 CKey를 채우게됨
// 만일 동시 접속이 금지된 경우
//		두 Client가 시간차이를 두고 로그인 한 경우 두번째 유저가 SubSessions 의 값을 지우고, Key가 유효한경우 LogOut을 하게 만들고
//		두 Client가 연달아 로그인 한경우 두번째 유저가 SubSessions 의 값을 지우고, Key가 무효한 경우 Logout 처리하지 않더라도, 이후 첫번째 Client가 Server로 로긴 시도시 SubSession에 값이 없으므로 로긴 실패하게 됨.
// SessionTimer에 대한 만료 처리시 Timer생성시 추가된 SessionCode와 Server에서 저장중인 SubSessions의 개별 SessionCode와 다르면 만료처리 하지 않음. 


// Messanger
// 내 친구의 상태는 그 친구가 존재하는 Master 서버에서 관리하며 내 Master 서버는 그냥 전달 받을 뿐


#pragma once

#include "Base.h"
#include <Rso/Core/Stream.h>
#include <Rso/Base/Base.h>
#include <functional>
#include <optional>
#include <Rso/Base/CycleCList.h>
#include <Rso/Net/Server.h>
#include <Rso/Net/ClientKeepConnect.h>
#include <Rso/Base/MultiIndexMap.h>
#include <Rso/Net/UDP.h>
#include <Rso/Dump/Dump.h>
#include <Rso/MSSQL/StoredProcedure.h>

namespace rso::game
{
	using namespace core;
	using namespace base;
	using namespace std;
	using namespace dump;
	using namespace mssql;
	using namespace net;

	class CMaster
	{
		static TState _GetState(TState State_, EFriendState FriendState_);

		template<typename _TSP> struct SDBBinder {};
		template<> struct SDBBinder<SMasterCreateInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::Create; };
		template<> struct SDBBinder<SMasterLoginInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::Login; };
		template<> struct SDBBinder<SMasterChangeNickInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::ChangeNick; };
		template<> struct SDBBinder<SMasterAddFriendBeginInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::AddFriendBegin; };
		template<> struct SDBBinder<SMasterAddFriendRequestInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::AddFriendRequest; };
		template<> struct SDBBinder<SMasterAddFriendEndInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::AddFriendEnd; };
		template<> struct SDBBinder<SMasterAddFriendFailInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::AddFriendFail; };
		template<> struct SDBBinder<SMasterAllowFriendInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::AllowFriend; };
		template<> struct SDBBinder<SMasterDenyFriendInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::DenyFriend; };
		template<> struct SDBBinder<SMasterBlockFriendInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::BlockFriend; };
		template<> struct SDBBinder<SMasterUnBlockFriendInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::UnBlockFriend; };
		template<> struct SDBBinder<SMasterChangeStateInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::ChangeState; };
		template<> struct SDBBinder<SMasterCheckInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::Check; };
		template<> struct SDBBinder<SMasterPunishInDb> { static const EMasterProtoDB SpNum = EMasterProtoDB::Punish; };

		using _TSP = CStoredProcedure<CKey>;

	public:
		using TOutObject = typename _TSP::SOutObject;
		using TSessionBeginFunc = function<void(TPeerCnt ServerNum_, TUID UID_)>;
		using TSessionEndFunc = function<void(TPeerCnt ServerNum_, TUID UID_)>;
		using TRecvPunishFunc = function<void(TUID UID_, const TTime& EndTime_)>;
		using TDBCallback = _TSP::TCallback;
		using TDBCallbacks = _TSP::TCallbacks;

	private:
		using _TAuths = CCycleCList<CKey>;
		using _TAuthsIt = typename _TAuths::iterator;
		using _TFriends = map<TUID, EFriendState>;
		using _TFriendsIt = typename _TFriends::iterator;
		struct _SClient
		{
			SMasterUser User;
			_TFriends Friends;
			TSessionCode SessionCode = 0; // 세션을 UID(또는 SubUID와 함께)를 키로만 쓰지 않고, SessionCode를 쓰는 이유는 Master에서 SessionCode가 증가했는데 Server에서 
			CKey ServerKey;

			_SClient(const SMasterUser& User_, const _TFriends& Friends_, TSessionCode SessionCode_, const CKey& ServerKey_) :
				User(User_), Friends(Friends_), SessionCode(SessionCode_), ServerKey(ServerKey_)
			{
			}
		};
		using _TSessions = map<TUID, _SClient>;
		struct _SMaster
		{
			TServerNets Servers;

			_SMaster(const TServerNets& Servers_) :
				Servers(Servers_)
			{
			}
		};
		using _TMasters = CMultiIndexMap<_SMaster, unique_index<TRangeUID>, unique_index<CNamePort>>;

		struct _SChangeNickClient
		{
			SKey ServerKey;
			SKey ClientKey;

			_SChangeNickClient(const SKey& ServerKey_, const SKey& ClientKey_) noexcept :
				ServerKey(ServerKey_), ClientKey(ClientKey_)
			{
			}
		};

	public:
		using TSessionsIt = typename _TSessions::iterator;
	private:
		struct _SServer
		{
			CKey Key;
			CNamePort ClientBindNamePortPub;
			CNamePort ServerBindNamePort;
			map<TUID, TSessionsIt> Sessions;
			bool Opened = true;

		public:
			_SServer(const CKey& Key_, const CNamePort& ClientBindNamePortPub_, const CNamePort& ServerBindNamePort_) :
				Key(Key_), ClientBindNamePortPub(ClientBindNamePortPub_), ServerBindNamePort(ServerBindNamePort_)
			{
			}
		};
		using _TServers = CList<_SServer>;
		using _TServersIt = typename _TServers::iterator;
		using _TUIDFriendInfosForNet = map<TPeerCnt, TUIDFriendInfos>;

		TLinkFunc _LinkFuncA;
		TLinkFailFunc _LinkFailFuncA;
		TUnLinkFunc _UnLinkFuncA;
		TRecvFunc _RecvFuncA;
		TLinkFunc _LinkFuncS;
		TUnLinkFunc _UnLinkFuncS;
		TRecvFunc _RecvFuncS;
		TLinkFunc _LinkFuncC;
		TUnLinkFunc _UnLinkFuncC;
		TSessionBeginFunc _SessionBeginFunc;
		TSessionEndFunc _SessionEndFunc;
		TRecvPunishFunc _RecvPunishFunc;
		TDBCallback _DBCallbackFunc;
		TDBCallbacks _DBCallbacks;
		CDump _Dump;

		CNamePort _AuthNamePort;
		TRangeUID _RangeUID;
		SNamePort _ClientBindNamePortPub;
		TSessionCode _SessionCounter = 0;
		_TAuths _Auths;
		_TServers _Servers;
		_TMasters _OtherMasters;
		CList<CKey> _Clients;
		_TSessions _Sessions;

		CClientKeepConnect _NetA;
		CUDP _NetM;
		CServer _NetS;
		CServer _NetC;
		SDBOption _DBOption; // Start() 에서 내부 MSSQL 쿼리 하기 위함.
		_TSP _DB;
		map<TUID, _SChangeNickClient> _ChangeNickClients; // Auth 와 접속이 끊어지면 Nick 변경중인 유저에게 모두 실패 처리하기 위함
		map<TUID, TFriendInfos> _FriendInfosForSee; // first : 다른 Master에 로그인 한 친구의 UID 이기 때문에 현 Session 에 없더라도 존재할 수 있음

		template<typename TIn, typename TOut>
		void _DBAddCmd(CMSSQL& MSSQL_, const wstring& SPName_, const wstring& AlterSPQuery_, _TSP::TCallback Callback_)
		{
			if (!AlterSPQuery_.empty())
				MSSQL_.CreateSP(SPName_, AlterSPQuery_);

			_DB.AddCmd<TIn, TOut>(static_cast<size_t>(SDBBinder<TIn>::SpNum), SPName_, true);
			_DBCallbacks.emplace_at(static_cast<size_t>(SDBBinder<TIn>::SpNum), Callback_);
		}
		template<typename TProto, typename... TProtos>
		void _DBPush(const CKey& Key_, const TProto& Proto_, const TProtos&... Protos_)
		{
			_DB.Push(static_cast<size_t>(SDBBinder<TProto>::SpNum), Key_, Proto_, Protos_...);
		}
		void _McLoginFail(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _DBCallback(TOutObject& OutObject_);
		void _EraseFriendStatesForSeeInMaster(const TRangeUID& RangeUID_);
		void _InsertFriendState(const SMmFriendStateChanged& Proto_);
		void _SendMsChangeNickFail(const CKey& ServerKey_, const SMsChangeNickFail& Proto_);
		void _SendMsAddFriendFail(const CKey& ServerKey_, const SMsAddFriendFail& Proto_);
		void _SendMaAddFriend(const SMaAddFriend& Proto_);
		void _StateChangedByMe(TSessionsIt itSession_, TState OldState_);
		void _StateChangedByFriend(TSessionsIt itSession_, _TFriendsIt itFriend_, EFriendState OldFriendState_);
		void _StateChanged(TSessionsIt itSession_, const TUIDFriendInfos& UIDFriendInfosForMe_, const _TUIDFriendInfosForNet& UIDFriendInfosForNet_);
		void _GetUIDFriendInfos(TSessionsIt itSession_, _TFriendsIt itFriend_, TUIDFriendInfos& UIDFriendInfosForMe_, _TUIDFriendInfosForNet& UIDFriendInfosForNet_, TState OldState_);
		bool _IsMyUID(TUID UID_) const;
		auto _GetServer(TPeerCnt ServerNum_);
		TSessionsIt _SessionBegin(TUID UID_, TPeerCnt ServerNum_, const SMasterUser& User_, const _TFriends& Friends_);
		void _SessionEnd(TSessionsIt itSession_);
		void _SessionLogin(TUID UID_, TUID SubUID_, const TID& ID_, const SMasterUser& MasterUser_, const TFriendDBs& Friends_, const CKey& UserKey_);
		void _SessionCheck(TUID UID_, const CKey& UserKey_, const CKey& AuthKey_);
		void _DBCreate(TOutObject& OutObject_);
		void _DBLogin(TOutObject& OutObject_);
		void _DBChangeNick(TOutObject& OutObject_);
		void _DBAddFriendBegin(TOutObject& OutObject_);
		void _DBAddFriendRequest(TOutObject& OutObject_);
		void _DBAddFriendEnd(TOutObject& OutObject_);
		void _DBAddFriendFail(TOutObject& OutObject_);
		void _DBAllowFriend(TOutObject& OutObject_);
		void _DBDenyFriend(TOutObject& OutObject_);
		void _DBBlockFriend(TOutObject& OutObject_);
		void _DBUnBlockFriend(TOutObject& OutObject_);
		void _DBChangeState(TOutObject& OutObject_);
		void _DBCheck(TOutObject& OutObject_);
		void _DBPunish(TOutObject& OutObject_);

		void _LinkA(const CKey& Key_);
		void _LinkFailA(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkA(const CKey& Key_, ENetRet NetRet_);
		void _RecvA(const CKey& Key_, CStream& Stream_);
		void _RecvAmMasterOn(const CKey& Key_, CStream& Stream_);
		void _RecvAmOtherMasterOn(const CKey& Key_, CStream& Stream_);
		void _RecvAmOtherMasterOff(const CKey& Key_, CStream& Stream_);
		void _RecvAmOtherMasterServerOn(const CKey& Key_, CStream& Stream_);
		void _RecvAmOtherMasterServerOff(const CKey& Key_, CStream& Stream_);
		void _RecvAmCreate(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmChangeNick(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmChangeNickFail(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmCheck(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmAddFriendGetUID(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmAddFriendGetUIDFail(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmAddFriendRequest(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmAddFriend(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmAddFriendFail(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmToServer(_TAuthsIt itAuth_, CStream& Stream_);
		void _RecvAmPunish(_TAuthsIt itAuth_, CStream& Stream_);

		void _LinkM(TIPEndPoint IPEndPoint_);
		void _UnLinkM(TIPEndPoint IPEndPoint_);
		void _RecvM(CUDP::SRecvData& Data_);
		void _RecvMmFriendStateChanged(CUDP::SRecvData& Data_);
		void _RecvMmFriendStateChangedRenew(CUDP::SRecvData& Data_);

		void _LinkS(const CKey& Key_);
		void _UnLinkS(const CKey& Key_, ENetRet NetRet_);
		void _RecvS(const CKey& Key_, CStream& Stream_);
		void _RecvSmServerOn(const CKey& Key_, CStream& Stream_);
		void _RecvSmLogin(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmChangeNick(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmCheck(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmAddFriend(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmAllowFriend(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmDenyFriend(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmBlockFriend(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmUnBlockFriend(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmChangeState(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmSetOpened(const CKey& Key_, CStream& Stream_);
		void _RecvSmToServer(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmSessionEnd(_TServersIt itServer_, CStream& Stream_);
		void _RecvSmSetOpened(_TServersIt itServer_, CStream& Stream_);

		void _LinkC(const CKey& Key_);
		void _UnLinkC(const CKey& Key_, ENetRet NetRet_);
		void _RecvC(const CKey& Key_, CStream& Stream_);
		void _RecvCmLogin(const CKey& Key_, CStream& Stream_);

	public:
		CMaster(
			EAddressFamily AddressFamily_,
			TLinkFunc LinkFuncA_, TLinkFailFunc LinkFailFuncA_, TUnLinkFunc UnLinkFuncA_, TRecvFunc RecvFuncA_,
			TLinkFunc LinkFuncS_, TUnLinkFunc UnLinkFuncS_, TRecvFunc RecvFuncS_,
			TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_,
			TSessionBeginFunc SessionBeginFunc_, TSessionEndFunc SessionEndFunc_, TRecvPunishFunc RecvPunishFunc_, TDBCallback DBCallbackFunc_,
			const TRangeUID& RangeUID_, const SDBOption& DBOption_,
			const CNamePort& AuthNamePort_, const CNamePort& ServerBindNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_,
			TPort MasterBindPort_);
		template<typename TIn, typename TOut>
		inline void DBAddCmd(size_t SPNum_, const wstring& SPName_, bool DoesHaveReturn_)
		{
			_DB.AddCmd<TIn, TOut>(_DBCallbacks.size() + SPNum_, SPName_, DoesHaveReturn_);
		}
		template<typename... TProtos>
		inline void DBPush(size_t SPNum_, const CKey& Key_, const TProtos&... Protos_)
		{
			_DB.Push(_DBCallbacks.size() + SPNum_, Key_, Protos_...);
		}
		inline void CloseA(TPeerCnt PeerNum_)
		{
			_NetA.Close(PeerNum_);
		}
		inline bool CloseA(const CKey& Key_)
		{
			return _NetA.Close(Key_);
		}
		inline void CloseS(TPeerCnt PeerNum_)
		{
			_NetS.Close(PeerNum_);
		}
		inline bool CloseS(const CKey& Key_)
		{
			return _NetS.Close(Key_);
		}
		template<typename... _TProtos>
		void SendS(TPeerCnt PeerNum_, const _TProtos&... Protos_)
		{
			if (!_Servers.get(PeerNum_)) // 인증받은 서버에게만 전송
				return;

			_NetS.Send(PeerNum_, SHeader(EProto::MsUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void SendS(const CKey& Key_, const _TProtos&... Protos_)
		{
			if (!_Servers.get(Key_.PeerNum)) // 인증받은 서버에게만 전송
				return;

			_NetS.Send(Key_, SHeader(EProto::MsUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void SendSAll(const _TProtos&... Protos_)
		{
			for (auto it = _Servers.begin(); it != _Servers.end(); ++it) // 인증받은 서버에게만 보내야 하기 때문에 SendAll 을 사용하지 않음.
				_NetS.Send((TPeerCnt)it.Index(), SHeader(EProto::MsUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void SendA(TPeerCnt PeerNum_, const _TProtos&... Protos_)
		{
			_NetA.Send(PeerNum_, SHeader(EProto::MaUserProto), Protos_...); // Auth 는 인증절차가 없으므로 그냥 전송
		}
		template<typename... _TProtos>
		void SendA(const CKey& Key_, const _TProtos&... Protos_)
		{
			_NetA.Send(Key_, SHeader(EProto::MaUserProto), Protos_...); // Auth 는 인증절차가 없으므로 그냥 전송
		}
		template<typename... _TProtos>
		void SendAAll(const _TProtos&... Protos_)
		{
			_NetA.SendAll(SHeader(EProto::MaUserProto), Protos_...); // Auth 는 인증절차가 없으므로 그냥 전송
		}
		inline TSessionsIt GetSessionEnd(void)
		{
			return _Sessions.end();
		}
		void SendToServerStream(TUID UID_, const CStream& Stream_)
		{
			auto itSession = _SessionBegin(UID_, c_PeerCnt_Null, SMasterUser(), _TFriends());
			_NetS.Send(itSession->second.ServerKey.PeerNum, SHeader(EProto::MsToServer), UID_, itSession->second.SessionCode, Stream_);
		}
		template<typename... _TProtos>
		inline void SendToServer(TUID UID_, const _TProtos&... Protos_)
		{
			SendToServerStream(UID_, CStream().Send(Protos_...));
		}
		template<typename... _TProtos>
		void SessionHold(TUID UID_, TPeerCnt ServerNum_, const _TProtos&... Protos_)
		{
			auto itSession = _SessionBegin(UID_, ServerNum_, SMasterUser(), _TFriends());
			_NetS.Send(itSession->second.ServerNum, SHeader(EProto::MsSessionHold), UID_, itSession->second.SessionCode, CStream().Send(Protos_...));
		}
		inline void Dump(void) { _Dump.UserDump(GetCurrentProcessId()); }
		void Start(void);
		inline size_t GetSessionCnt(void) const { return _Sessions.size(); }
		bool Punish(TUID UID_, const TTime& EndTime_);
		void Proc(void);
	};
}
