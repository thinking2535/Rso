#pragma once

#include "Base.h"
#include <Rso/Core/Stream.h>
#include <Rso/Base/Base.h>
#include <Rso/Base/Period.h>
#include <functional>
#include <Rso/Mobile/Server.h>
#include <Rso/Net/ClientKeepConnect.h>
#include <Rso/Dump/Dump.h>
#include <Rso/MSSQL/StoredProcedure.h>
#include <Rso/Net/UDP.h>

namespace rso::game
{
	using namespace core;
	using namespace base;
	using namespace std;
	using namespace dump;
	using namespace mssql;
	using namespace mobile;

	class CServer : public INet, public IServer
	{
		using _TSP = CStoredProcedure<CKey>;
		struct _SSessionCodeKey
		{
			TSessionCode SessionCode = 0; // 로그인 처리위함
										// SubUID 당 1개를 가지며 SubUID 까지 같은 유저가 동시에 접속할 경우 최신 값만 저장됨 (SessionTimer에 의해 만료시 OldSessionCode에 의해 만료되는 것을 방지)
			CKey Key;

			explicit _SSessionCodeKey(TSessionCode SessionCode_) :
				SessionCode(SessionCode_)
			{
			}
		};
		struct _SSession : public SUIDPair
		{
			TSessionCode SessionCode = 0;

			_SSession(const SUIDPair& Super_, TSessionCode SessionCode_) :
				SUIDPair(Super_), SessionCode(SessionCode_)
			{
			}
		};
		using _TSessionTimer = multimap<TTime, _SSession>;
		struct _SCheck
		{
			CKey UserKey;
			CKey AuthKey;

			_SCheck(const CKey& UserKey_, const CKey& AuthKey_) :
				UserKey(UserKey_), AuthKey(AuthKey_)
			{
			}
		};
		using _TChecks = map<TSessionCode, _SCheck>;
		struct _SClient
		{
			using _TSubSessions = map<TUID, _SSessionCodeKey>; // Key : SubUID
			using _TSubSessionsIt = typename _TSubSessions::iterator;
			using _TKeys = map<CKey, _TSubSessionsIt>;

		private:
			_TSubSessions _SubSessions; // 유저가 최소한 로그인 시도중이거나, 로그인 완료(Key==Valid) 임을 의미 (SubUID, SubSessionCode 여기에 있더라도 _Keys에는 없을 수 있음)
			_TKeys _Keys; // 여기에 있다면 _SubSessions 에도 있음을 보장

		public:
			CKey MasterKey;
			TSessionCode SessionCode = 0; // UID(MainUID) 당 1개 부여되는 값으로 최신 값으로 만료처리시 필요 (세션 만료시 Master로 보내지는 서버가 알고있는 최신 값)
			SAccount Account;
			TFriends Friends;
		private:
			_TChecks _Checks; // Key : Check 시점의 SessionCode
		public:
			bool Hold = false;
			bool SendToServer = false;
			int64 DBPushedCount = 0;
			bool IsChangingNick = false;

			inline const _TKeys& GetKeys(void) const
			{
				return _Keys;
			}
			inline bool HasKey(const CKey& Key_) const
			{
				return _Keys.find(Key_) != _Keys.end();
			}
			_SClient(const CKey& MasterKey_, const SMsLogin& MsLogin_) : // For Login
				MasterKey(MasterKey_), SessionCode(MsLogin_.SessionCode), Account(MsLogin_.Account), _SubSessions{ _TSubSessions::value_type(MsLogin_.SubUID, MsLogin_.SessionCode) }
			{
				for (auto& f : MsLogin_.Friends)
					Friends.emplace(f.first, f.second);
			}
			_SClient(const CKey& MasterKey_, TSessionCode SessionCode_, bool Hold_) : // For SessionHold, SendToServer
				MasterKey(MasterKey_), SessionCode(SessionCode_), Hold(Hold_), SendToServer(!Hold_)
			{
			}
			_SClient(const CKey& MasterKey_, TSessionCode SessionCode_, const _SCheck& Check_) : // For Check
				MasterKey(MasterKey_), SessionCode(SessionCode_), _Checks{ _TChecks::value_type(SessionCode_, Check_) }
			{
			}
			SScLogin GetScLogin(void) const
			{
				return SScLogin(Account.Nick, Friends);
			}
			void AddCheck(TSessionCode SessionCode_, const _SCheck& Check_)
			{
				_Checks.emplace(SessionCode_, Check_);
			}
			_SCheck DelCheck(TSessionCode SessionCode_)
			{
				auto it = _Checks.find(SessionCode_);
				auto Check = it->second;
				_Checks.erase(it);

				return Check;
			}
			inline bool IsValid(void) const noexcept
			{
				return (!_SubSessions.empty() || !_Checks.empty() || Hold || SendToServer || DBPushedCount > 0);
			}
			void MsLogin(const SMsLogin& Proto_)
			{
				SessionCode = Proto_.SessionCode;
				Account = Proto_.Account;

				for (auto& f : Proto_.Friends)
					Friends.emplace(f.first, f.second);

				auto it = _SubSessions.find(Proto_.SubUID);
				if (it == _SubSessions.end())
					_SubSessions.emplace(Proto_.SubUID, _SSessionCodeKey(Proto_.SessionCode));
				else
					it->second.SessionCode = Proto_.SessionCode;
			}
			bool CsLogin(const TID& ID_, TUID SubUID_, TSessionCode SessionCode_, const CKey& Key_, CKey& DupKey_)
			{
				if (ID_ != Account.ID)
					return false;

				auto it = _SubSessions.find(SubUID_);
				if (it == _SubSessions.end())
					return false;

				if (SessionCode_ != it->second.SessionCode)
					return false;

				if (it->second.Key)
				{
					_Keys.erase(it->second.Key);
					DupKey_ = it->second.Key;
				}

				it->second.Key = Key_;
				_Keys.emplace(Key_, it);

				return true;
			}
			bool UnLink(const CKey& Key_)
			{
				auto it = _Keys.find(Key_);
				if (it == _Keys.end())
					return false;

				_SubSessions.erase(it->second); // 지워질 것이라 간주
				_Keys.erase(it);

				return true;
			}
			bool TimeOut(TUID SubUID_, TSessionCode SessionCode_)
			{
				auto it = _SubSessions.find(SubUID_);
				if (it == _SubSessions.end() || it->second.SessionCode != SessionCode_ || it->second.Key) // 동일한 유저가 로그인 된 상태로 또다시 로그인 시도하고 TimerOut 처리되면 
																					// 기존 유저가 로긴 되어 있는 경우 (Key가 유효한 경우) 에는 만료되지 않도록
					return false;

				_SubSessions.erase(it);

				return true;
			}
		};
		struct _SMaster
		{
			TServerNets Servers;

			_SMaster()
			{
			}
			_SMaster(const TServerNets& Servers_) :
				Servers(Servers_)
			{
			}
		};

	public:
		using TSessions = map<TUID, _SClient>;
		using TSessionsIt = typename TSessions::iterator;
		using TOutObject = typename _TSP::SOutObject;
		using TCheckFunc = function<void(TSessionsIt itSession_, TSessionCode SessionCode_)>;
		using TLinkFunc = function<void(TSessionsIt itSession_, const CKey& Key_, bool Create_, CStream& Stream_)>;
		using TUnLinkFunc = function<void(TSessionsIt itSession_, const CKey& Key_, ENetRet NetRet_)>;
		using TRecvFunc = function<void(TSessionsIt Session_, const CKey& Key_, CStream& Stream_)>;
		using TRecvSessionHoldFunc = function<void(TSessionsIt Session_, CStream& Stream_)>;
		using TRecvSendToServerFunc = function<void(TSessionsIt Session_, CStream& Stream_)>;
		using TRecvPunishFunc = function<void(TUID UID_, const TTime& EndTime_)>;
		using TChangeNickFunc = function<void(const CKey& ClientKey_, EGameRet GameRet_)>;
		using TDBCallback = function<void(TSessionsIt Session_, TOutObject& OutObject_)>;
		using TDBCallbacks = CList<TDBCallback>;

	private:
		using _TClients = CList<TSessionsIt>;
		using _TClientsIt = typename _TClients::iterator;
		struct _SDBQuery
		{
			TSessionCode MasterSessionIndex = 0;
			TSessionsIt itSession;

			_SDBQuery(TSessionCode MasterSessionIndex_, TSessionsIt itSession_) :
				MasterSessionIndex(MasterSessionIndex_), itSession(itSession_)
			{
			}
		};

		TCheckFunc _CheckFunc;
		net::TLinkFunc _LinkFuncM;
		TLinkFailFunc _LinkFailFuncM;
		net::TRecvFunc _RecvFuncM;
		net::TUnLinkFunc _UnLinkFuncM;
		TLinkFunc _LinkFuncC;
		TUnLinkFunc _UnLinkFuncC;
		TRecvFunc _RecvFuncC;
		TRecvSessionHoldFunc _RecvSessionHoldFunc;
		TRecvSendToServerFunc _RecvSendToServerFunc;
		TRecvPunishFunc _RecvPunishFunc;
		TChangeNickFunc _ChangeNickFunc;
		TDBCallback _DBCallbackFunc;
		bool _Opened = true;
		CDump _Dump;
		TSessionCode _MasterSessionCounter = 0;
		CList<_SDBQuery> _DBQueries;
		SVersion _Version;
		CPeriod<seconds> _Period{ seconds(1) };
		SNamePort _ClientBindNamePortPub;
		TSessions _Sessions;
		_TSessionTimer _SessionTimer;

		bool _AllowDupLogin = false;
		SNamePort _ServerBindNamePort;
		_TClients _Clients;
		TServerNets _OtherServers;
		map<TRangeUID, _SMaster> _OtherMasters;
		CClientKeepConnect _NetM;
		CUDP _NetS;
		net::CServer _NetC;
		_TSP _DB;

		void _DBCallback(TOutObject& OutObject_);
		void _SendSmSessionEnd(TSessionsIt itSession_);
		template<typename... _TProtos>
		inline void _SendC(TSessionsIt itSession_, const _TProtos&... Protos_)
		{
			for (auto& i : itSession_->second.GetKeys())
				_NetC.Send(i.first.PeerNum, Protos_...);
		}
		void _SessionEnd(TSessionsIt itSession_);
		void _SessionUnHold(TSessionsIt itSession_);
		void _SessionUnSendToServer(TSessionsIt itSession_);
		void _SessionSubDBPushedCount(TSessionsIt itSession_);
		void _LinkM(const CKey& Key_);
		void _LinkFailM(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkM(const CKey& Key_, ENetRet NetRet_);
		void _RecvM(const CKey& Key_, CStream& Stream_);
		void _RecvMsMasterOn(const CKey& Key_, CStream& Stream_);
		void _RecvMsOtherMasterOn(const CKey& Key_, CStream& Stream_);
		void _RecvMsOtherMasterOff(const CKey& Key_, CStream& Stream_);
		void _RecvMsServerOn(const CKey& Key_, CStream& Stream_);
		void _RecvMsOtherServerOn(const CKey& Key_, CStream& Stream_);
		void _RecvMsOtherMasterServerOn(const CKey& Key_, CStream& Stream_);
		void _RecvMsOtherServerOff(const CKey& Key_, CStream& Stream_);
		void _RecvMsOtherMasterServerOff(const CKey& Key_, CStream& Stream_);
		void _RecvMsChangeNick(const CKey& Key_, CStream& Stream_);
		void _RecvMsChangeNickFail(const CKey& Key_, CStream& Stream_);
		void _RecvMsCheck(const CKey& Key_, CStream& Stream_);
		void _RecvMsLogin(const CKey& Key_, CStream& Stream_);
		void _RecvMsAddFriendRequest(const CKey& Key_, CStream& Stream_);
		void _RecvMsAddFriend(const CKey& Key_, CStream& Stream_);
		void _RecvMsAddFriendFail(const CKey& Key_, CStream& Stream_);
		void _RecvMsAllowFriend(const CKey& Key_, CStream& Stream_);
		void _RecvMsAllowFriendFail(const CKey& Key_, CStream& Stream_);
		void _RecvMsDenyFriend(const CKey& Key_, CStream& Stream_);
		void _RecvMsDenyFriendFail(const CKey& Key_, CStream& Stream_);
		void _RecvMsBlockFriend(const CKey& Key_, CStream& Stream_);
		void _RecvMsBlockFriendFail(const CKey& Key_, CStream& Stream_);
		void _RecvMsUnBlockFriend(const CKey& Key_, CStream& Stream_);
		void _RecvMsUnBlockFriendFail(const CKey& Key_, CStream& Stream_);
		void _RecvMsChangeState(const CKey& Key_, CStream& Stream_);
		void _RecvMsChangeStateFail(const CKey& Key_, CStream& Stream_);
		void _RecvMsFriendStateChanged(const CKey& Key_, CStream& Stream_);
		void _RecvMsFriendStateChangedOffline(const CKey& Key_, CStream& Stream_);
		void _RecvMsSessionHold(const CKey& Key_, CStream& Stream_);
		void _RecvMsToServer(const CKey& Key_, CStream& Stream_);
		void _RecvMsPunish(const CKey& Key_, CStream& Stream_);
		void _LinkS(TIPEndPoint IPEndPoint_);
		void _UnLinkS(TIPEndPoint IPEndPoint_);
		void _RecvS(CUDP::SRecvData& Data_);
		void _RecvSsMessageSend(CUDP::SRecvData& Data_);
		void _SendScMessageReceived(TUID ToUID_, const SScMessageReceived& Proto_);
		void _LinkC(const CKey& Key_);
		void _UnLinkC(const CKey& Key_, ENetRet NetRet_);
		void _RecvC(const CKey& Key_, CStream& Stream_);
		void _RecvCsLogin(const CKey& Key_, CStream& Stream_);
		void _RecvCsAddFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_);
		void _RecvCsAllowFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_);
		void _RecvCsDenyFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_);
		void _RecvCsBlockFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_);
		void _RecvCsUnBlockFriend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_);
		void _RecvCsChangeState(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_);
		void _RecvCsMessageSend(TSessionsIt itSession_, const CKey& Key_, CStream& Stream_);

	public:
		CServer(
			EAddressFamily AddressFamily_,
			TCheckFunc CheckFunc_, net::TLinkFunc LinkFuncM_, TLinkFailFunc LinkFailFuncM_, net::TUnLinkFunc UnLinkFuncM_, net::TRecvFunc RecvFuncM_,
			TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TRecvFunc RecvFuncC_,
			TRecvSessionHoldFunc RecvSessionHoldFunc_, TRecvSendToServerFunc RecvSendToServerFunc_, TRecvPunishFunc RecvPunishFunc_,
			TChangeNickFunc ChangeNickFunc_, TDBCallback DBCallbackFunc_,
			const SVersion& Version_, const SDBOption& DBOption_,
			const CNamePort& MasterNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_, const CNamePort& ClientBindNamePortPub_, TPort ServerBindPort_,
			bool AllowDupLogin_);
		virtual ~CServer() {}
		inline bool IsLinked(TPeerCnt PeerNum_) const
		{
			return _NetC.IsLinked(PeerNum_);
		}
		inline CNamePort GetNamePort(TPeerCnt PeerNum_) const
		{
			return _NetC.GetNamePort(PeerNum_);
		}
		inline void Close(TPeerCnt PeerNum_)
		{
			_NetC.Close(PeerNum_);
		}
		inline bool Close(const CKey& Key_)
		{
			return _NetC.Close(Key_);
		}
		inline void CloseAll(void)
		{
			_NetC.CloseAll();
		}
		inline void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
		{
			_NetC.WillClose(PeerNum_, WaitMilliseconds_);
		}
		inline bool WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
		{
			return _NetC.WillClose(Key_, WaitMilliseconds_);
		}
		inline TPeerCnt GetPeerCnt(void) const
		{
			return _NetC.GetPeerCnt();
		}
		inline milliseconds Latency(TPeerCnt PeerNum_) const
		{
			return _NetC.Latency(PeerNum_);
		}
		void Proc(void);
		void Certify(TPeerCnt PeerNum_);
		bool Certify(const CKey& Key_);
		inline bool IsCertified(TPeerCnt PeerNum_) const
		{
			return _NetC.IsCertified(PeerNum_);
		}
		template<typename _TParamType, typename _TResultType>
		inline void DBAddCmd(size_t SPNum_, const wstring& SPName_, bool DoesHaveReturn_)
		{
			_DB.AddCmd<_TParamType, _TResultType>(SPNum_, SPName_, DoesHaveReturn_);
		}
		template<typename... _TParams>
		void DBPush(TSessionsIt itSession_, size_t SPNum_, const CKey& Key_, const _TParams&... Params_)
		{
			_DBQueries.emplace(_SDBQuery(_MasterSessionCounter, itSession_));
			_DB.Push(SPNum_, Key_, Params_...);

			if (itSession_ != _Sessions.end())
				++itSession_->second.DBPushedCount;
		}
		template<typename... _TParams>
		void DBPush(size_t SPNum_, const CKey& Key_, const _TParams&... Params_)
		{
			DBPush(_Sessions.end(), SPNum_, Key_, Params_...);
		}

		// For Master ///////////////////////////////////////
		template<typename... _TProtos>
		inline void SendM(const CKey& Key_, const _TProtos&... Protos_)
		{
			_NetM.Send(Key_, SHeader(EProto::SmUserProto), Protos_...);
		}
		template<typename... _TProtos>
		inline void SendMAll(const _TProtos&... Protos_)
		{
			_NetM.SendAll(SHeader(EProto::SmUserProto), Protos_...);
		}
		template<typename... _TProtos>
		inline void SendMAllExcept(const CKey& Key_, const _TProtos&... Protos_)
		{
			_NetM.SendAllExcept(Key_, Protos_...);
		}
		inline void CloseM(const CKey& Key_)
		{
			_NetM.Close(Key_);
		}
		// ServerToMaster를 따로 두지 않는 이유는 대상 유저의 로그인 여부에 상관없이 Sever까지 프로토콜이 도달하도록 하기 위함
		// Server에 도달한 이후 외부에서 유저의 로그인 여부에 따라 별도로 처리
		template<typename... _TProtos>
		inline void SendToServer(TUID UID_, const _TProtos&... Protos_)
		{
			_NetM.SendAll(SHeader(EProto::SmToServer), UID_, CStream().Send(Protos_...));
		}
		void SessionHold(TSessionsIt itSession_);
		inline void SessionUnHold(TSessionsIt itSession_) { _SessionUnHold(itSession_); }

		// For Client ///////////////////////////////////////
		template<typename... _TProtos>
		void Send(TPeerCnt PeerNum_, const _TProtos&... Protos_)
		{
			if (!_Clients.get(PeerNum_))
				return;

			_NetC.Send(PeerNum_, SHeader(EProto::ScUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void Send(const CKey& Key_, const _TProtos&... Protos_)
		{
			if (!_Clients.get(Key_.PeerNum))
				return;

			_NetC.Send(Key_, SHeader(EProto::ScUserProto), Protos_...);
		}
		template<typename... _TProtos>
		void SendAll(const _TProtos&... Protos_)
		{
			for (auto it = _Clients.begin(); it != _Clients.end(); ++it) // 인증받은 클라에게만 보내야 하기 때문에 _NetC.SendAll 을 사용하지 않음.
				_NetC.Send(TPeerCnt(it.Index()), SHeader(EProto::ScUserProto), Protos_...);
		}
		template<typename... _TProtos>
		inline void SendAllExcept(const CKey& Key_, const _TProtos&... Protos_)
		{
			_NetC.SendAllExcept(Key_, Protos_...);
		}
		////////////////////////////////////////////////////
		inline void Dump(void) { _Dump.UserDump(GetCurrentProcessId()); }
		inline void Start(void) { _DB.Start(5); }
		inline bool GetOpened(void) const { return _Opened; }
		void SetOpened(bool Opened_);
		bool ChangeNick(const SSmChangeNick& Proto_);
		void SendCheck(TSessionsIt itSession_, TSessionCode SessionCode_, const CStream& Stream_);
	};
}
