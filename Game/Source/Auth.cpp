#include "Auth.h"

namespace rso
{
	namespace game
	{
		void CAuth::_DBCallback(TOutObject& OutObject_)
		{
			if (OutObject_.SPNum < _DBCallbacks.size())
			{
				_DBCallbacks[OutObject_.SPNum](OutObject_);
			}
			else
			{
				OutObject_.SPNum -= _DBCallbacks.size();
				_DBCallbackFunc(OutObject_);
			}
		}
		void CAuth::_DBCreate(TOutObject& OutObject_)
		{
			auto itClient = _Clients.get(OutObject_.Key.PeerNum);
			if (!itClient)
				return;

			if (itClient->Key != OutObject_.Key)
				return;

			SAuthCreateInDb In;
			OutObject_.InStream >> In;
			TState State = 0;
			OutObject_.InStream >> State;

			try
			{
				if (OutObject_.Ret != 0)
					throw EGameRet::DataBaseError;

				if (OutObject_.SPRet == 2601 || OutObject_.SPRet == 2627)
					throw EGameRet::AlreadyExist;
				else if (OutObject_.SPRet != 0)
					throw EGameRet::DataBaseError;

				SAuthLoginOutDb Out;
				OutObject_.OutStream >> Out;

				auto itUser = Out.Users.begin();
				if (itUser == Out.Users.end())
					throw EGameRet::InvalidID;

				if (itUser->UID <= 0)
					throw EGameRet::SystemError;

				auto itMaster = _Masters.find<0>(TRangeUID(itUser->UID));
				if (itMaster == _Masters.end<0>())
					throw EGameRet::NoMasterToConnect;

				_NetM.Send((TPeerCnt)itMaster->second, SHeader(EProto::AmCreate), SAmCreate(itUser->UID, SAccount(In.ID, itUser->Nick), State, OutObject_.Key)); // 전송 실패해도 클라는 Certify 안하였으므로 Disconnect 될 것이고, Hub로 재접속 하게됨.
			}
			catch (const EGameRet GameRet_)
			{
				_NetC.Send(OutObject_.Key, SHeader(EProto::AcLoginFail), SAcLoginFail(GameRet_));
				_NetC.WillClose(OutObject_.Key, milliseconds(3000));
			}
		}
		void CAuth::_DBLogin(TOutObject& OutObject_)
		{
			auto itClient = _Clients.get(OutObject_.Key.PeerNum);
			if (!itClient)
				return;

			if (itClient->Key != OutObject_.Key)
				return;

			SAuthLoginInDb In;
			OutObject_.InStream >> In;

			try
			{
				if (OutObject_.Ret != 0)
					throw EGameRet::DataBaseError;

				if (OutObject_.SPRet == 2601 || OutObject_.SPRet == 2627)
					throw EGameRet::AlreadyExist;
				else if (OutObject_.SPRet != 0)
					throw EGameRet::DataBaseError;

				SAuthLoginOutDb Out;
				OutObject_.OutStream >> Out;

				auto itUser = Out.Users.begin();
				if (itUser == Out.Users.end())
					throw EGameRet::InvalidID;

				if (itUser->UID <= 0)
					throw EGameRet::SystemError;

				auto itMaster = _Masters.find<0>(TRangeUID(itUser->UID));
				if (itMaster == _Masters.end<0>())
					throw EGameRet::NoMasterToConnect;

				_SendAcLogin(OutObject_.Key, itUser->UID, _Masters[itMaster->second].second.ClientBindNamePort);
			}
			catch (const EGameRet GameRet_)
			{
				_NetC.Send(OutObject_.Key, SHeader(EProto::AcLoginFail), SAcLoginFail(GameRet_));
				_NetC.WillClose(OutObject_.Key, milliseconds(3000));
			}
		}
		void CAuth::_DBAddFriendGetUID(TOutObject& OutObject_)
		{
			SAuthAddFriendGetUIDInDb In;
			OutObject_.InStream >> In;

			SMaAddFriendGetUID MaAddFriendGetUID;
			OutObject_.InStream >> MaAddFriendGetUID;

			try
			{
				if (OutObject_.Ret != 0)
					throw EGameRet::DataBaseError;

				if (OutObject_.SPRet != 0)
					throw EGameRet::DataBaseError;

				SAuthAddFriendGetUIDOutDb Out;
				OutObject_.OutStream >> Out;

				auto itUID = Out.UIDs.begin();
				if (itUID == Out.UIDs.end())
					throw EGameRet::InvalidNick;

				if (*itUID <= 0)
					throw EGameRet::SystemError;

				if (*itUID == MaAddFriendGetUID.FromUID)
					throw EGameRet::InvalidNick;

				_NetM.Send(OutObject_.Key, SHeader(EProto::AmAddFriendGetUID), SAmAddFriendGetUID(*itUID, In.Nick, MaAddFriendGetUID.FromUID, MaAddFriendGetUID.FromNick, MaAddFriendGetUID.FromServerKey));
			}
			catch (const EGameRet GameRet_)
			{
				_NetM.Send(OutObject_.Key, SHeader(EProto::AmAddFriendGetUIDFail), SAmAddFriendGetUIDFail(GameRet_, MaAddFriendGetUID.FromUID, MaAddFriendGetUID.FromServerKey));
			}
		}
		void CAuth::_DBChangeNick(TOutObject& OutObject_)
		{
			SAuthChangeNickInDb In;
			OutObject_.InStream >> In;

			SKey ClientKey;
			OutObject_.InStream >> ClientKey;

			SKey ServerKey;
			OutObject_.InStream >> ServerKey;

			try
			{
				if (OutObject_.Ret != 0)
					throw EGameRet::DataBaseError;

				if (OutObject_.SPRet == 2601 || OutObject_.SPRet == 2627)
					throw EGameRet::AlreadyExist;
				else if (OutObject_.SPRet != 0)
					throw EGameRet::DataBaseError;

				// OutObject_.Key 가 다르면 전송 실패하므로 _Masters 에 있는지 찾을 필요 없음.
				_NetM.Send(OutObject_.Key, SHeader(EProto::AmChangeNick), SAmChangeNick(SMsChangeNick(In.Nick, ClientKey, In.UID), ServerKey));
			}
			catch (const EGameRet GameRet_)
			{
				// OutObject_.Key 가 다르면 전송 실패하므로 _Masters 에 있는지 찾을 필요 없음.
				_NetM.Send(OutObject_.Key, SHeader(EProto::AmChangeNickFail), SAmChangeNickFail(SMsChangeNickFail(GameRet_, ClientKey, In.UID), ServerKey));
			}
		}
		void CAuth::_DBCheck(TOutObject& OutObject_)
		{
			auto itClient = _Clients.get(OutObject_.Key.PeerNum);
			if (!itClient)
				return;

			if (itClient->Key != OutObject_.Key)
				return;

			SAuthLoginInDb In;
			OutObject_.InStream >> In;

			try
			{
				SAuthLoginOutDb Out;
				OutObject_.OutStream >> Out;

				auto itLogin = Out.Users.begin();
				if (itLogin == Out.Users.end())
				{
					_NetC.Send(OutObject_.Key, SHeader(EProto::AcCheck), SAcCheck());
					return;
				}

				auto itMaster = _Masters.find<0>(TRangeUID(itLogin->UID));
				if (itMaster == _Masters.end<0>())
					throw EGameRet::NoMasterToConnect;

				_NetM.Send(static_cast<TPeerCnt>(itMaster->second), SHeader(EProto::AmCheck), SAmCheck(itLogin->UID, OutObject_.Key));
			}
			catch (const EGameRet GameRet_)
			{
				_NetC.Send(OutObject_.Key, SHeader(EProto::AcCheckFail), SAcCheckFail(GameRet_));
				_NetC.WillClose(OutObject_.Key, milliseconds(3000));
			}
		}

		void CAuth::_LinkM(const CKey& /*Key_*/)
		{
		}
		void CAuth::_UnLinkM(const CKey& Key_, ENetRet NetRet_)
		{
			auto itMaster = _Masters.get(Key_.PeerNum);
			if (itMaster)
			{
				_NetM.SendAll(SHeader(EProto::AmOtherMasterOff), SAmOtherMasterOff(std::get<0>(itMaster->first)->first));
				_Masters.erase(itMaster);
				_UnLinkFuncM(Key_, NetRet_);
			}
		}
		void CAuth::_RecvM(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			auto itMaster = _Masters.get(Key_.PeerNum);
			if (itMaster)
			{
				switch (Header.Proto)
				{
				case EProto::MaServerOn: return _RecvMaServerOn(itMaster, Stream_);
				case EProto::MaServerOff: return _RecvMaServerOff(itMaster, Stream_);
				case EProto::MaCreate: return _RecvMaCreate(itMaster, Stream_);
				case EProto::MaCreateFail: return _RecvMaCreateFail(itMaster, Stream_);
				case EProto::MaChangeNick: return _RecvMaChangeNick(itMaster, Stream_);
				case EProto::MaCheck: return _RecvMaCheck(itMaster, Stream_);
				case EProto::MaCheckFail: return _RecvMaCheckFail(itMaster, Stream_);

				case EProto::MaAddFriendGetUID: return _RecvMaAddFriendGetUID(itMaster, Stream_);
				case EProto::MaAddFriend: return _RecvMaAddFriend(itMaster, Stream_);
				case EProto::MaAddFriendRequest: return _RecvMaAddFriendRequest(itMaster, Stream_);
				case EProto::MaAddFriendRequestFail: return _RecvMaAddFriendRequestFail(itMaster, Stream_);

				case EProto::MaToServer: return _RecvMaToServer(itMaster, Stream_);
				case EProto::MaUserProto: return _RecvFuncM(Key_, Stream_);
				default: return _NetM.Close(Key_.PeerNum);
				}
			}
			else
			{
				switch (Header.Proto)
				{
				case EProto::MaMasterOn: return _RecvMaMasterOn(Key_, Stream_);
				default: return _NetM.Close(Key_.PeerNum);
				}
			}
		}
		void CAuth::_RecvMaMasterOn(const CKey& Key_, CStream& Stream_)
		{
			SMaMasterOn Proto;
			Stream_ >> Proto;

			list<SMaster> OtherMasters;
			for (auto it = _Masters.begin(); it != _Masters.end(); ++it)
				OtherMasters.emplace_back(SMaster(SMasterNet(std::get<0>(it->first)->first, it->second.Servers), it->second.MasterBindNamePort));

			SNamePort MasterBindNamePort(_NetM.GetNamePort(Key_.PeerNum).Name, Proto.MasterBindPort);
			_Masters.emplace_at(Key_.PeerNum,
				_SMaster(
					Key_,
					CNamePort(Proto.ClientBindNamePortPub),
					CNamePort(MasterBindNamePort),
					Proto.Servers),
				TRangeUID(Proto.RangeUID));

			_NetM.SendAllExcept(Key_.PeerNum, SHeader(EProto::AmOtherMasterOn), SAmOtherMasterOn(SMaster(SMasterNet(Proto.RangeUID, Proto.Servers), MasterBindNamePort)));
			_NetM.Send(Key_.PeerNum, SHeader(EProto::AmMasterOn), SAmMasterOn(std::move(OtherMasters)));
			_NetM.Certify(Key_.PeerNum);
			_LinkFuncM(Key_);
		}
		void CAuth::_RecvMaServerOn(_TMastersIt itMaster_, CStream& Stream_)
		{
			SMaServerOn Proto;
			Stream_ >> Proto;

			SServerNet Server(Proto.ServerBindNamePort);
			itMaster_->second.Servers.emplace(Server);
			_NetM.SendAllExcept(itMaster_->second.Key.PeerNum, SHeader(EProto::AmOtherMasterServerOn), SAmOtherMasterServerOn(std::get<0>(itMaster_->first)->first, Server));
		}
		void CAuth::_RecvMaServerOff(_TMastersIt itMaster_, CStream& Stream_)
		{
			SMaServerOff Proto;
			Stream_ >> Proto;

			itMaster_->second.Servers.erase(Proto.Server);

			_NetM.SendAllExcept(static_cast<TPeerCnt>(itMaster_.Index()),
				SHeader(EProto::AmOtherMasterServerOff), SAmOtherMasterServerOff(std::get<0>(itMaster_->first)->first, Proto.Server));
		}
		void CAuth::_RecvMaCreate(_TMastersIt itMaster_, CStream& Stream_)
		{
			SMaCreate Proto;
			Stream_ >> Proto;

			_SendAcLogin(Proto.UserKey, Proto.UID, itMaster_->second.ClientBindNamePort);
		}
		void CAuth::_RecvMaCreateFail(_TMastersIt /*Master_*/, CStream& Stream_)
		{
			SMaCreateFail Proto;
			Stream_ >> Proto;

			_NetC.Send(Proto.UserKey, SHeader(EProto::AcLoginFail), SAcLoginFail(Proto.GameRet));
			_NetC.WillClose(Proto.UserKey, milliseconds(3000));
		}
		void CAuth::_RecvMaChangeNick(_TMastersIt itMaster_, CStream& Stream_)
		{
			SMaChangeNick Proto;
			Stream_ >> Proto;

			_DBPush(itMaster_->second.Key, SAuthChangeNickInDb(Proto.UID, Proto.Nick), Proto.ClientKey, Proto.ServerKey);
		}
		void CAuth::_RecvMaAddFriendGetUID(_TMastersIt itMaster_, CStream& Stream_)
		{
			SMaAddFriendGetUID Proto;
			Stream_ >> Proto;

			// get uid with nick
			_DBPush(itMaster_->second.Key, SAuthAddFriendGetUIDInDb(Proto.ToNick), Proto);
		}
		void CAuth::_RecvMaAddFriend(_TMastersIt itMaster_, CStream& Stream_)
		{
			SMaAddFriend Proto;
			Stream_ >> Proto;

			try
			{
				auto itMaster = _Masters.find<0>(TRangeUID(Proto.ToUID));
				if (itMaster == _Masters.end<0>())
					throw EGameRet::InvalidUID;

				_NetM.Send(_Masters[itMaster->second].second.Key.PeerNum,
					SHeader(EProto::AmAddFriendRequest),
					SAmAddFriendRequest(Proto.ToUID, Proto.ToNick, Proto.FromUID, Proto.FromNick, Proto.FromServerKey, itMaster_->second.Key));
			}
			catch (const EGameRet GameRet_)
			{
				_NetM.Send(itMaster_->second.Key.PeerNum,
					SHeader(EProto::AmAddFriendFail),
					SAmAddFriendFail(GameRet_, Proto.ToUID, Proto.FromUID, Proto.FromServerKey));
			}
		}
		void CAuth::_RecvMaAddFriendRequest(_TMastersIt /*itMaster_*/, CStream& Stream_)
		{
			SMaAddFriendRequest Proto;
			Stream_ >> Proto;

			_NetM.Send(Proto.FromMasterKey, SHeader(EProto::AmAddFriend), SAmAddFriend(Proto.ToUID, Proto.ToNick, Proto.FromUID, Proto.FromServerKey));
		}
		void CAuth::_RecvMaAddFriendRequestFail(_TMastersIt /*itMaster_*/, CStream& Stream_)
		{
			SMaAddFriendRequestFail Proto;
			Stream_ >> Proto;

			_NetM.Send(Proto.FromMasterKey,
				SHeader(EProto::AmAddFriendFail),
				SAmAddFriendFail(Proto.GameRet, Proto.ToUID, Proto.FromUID, Proto.FromServerKey));
		}
		void CAuth::_RecvMaCheck(_TMastersIt /*itMaster_*/, CStream& Stream_)
		{
			SMaCheck Proto;
			Stream_ >> Proto;

			_NetC.Send(Proto.UserKey, SHeader(EProto::AcCheck), SAcCheck(Proto.UID, Proto.Stream));
			_NetC.WillClose(Proto.UserKey, milliseconds(3000));
		}
		void CAuth::_RecvMaCheckFail(_TMastersIt /*Master_*/, CStream& Stream_)
		{
			SMaCheckFail Proto;
			Stream_ >> Proto;

			_NetC.Send(Proto.UserKey, SHeader(EProto::AcCheckFail), SAcCheckFail(Proto.GameRet));
			_NetC.WillClose(Proto.UserKey, milliseconds(3000));
		}
		void CAuth::_RecvMaToServer(_TMastersIt /*itMaster_*/, CStream& Stream_)
		{
			TUID UID = 0;
			Stream_ >> UID;

			CStream Stream;
			Stream_ >> Stream;

			SendToServerStream(UID, Stream);
		}
		void CAuth::_LinkC(const CKey& Key_)
		{
			_LinkFuncC(Key_);
		}
		void CAuth::_UnLinkC(const CKey& Key_, ENetRet NetRet_)
		{
			_Clients.erase(Key_.PeerNum);
			_UnLinkFuncC(Key_, NetRet_);
		}
		void CAuth::_RecvC(const CKey& Key_, CStream& Stream_)
		{
			SHeader Header;
			Stream_ >> Header;

			auto itClient = _Clients.get(Key_.PeerNum);
			if (itClient)
			{
				return _NetC.Close(Key_.PeerNum);
			}
			else
			{
				switch (Header.Proto)
				{
				case EProto::CaCreate: return _RecvCaCreate(Key_, Stream_);
				case EProto::CaLogin: return _RecvCaLogin(Key_, Stream_);
				case EProto::CaCheck: return _RecvCaCheck(Key_, Stream_);
				default: return _NetC.Close(Key_.PeerNum);
				}
			}
		}
		void CAuth::_RecvCaCreate(const CKey& Key_, CStream& Stream_)
		{
			SCaCreate Proto;
			Stream_ >> Proto;

			try
			{
				if (Proto.ID.empty())
					throw EGameRet::InvalidIDLength;

				if (Proto.Nick.empty())
					throw EGameRet::InvalidNickLength;

				auto GameRet = _CheckCreateUser(Proto);
				if (GameRet != EGameRet::Ok)
					throw GameRet;

				_Clients.emplace_at(Key_.PeerNum, _SClient(Key_));
				_DBPush(Key_, SAuthCreateInDb(SAuthLoginInDb(Proto.ID), Proto.Nick), Proto.State);
			}
			catch (const EGameRet GameRet_)
			{
				_NetC.Send(Key_.PeerNum, SHeader(EProto::AcLoginFail), SAcLoginFail(GameRet_));
				_NetC.WillClose(Key_.PeerNum, milliseconds(3000));
			}
		}
		void CAuth::_RecvCaLogin(const CKey& Key_, CStream& Stream_)
		{
			SCaLogin Proto;
			Stream_ >> Proto;

			try
			{
				_Clients.emplace_at(Key_.PeerNum, _SClient(Key_));
				_DBPush(Key_, SAuthLoginInDb(Proto.ID));
			}
			catch (const EGameRet GameRet_)
			{
				_NetC.Send(Key_.PeerNum, SHeader(EProto::AcLoginFail), SAcLoginFail(GameRet_));
				_NetC.WillClose(Key_.PeerNum, milliseconds(3000));
			}
		}
		void CAuth::_RecvCaCheck(const CKey& Key_, CStream& Stream_)
		{
			SCaCheck Proto;
			Stream_ >> Proto;

			try
			{
				_Clients.emplace_at(Key_.PeerNum, _SClient(Key_));
				_DBPush(Key_, SAuthLoginInDb(Proto.ID));
			}
			catch (const EGameRet GameRet_)
			{
				_NetC.Send(Key_.PeerNum, SHeader(EProto::AcCheckFail), SAcCheckFail(GameRet_));
				_NetC.WillClose(Key_.PeerNum, milliseconds(3000));
			}
		}
		void CAuth::_SendAcLogin(const CKey& ClientKey_, TUID UID_, const CNamePort& ClientBindNamePortPubToMaster_)
		{
			_NetC.Send(ClientKey_, SHeader(EProto::AcLogin), SAcLogin(UID_, ClientBindNamePortPubToMaster_));
			_NetC.WillClose(ClientKey_, milliseconds(3000));
		}

		CAuth::CAuth(
			EAddressFamily AddressFamily_,
			TCheckCreateUser CheckCreateUser_,
			TLinkFunc LinkFuncM_, TUnLinkFunc UnLinkFuncM_, TRecvFunc RecvFuncM_,
			TLinkFunc LinkFuncC_, TUnLinkFunc UnLinkFuncC_, TDBCallback DBCallbackFunc_,
			const SDBOption& DBOption_, const CNamePort& MasterBindNamePort_, size_t NetCThreadCnt_, const CNamePort& ClientBindNamePort_) :
			_CheckCreateUser(CheckCreateUser_),
			_LinkFuncM(LinkFuncM_), _UnLinkFuncM(UnLinkFuncM_), _RecvFuncM(RecvFuncM_),
			_LinkFuncC(LinkFuncC_),
			_UnLinkFuncC(UnLinkFuncC_),
			_DBCallbackFunc(DBCallbackFunc_), _DB(DBOption_, std::bind(&CAuth::_DBCallback, this, _1)),
			_NetM(
				AddressFamily_,
				std::bind(&CAuth::_LinkM, this, _1),
				std::bind(&CAuth::_UnLinkM, this, _1, _2),
				std::bind(&CAuth::_RecvM, this, _1, _2),
				false, 10240000, 10240000,
				milliseconds(), milliseconds(), 1, 0,
				MasterBindNamePort_, SOMAXCONN),
			_NetC(
				AddressFamily_,
				std::bind(&CAuth::_LinkC, this, _1),
				std::bind(&CAuth::_UnLinkC, this, _1, _2),
				std::bind(&CAuth::_RecvC, this, _1, _2),
				false, 10240000, 10240000,
				milliseconds(), milliseconds(), NetCThreadCnt_, 0,
				ClientBindNamePort_, SOMAXCONN)
		{
			CMSSQL MSSQL(DBOption_);

			if (!MSSQL.Connect())
				THROWEX();

			const wstring TableName = L"sys_tbl_User";
			const wstring CreateSPName = L"sys_sp_Create";
			const wstring GetUIDSPName = L"sys_sp_GetUID";
			const wstring LoginSPName = L"sys_sp_Login";
			const wstring AddFriendGetUIDSPName = L"sys_sp_AddFriendGetUID";
			const wstring ChangeNickSPName = L"sys_sp_ChangeNick";

			// CREATE User Table If not exists
			auto CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[)" + TableName + LR"(]') AND type in (N'U'))
BEGIN
CREATE TABLE [dbo].[)" + TableName + LR"(](
	[UID] [bigint] IDENTITY(1,1) NOT NULL,
	[ID] [varbinary](MAX) NOT NULL,
	[Nick] [nvarchar](MAX) NOT NULL,
	[InsertedTime] [datetime] NOT NULL,
	CONSTRAINT [PK_)" + TableName + LR"(] PRIMARY KEY CLUSTERED
(
	[UID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
END)";
			if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
				THROWEX();

			// CREATE Table Index
			CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[)" + TableName + LR"(]') AND name = N'IX_)" + TableName + LR"(')
CREATE UNIQUE NONCLUSTERED INDEX [IX_)" + TableName + LR"(] ON [dbo].[)" + TableName + LR"(]
(
	[UID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY])";

			if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
				THROWEX();

			// CREATE Table Index Nick
			CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.indexes WHERE object_id = OBJECT_ID(N'[dbo].[)" + TableName + LR"(]') AND name = N'IX_Nick_)" + TableName + LR"(')
CREATE UNIQUE NONCLUSTERED INDEX [IX_Nick_)" + TableName + LR"(] ON [dbo].[)" + TableName + LR"(]
(
	[Nick] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY])";

			if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
				THROWEX();

			// CREATE Table Constraint
			CreateTableQuery = LR"(
IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_)" + TableName + LR"(_InsertedTime]') AND type = 'D')
BEGIN
	ALTER TABLE [dbo].[)" + TableName + LR"(] ADD  CONSTRAINT [DF_)" + TableName + LR"(_InsertedTime]  DEFAULT (getdate()) FOR [InsertedTime]
END)";
			if (!SQL_SUCCEEDED(MSSQL.ExecuteDirect(CreateTableQuery)))
				THROWEX();

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto CreateSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + CreateSPName + LR"(]
	@ID_ NVARCHAR(MAX)
,	@Nick_ NVARCHAR(MAX)
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	BEGIN TRY

		DECLARE @UID BIGINT
		DECLARE @Nick NVARCHAR(MAX) = @Nick_

		-- MERGE는 행 추가 실패해도 Index가 증가하므로 아래와 같이 구현
		SELECT	@UID = [UID], @Nick = Nick
		FROM	dbo.)" + TableName + LR"(
		WHERE	ID = HASHBYTES('SHA2_256', @ID_)

		IF @UID IS NOT NULL
		BEGIN
			SELECT	@UID, @Nick
		END
		ELSE
		BEGIN
			INSERT INTO dbo.)" + TableName + LR"((ID, Nick)
			VALUES (HASHBYTES('SHA2_256', @ID_), @Nick)

			SELECT	@@IDENTITY, @Nick
		END

		RETURN 0

	END TRY
	BEGIN CATCH

		RETURN ERROR_NUMBER()

	END CATCH

END)";

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto LoginSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + LoginSPName + LR"(]
	@ID_ NVARCHAR(MAX)
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	SELECT	[UID], Nick
	FROM	dbo.)" + TableName + LR"(
	WHERE	ID = HASHBYTES('SHA2_256', @ID_)

	RETURN 0
END)";

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto AddFriendGetUIDSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + AddFriendGetUIDSPName + LR"(]
	@Nick_ NVARCHAR(MAX)
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	SELECT	[UID]
	FROM	dbo.)" + TableName + LR"(
	WHERE	Nick = @Nick_

	RETURN 0
END)";

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auto ChangeNickSPQuery = LR"(
ALTER PROCEDURE [dbo].[)" + ChangeNickSPName + LR"(]
	@UID_ BIGINT
,	@Nick_ NVARCHAR(MAX)
AS
BEGIN
	SET XACT_ABORT ON
	SET NOCOUNT ON

	BEGIN TRY

		UPDATE	dbo.)" + TableName + LR"(
		SET		Nick = @Nick_
		WHERE	[UID] = @UID_

		RETURN 0

	END TRY
	BEGIN CATCH

		RETURN ERROR_NUMBER()

	END CATCH

END)";

			_DBAddCmd<SAuthCreateInDb, SAuthLoginOutDb>(MSSQL, CreateSPName, CreateSPQuery, std::bind(&CAuth::_DBCreate, this, _1));
			_DBAddCmd<SAuthLoginInDb, SAuthLoginOutDb>(MSSQL, LoginSPName, LoginSPQuery, std::bind(&CAuth::_DBLogin, this, _1));
			_DBAddCmd<SAuthAddFriendGetUIDInDb, SAuthAddFriendGetUIDOutDb>(MSSQL, AddFriendGetUIDSPName, AddFriendGetUIDSPQuery, std::bind(&CAuth::_DBAddFriendGetUID, this, _1));
			_DBAddCmd<SAuthChangeNickInDb, SDummyOutDb>(MSSQL, ChangeNickSPName, ChangeNickSPQuery, std::bind(&CAuth::_DBChangeNick, this, _1));
			_DBAddCmd<SAuthCheckInDb, SAuthLoginOutDb>(MSSQL, LoginSPName, LoginSPQuery, std::bind(&CAuth::_DBCheck, this, _1));
		}
		TPeerCnt CAuth::GetMasterNum(TUID UID_) const
		{
			auto itMaster = _Masters.find<0>(TRangeUID(UID_));
			if (itMaster == _Masters.end<0>())
				return c_PeerCnt_Null;

			return (TPeerCnt)itMaster->second;
		}
		bool CAuth::Punish(TUID UID_, const TTime& EndTime_)
		{
			auto itMaster = _Masters.find<0>(TRangeUID(UID_));
			if (itMaster == _Masters.end<0>())
				return false;

			_NetM.Send((TPeerCnt)itMaster->second, SHeader(EProto::AmPunish), SAmPunish(UID_, EndTime_));

			return true;
		}
		void CAuth::Proc(void)
		{
			_DB.Proc();
			_NetM.Proc();
			_NetC.Proc();
		}
	}
}
