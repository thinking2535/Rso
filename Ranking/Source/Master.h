#pragma once

#include "Server.h"
#include <Rso/MSSQL/StoredProcedure.h>

namespace rso::ranking
{
	using namespace mssql;

	class CMaster : public CServer
	{
		using _TDB = CStoredProcedure<TUID>;
		using _TOutObject = _TDB::SOutObject;

		TLinkFunc _LinkFuncA;
		TUnLinkFunc _UnLinkFuncA;
		SMasterOption _MasterOption;
		_TDB::TCallbacks _Binder;
		_TDB _DB;
		bool _Loaded = false;
		net::CServer _NetA;
		CPeriod<seconds> _TimerPeriod = seconds(5);
		SConfigDB _Config;

		wstring _SPNameRankingLoad = L"sys_sp_RankingLoad";
		wstring _SPNameRewardLoad = L"sys_sp_RewardLoad";
		wstring _SPNameConfigLoad = L"sys_sp_ConfigLoad";
		wstring _SPNameRefresh = L"sys_sp_Refresh";
		wstring _SPNameUpdate = L"sys_sp_Update";

		void _CreateDataBase(void);

		SUserMinPointSa _GetUserMinPointSa(void);
		SInfoSa _GetInfoSa(void);
		bool _RowCallback(SQLLEN RowNum_, CStream& Row_);
		void _ParamCallback(SQLRETURN Ret_, int32 SpRet_, CStream& OutParams_);
		bool _RowCallbackReward(SQLLEN RowNum_, CStream& Row_);
		void _ParamCallbackReward(SQLRETURN Ret_, int32 SpRet_, CStream& OutParams_);
		void _Callback(_TOutObject& OutObject_);
		void _CallbackDummy(_TOutObject& OutObject_);
		void _CallbackConfigLoad(_TOutObject& OutObject_);
		void _LinkA(const CKey& Key_);
		void _UnLinkA(const CKey& Key_, ENetRet NetRet_);
		void _RecvA(const CKey& Key_, CStream& Stream_);
		void _RecvAUpdate(const CKey& Key_, CStream& Stream_);

		template<typename _TIn, typename _TOut>
		void _AddCmd(const wstring& SPName_, bool Return_, _TDB::TCallback Callback_)
		{
			_DB.AddCmd<_TIn, _TOut>(static_cast<size_t>(SDBBinder<_TIn>::SpNum), SPName_, Return_);
			_Binder.emplace_at(static_cast<size_t>(SDBBinder<_TIn>::SpNum), Callback_);
		}
		template<typename _TParam, typename... _TParams>
		void _Push(const _TParam& Param_, const _TParams&... Params_)
		{
			_DB.Push(static_cast<size_t>(SDBBinder<_TParam>::SpNum), 0, Param_, Params_...);
		}

	public:
		CMaster(
			TLinkFunc LinkFuncU_, TLinkFailFunc LinkFailFuncU_, TUnLinkFunc UnLinkFuncU_,
			TLinkFunc LinkFuncD_, TUnLinkFunc UnLinkFuncD_,
			const SOption& Option_,
			TLinkFunc LinkFuncA_, TUnLinkFunc UnLinkFuncA_,
			const SMasterOption& MasterOption_, const SDBOption& DBOption_);
		void Proc(void);
	};
}
