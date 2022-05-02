#include "FCMServer.h"

namespace rso
{
	namespace mobileutil
	{
		void CFCMServer::_DBCallback(_TOutObject& /*OutObject_*/)
		{
		}
		void CFCMServer::_SetUser(const _TUsers::key_type& UserKey_, const _SUser& User_)
		{
			_Tokens.emplace(User_.Token, _Users.emplace(UserKey_, User_).first);
		}
		void CFCMServer::_SetUserHeader(void)
		{
			_UserHeader = "Content-Type: application/json;\r\n";
			_UserHeader += "Authorization: key=" + _ServerKey + "\r\n";
		}
		void CFCMServer::_ChangedCallback(size_t /*SessionIndex_*/, const string& /*ObjectName_*/, DWORD /*Received_*/, DWORD /*Total_*/)
		{
		}
		void CFCMServer::_CompletedCallback(size_t /*SessionIndex_*/, EHttpRet /*Ret_*/, const string& /*ObjectName_*/, const TBuffer& /*Buffer_*/)
		{
			if (_CCT > 0)
				--_CCT;
		}
		void CFCMServer::_PushHttp(const string& Params_)
		{
			try
			{
				_Http.Push(SInObj(_UserHeader, "fcm.googleapis.com", 443, "", "", EMethod::Post, "/fcm/send", Params_, true));
				++_CCT;
			}
			catch (...)
			{
				// 여기 온 경우 어차피 이후 보내기 실패할 수 있으므로 그냥 보내진 겻으로 처리하여 _Msgs 를 비워줌.
			}
		}
		void CFCMServer::_PushMany(void)
		{
			_PushHttp(_JsonMsgAll.toStyledString());
			_JsonMsgAll["registration_ids"].clear();
		}
		CFCMServer::CFCMServer(const SDBOption& DBOption_, size_t HttpSessionCnt_, size_t MsgQueSize_, const string& ServerKey_) :
			_Http(HttpSessionCnt_, std::bind(&CFCMServer::_ChangedCallback, this, _1, _2, _3, _4), std::bind(&CFCMServer::_CompletedCallback, this, _1, _2, _3, _4)),
			_ServerKey(ServerKey_),
			_MsgQue(MsgQueSize_),
			_DB(DBOption_, std::bind(&CFCMServer::_DBCallback, this, _1))
		{
			_SetUserHeader();

			wstring TableName = L"sys_tbl_User";
			wstring UserLoadAllSPName = L"sys_sp_UserLoadAll";
			wstring UserInsertSPName = L"sys_sp_UserInsert";
			wstring UserChangeTokenSPName = L"sys_sp_UserChangeToken";
			wstring UserChangeUIDSPName = L"sys_sp_UserChangeUID";
			wstring UserDelUIDChangeTokenSPName = L"sys_sp_UserDelUIDChangeToken";
			wstring UserDelSPName = L"sys_sp_UserDel";
			wstring UserChangeSPName = L"sys_sp_UserChange";

			_DB.AddCmd<SUserInsertDBIn, SDummyDBOut>(0, L"dbo." + UserInsertSPName, true);
			_DB.AddCmd<SUserChangeTokenDBIn, SDummyDBOut>(1, L"dbo." + UserChangeTokenSPName, true);
			_DB.AddCmd<SUserChangeUIDDBIn, SDummyDBOut>(2, L"dbo." + UserChangeUIDSPName, true);
			_DB.AddCmd<SUserDelUIDChangeTokenDBIn, SDummyDBOut>(3, L"dbo." + UserDelUIDChangeTokenSPName, true);
			_DB.AddCmd<SUserDelDBIn, SDummyDBOut>(4, L"dbo." + UserDelSPName, true);
			_DB.AddCmd<SUserChangeDBIn, SDummyDBOut>(5, L"dbo." + UserChangeSPName, true);
			_DB.Start(5);

			while (!_DB.Connected())
				this_thread::sleep_for(milliseconds(100));

			wstring Query;

			// CREATE User Table If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + TableName + LR"(]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[)" + TableName + LR"(](
	[UID] [bigint] NOT NULL,
	[Token] [varchar]()" + to_wstring(c_TokenLen_Max) + LR"() NOT NULL,
	[CanPushAtNight] [tinyint] NOT NULL,
	[MinuteOffset] [int] NOT NULL,
	[InsertedTime] [datetime] NOT NULL
 CONSTRAINT [PK_)" + TableName + LR"(] PRIMARY KEY CLUSTERED 
(
	[UID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE Table Index
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[)" + TableName + LR"(]') AND name = N'IX_)" + TableName + LR"(')
CREATE UNIQUE NONCLUSTERED INDEX [IX_)" + TableName + LR"(] ON [dbo].[)" + TableName + LR"(]
(
	[Token] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY])";

			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE Table Constraint
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_)" + TableName + LR"(_CanPushAtNight]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[)" + TableName + LR"(] ADD  CONSTRAINT [DF_)" + TableName + LR"(_CanPushAtNight]  DEFAULT ((0)) FOR [CanPushAtNight]
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_)" + TableName + LR"(_MinuteOffset]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[)" + TableName + LR"(] ADD  CONSTRAINT [DF_)" + TableName + LR"(_MinuteOffset]  DEFAULT ((0)) FOR [MinuteOffset]
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_)" + TableName + LR"(_InsertedTime]') AND type = 'D')
BEGIN
ALTER TABLE [dbo].[)" + TableName + LR"(] ADD  CONSTRAINT [DF_)" + TableName + LR"(_InsertedTime]  DEFAULT (getdate()) FOR [InsertedTime]
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE UserLoadAll SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserLoadAllSPName + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + UserLoadAllSPName + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// ALTER UserLoadAll
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + UserLoadAllSPName + LR"(]
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON
	
	SELECT	[UID], Token, CanPushAtNight, MinuteOffset
	FROM	dbo.)" + TableName + LR"(

	RETURN 0

END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE UserInsert SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserInsertSPName + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + UserInsertSPName + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// ALTER UserInsert
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + UserInsertSPName + LR"(]
	@UID_ BIGINT
,	@Token_ VARCHAR()" + to_wstring(c_TokenLen_Max) + LR"()
,	@CanPushAtNight_ TINYINT
,	@MinuteOffset_ INT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	INSERT INTO dbo.)" + TableName + LR"( ([UID], Token, CanPushAtNight, MinuteOffset)
	VALUES (@UID_, @Token_, @CanPushAtNight_, @MinuteOffset_)
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE UserChangeToken SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserChangeTokenSPName + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + UserChangeTokenSPName + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// ALTER UserChangeToken
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + UserChangeTokenSPName + LR"(]
	@UID_ BIGINT
,	@Token_ VARCHAR()" + to_wstring(c_TokenLen_Max) + LR"()
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + TableName + LR"(
	SET		Token = @Token_
	WHERE	[UID] = @UID_
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE UserChangeUID SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserChangeUIDSPName + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + UserChangeUIDSPName + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// ALTER UserChangeUID
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + UserChangeUIDSPName + LR"(]
	@UID_ BIGINT
,	@Token_ VARCHAR()" + to_wstring(c_TokenLen_Max) + LR"()
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + TableName + LR"(
	SET		[UID] = @UID_
	WHERE	Token = @Token_
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE UserDelUIDChangeToken SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserDelUIDChangeTokenSPName + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + UserDelUIDChangeTokenSPName + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// ALTER UserDelUIDChangeToken
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + UserDelUIDChangeTokenSPName + LR"(]
	@DelUID_ BIGINT
,	@UID_ BIGINT
,	@Token_ VARCHAR()" + to_wstring(c_TokenLen_Max) + LR"()
,	@MinuteOffset_ INT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	DELETE
	FROM	dbo.)" + TableName + LR"(
	WHERE	[UID] = @DelUID_

	UPDATE	dbo.)" + TableName + LR"(
	SET		Token = @Token_
		,	MinuteOffset = @MinuteOffset_
	WHERE	[UID] = @UID_
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// CREATE UserDel SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserDelSPName + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + UserDelSPName + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// ALTER UserDel
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + UserDelSPName + LR"(]
	@UID_ BIGINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	DELETE
	FROM	dbo.)" + TableName + LR"(
	WHERE	[UID] = @UID_

	RETURN 0
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();


			// CREATE UserChangeMinuteOffset SP If not exists
			Query = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + UserChangeSPName + LR"(]') AND type in (N'P', N'PC'))
BEGIN
EXEC dbo.sp_executesql @statement = N'CREATE PROCEDURE [dbo].[)" + UserChangeSPName + LR"(] AS' 
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();

			// ALTER UserChangeMinuteOffset
			Query = LR"(
ALTER PROCEDURE [dbo].[)" + UserChangeSPName + LR"(]
	@UID_ BIGINT
,	@CanPushAtNight_ TINYINT
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	UPDATE	dbo.)" + TableName + LR"(
	SET		CanPushAtNight = @CanPushAtNight_
	WHERE	[UID] = @UID_
END)";
			if (!SQL_SUCCEEDED(_DB.ExecuteDirect(Query)))
				THROWEX();



			if (!CBulkSelect<SUserLoadAllDBIn, SUserLoadAllDBOut>(
				DBOption_,
				[&](SQLLEN /*RowNum_*/, CStream& Row_) { SUserDB User; Row_ >> User; _SetUser(User.UID, _SUser(User.Token, User.CanPushAtNight, minutes(User.MinuteOffset))); return true; },
				[](SQLRETURN Ret_, int32 SpRet_, CStream& /*OutParams_*/) { ASSERTION(Ret_ == SQL_SUCCESS && SpRet_ == 0); },
				UserLoadAllSPName).Push(SUserLoadAllDBIn()))
				THROWEX();
		}
		void CFCMServer::SetServerKey(const string& ServerKey_)
		{
			_ServerKey = ServerKey_;
			_SetUserHeader();
		}
		void CFCMServer::SetUser(const _TUsers::key_type& UserKey_, const string& Token_, const minutes& MinuteOffset_)
		{
			auto ibUser = _Users.emplace(UserKey_, _SUser(Token_));
			if (!ibUser.second)
			{
				if (ibUser.first->second.Token != Token_)
				{
					auto ibToken = _Tokens.emplace(Token_, ibUser.first);
					if (!ibToken.second)
					{
						_DB.Push(3, UserKey_, SUserDelUIDChangeTokenDBIn(ibToken.first->second->first, UserKey_, Token_, MinuteOffset_.count()));

						_Users.erase(ibToken.first->second); // 기존 Token의 UID를 컨테이너에서 제거
						ibToken.first->second = ibUser.first;
					}
					else
					{
						_DB.Push(1, UserKey_, SUserChangeTokenDBIn(UserKey_, Token_));
					}

					_Tokens.erase(ibUser.first->second.Token); // 기존 UID의 Token을 컨테이너에서 제거
					ibUser.first->second.Token = Token_;
				}
			}
			else
			{
				try
				{
					auto ibToken = _Tokens.emplace(Token_, ibUser.first);
					if (!ibToken.second)
					{
						_DB.Push(2, UserKey_, SUserChangeUIDDBIn(UserKey_, Token_));

						_Users.erase(ibToken.first->second);
						ibToken.first->second = ibUser.first;
					}
					else
					{
						_DB.Push(0, UserKey_, SUserInsertDBIn(SUserDB(UserKey_, ibUser.first->second.Token, ibUser.first->second.CanPushAtNight, ibUser.first->second.MinuteOffset.count())));
					}
				}
				catch (...)
				{
					_Users.erase(ibUser.first);
					throw;
				}
			}
		}
		void CFCMServer::DelUser(const _TUsers::key_type& UserKey_)
		{
			auto itUser = _Users.find(UserKey_);
			if (itUser == _Users.end())
				return;

			_Tokens.erase(itUser->second.Token);
			_Users.erase(itUser);
			_DB.Push(4, UserKey_, SUserDelDBIn(UserKey_));
		}
		void CFCMServer::ChangeUser(const _TUsers::key_type& UserKey_, bool CanPushAtNight_)
		{
			auto itUser = _Users.find(UserKey_);
			if (itUser == _Users.end())
				return;

			itUser->second.CanPushAtNight = CanPushAtNight_;

			_DB.Push(5, UserKey_, SUserChangeDBIn(UserKey_, CanPushAtNight_));
		}
		void CFCMServer::PushOne(TUID UID_, const string& Title_, const string& Msg_)
		{
			auto itUser = _Users.find(UID_);
			if (itUser == _Users.end())
				return;

			if (!_CanPush(itUser))
				return;

			PushOne(itUser->second.Token, Title_, Msg_);
		}
		void CFCMServer::PushOne(const string& RID_, const string& Title_, const string& Msg_)
		{
			_MsgQue.Push(SMsg{ RID_, Title_, Msg_ });
		}
		void CFCMServer::PushAll(const string& Title_, const string& Msg_)
		{
			_MsgQue.Push(SMsg{ "", Title_, Msg_ });
		}
		void CFCMServer::Proc(void)
		{
			_DB.Proc();
			_Http.Proc();

			try
			{
				while (!_Full())
				{
					bool NewMsg = false;
					auto Msg = _MsgQue.Get(NewMsg);
					if (!Msg)
						break;

					if (Msg->RID == "")
					{
						if (NewMsg)
						{
							_JsonMsgAll["notification"]["title"] = Msg->Title;
							_JsonMsgAll["notification"]["body"] = Msg->Msg;
						}

						int32	UserCntToPush = 0;

						while (true)
						{
							auto it = _Users.cycle();
							if (it == _Users.end())
							{
								if (UserCntToPush > 0) // _PushMany() 이후 UserCntToPush = 0 하기 때문에 UserCntToPush > 0 이면 아직 !_Full()
									_PushMany();

								_MsgQue.Pop();
								break;
							}

							if (!_CanPush(it))
								continue;

							_JsonMsgAll["registration_ids"][UserCntToPush++] = it->second.Token;

							if (UserCntToPush == c_SendRIDCnt_Max)
							{
								_PushMany();
								UserCntToPush = 0;

							if (_Full())
								break;
							}
						}
					}
					else
					{
						_JsonMsgOne["to"] = Msg->RID;
						_JsonMsgOne["notification"]["title"] = Msg->Title;
						_JsonMsgOne["notification"]["body"] = Msg->Msg;
						_PushHttp(_JsonMsgOne.toStyledString());
						_MsgQue.Pop();
					}
				}
			}
			catch (...)
			{
			}
		}
	}
}