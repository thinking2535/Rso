#include "Master.h"
#include <Rso/Game/Base.h>
#include <Rso/MSSQL/BulkSelect.h>

namespace rso::ranking
{
	using namespace game;

	void CMaster::_CreateDataBase(void)
	{
		wstring TypeNameReward = L"sys_type_Reward";
		wstring TypeNameUser = L"sys_type_User";
		wstring TypeNameUID = L"sys_type_UID";
		wstring TableNameConfig = L"sys_tbl_Config";
		wstring TableNameRanking = L"sys_tbl_Ranking";
		wstring TableNameReward = L"sys_tbl_Reward";

		// TRankingReward ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring TypeName = CMSSQL::GetFullName(TypeNameReward);
			wstring Query = LR"(
IF TYPE_ID(N')" + TypeName + LR"(') IS NULL
BEGIN
CREATE TYPE )" + TypeNameReward + LR"( AS TABLE(
	[UID] [bigint] NULL,
	[Ranking] [int] NULL
)
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// TRankingUser ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring TypeName = CMSSQL::GetFullName(TypeNameUser);
			wstring Query = LR"(
IF TYPE_ID(N')" + TypeName + LR"(') IS NULL
BEGIN
CREATE TYPE )" + TypeName + LR"( AS TABLE(
	[Nick] [nvarchar]()" + to_wstring(c_NickLengthMax) + LR"() NULL,
	[CharCode] [int] NULL,
	[CountryCode] [nvarchar](2) NULL,
	[UID] [bigint] NULL,
	[Point] [int] NULL
)
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// TUID ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring TypeName = CMSSQL::GetFullName(TypeNameUID);
			wstring Query = LR"(
IF TYPE_ID(N')" + TypeName + LR"(') IS NULL
BEGIN
CREATE TYPE )" + TypeName + LR"( AS TABLE(
	[UID] [bigint] NULL
)
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// Config ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring TableName = CMSSQL::GetFullName(TableNameConfig);
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + TableName + LR"(') AND type in (N'U'))
BEGIN
CREATE TABLE )" + TableName + LR"((
	[Counter] [bigint] NOT NULL,
	[ExpireTimePoint] [bigint] NOT NULL
) ON [PRIMARY]
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// Ranking ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring ClusteredIndexName = L"IX_" + TableNameRanking;
			wstring UniqueIndexName = ClusteredIndexName + L"_1";
			wstring ConstraintName = L"DF_" + TableNameRanking + L"_InsertedTime";
			wstring TableName = CMSSQL::GetFullName(TableNameRanking);
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + TableName + LR"(') AND type in (N'U'))
BEGIN
CREATE TABLE )" + TableName + LR"((
	[UID] [bigint] NOT NULL,
	[Point] [int] NOT NULL,
	[Nick] [nvarchar]()" + to_wstring(c_NickLengthMax) + LR"() NOT NULL,
	[CharCode] [int] NOT NULL,
	[CountryCode] [nvarchar](2) NOT NULL,
	[InsertedTime] [datetime] NOT NULL
) ON [PRIMARY]
END
)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();


			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N')" + TableName + LR"(') AND name = N')" + ClusteredIndexName + LR"(')
CREATE CLUSTERED INDEX [)" + ClusteredIndexName + LR"(] ON )" + TableName + LR"(
(
	[Point] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();


			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N')" + TableName + LR"(') AND name = N')" + UniqueIndexName + LR"(')
CREATE UNIQUE NONCLUSTERED INDEX [)" + UniqueIndexName + LR"(] ON )" + TableName + LR"(
(
	[UID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();


			Query = LR"(
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[)" + ConstraintName + LR"(]') AND type = 'D')
BEGIN
ALTER TABLE )" + TableName + LR"( ADD  CONSTRAINT [)" + ConstraintName + LR"(]  DEFAULT (getdate()) FOR [InsertedTime]
END
)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}

		// Reward ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring TableName = CMSSQL::GetFullName(TableNameReward);
			wstring IndexName = L"PK_" + TableNameReward;
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + TableName + LR"(') AND type in (N'U'))
BEGIN
CREATE TABLE )" + TableName + LR"((
	[UID] [bigint] NOT NULL,
	[Ranking] [int] NOT NULL,
 CONSTRAINT [)" + IndexName + LR"(] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// RankingLoad ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring SPName = CMSSQL::GetFullName(_SPNameRankingLoad);
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + SPName + LR"(') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE )" + SPName + LR"( AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			Query = LR"(
ALTER PROCEDURE )" + SPName + LR"(
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	SELECT	Nick, CharCode, CountryCode, [UID], Point
	FROM	)" + CMSSQL::GetFullName(TableNameRanking) + LR"(
	ORDER BY Point DESC, InsertedTime

	RETURN 0

END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// RewardLoad ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring SPName = CMSSQL::GetFullName(_SPNameRewardLoad);
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + SPName + LR"(') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE )" + SPName + LR"( AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			Query = LR"(
ALTER PROCEDURE )" + SPName + LR"(
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	SELECT	[UID], Ranking
	FROM	)" + CMSSQL::GetFullName(TableNameReward) + LR"(

	RETURN 0

END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// ConfigLoad ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring SPName = CMSSQL::GetFullName(_SPNameConfigLoad);
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + SPName + LR"(') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE )" + SPName + LR"( AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			Query = LR"(
ALTER PROCEDURE )" + SPName + LR"(
	@ExpireTimePoint_ BIGINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	MERGE )" + CMSSQL::GetFullName(TableNameConfig) + LR"( AS tT
	USING (VALUES (0)) AS Src(x)
	ON [Counter] IS NOT NULL
	WHEN NOT MATCHED THEN
		INSERT ([Counter], ExpireTimePoint)
		VALUES (1, @ExpireTimePoint_);

	SELECT	[Counter], ExpireTimePoint
	FROM	)" + CMSSQL::GetFullName(TableNameConfig) + LR"(

	RETURN 0

END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// Refresh ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring SPName = CMSSQL::GetFullName(_SPNameRefresh);
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + SPName + LR"(') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE )" + SPName + LR"( AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			Query = LR"(
ALTER PROCEDURE )" + SPName + LR"(
	@Counter_ BIGINT
,	@ExpireTimePoint_ BIGINT
,	@tRewards_ )" + CMSSQL::GetFullName(TypeNameReward) + LR"( READONLY
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	)" + CMSSQL::GetFullName(TableNameConfig) + LR"(
	SET		[Counter] = @Counter_
		,	ExpireTimePoint = @ExpireTimePoint_

	TRUNCATE TABLE )" + CMSSQL::GetFullName(TableNameRanking) + LR"(
	TRUNCATE TABLE )" + CMSSQL::GetFullName(TableNameReward) + LR"(

	INSERT INTO )" + CMSSQL::GetFullName(TableNameReward) + LR"( ([UID], [Ranking])
		SELECT	[UID], [Ranking]
		FROM	@tRewards_

	RETURN 0

END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}


		// Update ////////////////////////////////////////////////////////////////////////////////////////////
		{
			wstring SPName = CMSSQL::GetFullName(_SPNameUpdate);
			wstring Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N')" + SPName + LR"(') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE )" + SPName + LR"( AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			Query = LR"(
ALTER PROCEDURE )" + SPName + LR"(
	@Merged_ )" + CMSSQL::GetFullName(TypeNameUser) + LR"(  READONLY
,	@Deleted_ )" + CMSSQL::GetFullName(TypeNameUID) + LR"(  READONLY
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	DECLARE @Ret INT = 0
	
	BEGIN TRY

		BEGIN TRAN

		MERGE )" + CMSSQL::GetFullName(TableNameRanking) + LR"( AS tT
		USING @Merged_ AS tS
		ON (tT.[UID] = tS.[UID])
		WHEN MATCHED THEN
			UPDATE
			SET	Point = tS.Point
			,	Nick = tS.Nick
			,	CharCode = tS.CharCode
			,	CountryCode = tS.CountryCode
		WHEN NOT MATCHED THEN
			INSERT ([UID], Point, Nick, CharCode, CountryCode)
			VALUES ([UID], Point, Nick, CharCode, CountryCode);

		DELETE	tR
		FROM	)" + CMSSQL::GetFullName(TableNameRanking) + LR"( tR
			,	@Deleted_ tD
		WHERE	tR.[UID] = tD.[UID]

		COMMIT TRAN

		RETURN 0
		
	END TRY
	BEGIN CATCH
		IF
			ERROR_NUMBER() <> 50000 AND
			ERROR_NUMBER() <> 266
		SET @Ret = ERROR_NUMBER()
		
		IF XACT_STATE() <> 0
			ROLLBACK TRAN
		
		RETURN @Ret
		
	END CATCH

END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();
		}
	}
	SUserMinPointSa CMaster::_GetUserMinPointSa(void)
	{
		int64 UserPointMin = 0;

		if (_MasterOption.MaxDataCount > 0 && _Ranking.size() >= _MasterOption.MaxDataCount)
			UserPointMin = _Ranking.operator() < 1 > ().rbegin()->first;

		return SUserMinPointSa(UserPointMin);
	}
	SInfoSa CMaster::_GetInfoSa(void)
	{
		return SInfoSa(_Config.Counter, _GetUserMinPointSa(), _Rewards);
	}
	bool CMaster::_RowCallback(SQLLEN /*RowNum_*/, CStream& Row_)
	{
		SUser User;
		Row_ >> User;

		_Ranking.emplace(User, User.UID, User.Point);
		return true;
	}
	void CMaster::_ParamCallback(SQLRETURN Ret_, int32 SpRet_, CStream& /*OutParams_*/)
	{
		if (Ret_ != 0 || SpRet_ != 0)
			THROWEX();

		_Loaded = true;
	}
	bool CMaster::_RowCallbackReward(SQLLEN /*RowNum_*/, CStream& Row_)
	{
		TUID UID;
		TRanking Ranking;
		Row_ >> UID;
		Row_ >> Ranking;

		_Rewards.emplace(UID, Ranking);
		return true;
	}
	void CMaster::_ParamCallbackReward(SQLRETURN Ret_, int32 SpRet_, CStream& /*OutParams_*/)
	{
		if (Ret_ != 0 || SpRet_ != 0)
			THROWEX();

		_RewardLoaded = true;
	}
	void CMaster::_Callback(_TOutObject& OutObject_)
	{
		_Binder[OutObject_.SPNum](OutObject_);
	}
	void CMaster::_CallbackDummy(_TOutObject& OutObject_)
	{
		if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
			THROWEX();
	}
	void CMaster::_CallbackConfigLoad(_TOutObject& OutObject_)
	{
		SConfigLoadDBIn In;
		SConfigLoadDBOut Out;

		if (OutObject_.Ret != 0 || OutObject_.SPRet != 0)
			THROWEX();

		OutObject_.InStream >> In;
		OutObject_.OutStream >> Out;

		if (Out.Configs.empty())
			THROWEX();

		_Config = Out.Configs.front();
	}
	void CMaster::_LinkA(const CKey& Key_)
	{
		_NetA.Certify(Key_.PeerNum);
		_NetA.Send(Key_.PeerNum, EProto::InfoSa, _GetInfoSa());
		_LinkFuncA(Key_);
	}
	void CMaster::_UnLinkA(const CKey& Key_, ENetRet NetRet_)
	{
		_UnLinkFuncA(Key_, NetRet_);
	}
	void CMaster::_RecvA(const CKey& Key_, CStream& Stream_)
	{
		EProto ProtoNum;
		Stream_ >> ProtoNum;

		switch (ProtoNum)
		{
		case EProto::UpdateAs: _RecvAUpdate(Key_, Stream_); break;
		default: return _NetA.Close(Key_.PeerNum);
		}
	}
	void CMaster::_RecvAUpdate(const CKey& /*Key_*/, CStream& Stream_)
	{
		SUpdateAs Proto;
		Stream_ >> Proto;

		list<TUID> Deleted;

		for (auto it = Proto.Changed.begin(); it != Proto.Changed.end();)
		{
			auto itCheck = it;
			++it;

			auto itRanking = _Ranking.find<0>(itCheck->UID);
			if (itRanking != _Ranking.end<0>())
			{
				_Ranking.erase(itRanking->second); // 멀티점수는 실시간으로 줄어드는 것을 허용하므로 기존 랭킹 값보다 작은값이 들어와도 갱신
			}
			else
			{
				if (_Ranking.size() >= _MasterOption.MaxDataCount)
				{
					auto itBottom = --_Ranking.rbegin<1>().base();
					if (itCheck->Point <= itBottom->first)
					{
						Proto.Changed.erase(itCheck);
						continue;
					}

					Deleted.emplace_back(std::get<0>(_Ranking[itBottom->second].first)->first);
					_Ranking.erase(itBottom->second);
				}
			}

			_Ranking.emplace(*itCheck, itCheck->UID, itCheck->Point);
		}

		_Push(SUpdateDBIn(std::move(Proto.Changed), std::move(Deleted)));
	}
	CMaster::CMaster(
		TLinkFunc LinkFuncU_, TLinkFailFunc LinkFailFuncU_, TUnLinkFunc UnLinkFuncU_,
		TLinkFunc LinkFuncD_, TUnLinkFunc UnLinkFuncD_,
		const SOption& Option_,
		TLinkFunc LinkFuncA_, TUnLinkFunc UnLinkFuncA_,
		const SMasterOption& MasterOption_, const SDBOption& DBOption_) :
		CServer(
			LinkFuncU_, LinkFailFuncU_, UnLinkFuncU_,
			LinkFuncD_, UnLinkFuncD_,
			Option_),
		_LinkFuncA(LinkFuncA_), _UnLinkFuncA(UnLinkFuncA_),
		_MasterOption(MasterOption_),
		_DB(DBOption_, std::bind(&CMaster::_Callback, this, _1)),
		_NetA(
			EAddressFamily::INET,
			std::bind(&CMaster::_LinkA, this, _1),
			std::bind(&CMaster::_UnLinkA, this, _1, _2),
			std::bind(&CMaster::_RecvA, this, _1, _2),
			false, 10240000, 10240000,
			milliseconds(), milliseconds(0), 4, 0,
			CNamePort(MasterOption_.AgentBindPort), SOMAXCONN)
	{
		_AddCmd<SConfigLoadDBIn, SConfigLoadDBOut>(CMSSQL::GetFullName(_SPNameConfigLoad), true, std::bind(&CMaster::_CallbackConfigLoad, this, _1));
		_AddCmd<SRefreshDBIn, SDummyDBOut>(CMSSQL::GetFullName(_SPNameRefresh), true, std::bind(&CMaster::_CallbackDummy, this, _1));
		_AddCmd<SUpdateDBIn, SDummyDBOut>(CMSSQL::GetFullName(_SPNameUpdate), true, std::bind(&CMaster::_CallbackDummy, this, _1));

		_DB.Start(5);

		_CreateDataBase();

		{
			_Push(SConfigLoadDBIn(CDateTime(SDateTime(_MasterOption.PeriodBaseTime), true).ToTimePoint() + minutes(_MasterOption.PeriodMinutes)));

			while (_Config.Counter == 0)
			{
				_DB.Proc();
				this_thread::sleep_for(milliseconds(10));
			}
		}

		{
			CBulkSelect<SLoadDBIn, SLoadDBOut> bs(
				DBOption_,
				std::bind(&CMaster::_RowCallback, this, _1, _2),
				std::bind(&CMaster::_ParamCallback, this, _1, _2, _3),
				CMSSQL::GetFullName(_SPNameRankingLoad));

			if (!bs.Push(SLoadDBIn()))
				THROWEX();

			while (!_Loaded)
			{
				bs.Proc();
				this_thread::sleep_for(milliseconds(10));
			}
		}

		{
			CBulkSelect<SRewardLoadDBIn, SRewardLoadDBOut> bs(
				DBOption_,
				std::bind(&CMaster::_RowCallbackReward, this, _1, _2),
				std::bind(&CMaster::_ParamCallbackReward, this, _1, _2, _3),
				CMSSQL::GetFullName(_SPNameRewardLoad));

			if (!bs.Push(SRewardLoadDBIn()))
				THROWEX();

			while (!_RewardLoaded)
			{
				bs.Proc();
				this_thread::sleep_for(milliseconds(10));
			}
		}
	}
	void CMaster::Proc(void)
	{
		CServer::Proc();
		_DB.Proc();
		_NetA.Proc();

		if (_TimerPeriod.CheckAndNextLoose())
		{
			_BroadCast();

			auto Now = system_clock::now();
			if (_Config.ExpireTimePoint <= Now)
			{
				// Refresh ////////////////////////////////
				++_Config.Counter;
				_Config.ExpireTimePoint += (((Now - _Config.ExpireTimePoint) / minutes(_MasterOption.PeriodMinutes)) + 1) * minutes(_MasterOption.PeriodMinutes);

				{
					_Rewards.clear();
					int32 Ranking = 0;
					for (auto& i : _Ranking.operator() < 1 > ())
					{
						if (_MasterOption.MaxRankingPoint < Ranking)
							break;

						_Rewards.emplace(std::get<0>(_Ranking[i.second].first)->first, Ranking);
						++Ranking;
					}
				}

				_Ranking.clear();

				_Push(SRefreshDBIn(_Config, _Rewards));
				_NetA.SendAll(EProto::InfoSa, _GetInfoSa());
			}
			else
			{
				_NetA.SendAll(EProto::UserMinPointSa, _GetUserMinPointSa());
			}
		}
	}
}
