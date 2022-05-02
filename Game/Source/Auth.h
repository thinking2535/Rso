// Auth 서버의 DB 임무는 계정 Login, Create 를 처리하고 Create 시 계정이 있으면 Select

#pragma once

#include "Base.h"
#include <Rso/Core/Stream.h>
#include <Rso/Base/Base.h>
#include <functional>
#include <optional>
#include <Rso/Base/MultiIndexMap.h>
#include <Rso/Dump/Dump.h>
#include <Rso/MSSQL/StoredProcedure.h>
#include <Rso/Net/Server.h>

namespace rso::game
{
	using namespace core;
	using namespace base;
	using namespace std;
	using namespace win;
	using namespace dump;
	using namespace mssql;
	using namespace net;

	class CAuth
	{
		template<typename _TSP> struct SDBBinder {};
		template<> struct SDBBinder<SAuthCreateInDb> { static const EAuthProtoDB SpNum = EAuthProtoDB::Create; };
		template<> struct SDBBinder<SAuthLoginInDb> { static const EAuthProtoDB SpNum = EAuthProtoDB::Login; };
		template<> struct SDBBinder<SAuthAddFriendGetUIDInDb> { static const EAuthProtoDB SpNum = EAuthProtoDB::AddFriendGetUID; };
		template<> struct SDBBinder<SAuthChangeNickInDb> { static const EAuthProtoDB SpNum = EAuthProtoDB::ChangeNick; };
		template<> struct SDBBinder<SAuthCheckInDb> { static const EAuthProtoDB SpNum = EAuthProtoDB::Check; };

		struct _SMaster
		{
			CKey Key;
			CNamePort ClientBindNamePort;
			CNamePort MasterBindNamePort;
			TServerNets Servers;

			_SMaster(const CKey& Key_, const CNamePort& ClientBindNamePort_, const CNamePort& MasterBindNamePort_, const TServerNets& Servers_) :
				Key(Key_), ClientBindNamePort(ClientBindNamePort_), MasterBindNamePort(MasterBindNamePort_), Servers(Servers_)
			{
			}
		};
		using _TMasters = CMultiIndexMap<_SMaster, unique_index<TRangeUID>>;
		using _TMastersIt = typename _TMasters::iterator;
		struct _SClient
		{
			CKey Key;

			explicit _SClient(const CKey& Key_) :
				Key(Key_)
			{
			}
		};
		using _TSP = CStoredProcedure<CKey>;

	public:
		using TOutObject = typename _TSP::SOutObject;
		using TDBCallback = function<void(TOutObject& OutObject_)>;
		using TDBCallbacks = CList<TDBCallback>;

	private:
		TLinkFunc _LinkFuncM;
		TUnLinkFunc _UnLinkFuncM;
		TRecvFunc _RecvFuncM;
		TLinkFunc _LinkFuncC;
		TUnLinkFunc _UnLinkFuncC;
		TDBCallback _DBCallbackFunc;
		_TSP::TCallbacks _DBCallbacks;
		CDump _Dump;

		_TMasters _Masters;
		CList<_SClient> _Clients;

		net::CServer _NetM;
		net::CServer _NetC;
		_TSP _DB;

		template<typename TIn, typename TOut>
		void _DBAddCmd(CMSSQL& MSSQL_, const wstring& SPName_, const wstring& AlterSPQuery_, _TSP::TCallback Callback_)
		{
			MSSQL_.CreateSP(SPName_, AlterSPQuery_);
			_DB.AddCmd<TIn, TOut>(static_cast<size_t>(SDBBinder<TIn>::SpNum), SPName_, true);
			_DBCallbacks.emplace_at(static_cast<size_t>(SDBBinder<TIn>::SpNum), Callback_);
		}
		template<typename TProto, typename... TProtos>
		void _DBPush(const CKey& Key_, const TProto& Proto_, const TProtos&... Protos_)
		{
			_DB.Push(static_cast<size_t>(SDBBinder<TProto>::SpNum), Key_, Proto_, Protos_...);
		}
		EGameRet _IsValidID(const TID& ID_) const;
		EGameRet _IsValidNick(const TNick& Nick_) const;
		void _DBCallback(TOutObject& OutObject_);
		void _DBCreate(TOutObject& OutObject_);
		void _DBLogin(TOutObject& OutObject_);
		void _DBAddFriendGetUID(TOutObject& OutObject_);
		void _DBChangeNick(TOutObject& OutObject_);
		void _DBCheck(TOutObject& OutObject_);

		void _LinkM(const CKey& Key_);
		void _UnLinkM(const CKey& Key_, ENetRet NetRet_);
		void _RecvM(const CKey& Key_, CStream& Stream_);
		void _RecvMaMasterOn(const CKey& Key_, CStream& Stream_);
		void _RecvMaServerOn(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaServerOff(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaCreate(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaCreateFail(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaChangeNick(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaAddFriendGetUID(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaAddFriend(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaAddFriendRequest(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaAddFriendRequestFail(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaCheck(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaCheckFail(_TMastersIt itMaster_, CStream& Stream_);
		void _RecvMaToServer(_TMastersIt itMaster_, CStream& Stream_);

		void _LinkC(const CKey& Key_);
		void _UnLinkC(const CKey& Key_, ENetRet NetRet_);
		void _RecvC(const CKey& Key_, CStream& Stream_);
		void _RecvCaCreate(const CKey& Key_, CStream& Stream_);
		void _RecvCaLogin(const CKey& Key_, CStream& Stream_);
		void _RecvCaCheck(const CKey& Key_, CStream& Stream_);

		void _SendAcLogin(const CKey& ClientKey_, TUID UID_, const CNamePort& ClientBindNamePortPubToMaster_);

	public:
		CAuth(
			EAddressFamily AddressFamily_,
			TLinkFunc LinkFuncM_, TUnLinkFunc UnLinkFuncM_, TRecvFunc RecvFuncM_,
			TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TDBCallback DBCallbackFunc_,
			const SDBOption& DBOption_, const CNamePort& MasterBindNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_);
		template<typename TIn, typename TOut>
		inline void DBAddCmd(size_t SPNum_, const wstring& SPName_)
		{
			_DB.AddCmd<TIn, TOut>(_DBCallbacks.size() + SPNum_, SPName_, true);
		}
		template<typename... TProtos>
		inline void DBPush(size_t SPNum_, const CKey& Key_, const TProtos&... Protos_)
		{
			_DB.Push(_DBCallbacks.size() + SPNum_, Key_, Protos_...);
		}
		inline void CloseM(TPeerCnt PeerNum_)
		{
			_NetM.Close(PeerNum_);
		}
		inline bool CloseM(const CKey& Key_)
		{
			return _NetM.Close(Key_);
		}
		template<typename... _TProtos>
		inline void SendM(TPeerCnt PeerNum_, const _TProtos&... Protos_)
		{
			// 원칙은 인증받은 Master에만 전송해야 하지만 _Masters외에 또다른 컨테이너 필요하므로 그냥 전송
			_NetM.Send(PeerNum_, SHeader(EProto::AmUserProto), Protos_...);
		}
		template<typename... _TProtos>
		inline void SendM(const CKey& Key_, const _TProtos&... Protos_)
		{
			// 원칙은 인증받은 Master에만 전송해야 하지만 _Masters외에 또다른 컨테이너 필요하므로 그냥 전송
			_NetM.Send(Key_, SHeader(EProto::AmUserProto), Protos_...);
		}
		template<typename... _TProtos>
		inline void SendMAll(const _TProtos&... Protos_)
		{
			// 원칙은 인증받은 Master에만 전송해야 하지만 _Masters외에 또다른 컨테이너 필요하므로 그냥 전송
			_NetM.SendAll(SHeader(EProto::AmUserProto), Protos_...);
		}
		void SendToServerStream(TUID UID_, const CStream& Stream_)
		{
			auto itMaster = _Masters.find<0>(TRangeUID(UID_));
			if (itMaster == _Masters.end<0>())
				return;

			_NetM.Send(TPeerCnt(itMaster->second), SHeader(EProto::AmToServer), UID_, Stream_);
		}
		template<typename... _TProtos>
		inline void SendToServer(TUID UID_, const _TProtos&... Protos_)
		{
			SendToServerStream(UID_, CStream().Send(Protos_...));
		}
		TPeerCnt GetMasterNum(TUID UID_) const;
		inline void Dump(void) { _Dump.UserDump(GetCurrentProcessId()); }
		inline void Start(void) { _DB.Start(5); }
		bool Punish(TUID UID_, const TTime& EndTime_);
		void Proc(void);
	};
}
