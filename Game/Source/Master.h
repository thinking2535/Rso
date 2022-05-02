// Master ������ DB �ӹ��� Create, Login, Check(���� ���� ���� Ȯ��)

// Session ���� �˰���
// ������ UID (MainUID) �� 1���� ����
// Master������ Session�� �����ϰ�, Server�� �����ϰ�, ������ �ݴ�� Server���� Master�� ���ĵ�

// Master���� ������ Session�� ���� �߰��� �ش� �������� �ٸ� ��� �Ǵ� SubUID �� �α��� ������ Server�� ���ĵǱ� �� Server���� SessionEnd �� ȣ���Ͽ� Master�� Session�� �����Ǵ� ���� �����ϱ� ���Ͽ�
// Master������ Session �����Ǵ� ���� ���ǿ� �߰��� SubUID ���� �α��� �ϴ� ��� SessionCode�� ������ ������Ŵ

// Client �� ª���ð��� ������ UID�� (SubUID�� �ٸ�����) Server�� ���� ���ĵ� ��� ù��° Client�� SessionCode�� �޶� Server�� �α��� ���� ���ϴ� ������ ���� �� �����Ƿ�
// Client �� Server�� �α��� �ϱ����� SessionCode�˻�� ������ ó����
// SessionCode�˻�� ������ ó���ϱ� ���� Server���� [SubUID]�� Key��, [CKey, ���� SessionCode] �� Value�� �ϴ� SubSessions �� ����

// Server�� SessionTime�� Session �� Server�� ���� ���Ľ�Ų ���� Client �� Server�� �������� ���� ��� ������ Session�� Server, Master�� ���� �����ϱ� ����.
// Master���� Server�� ���� ���Ľ� Server�� SubSessions���� CKey�� Null �� ���� ä���, �α��� ������ CKey�� ä��Ե�
// ���� ���� ������ ������ ���
//		�� Client�� �ð����̸� �ΰ� �α��� �� ��� �ι�° ������ SubSessions �� ���� �����, Key�� ��ȿ�Ѱ�� LogOut�� �ϰ� �����
//		�� Client�� ���޾� �α��� �Ѱ�� �ι�° ������ SubSessions �� ���� �����, Key�� ��ȿ�� ��� Logout ó������ �ʴ���, ���� ù��° Client�� Server�� �α� �õ��� SubSession�� ���� �����Ƿ� �α� �����ϰ� ��.
// SessionTimer�� ���� ���� ó���� Timer������ �߰��� SessionCode�� Server���� �������� SubSessions�� ���� SessionCode�� �ٸ��� ����ó�� ���� ����. 


// Messanger
// �� ģ���� ���´� �� ģ���� �����ϴ� Master �������� �����ϸ� �� Master ������ �׳� ���� ���� ��


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
			TSessionCode SessionCode = 0; // ������ UID(�Ǵ� SubUID�� �Բ�)�� Ű�θ� ���� �ʰ�, SessionCode�� ���� ������ Master���� SessionCode�� �����ߴµ� Server���� 
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
		SDBOption _DBOption; // Start() ���� ���� MSSQL ���� �ϱ� ����.
		_TSP _DB;
		map<TUID, _SChangeNickClient> _ChangeNickClients; // Auth �� ������ �������� Nick �������� �������� ��� ���� ó���ϱ� ����
		map<TUID, TFriendInfos> _FriendInfosForSee; // first : �ٸ� Master�� �α��� �� ģ���� UID �̱� ������ �� Session �� ������ ������ �� ����

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
			if (!_Servers.get(PeerNum_)) // �������� �������Ը� ����
				return;

			_NetS.Send(PeerNum_, SHeader(EProto::MsUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void SendS(const CKey& Key_, const _TProtos&... Protos_)
		{
			if (!_Servers.get(Key_.PeerNum)) // �������� �������Ը� ����
				return;

			_NetS.Send(Key_, SHeader(EProto::MsUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void SendSAll(const _TProtos&... Protos_)
		{
			for (auto it = _Servers.begin(); it != _Servers.end(); ++it) // �������� �������Ը� ������ �ϱ� ������ SendAll �� ������� ����.
				_NetS.Send((TPeerCnt)it.Index(), SHeader(EProto::MsUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void SendA(TPeerCnt PeerNum_, const _TProtos&... Protos_)
		{
			_NetA.Send(PeerNum_, SHeader(EProto::MaUserProto), Protos_...); // Auth �� ���������� �����Ƿ� �׳� ����
		}
		template<typename... _TProtos>
		void SendA(const CKey& Key_, const _TProtos&... Protos_)
		{
			_NetA.Send(Key_, SHeader(EProto::MaUserProto), Protos_...); // Auth �� ���������� �����Ƿ� �׳� ����
		}
		template<typename... _TProtos>
		void SendAAll(const _TProtos&... Protos_)
		{
			_NetA.SendAll(SHeader(EProto::MaUserProto), Protos_...); // Auth �� ���������� �����Ƿ� �׳� ����
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
