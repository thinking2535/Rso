#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

#include <Rso/Net/Protocol.h>
namespace rso
{
	namespace game
	{
		using namespace std;
		using namespace core;
		using namespace json;
		using namespace net;
		using TVer = int8;
		using TID = u16string;
		using TNick = u16string;
		using TMessage = u16string;
		using TState = uint8;
		const int32 c_IDLengthMax = 255;
		const int32 c_BinaryIDLengthMax = 32;
		const int32 c_NickLengthMin = 2;
		const int32 c_NickLengthMax = 32;
		const TState c_Default_State = 0;
		enum class EProto
		{
			MaMasterOn,
			AmMasterOn,
			MsMasterOn,
			AmOtherMasterOn,
			MsOtherMasterOn,
			AmOtherMasterOff,
			MsOtherMasterOff,
			SmServerOn,
			MsServerOn,
			MsOtherServerOn,
			MaServerOn,
			AmOtherMasterServerOn,
			MsOtherMasterServerOn,
			MsOtherServerOff,
			MaServerOff,
			AmOtherMasterServerOff,
			MsOtherMasterServerOff,
			CaCreate,
			AmCreate,
			MaCreate,
			MaCreateFail,
			CaLogin,
			AcLogin,
			AcLoginFail,
			SmChangeNick,
			MaChangeNick,
			AmChangeNick,
			AmChangeNickFail,
			MsChangeNick,
			MsChangeNickFail,
			CaCheck,
			AmCheck,
			MsCheck,
			SmCheck,
			MaCheck,
			MaCheckFail,
			AcCheck,
			AcCheckFail,
			CmLogin,
			MsLogin,
			SmLogin,
			SmSessionEnd,
			SmSetOpened,
			McLogin,
			McLoginFail,
			CsLogin,
			ScLogin,
			ScLoginFail,
			ScError,
			CsAddFriend,
			SmAddFriend,
			MaAddFriendGetUID,
			AmAddFriendGetUID,
			AmAddFriendGetUIDFail,
			MaAddFriend,
			AmAddFriendRequest,
			MsAddFriendRequest,
			ScAddFriendRequest,
			MaAddFriendRequest,
			MaAddFriendRequestFail,
			AmAddFriend,
			AmAddFriendFail,
			MsAddFriend,
			MsAddFriendFail,
			ScAddFriend,
			CsAllowFriend,
			SmAllowFriend,
			MsAllowFriend,
			MsAllowFriendFail,
			ScAllowFriend,
			CsDenyFriend,
			SmDenyFriend,
			MsDenyFriend,
			MsDenyFriendFail,
			ScDenyFriend,
			CsBlockFriend,
			SmBlockFriend,
			MsBlockFriend,
			MsBlockFriendFail,
			ScBlockFriend,
			CsUnBlockFriend,
			SmUnBlockFriend,
			MsUnBlockFriend,
			MsUnBlockFriendFail,
			ScUnBlockFriend,
			CsMessageSend,
			SsMessageSend,
			ScMessageReceived,
			CsChangeState,
			SmChangeState,
			MsChangeState,
			MsChangeStateFail,
			ScChangeState,
			MmFriendStateChanged,
			MsFriendStateChanged,
			ScFriendStateChanged,
			MmFriendStateChangedRenew,
			MsFriendStateChangedOffline,
			ScFriendStateChangedOffline,
			SmToServer,
			MaToServer,
			AmToServer,
			MsToServer,
			MsSessionHold,
			AmPunish,
			MsPunish,
			AmUserProto,
			MaUserProto,
			MsUserProto,
			SmUserProto,
			ScUserProto,
			CsUserProto,
			Max,
			Null,
		};
		enum class EGameRet
		{
			Ok,
			UserClose,
			HeartBeatFail,
			KeepConnectTimeOut,
			ConnectFail,
			CertifyFail,
			SystemError,
			SocketError,
			InvalidPacket,
			NetError,
			InvalidAccess,
			InvalidVersion,
			InvalidUID,
			InvalidID,
			InvalidIDLength,
			InvalidNick,
			InvalidNickLength,
			InvalidSession,
			AlreadyExist,
			DataBaseError,
			DataBasePushError,
			NoAuthToConnect,
			NoMasterToConnect,
			NoServerToConnect,
			ConnectAuthFail,
			ConnectMasterFail,
			ConnectServerFail,
			SessionNotFound,
			AddSessionFail,
			Punished,
			AuthDisconnected,
			Max,
			Null,
		};
		enum class EFriendState
		{
			Adding,
			Request,
			Normal,
			Blocked,
			Max,
			Null,
		};
		struct SHeader : public SProto
		{
			EProto Proto{};
			SHeader()
			{
			}
			SHeader(const EProto& Proto_) : Proto(Proto_)
			{
			}
			SHeader(EProto&& Proto_) : Proto(std::move(Proto_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Proto;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Proto"] >> Proto;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Proto;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Proto"] = Proto;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EProto());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EProto(), L"Proto");
			}
		};
		struct SUIDPair : public SProto
		{
			TUID UID{};
			TUID SubUID{};
			SUIDPair()
			{
			}
			SUIDPair(const TUID& UID_, const TUID& SubUID_) : UID(UID_), SubUID(SubUID_)
			{
			}
			SUIDPair(TUID&& UID_, TUID&& SubUID_) : UID(std::move(UID_)), SubUID(std::move(SubUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> SubUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["SubUID"] >> SubUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << SubUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["SubUID"] = SubUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"SubUID");
			}
		};
		struct SServerNet : public SProto
		{
			SNamePort ServerBindNamePort{};
			SServerNet()
			{
			}
			SServerNet(const SNamePort& ServerBindNamePort_) : ServerBindNamePort(ServerBindNamePort_)
			{
			}
			SServerNet(SNamePort&& ServerBindNamePort_) : ServerBindNamePort(std::move(ServerBindNamePort_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerBindNamePort;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerBindNamePort"] >> ServerBindNamePort;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerBindNamePort;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerBindNamePort"] = ServerBindNamePort;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ServerBindNamePort");
			}
		};
		using TServerNets = set<SServerNet>;
		struct SMasterNet : public SProto
		{
			SRangeUID RangeUID{};
			TServerNets Servers{};
			SMasterNet()
			{
			}
			SMasterNet(const SRangeUID& RangeUID_, const TServerNets& Servers_) : RangeUID(RangeUID_), Servers(Servers_)
			{
			}
			SMasterNet(SRangeUID&& RangeUID_, TServerNets&& Servers_) : RangeUID(std::move(RangeUID_)), Servers(std::move(Servers_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> RangeUID;
				Stream_ >> Servers;
			}
			void operator << (const Value& Value_) override
			{
				Value_["RangeUID"] >> RangeUID;
				Value_["Servers"] >> Servers;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << RangeUID;
				Stream_ << Servers;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["RangeUID"] = RangeUID;
				Value_["Servers"] = Servers;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRangeUID()) + L"," + 
					GetStdName(TServerNets());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRangeUID(), L"RangeUID") + L"," + 
					GetMemberName(TServerNets(), L"Servers");
			}
		};
		using TMasterNets = list<SMasterNet>;
		struct SMaMasterOn : public SMasterNet
		{
			SNamePort ClientBindNamePortPub{};
			TPort MasterBindPort{};
			SMaMasterOn()
			{
			}
			SMaMasterOn(const SMasterNet& Super_, const SNamePort& ClientBindNamePortPub_, const TPort& MasterBindPort_) : SMasterNet(Super_), ClientBindNamePortPub(ClientBindNamePortPub_), MasterBindPort(MasterBindPort_)
			{
			}
			SMaMasterOn(SMasterNet&& Super_, SNamePort&& ClientBindNamePortPub_, TPort&& MasterBindPort_) : SMasterNet(std::move(Super_)), ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), MasterBindPort(std::move(MasterBindPort_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SMasterNet::operator << (Stream_);
				Stream_ >> ClientBindNamePortPub;
				Stream_ >> MasterBindPort;
			}
			void operator << (const Value& Value_) override
			{
				SMasterNet::operator << (Value_);
				Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
				Value_["MasterBindPort"] >> MasterBindPort;
			}
			void operator >> (CStream& Stream_) const override
			{
				SMasterNet::operator >> (Stream_);
				Stream_ << ClientBindNamePortPub;
				Stream_ << MasterBindPort;
			}
			void operator >> (Value& Value_) const override
			{
				SMasterNet::operator >> (Value_);
				Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
				Value_["MasterBindPort"] = MasterBindPort;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SMasterNet()) + L"," + 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(TPort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SMasterNet(), L"") + L"," + 
					GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
					GetMemberName(TPort(), L"MasterBindPort");
			}
		};
		struct SMaster : public SMasterNet
		{
			SNamePort MasterBindNamePort{};
			SMaster()
			{
			}
			SMaster(const SMasterNet& Super_, const SNamePort& MasterBindNamePort_) : SMasterNet(Super_), MasterBindNamePort(MasterBindNamePort_)
			{
			}
			SMaster(SMasterNet&& Super_, SNamePort&& MasterBindNamePort_) : SMasterNet(std::move(Super_)), MasterBindNamePort(std::move(MasterBindNamePort_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SMasterNet::operator << (Stream_);
				Stream_ >> MasterBindNamePort;
			}
			void operator << (const Value& Value_) override
			{
				SMasterNet::operator << (Value_);
				Value_["MasterBindNamePort"] >> MasterBindNamePort;
			}
			void operator >> (CStream& Stream_) const override
			{
				SMasterNet::operator >> (Stream_);
				Stream_ << MasterBindNamePort;
			}
			void operator >> (Value& Value_) const override
			{
				SMasterNet::operator >> (Value_);
				Value_["MasterBindNamePort"] = MasterBindNamePort;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SMasterNet()) + L"," + 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SMasterNet(), L"") + L"," + 
					GetMemberName(SNamePort(), L"MasterBindNamePort");
			}
		};
		struct SAmMasterOn : public SProto
		{
			list<SMaster> OtherMasters{};
			SAmMasterOn()
			{
			}
			SAmMasterOn(const list<SMaster>& OtherMasters_) : OtherMasters(OtherMasters_)
			{
			}
			SAmMasterOn(list<SMaster>&& OtherMasters_) : OtherMasters(std::move(OtherMasters_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> OtherMasters;
			}
			void operator << (const Value& Value_) override
			{
				Value_["OtherMasters"] >> OtherMasters;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << OtherMasters;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["OtherMasters"] = OtherMasters;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SMaster>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SMaster>(), L"OtherMasters");
			}
		};
		struct SMsMasterOn : public SProto
		{
			list<SMaster> OtherMasters{};
			SMsMasterOn()
			{
			}
			SMsMasterOn(const list<SMaster>& OtherMasters_) : OtherMasters(OtherMasters_)
			{
			}
			SMsMasterOn(list<SMaster>&& OtherMasters_) : OtherMasters(std::move(OtherMasters_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> OtherMasters;
			}
			void operator << (const Value& Value_) override
			{
				Value_["OtherMasters"] >> OtherMasters;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << OtherMasters;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["OtherMasters"] = OtherMasters;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SMaster>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SMaster>(), L"OtherMasters");
			}
		};
		struct SAmOtherMasterOn : public SProto
		{
			SMaster Master{};
			SAmOtherMasterOn()
			{
			}
			SAmOtherMasterOn(const SMaster& Master_) : Master(Master_)
			{
			}
			SAmOtherMasterOn(SMaster&& Master_) : Master(std::move(Master_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Master;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Master"] >> Master;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Master;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Master"] = Master;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SMaster());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SMaster(), L"Master");
			}
		};
		struct SMsOtherMasterOn : public SMasterNet
		{
			SMsOtherMasterOn()
			{
			}
			SMsOtherMasterOn(const SMasterNet& Super_) : SMasterNet(Super_)
			{
			}
			SMsOtherMasterOn(SMasterNet&& Super_) : SMasterNet(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SMasterNet::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SMasterNet::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SMasterNet::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SMasterNet::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SMasterNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SMasterNet(), L"");
			}
		};
		struct SAmOtherMasterOff : public SProto
		{
			SRangeUID MasterRangeUID{};
			SAmOtherMasterOff()
			{
			}
			SAmOtherMasterOff(const SRangeUID& MasterRangeUID_) : MasterRangeUID(MasterRangeUID_)
			{
			}
			SAmOtherMasterOff(SRangeUID&& MasterRangeUID_) : MasterRangeUID(std::move(MasterRangeUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MasterRangeUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MasterRangeUID"] >> MasterRangeUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MasterRangeUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MasterRangeUID"] = MasterRangeUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRangeUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRangeUID(), L"MasterRangeUID");
			}
		};
		struct SMsOtherMasterOff : public SProto
		{
			SRangeUID MasterRangeUID{};
			SMsOtherMasterOff()
			{
			}
			SMsOtherMasterOff(const SRangeUID& MasterRangeUID_) : MasterRangeUID(MasterRangeUID_)
			{
			}
			SMsOtherMasterOff(SRangeUID&& MasterRangeUID_) : MasterRangeUID(std::move(MasterRangeUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MasterRangeUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MasterRangeUID"] >> MasterRangeUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MasterRangeUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MasterRangeUID"] = MasterRangeUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRangeUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRangeUID(), L"MasterRangeUID");
			}
		};
		struct SSmServerOn : public SProto
		{
			SNamePort ClientBindNamePortPub{};
			TPort ServerBindPort{};
			SSmServerOn()
			{
			}
			SSmServerOn(const SNamePort& ClientBindNamePortPub_, const TPort& ServerBindPort_) : ClientBindNamePortPub(ClientBindNamePortPub_), ServerBindPort(ServerBindPort_)
			{
			}
			SSmServerOn(SNamePort&& ClientBindNamePortPub_, TPort&& ServerBindPort_) : ClientBindNamePortPub(std::move(ClientBindNamePortPub_)), ServerBindPort(std::move(ServerBindPort_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientBindNamePortPub;
				Stream_ >> ServerBindPort;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
				Value_["ServerBindPort"] >> ServerBindPort;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientBindNamePortPub;
				Stream_ << ServerBindPort;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
				Value_["ServerBindPort"] = ServerBindPort;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(TPort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ClientBindNamePortPub") + L"," + 
					GetMemberName(TPort(), L"ServerBindPort");
			}
		};
		struct SMsServerOn : public SProto
		{
			SNamePort ServerBindNamePort{};
			TServerNets OtherServers{};
			TMasterNets OtherMasters{};
			SMsServerOn()
			{
			}
			SMsServerOn(const SNamePort& ServerBindNamePort_, const TServerNets& OtherServers_, const TMasterNets& OtherMasters_) : ServerBindNamePort(ServerBindNamePort_), OtherServers(OtherServers_), OtherMasters(OtherMasters_)
			{
			}
			SMsServerOn(SNamePort&& ServerBindNamePort_, TServerNets&& OtherServers_, TMasterNets&& OtherMasters_) : ServerBindNamePort(std::move(ServerBindNamePort_)), OtherServers(std::move(OtherServers_)), OtherMasters(std::move(OtherMasters_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerBindNamePort;
				Stream_ >> OtherServers;
				Stream_ >> OtherMasters;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerBindNamePort"] >> ServerBindNamePort;
				Value_["OtherServers"] >> OtherServers;
				Value_["OtherMasters"] >> OtherMasters;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerBindNamePort;
				Stream_ << OtherServers;
				Stream_ << OtherMasters;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerBindNamePort"] = ServerBindNamePort;
				Value_["OtherServers"] = OtherServers;
				Value_["OtherMasters"] = OtherMasters;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(TServerNets()) + L"," + 
					GetStdName(TMasterNets());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ServerBindNamePort") + L"," + 
					GetMemberName(TServerNets(), L"OtherServers") + L"," + 
					GetMemberName(TMasterNets(), L"OtherMasters");
			}
		};
		struct SMsOtherServerOn : public SProto
		{
			SServerNet Server{};
			SMsOtherServerOn()
			{
			}
			SMsOtherServerOn(const SServerNet& Server_) : Server(Server_)
			{
			}
			SMsOtherServerOn(SServerNet&& Server_) : Server(std::move(Server_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Server;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Server"] >> Server;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Server;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Server"] = Server;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SServerNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SServerNet(), L"Server");
			}
		};
		struct SMaServerOn : public SProto
		{
			SNamePort ServerBindNamePort{};
			SMaServerOn()
			{
			}
			SMaServerOn(const SNamePort& ServerBindNamePort_) : ServerBindNamePort(ServerBindNamePort_)
			{
			}
			SMaServerOn(SNamePort&& ServerBindNamePort_) : ServerBindNamePort(std::move(ServerBindNamePort_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerBindNamePort;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerBindNamePort"] >> ServerBindNamePort;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerBindNamePort;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerBindNamePort"] = ServerBindNamePort;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ServerBindNamePort");
			}
		};
		struct SAmOtherMasterServerOn : public SProto
		{
			SRangeUID MasterRangeUID{};
			SServerNet Server{};
			SAmOtherMasterServerOn()
			{
			}
			SAmOtherMasterServerOn(const SRangeUID& MasterRangeUID_, const SServerNet& Server_) : MasterRangeUID(MasterRangeUID_), Server(Server_)
			{
			}
			SAmOtherMasterServerOn(SRangeUID&& MasterRangeUID_, SServerNet&& Server_) : MasterRangeUID(std::move(MasterRangeUID_)), Server(std::move(Server_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MasterRangeUID;
				Stream_ >> Server;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MasterRangeUID"] >> MasterRangeUID;
				Value_["Server"] >> Server;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MasterRangeUID;
				Stream_ << Server;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MasterRangeUID"] = MasterRangeUID;
				Value_["Server"] = Server;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRangeUID()) + L"," + 
					GetStdName(SServerNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRangeUID(), L"MasterRangeUID") + L"," + 
					GetMemberName(SServerNet(), L"Server");
			}
		};
		struct SMsOtherMasterServerOn : public SProto
		{
			SRangeUID MasterRangeUID{};
			SServerNet Server{};
			SMsOtherMasterServerOn()
			{
			}
			SMsOtherMasterServerOn(const SRangeUID& MasterRangeUID_, const SServerNet& Server_) : MasterRangeUID(MasterRangeUID_), Server(Server_)
			{
			}
			SMsOtherMasterServerOn(SRangeUID&& MasterRangeUID_, SServerNet&& Server_) : MasterRangeUID(std::move(MasterRangeUID_)), Server(std::move(Server_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MasterRangeUID;
				Stream_ >> Server;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MasterRangeUID"] >> MasterRangeUID;
				Value_["Server"] >> Server;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MasterRangeUID;
				Stream_ << Server;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MasterRangeUID"] = MasterRangeUID;
				Value_["Server"] = Server;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRangeUID()) + L"," + 
					GetStdName(SServerNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRangeUID(), L"MasterRangeUID") + L"," + 
					GetMemberName(SServerNet(), L"Server");
			}
		};
		struct SMsOtherServerOff : public SProto
		{
			SServerNet Server{};
			SMsOtherServerOff()
			{
			}
			SMsOtherServerOff(const SServerNet& Server_) : Server(Server_)
			{
			}
			SMsOtherServerOff(SServerNet&& Server_) : Server(std::move(Server_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Server;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Server"] >> Server;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Server;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Server"] = Server;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SServerNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SServerNet(), L"Server");
			}
		};
		struct SMaServerOff : public SProto
		{
			SServerNet Server{};
			SMaServerOff()
			{
			}
			SMaServerOff(const SServerNet& Server_) : Server(Server_)
			{
			}
			SMaServerOff(SServerNet&& Server_) : Server(std::move(Server_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Server;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Server"] >> Server;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Server;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Server"] = Server;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SServerNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SServerNet(), L"Server");
			}
		};
		struct SAmOtherMasterServerOff : public SProto
		{
			SRangeUID MasterRangeUID{};
			SServerNet Server{};
			SAmOtherMasterServerOff()
			{
			}
			SAmOtherMasterServerOff(const SRangeUID& MasterRangeUID_, const SServerNet& Server_) : MasterRangeUID(MasterRangeUID_), Server(Server_)
			{
			}
			SAmOtherMasterServerOff(SRangeUID&& MasterRangeUID_, SServerNet&& Server_) : MasterRangeUID(std::move(MasterRangeUID_)), Server(std::move(Server_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MasterRangeUID;
				Stream_ >> Server;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MasterRangeUID"] >> MasterRangeUID;
				Value_["Server"] >> Server;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MasterRangeUID;
				Stream_ << Server;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MasterRangeUID"] = MasterRangeUID;
				Value_["Server"] = Server;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRangeUID()) + L"," + 
					GetStdName(SServerNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRangeUID(), L"MasterRangeUID") + L"," + 
					GetMemberName(SServerNet(), L"Server");
			}
		};
		struct SMsOtherMasterServerOff : public SProto
		{
			SRangeUID MasterRangeUID{};
			SServerNet Server{};
			SMsOtherMasterServerOff()
			{
			}
			SMsOtherMasterServerOff(const SRangeUID& MasterRangeUID_, const SServerNet& Server_) : MasterRangeUID(MasterRangeUID_), Server(Server_)
			{
			}
			SMsOtherMasterServerOff(SRangeUID&& MasterRangeUID_, SServerNet&& Server_) : MasterRangeUID(std::move(MasterRangeUID_)), Server(std::move(Server_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MasterRangeUID;
				Stream_ >> Server;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MasterRangeUID"] >> MasterRangeUID;
				Value_["Server"] >> Server;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MasterRangeUID;
				Stream_ << Server;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MasterRangeUID"] = MasterRangeUID;
				Value_["Server"] = Server;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRangeUID()) + L"," + 
					GetStdName(SServerNet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRangeUID(), L"MasterRangeUID") + L"," + 
					GetMemberName(SServerNet(), L"Server");
			}
		};
		struct SCaCreate : public SProto
		{
			TID ID{};
			TNick Nick{};
			TState State{};
			SCaCreate()
			{
			}
			SCaCreate(const TID& ID_, const TNick& Nick_, const TState& State_) : ID(ID_), Nick(Nick_), State(State_)
			{
			}
			SCaCreate(TID&& ID_, TNick&& Nick_, TState&& State_) : ID(std::move(ID_)), Nick(std::move(Nick_)), State(std::move(State_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ID;
				Stream_ >> Nick;
				Stream_ >> State;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ID"] >> ID;
				Value_["Nick"] >> Nick;
				Value_["State"] >> State;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ID;
				Stream_ << Nick;
				Stream_ << State;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ID"] = ID;
				Value_["Nick"] = Nick;
				Value_["State"] = State;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(TState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TID(), L"ID") + L"," + 
					GetMemberName(TNick(), L"Nick") + L"," + 
					GetMemberName(TState(), L"State");
			}
		};
		struct SAccount : public SProto
		{
			TID ID{};
			TNick Nick{};
			SAccount()
			{
			}
			SAccount(const TID& ID_, const TNick& Nick_) : ID(ID_), Nick(Nick_)
			{
			}
			SAccount(TID&& ID_, TNick&& Nick_) : ID(std::move(ID_)), Nick(std::move(Nick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ID;
				Stream_ >> Nick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ID"] >> ID;
				Value_["Nick"] >> Nick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ID;
				Stream_ << Nick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ID"] = ID;
				Value_["Nick"] = Nick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TID()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TID(), L"ID") + L"," + 
					GetMemberName(TNick(), L"Nick");
			}
		};
		struct SAmCreate : public SProto
		{
			TUID UID{};
			SAccount Account{};
			TState State{};
			SKey UserKey{};
			SAmCreate()
			{
			}
			SAmCreate(const TUID& UID_, const SAccount& Account_, const TState& State_, const SKey& UserKey_) : UID(UID_), Account(Account_), State(State_), UserKey(UserKey_)
			{
			}
			SAmCreate(TUID&& UID_, SAccount&& Account_, TState&& State_, SKey&& UserKey_) : UID(std::move(UID_)), Account(std::move(Account_)), State(std::move(State_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Account;
				Stream_ >> State;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Account"] >> Account;
				Value_["State"] >> State;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Account;
				Stream_ << State;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Account"] = Account;
				Value_["State"] = State;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SAccount()) + L"," + 
					GetStdName(TState()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SAccount(), L"Account") + L"," + 
					GetMemberName(TState(), L"State") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMaCreate : public SProto
		{
			SKey UserKey{};
			TUID UID{};
			SMaCreate()
			{
			}
			SMaCreate(const SKey& UserKey_, const TUID& UID_) : UserKey(UserKey_), UID(UID_)
			{
			}
			SMaCreate(SKey&& UserKey_, TUID&& UID_) : UserKey(std::move(UserKey_)), UID(std::move(UID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UserKey;
				Stream_ >> UID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UserKey"] >> UserKey;
				Value_["UID"] >> UID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UserKey;
				Stream_ << UID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UserKey"] = UserKey;
				Value_["UID"] = UID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(TUID(), L"UID");
			}
		};
		struct SMaCreateFail : public SProto
		{
			SKey UserKey{};
			EGameRet GameRet{};
			SMaCreateFail()
			{
			}
			SMaCreateFail(const SKey& UserKey_, const EGameRet& GameRet_) : UserKey(UserKey_), GameRet(GameRet_)
			{
			}
			SMaCreateFail(SKey&& UserKey_, EGameRet&& GameRet_) : UserKey(std::move(UserKey_)), GameRet(std::move(GameRet_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UserKey;
				Stream_ >> GameRet;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UserKey"] >> UserKey;
				Value_["GameRet"] >> GameRet;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UserKey;
				Stream_ << GameRet;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UserKey"] = UserKey;
				Value_["GameRet"] = GameRet;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(EGameRet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(EGameRet(), L"GameRet");
			}
		};
		struct SCaLogin : public SProto
		{
			TUID UID{};
			TID ID{};
			SCaLogin()
			{
			}
			SCaLogin(const TUID& UID_, const TID& ID_) : UID(UID_), ID(ID_)
			{
			}
			SCaLogin(TUID&& UID_, TID&& ID_) : UID(std::move(UID_)), ID(std::move(ID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> ID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["ID"] >> ID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << ID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["ID"] = ID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TID(), L"ID");
			}
		};
		struct SAcLogin : public SProto
		{
			TUID UID{};
			SNamePort ClientBindNamePortPubToMaster{};
			SAcLogin()
			{
			}
			SAcLogin(const TUID& UID_, const SNamePort& ClientBindNamePortPubToMaster_) : UID(UID_), ClientBindNamePortPubToMaster(ClientBindNamePortPubToMaster_)
			{
			}
			SAcLogin(TUID&& UID_, SNamePort&& ClientBindNamePortPubToMaster_) : UID(std::move(UID_)), ClientBindNamePortPubToMaster(std::move(ClientBindNamePortPubToMaster_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> ClientBindNamePortPubToMaster;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["ClientBindNamePortPubToMaster"] >> ClientBindNamePortPubToMaster;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << ClientBindNamePortPubToMaster;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["ClientBindNamePortPubToMaster"] = ClientBindNamePortPubToMaster;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SNamePort(), L"ClientBindNamePortPubToMaster");
			}
		};
		struct SAcLoginFail : public SProto
		{
			EGameRet GameRet{};
			SAcLoginFail()
			{
			}
			SAcLoginFail(const EGameRet& GameRet_) : GameRet(GameRet_)
			{
			}
			SAcLoginFail(EGameRet&& GameRet_) : GameRet(std::move(GameRet_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet");
			}
		};
		struct SSmChangeNick : public SProto
		{
			TUID UID{};
			TNick Nick{};
			SKey ClientKey{};
			SSmChangeNick()
			{
			}
			SSmChangeNick(const TUID& UID_, const TNick& Nick_, const SKey& ClientKey_) : UID(UID_), Nick(Nick_), ClientKey(ClientKey_)
			{
			}
			SSmChangeNick(TUID&& UID_, TNick&& Nick_, SKey&& ClientKey_) : UID(std::move(UID_)), Nick(std::move(Nick_)), ClientKey(std::move(ClientKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Nick;
				Stream_ >> ClientKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Nick"] >> Nick;
				Value_["ClientKey"] >> ClientKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Nick;
				Stream_ << ClientKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Nick"] = Nick;
				Value_["ClientKey"] = ClientKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TNick(), L"Nick") + L"," + 
					GetMemberName(SKey(), L"ClientKey");
			}
		};
		struct SMaChangeNick : public SSmChangeNick
		{
			SKey ServerKey{};
			SMaChangeNick()
			{
			}
			SMaChangeNick(const SSmChangeNick& Super_, const SKey& ServerKey_) : SSmChangeNick(Super_), ServerKey(ServerKey_)
			{
			}
			SMaChangeNick(SSmChangeNick&& Super_, SKey&& ServerKey_) : SSmChangeNick(std::move(Super_)), ServerKey(std::move(ServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SSmChangeNick::operator << (Stream_);
				Stream_ >> ServerKey;
			}
			void operator << (const Value& Value_) override
			{
				SSmChangeNick::operator << (Value_);
				Value_["ServerKey"] >> ServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				SSmChangeNick::operator >> (Stream_);
				Stream_ << ServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				SSmChangeNick::operator >> (Value_);
				Value_["ServerKey"] = ServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SSmChangeNick()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SSmChangeNick(), L"") + L"," + 
					GetMemberName(SKey(), L"ServerKey");
			}
		};
		struct SMsChangeNick : public SProto
		{
			TNick Nick{};
			SKey ClientKey{};
			TUID UID{};
			SMsChangeNick()
			{
			}
			SMsChangeNick(const TNick& Nick_, const SKey& ClientKey_, const TUID& UID_) : Nick(Nick_), ClientKey(ClientKey_), UID(UID_)
			{
			}
			SMsChangeNick(TNick&& Nick_, SKey&& ClientKey_, TUID&& UID_) : Nick(std::move(Nick_)), ClientKey(std::move(ClientKey_)), UID(std::move(UID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Nick;
				Stream_ >> ClientKey;
				Stream_ >> UID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Nick"] >> Nick;
				Value_["ClientKey"] >> ClientKey;
				Value_["UID"] >> UID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Nick;
				Stream_ << ClientKey;
				Stream_ << UID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Nick"] = Nick;
				Value_["ClientKey"] = ClientKey;
				Value_["UID"] = UID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"Nick") + L"," + 
					GetMemberName(SKey(), L"ClientKey") + L"," + 
					GetMemberName(TUID(), L"UID");
			}
		};
		struct SMsChangeNickFail : public SProto
		{
			EGameRet GameRet{};
			SKey ClientKey{};
			TUID UID{};
			SMsChangeNickFail()
			{
			}
			SMsChangeNickFail(const EGameRet& GameRet_, const SKey& ClientKey_, const TUID& UID_) : GameRet(GameRet_), ClientKey(ClientKey_), UID(UID_)
			{
			}
			SMsChangeNickFail(EGameRet&& GameRet_, SKey&& ClientKey_, TUID&& UID_) : GameRet(std::move(GameRet_)), ClientKey(std::move(ClientKey_)), UID(std::move(UID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> ClientKey;
				Stream_ >> UID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["ClientKey"] >> ClientKey;
				Value_["UID"] >> UID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << ClientKey;
				Stream_ << UID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["ClientKey"] = ClientKey;
				Value_["UID"] = UID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(SKey(), L"ClientKey") + L"," + 
					GetMemberName(TUID(), L"UID");
			}
		};
		struct SAmChangeNick : public SMsChangeNick
		{
			SKey ServerKey{};
			SAmChangeNick()
			{
			}
			SAmChangeNick(const SMsChangeNick& Super_, const SKey& ServerKey_) : SMsChangeNick(Super_), ServerKey(ServerKey_)
			{
			}
			SAmChangeNick(SMsChangeNick&& Super_, SKey&& ServerKey_) : SMsChangeNick(std::move(Super_)), ServerKey(std::move(ServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SMsChangeNick::operator << (Stream_);
				Stream_ >> ServerKey;
			}
			void operator << (const Value& Value_) override
			{
				SMsChangeNick::operator << (Value_);
				Value_["ServerKey"] >> ServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				SMsChangeNick::operator >> (Stream_);
				Stream_ << ServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				SMsChangeNick::operator >> (Value_);
				Value_["ServerKey"] = ServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SMsChangeNick()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SMsChangeNick(), L"") + L"," + 
					GetMemberName(SKey(), L"ServerKey");
			}
		};
		struct SAmChangeNickFail : public SMsChangeNickFail
		{
			SKey ServerKey{};
			SAmChangeNickFail()
			{
			}
			SAmChangeNickFail(const SMsChangeNickFail& Super_, const SKey& ServerKey_) : SMsChangeNickFail(Super_), ServerKey(ServerKey_)
			{
			}
			SAmChangeNickFail(SMsChangeNickFail&& Super_, SKey&& ServerKey_) : SMsChangeNickFail(std::move(Super_)), ServerKey(std::move(ServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SMsChangeNickFail::operator << (Stream_);
				Stream_ >> ServerKey;
			}
			void operator << (const Value& Value_) override
			{
				SMsChangeNickFail::operator << (Value_);
				Value_["ServerKey"] >> ServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				SMsChangeNickFail::operator >> (Stream_);
				Stream_ << ServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				SMsChangeNickFail::operator >> (Value_);
				Value_["ServerKey"] = ServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SMsChangeNickFail()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SMsChangeNickFail(), L"") + L"," + 
					GetMemberName(SKey(), L"ServerKey");
			}
		};
		struct SCaCheck : public SProto
		{
			TID ID{};
			SCaCheck()
			{
			}
			SCaCheck(const TID& ID_) : ID(ID_)
			{
			}
			SCaCheck(TID&& ID_) : ID(std::move(ID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ID"] >> ID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ID"] = ID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TID(), L"ID");
			}
		};
		struct SAmCheck : public SProto
		{
			TUID UID{};
			SKey UserKey{};
			SAmCheck()
			{
			}
			SAmCheck(const TUID& UID_, const SKey& UserKey_) : UID(UID_), UserKey(UserKey_)
			{
			}
			SAmCheck(TUID&& UID_, SKey&& UserKey_) : UID(std::move(UID_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsCheck : public SProto
		{
			TUID UID{};
			SKey UserKey{};
			SKey AuthKey{};
			TSessionCode SessionCode{};
			SMsCheck()
			{
			}
			SMsCheck(const TUID& UID_, const SKey& UserKey_, const SKey& AuthKey_, const TSessionCode& SessionCode_) : UID(UID_), UserKey(UserKey_), AuthKey(AuthKey_), SessionCode(SessionCode_)
			{
			}
			SMsCheck(TUID&& UID_, SKey&& UserKey_, SKey&& AuthKey_, TSessionCode&& SessionCode_) : UID(std::move(UID_)), UserKey(std::move(UserKey_)), AuthKey(std::move(AuthKey_)), SessionCode(std::move(SessionCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> UserKey;
				Stream_ >> AuthKey;
				Stream_ >> SessionCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["UserKey"] >> UserKey;
				Value_["AuthKey"] >> AuthKey;
				Value_["SessionCode"] >> SessionCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << UserKey;
				Stream_ << AuthKey;
				Stream_ << SessionCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["UserKey"] = UserKey;
				Value_["AuthKey"] = AuthKey;
				Value_["SessionCode"] = SessionCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(TSessionCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(SKey(), L"AuthKey") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode");
			}
		};
		struct SSmCheck : public SProto
		{
			TUID UID{};
			SKey UserKey{};
			SKey AuthKey{};
			CStream Stream{};
			SSmCheck()
			{
			}
			SSmCheck(const TUID& UID_, const SKey& UserKey_, const SKey& AuthKey_, const CStream& Stream_) : UID(UID_), UserKey(UserKey_), AuthKey(AuthKey_), Stream(Stream_)
			{
			}
			SSmCheck(TUID&& UID_, SKey&& UserKey_, SKey&& AuthKey_, CStream&& Stream_) : UID(std::move(UID_)), UserKey(std::move(UserKey_)), AuthKey(std::move(AuthKey_)), Stream(std::move(Stream_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> UserKey;
				Stream_ >> AuthKey;
				Stream_ >> Stream;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["UserKey"] >> UserKey;
				Value_["AuthKey"] >> AuthKey;
				Value_["Stream"] >> Stream;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << UserKey;
				Stream_ << AuthKey;
				Stream_ << Stream;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["UserKey"] = UserKey;
				Value_["AuthKey"] = AuthKey;
				Value_["Stream"] = Stream;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(CStream());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(SKey(), L"AuthKey") + L"," + 
					GetMemberName(CStream(), L"Stream");
			}
		};
		struct SMaCheck : public SProto
		{
			TUID UID{};
			SKey UserKey{};
			CStream Stream{};
			SMaCheck()
			{
			}
			SMaCheck(const TUID& UID_, const SKey& UserKey_, const CStream& Stream_) : UID(UID_), UserKey(UserKey_), Stream(Stream_)
			{
			}
			SMaCheck(TUID&& UID_, SKey&& UserKey_, CStream&& Stream_) : UID(std::move(UID_)), UserKey(std::move(UserKey_)), Stream(std::move(Stream_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> UserKey;
				Stream_ >> Stream;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["UserKey"] >> UserKey;
				Value_["Stream"] >> Stream;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << UserKey;
				Stream_ << Stream;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["UserKey"] = UserKey;
				Value_["Stream"] = Stream;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(CStream());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(CStream(), L"Stream");
			}
		};
		struct SMaCheckFail : public SProto
		{
			SKey UserKey{};
			EGameRet GameRet{};
			SMaCheckFail()
			{
			}
			SMaCheckFail(const SKey& UserKey_, const EGameRet& GameRet_) : UserKey(UserKey_), GameRet(GameRet_)
			{
			}
			SMaCheckFail(SKey&& UserKey_, EGameRet&& GameRet_) : UserKey(std::move(UserKey_)), GameRet(std::move(GameRet_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UserKey;
				Stream_ >> GameRet;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UserKey"] >> UserKey;
				Value_["GameRet"] >> GameRet;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UserKey;
				Stream_ << GameRet;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UserKey"] = UserKey;
				Value_["GameRet"] = GameRet;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(EGameRet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(EGameRet(), L"GameRet");
			}
		};
		struct SAcCheck : public SProto
		{
			TUID UID{};
			CStream Stream{};
			SAcCheck()
			{
			}
			SAcCheck(const TUID& UID_, const CStream& Stream_) : UID(UID_), Stream(Stream_)
			{
			}
			SAcCheck(TUID&& UID_, CStream&& Stream_) : UID(std::move(UID_)), Stream(std::move(Stream_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Stream;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Stream"] >> Stream;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Stream;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Stream"] = Stream;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(CStream());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(CStream(), L"Stream");
			}
		};
		struct SAcCheckFail : public SProto
		{
			EGameRet GameRet{};
			SAcCheckFail()
			{
			}
			SAcCheckFail(const EGameRet& GameRet_) : GameRet(GameRet_)
			{
			}
			SAcCheckFail(EGameRet&& GameRet_) : GameRet(std::move(GameRet_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet");
			}
		};
		struct SCmLogin : public SProto
		{
			TUID UID{};
			TID ID{};
			TUID SubUID{};
			SCmLogin()
			{
			}
			SCmLogin(const TUID& UID_, const TID& ID_, const TUID& SubUID_) : UID(UID_), ID(ID_), SubUID(SubUID_)
			{
			}
			SCmLogin(TUID&& UID_, TID&& ID_, TUID&& SubUID_) : UID(std::move(UID_)), ID(std::move(ID_)), SubUID(std::move(SubUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> ID;
				Stream_ >> SubUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["ID"] >> ID;
				Value_["SubUID"] >> SubUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << ID;
				Stream_ << SubUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["ID"] = ID;
				Value_["SubUID"] = SubUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TID()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TID(), L"ID") + L"," + 
					GetMemberName(TUID(), L"SubUID");
			}
		};
		struct SFriendDB : public SProto
		{
			TNick Nick{};
			EFriendState FriendState{};
			SFriendDB()
			{
			}
			SFriendDB(const TNick& Nick_, const EFriendState& FriendState_) : Nick(Nick_), FriendState(FriendState_)
			{
			}
			SFriendDB(TNick&& Nick_, EFriendState&& FriendState_) : Nick(std::move(Nick_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Nick;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Nick"] >> Nick;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Nick;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Nick"] = Nick;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"Nick") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SFriendInfo : public SProto
		{
			SNamePort ServerBindNamePort{};
			TState State{};
			SFriendInfo()
			{
			}
			SFriendInfo(const SNamePort& ServerBindNamePort_, const TState& State_) : ServerBindNamePort(ServerBindNamePort_), State(State_)
			{
			}
			SFriendInfo(SNamePort&& ServerBindNamePort_, TState&& State_) : ServerBindNamePort(std::move(ServerBindNamePort_)), State(std::move(State_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerBindNamePort;
				Stream_ >> State;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerBindNamePort"] >> ServerBindNamePort;
				Value_["State"] >> State;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerBindNamePort;
				Stream_ << State;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerBindNamePort"] = ServerBindNamePort;
				Value_["State"] = State;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(TState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ServerBindNamePort") + L"," + 
					GetMemberName(TState(), L"State");
			}
		};
		struct SFriend : public SFriendDB
		{
			SFriendInfo Info{};
			SFriend()
			{
			}
			SFriend(const SFriendDB& Super_, const SFriendInfo& Info_) : SFriendDB(Super_), Info(Info_)
			{
			}
			SFriend(SFriendDB&& Super_, SFriendInfo&& Info_) : SFriendDB(std::move(Super_)), Info(std::move(Info_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SFriendDB::operator << (Stream_);
				Stream_ >> Info;
			}
			void operator << (const Value& Value_) override
			{
				SFriendDB::operator << (Value_);
				Value_["Info"] >> Info;
			}
			void operator >> (CStream& Stream_) const override
			{
				SFriendDB::operator >> (Stream_);
				Stream_ << Info;
			}
			void operator >> (Value& Value_) const override
			{
				SFriendDB::operator >> (Value_);
				Value_["Info"] = Info;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SFriendDB()) + L"," + 
					GetStdName(SFriendInfo());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SFriendDB(), L"") + L"," + 
					GetMemberName(SFriendInfo(), L"Info");
			}
		};
		using TFriendDBs = map<TUID,SFriendDB>;
		using TFriends = map<TUID,SFriend>;
		struct SMsLogin : public SProto
		{
			TUID UID{};
			SAccount Account{};
			TUID SubUID{};
			TFriends Friends{};
			SKey UserKey{};
			TSessionCode SessionCode{};
			SMsLogin()
			{
			}
			SMsLogin(const TUID& UID_, const SAccount& Account_, const TUID& SubUID_, const TFriends& Friends_, const SKey& UserKey_, const TSessionCode& SessionCode_) : UID(UID_), Account(Account_), SubUID(SubUID_), Friends(Friends_), UserKey(UserKey_), SessionCode(SessionCode_)
			{
			}
			SMsLogin(TUID&& UID_, SAccount&& Account_, TUID&& SubUID_, TFriends&& Friends_, SKey&& UserKey_, TSessionCode&& SessionCode_) : UID(std::move(UID_)), Account(std::move(Account_)), SubUID(std::move(SubUID_)), Friends(std::move(Friends_)), UserKey(std::move(UserKey_)), SessionCode(std::move(SessionCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Account;
				Stream_ >> SubUID;
				Stream_ >> Friends;
				Stream_ >> UserKey;
				Stream_ >> SessionCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Account"] >> Account;
				Value_["SubUID"] >> SubUID;
				Value_["Friends"] >> Friends;
				Value_["UserKey"] >> UserKey;
				Value_["SessionCode"] >> SessionCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Account;
				Stream_ << SubUID;
				Stream_ << Friends;
				Stream_ << UserKey;
				Stream_ << SessionCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Account"] = Account;
				Value_["SubUID"] = SubUID;
				Value_["Friends"] = Friends;
				Value_["UserKey"] = UserKey;
				Value_["SessionCode"] = SessionCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SAccount()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TFriends()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(TSessionCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SAccount(), L"Account") + L"," + 
					GetMemberName(TUID(), L"SubUID") + L"," + 
					GetMemberName(TFriends(), L"Friends") + L"," + 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode");
			}
		};
		struct SSmLogin : public SProto
		{
			SKey UserKey{};
			TSessionCode SessionCode{};
			SSmLogin()
			{
			}
			SSmLogin(const SKey& UserKey_, const TSessionCode& SessionCode_) : UserKey(UserKey_), SessionCode(SessionCode_)
			{
			}
			SSmLogin(SKey&& UserKey_, TSessionCode&& SessionCode_) : UserKey(std::move(UserKey_)), SessionCode(std::move(SessionCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UserKey;
				Stream_ >> SessionCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UserKey"] >> UserKey;
				Value_["SessionCode"] >> SessionCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UserKey;
				Stream_ << SessionCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UserKey"] = UserKey;
				Value_["SessionCode"] = SessionCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(TSessionCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"UserKey") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode");
			}
		};
		struct SSmSessionEnd : public SProto
		{
			TUID UID{};
			TSessionCode SessionCode{};
			SSmSessionEnd()
			{
			}
			SSmSessionEnd(const TUID& UID_, const TSessionCode& SessionCode_) : UID(UID_), SessionCode(SessionCode_)
			{
			}
			SSmSessionEnd(TUID&& UID_, TSessionCode&& SessionCode_) : UID(std::move(UID_)), SessionCode(std::move(SessionCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> SessionCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["SessionCode"] >> SessionCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << SessionCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["SessionCode"] = SessionCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TSessionCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode");
			}
		};
		struct SSmSetOpened : public SProto
		{
			bool Opened{};
			SSmSetOpened()
			{
			}
			SSmSetOpened(const bool& Opened_) : Opened(Opened_)
			{
			}
			SSmSetOpened(bool&& Opened_) : Opened(std::move(Opened_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Opened;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Opened"] >> Opened;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Opened;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Opened"] = Opened;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(bool());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(bool(), L"Opened");
			}
		};
		struct SMcLogin : public SProto
		{
			SNamePort ClientBindNamePortPubToServer{};
			TSessionCode SessionCode{};
			SMcLogin()
			{
			}
			SMcLogin(const SNamePort& ClientBindNamePortPubToServer_, const TSessionCode& SessionCode_) : ClientBindNamePortPubToServer(ClientBindNamePortPubToServer_), SessionCode(SessionCode_)
			{
			}
			SMcLogin(SNamePort&& ClientBindNamePortPubToServer_, TSessionCode&& SessionCode_) : ClientBindNamePortPubToServer(std::move(ClientBindNamePortPubToServer_)), SessionCode(std::move(SessionCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ClientBindNamePortPubToServer;
				Stream_ >> SessionCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ClientBindNamePortPubToServer"] >> ClientBindNamePortPubToServer;
				Value_["SessionCode"] >> SessionCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ClientBindNamePortPubToServer;
				Stream_ << SessionCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ClientBindNamePortPubToServer"] = ClientBindNamePortPubToServer;
				Value_["SessionCode"] = SessionCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(TSessionCode());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"ClientBindNamePortPubToServer") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode");
			}
		};
		struct SMcLoginFail : public SProto
		{
			EGameRet GameRet{};
			SMcLoginFail()
			{
			}
			SMcLoginFail(const EGameRet& GameRet_) : GameRet(GameRet_)
			{
			}
			SMcLoginFail(EGameRet&& GameRet_) : GameRet(std::move(GameRet_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet");
			}
		};
		struct SVersion : public SProto
		{
			TVer Main{};
			uint64 Data{};
			SVersion()
			{
			}
			SVersion(const TVer& Main_, const uint64& Data_) : Main(Main_), Data(Data_)
			{
			}
			SVersion(TVer&& Main_, uint64&& Data_) : Main(std::move(Main_)), Data(std::move(Data_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Main;
				Stream_ >> Data;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Main"] >> Main;
				Value_["Data"] >> Data;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Main;
				Stream_ << Data;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Main"] = Main;
				Value_["Data"] = Data;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TVer()) + L"," + 
					GetStdName(uint64());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TVer(), L"Main") + L"," + 
					GetMemberName(uint64(), L"Data");
			}
		};
		struct SCsLogin : public SProto
		{
			TUID UID{};
			TID ID{};
			TUID SubUID{};
			TSessionCode SessionCode{};
			SVersion Version{};
			bool Create{};
			CStream Stream{};
			SCsLogin()
			{
			}
			SCsLogin(const TUID& UID_, const TID& ID_, const TUID& SubUID_, const TSessionCode& SessionCode_, const SVersion& Version_, const bool& Create_, const CStream& Stream_) : UID(UID_), ID(ID_), SubUID(SubUID_), SessionCode(SessionCode_), Version(Version_), Create(Create_), Stream(Stream_)
			{
			}
			SCsLogin(TUID&& UID_, TID&& ID_, TUID&& SubUID_, TSessionCode&& SessionCode_, SVersion&& Version_, bool&& Create_, CStream&& Stream_) : UID(std::move(UID_)), ID(std::move(ID_)), SubUID(std::move(SubUID_)), SessionCode(std::move(SessionCode_)), Version(std::move(Version_)), Create(std::move(Create_)), Stream(std::move(Stream_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> ID;
				Stream_ >> SubUID;
				Stream_ >> SessionCode;
				Stream_ >> Version;
				Stream_ >> Create;
				Stream_ >> Stream;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["ID"] >> ID;
				Value_["SubUID"] >> SubUID;
				Value_["SessionCode"] >> SessionCode;
				Value_["Version"] >> Version;
				Value_["Create"] >> Create;
				Value_["Stream"] >> Stream;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << ID;
				Stream_ << SubUID;
				Stream_ << SessionCode;
				Stream_ << Version;
				Stream_ << Create;
				Stream_ << Stream;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["ID"] = ID;
				Value_["SubUID"] = SubUID;
				Value_["SessionCode"] = SessionCode;
				Value_["Version"] = Version;
				Value_["Create"] = Create;
				Value_["Stream"] = Stream;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TSessionCode()) + L"," + 
					GetStdName(SVersion()) + L"," + 
					GetStdName(bool()) + L"," + 
					GetStdName(CStream());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TID(), L"ID") + L"," + 
					GetMemberName(TUID(), L"SubUID") + L"," + 
					GetMemberName(TSessionCode(), L"SessionCode") + L"," + 
					GetMemberName(SVersion(), L"Version") + L"," + 
					GetMemberName(bool(), L"Create") + L"," + 
					GetMemberName(CStream(), L"Stream");
			}
		};
		struct SScLogin : public SProto
		{
			TNick Nick{};
			TFriends Friends{};
			SScLogin()
			{
			}
			SScLogin(const TNick& Nick_, const TFriends& Friends_) : Nick(Nick_), Friends(Friends_)
			{
			}
			SScLogin(TNick&& Nick_, TFriends&& Friends_) : Nick(std::move(Nick_)), Friends(std::move(Friends_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Nick;
				Stream_ >> Friends;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Nick"] >> Nick;
				Value_["Friends"] >> Friends;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Nick;
				Stream_ << Friends;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Nick"] = Nick;
				Value_["Friends"] = Friends;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick()) + L"," + 
					GetStdName(TFriends());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"Nick") + L"," + 
					GetMemberName(TFriends(), L"Friends");
			}
		};
		struct SScLoginFail : public SProto
		{
			EGameRet GameRet{};
			SScLoginFail()
			{
			}
			SScLoginFail(const EGameRet& GameRet_) : GameRet(GameRet_)
			{
			}
			SScLoginFail(EGameRet&& GameRet_) : GameRet(std::move(GameRet_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet");
			}
		};
		struct SScError : public SProto
		{
			EGameRet GameRet{};
			SScError()
			{
			}
			SScError(const EGameRet& GameRet_) : GameRet(GameRet_)
			{
			}
			SScError(EGameRet&& GameRet_) : GameRet(std::move(GameRet_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet");
			}
		};
		struct SCsAddFriend : public SProto
		{
			TNick Nick{};
			SCsAddFriend()
			{
			}
			SCsAddFriend(const TNick& Nick_) : Nick(Nick_)
			{
			}
			SCsAddFriend(TNick&& Nick_) : Nick(std::move(Nick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Nick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Nick"] >> Nick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Nick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Nick"] = Nick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"Nick");
			}
		};
		struct SSmAddFriend : public SProto
		{
			TNick ToNick{};
			TUID FromUID{};
			TNick FromNick{};
			SSmAddFriend()
			{
			}
			SSmAddFriend(const TNick& ToNick_, const TUID& FromUID_, const TNick& FromNick_) : ToNick(ToNick_), FromUID(FromUID_), FromNick(FromNick_)
			{
			}
			SSmAddFriend(TNick&& ToNick_, TUID&& FromUID_, TNick&& FromNick_) : ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_)), FromNick(std::move(FromNick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToNick;
				Stream_ >> FromUID;
				Stream_ >> FromNick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
				Value_["FromNick"] >> FromNick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToNick;
				Stream_ << FromUID;
				Stream_ << FromNick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
				Value_["FromNick"] = FromNick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TNick(), L"FromNick");
			}
		};
		struct SMaAddFriendGetUID : public SProto
		{
			TNick ToNick{};
			TUID FromUID{};
			TNick FromNick{};
			SKey FromServerKey{};
			SMaAddFriendGetUID()
			{
			}
			SMaAddFriendGetUID(const TNick& ToNick_, const TUID& FromUID_, const TNick& FromNick_, const SKey& FromServerKey_) : ToNick(ToNick_), FromUID(FromUID_), FromNick(FromNick_), FromServerKey(FromServerKey_)
			{
			}
			SMaAddFriendGetUID(TNick&& ToNick_, TUID&& FromUID_, TNick&& FromNick_, SKey&& FromServerKey_) : ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_)), FromNick(std::move(FromNick_)), FromServerKey(std::move(FromServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToNick;
				Stream_ >> FromUID;
				Stream_ >> FromNick;
				Stream_ >> FromServerKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
				Value_["FromNick"] >> FromNick;
				Value_["FromServerKey"] >> FromServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToNick;
				Stream_ << FromUID;
				Stream_ << FromNick;
				Stream_ << FromServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
				Value_["FromNick"] = FromNick;
				Value_["FromServerKey"] = FromServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TNick(), L"FromNick") + L"," + 
					GetMemberName(SKey(), L"FromServerKey");
			}
		};
		struct SAmAddFriendGetUID : public SProto
		{
			TUID ToUID{};
			TNick ToNick{};
			TUID FromUID{};
			TNick FromNick{};
			SKey FromServerKey{};
			SAmAddFriendGetUID()
			{
			}
			SAmAddFriendGetUID(const TUID& ToUID_, const TNick& ToNick_, const TUID& FromUID_, const TNick& FromNick_, const SKey& FromServerKey_) : ToUID(ToUID_), ToNick(ToNick_), FromUID(FromUID_), FromNick(FromNick_), FromServerKey(FromServerKey_)
			{
			}
			SAmAddFriendGetUID(TUID&& ToUID_, TNick&& ToNick_, TUID&& FromUID_, TNick&& FromNick_, SKey&& FromServerKey_) : ToUID(std::move(ToUID_)), ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_)), FromNick(std::move(FromNick_)), FromServerKey(std::move(FromServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> ToNick;
				Stream_ >> FromUID;
				Stream_ >> FromNick;
				Stream_ >> FromServerKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
				Value_["FromNick"] >> FromNick;
				Value_["FromServerKey"] >> FromServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << ToNick;
				Stream_ << FromUID;
				Stream_ << FromNick;
				Stream_ << FromServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
				Value_["FromNick"] = FromNick;
				Value_["FromServerKey"] = FromServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TNick(), L"FromNick") + L"," + 
					GetMemberName(SKey(), L"FromServerKey");
			}
		};
		struct SAmAddFriendGetUIDFail : public SProto
		{
			EGameRet GameRet{};
			TUID FromUID{};
			SKey FromServerKey{};
			SAmAddFriendGetUIDFail()
			{
			}
			SAmAddFriendGetUIDFail(const EGameRet& GameRet_, const TUID& FromUID_, const SKey& FromServerKey_) : GameRet(GameRet_), FromUID(FromUID_), FromServerKey(FromServerKey_)
			{
			}
			SAmAddFriendGetUIDFail(EGameRet&& GameRet_, TUID&& FromUID_, SKey&& FromServerKey_) : GameRet(std::move(GameRet_)), FromUID(std::move(FromUID_)), FromServerKey(std::move(FromServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> FromUID;
				Stream_ >> FromServerKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["FromUID"] >> FromUID;
				Value_["FromServerKey"] >> FromServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << FromUID;
				Stream_ << FromServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["FromUID"] = FromUID;
				Value_["FromServerKey"] = FromServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(SKey(), L"FromServerKey");
			}
		};
		struct SMaAddFriend : public SProto
		{
			TUID ToUID{};
			TNick ToNick{};
			TUID FromUID{};
			TNick FromNick{};
			SKey FromServerKey{};
			SMaAddFriend()
			{
			}
			SMaAddFriend(const TUID& ToUID_, const TNick& ToNick_, const TUID& FromUID_, const TNick& FromNick_, const SKey& FromServerKey_) : ToUID(ToUID_), ToNick(ToNick_), FromUID(FromUID_), FromNick(FromNick_), FromServerKey(FromServerKey_)
			{
			}
			SMaAddFriend(TUID&& ToUID_, TNick&& ToNick_, TUID&& FromUID_, TNick&& FromNick_, SKey&& FromServerKey_) : ToUID(std::move(ToUID_)), ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_)), FromNick(std::move(FromNick_)), FromServerKey(std::move(FromServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> ToNick;
				Stream_ >> FromUID;
				Stream_ >> FromNick;
				Stream_ >> FromServerKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
				Value_["FromNick"] >> FromNick;
				Value_["FromServerKey"] >> FromServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << ToNick;
				Stream_ << FromUID;
				Stream_ << FromNick;
				Stream_ << FromServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
				Value_["FromNick"] = FromNick;
				Value_["FromServerKey"] = FromServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TNick(), L"FromNick") + L"," + 
					GetMemberName(SKey(), L"FromServerKey");
			}
		};
		struct SAmAddFriendRequest : public SProto
		{
			TUID ToUID{};
			TNick ToNick{};
			TUID FromUID{};
			TNick FromNick{};
			SKey FromServerKey{};
			SKey FromMasterKey{};
			SAmAddFriendRequest()
			{
			}
			SAmAddFriendRequest(const TUID& ToUID_, const TNick& ToNick_, const TUID& FromUID_, const TNick& FromNick_, const SKey& FromServerKey_, const SKey& FromMasterKey_) : ToUID(ToUID_), ToNick(ToNick_), FromUID(FromUID_), FromNick(FromNick_), FromServerKey(FromServerKey_), FromMasterKey(FromMasterKey_)
			{
			}
			SAmAddFriendRequest(TUID&& ToUID_, TNick&& ToNick_, TUID&& FromUID_, TNick&& FromNick_, SKey&& FromServerKey_, SKey&& FromMasterKey_) : ToUID(std::move(ToUID_)), ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_)), FromNick(std::move(FromNick_)), FromServerKey(std::move(FromServerKey_)), FromMasterKey(std::move(FromMasterKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> ToNick;
				Stream_ >> FromUID;
				Stream_ >> FromNick;
				Stream_ >> FromServerKey;
				Stream_ >> FromMasterKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
				Value_["FromNick"] >> FromNick;
				Value_["FromServerKey"] >> FromServerKey;
				Value_["FromMasterKey"] >> FromMasterKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << ToNick;
				Stream_ << FromUID;
				Stream_ << FromNick;
				Stream_ << FromServerKey;
				Stream_ << FromMasterKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
				Value_["FromNick"] = FromNick;
				Value_["FromServerKey"] = FromServerKey;
				Value_["FromMasterKey"] = FromMasterKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TNick(), L"FromNick") + L"," + 
					GetMemberName(SKey(), L"FromServerKey") + L"," + 
					GetMemberName(SKey(), L"FromMasterKey");
			}
		};
		struct SMsAddFriendRequest : public SProto
		{
			TUID ToUID{};
			TUID FromUID{};
			TNick FromNick{};
			SMsAddFriendRequest()
			{
			}
			SMsAddFriendRequest(const TUID& ToUID_, const TUID& FromUID_, const TNick& FromNick_) : ToUID(ToUID_), FromUID(FromUID_), FromNick(FromNick_)
			{
			}
			SMsAddFriendRequest(TUID&& ToUID_, TUID&& FromUID_, TNick&& FromNick_) : ToUID(std::move(ToUID_)), FromUID(std::move(FromUID_)), FromNick(std::move(FromNick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> FromUID;
				Stream_ >> FromNick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["FromUID"] >> FromUID;
				Value_["FromNick"] >> FromNick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << FromUID;
				Stream_ << FromNick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["FromUID"] = FromUID;
				Value_["FromNick"] = FromNick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TNick(), L"FromNick");
			}
		};
		struct SScAddFriendRequest : public SProto
		{
			TUID FromUID{};
			TNick FromNick{};
			SScAddFriendRequest()
			{
			}
			SScAddFriendRequest(const TUID& FromUID_, const TNick& FromNick_) : FromUID(FromUID_), FromNick(FromNick_)
			{
			}
			SScAddFriendRequest(TUID&& FromUID_, TNick&& FromNick_) : FromUID(std::move(FromUID_)), FromNick(std::move(FromNick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FromUID;
				Stream_ >> FromNick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FromUID"] >> FromUID;
				Value_["FromNick"] >> FromNick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FromUID;
				Stream_ << FromNick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FromUID"] = FromUID;
				Value_["FromNick"] = FromNick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TNick(), L"FromNick");
			}
		};
		struct SMaAddFriendRequest : public SProto
		{
			TUID ToUID{};
			TNick ToNick{};
			TUID FromUID{};
			SKey FromServerKey{};
			SKey FromMasterKey{};
			SMaAddFriendRequest()
			{
			}
			SMaAddFriendRequest(const TUID& ToUID_, const TNick& ToNick_, const TUID& FromUID_, const SKey& FromServerKey_, const SKey& FromMasterKey_) : ToUID(ToUID_), ToNick(ToNick_), FromUID(FromUID_), FromServerKey(FromServerKey_), FromMasterKey(FromMasterKey_)
			{
			}
			SMaAddFriendRequest(TUID&& ToUID_, TNick&& ToNick_, TUID&& FromUID_, SKey&& FromServerKey_, SKey&& FromMasterKey_) : ToUID(std::move(ToUID_)), ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_)), FromServerKey(std::move(FromServerKey_)), FromMasterKey(std::move(FromMasterKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> ToNick;
				Stream_ >> FromUID;
				Stream_ >> FromServerKey;
				Stream_ >> FromMasterKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
				Value_["FromServerKey"] >> FromServerKey;
				Value_["FromMasterKey"] >> FromMasterKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << ToNick;
				Stream_ << FromUID;
				Stream_ << FromServerKey;
				Stream_ << FromMasterKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
				Value_["FromServerKey"] = FromServerKey;
				Value_["FromMasterKey"] = FromMasterKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(SKey(), L"FromServerKey") + L"," + 
					GetMemberName(SKey(), L"FromMasterKey");
			}
		};
		struct SMaAddFriendRequestFail : public SProto
		{
			EGameRet GameRet{};
			TUID ToUID{};
			TUID FromUID{};
			SKey FromServerKey{};
			SKey FromMasterKey{};
			SMaAddFriendRequestFail()
			{
			}
			SMaAddFriendRequestFail(const EGameRet& GameRet_, const TUID& ToUID_, const TUID& FromUID_, const SKey& FromServerKey_, const SKey& FromMasterKey_) : GameRet(GameRet_), ToUID(ToUID_), FromUID(FromUID_), FromServerKey(FromServerKey_), FromMasterKey(FromMasterKey_)
			{
			}
			SMaAddFriendRequestFail(EGameRet&& GameRet_, TUID&& ToUID_, TUID&& FromUID_, SKey&& FromServerKey_, SKey&& FromMasterKey_) : GameRet(std::move(GameRet_)), ToUID(std::move(ToUID_)), FromUID(std::move(FromUID_)), FromServerKey(std::move(FromServerKey_)), FromMasterKey(std::move(FromMasterKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> ToUID;
				Stream_ >> FromUID;
				Stream_ >> FromServerKey;
				Stream_ >> FromMasterKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["ToUID"] >> ToUID;
				Value_["FromUID"] >> FromUID;
				Value_["FromServerKey"] >> FromServerKey;
				Value_["FromMasterKey"] >> FromMasterKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << ToUID;
				Stream_ << FromUID;
				Stream_ << FromServerKey;
				Stream_ << FromMasterKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["ToUID"] = ToUID;
				Value_["FromUID"] = FromUID;
				Value_["FromServerKey"] = FromServerKey;
				Value_["FromMasterKey"] = FromMasterKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(SKey(), L"FromServerKey") + L"," + 
					GetMemberName(SKey(), L"FromMasterKey");
			}
		};
		struct SAmAddFriend : public SProto
		{
			TUID ToUID{};
			TNick ToNick{};
			TUID FromUID{};
			SKey FromServerKey{};
			SAmAddFriend()
			{
			}
			SAmAddFriend(const TUID& ToUID_, const TNick& ToNick_, const TUID& FromUID_, const SKey& FromServerKey_) : ToUID(ToUID_), ToNick(ToNick_), FromUID(FromUID_), FromServerKey(FromServerKey_)
			{
			}
			SAmAddFriend(TUID&& ToUID_, TNick&& ToNick_, TUID&& FromUID_, SKey&& FromServerKey_) : ToUID(std::move(ToUID_)), ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_)), FromServerKey(std::move(FromServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> ToNick;
				Stream_ >> FromUID;
				Stream_ >> FromServerKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
				Value_["FromServerKey"] >> FromServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << ToNick;
				Stream_ << FromUID;
				Stream_ << FromServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
				Value_["FromServerKey"] = FromServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(SKey(), L"FromServerKey");
			}
		};
		struct SAmAddFriendFail : public SProto
		{
			EGameRet GameRet{};
			TUID ToUID{};
			TUID FromUID{};
			SKey FromServerKey{};
			SAmAddFriendFail()
			{
			}
			SAmAddFriendFail(const EGameRet& GameRet_, const TUID& ToUID_, const TUID& FromUID_, const SKey& FromServerKey_) : GameRet(GameRet_), ToUID(ToUID_), FromUID(FromUID_), FromServerKey(FromServerKey_)
			{
			}
			SAmAddFriendFail(EGameRet&& GameRet_, TUID&& ToUID_, TUID&& FromUID_, SKey&& FromServerKey_) : GameRet(std::move(GameRet_)), ToUID(std::move(ToUID_)), FromUID(std::move(FromUID_)), FromServerKey(std::move(FromServerKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> ToUID;
				Stream_ >> FromUID;
				Stream_ >> FromServerKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["ToUID"] >> ToUID;
				Value_["FromUID"] >> FromUID;
				Value_["FromServerKey"] >> FromServerKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << ToUID;
				Stream_ << FromUID;
				Stream_ << FromServerKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["ToUID"] = ToUID;
				Value_["FromUID"] = FromUID;
				Value_["FromServerKey"] = FromServerKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(SKey(), L"FromServerKey");
			}
		};
		struct SMsAddFriend : public SProto
		{
			TUID ToUID{};
			TNick ToNick{};
			TUID FromUID{};
			SMsAddFriend()
			{
			}
			SMsAddFriend(const TUID& ToUID_, const TNick& ToNick_, const TUID& FromUID_) : ToUID(ToUID_), ToNick(ToNick_), FromUID(FromUID_)
			{
			}
			SMsAddFriend(TUID&& ToUID_, TNick&& ToNick_, TUID&& FromUID_) : ToUID(std::move(ToUID_)), ToNick(std::move(ToNick_)), FromUID(std::move(FromUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> ToNick;
				Stream_ >> FromUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["ToNick"] >> ToNick;
				Value_["FromUID"] >> FromUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << ToNick;
				Stream_ << FromUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["ToNick"] = ToNick;
				Value_["FromUID"] = FromUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TNick(), L"ToNick") + L"," + 
					GetMemberName(TUID(), L"FromUID");
			}
		};
		struct SMsAddFriendFail : public SProto
		{
			EGameRet GameRet{};
			TUID FromUID{};
			SMsAddFriendFail()
			{
			}
			SMsAddFriendFail(const EGameRet& GameRet_, const TUID& FromUID_) : GameRet(GameRet_), FromUID(FromUID_)
			{
			}
			SMsAddFriendFail(EGameRet&& GameRet_, TUID&& FromUID_) : GameRet(std::move(GameRet_)), FromUID(std::move(FromUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> FromUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["FromUID"] >> FromUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << FromUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["FromUID"] = FromUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(TUID(), L"FromUID");
			}
		};
		struct SScAddFriend : public SProto
		{
			TUID ToUID{};
			SFriend Friend{};
			SScAddFriend()
			{
			}
			SScAddFriend(const TUID& ToUID_, const SFriend& Friend_) : ToUID(ToUID_), Friend(Friend_)
			{
			}
			SScAddFriend(TUID&& ToUID_, SFriend&& Friend_) : ToUID(std::move(ToUID_)), Friend(std::move(Friend_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> Friend;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["Friend"] >> Friend;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << Friend;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["Friend"] = Friend;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SFriend());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(SFriend(), L"Friend");
			}
		};
		struct SCsAllowFriend : public SProto
		{
			TUID FriendUID{};
			SCsAllowFriend()
			{
			}
			SCsAllowFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SCsAllowFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SSmAllowFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			SKey UserKey{};
			SSmAllowFriend()
			{
			}
			SSmAllowFriend(const TUID& UID_, const TUID& FriendUID_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), UserKey(UserKey_)
			{
			}
			SSmAllowFriend(TUID&& UID_, TUID&& FriendUID_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsAllowFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			SKey UserKey{};
			SMsAllowFriend()
			{
			}
			SMsAllowFriend(const TUID& UID_, const TUID& FriendUID_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), UserKey(UserKey_)
			{
			}
			SMsAllowFriend(TUID&& UID_, TUID&& FriendUID_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsAllowFriendFail : public SProto
		{
			EGameRet GameRet{};
			SKey UserKey{};
			SMsAllowFriendFail()
			{
			}
			SMsAllowFriendFail(const EGameRet& GameRet_, const SKey& UserKey_) : GameRet(GameRet_), UserKey(UserKey_)
			{
			}
			SMsAllowFriendFail(EGameRet&& GameRet_, SKey&& UserKey_) : GameRet(std::move(GameRet_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SScAllowFriend : public SProto
		{
			TUID FriendUID{};
			SScAllowFriend()
			{
			}
			SScAllowFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SScAllowFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SCsDenyFriend : public SProto
		{
			TUID FriendUID{};
			SCsDenyFriend()
			{
			}
			SCsDenyFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SCsDenyFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SSmDenyFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			SKey UserKey{};
			SSmDenyFriend()
			{
			}
			SSmDenyFriend(const TUID& UID_, const TUID& FriendUID_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), UserKey(UserKey_)
			{
			}
			SSmDenyFriend(TUID&& UID_, TUID&& FriendUID_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsDenyFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			SKey UserKey{};
			SMsDenyFriend()
			{
			}
			SMsDenyFriend(const TUID& UID_, const TUID& FriendUID_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), UserKey(UserKey_)
			{
			}
			SMsDenyFriend(TUID&& UID_, TUID&& FriendUID_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsDenyFriendFail : public SProto
		{
			EGameRet GameRet{};
			SKey UserKey{};
			SMsDenyFriendFail()
			{
			}
			SMsDenyFriendFail(const EGameRet& GameRet_, const SKey& UserKey_) : GameRet(GameRet_), UserKey(UserKey_)
			{
			}
			SMsDenyFriendFail(EGameRet&& GameRet_, SKey&& UserKey_) : GameRet(std::move(GameRet_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SScDenyFriend : public SProto
		{
			TUID FriendUID{};
			SScDenyFriend()
			{
			}
			SScDenyFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SScDenyFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SCsBlockFriend : public SProto
		{
			TUID FriendUID{};
			SCsBlockFriend()
			{
			}
			SCsBlockFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SCsBlockFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SSmBlockFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			SKey UserKey{};
			SSmBlockFriend()
			{
			}
			SSmBlockFriend(const TUID& UID_, const TUID& FriendUID_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), UserKey(UserKey_)
			{
			}
			SSmBlockFriend(TUID&& UID_, TUID&& FriendUID_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsBlockFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			EFriendState FriendState{};
			SKey UserKey{};
			SMsBlockFriend()
			{
			}
			SMsBlockFriend(const TUID& UID_, const TUID& FriendUID_, const EFriendState& FriendState_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), FriendState(FriendState_), UserKey(UserKey_)
			{
			}
			SMsBlockFriend(TUID&& UID_, TUID&& FriendUID_, EFriendState&& FriendState_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendState;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(EFriendState()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(EFriendState(), L"FriendState") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsBlockFriendFail : public SProto
		{
			EGameRet GameRet{};
			SKey UserKey{};
			SMsBlockFriendFail()
			{
			}
			SMsBlockFriendFail(const EGameRet& GameRet_, const SKey& UserKey_) : GameRet(GameRet_), UserKey(UserKey_)
			{
			}
			SMsBlockFriendFail(EGameRet&& GameRet_, SKey&& UserKey_) : GameRet(std::move(GameRet_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SScBlockFriend : public SProto
		{
			TUID FriendUID{};
			SScBlockFriend()
			{
			}
			SScBlockFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SScBlockFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SCsUnBlockFriend : public SProto
		{
			TUID FriendUID{};
			SCsUnBlockFriend()
			{
			}
			SCsUnBlockFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SCsUnBlockFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SSmUnBlockFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			SKey UserKey{};
			SSmUnBlockFriend()
			{
			}
			SSmUnBlockFriend(const TUID& UID_, const TUID& FriendUID_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), UserKey(UserKey_)
			{
			}
			SSmUnBlockFriend(TUID&& UID_, TUID&& FriendUID_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsUnBlockFriend : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			EFriendState FriendState{};
			SKey UserKey{};
			SMsUnBlockFriend()
			{
			}
			SMsUnBlockFriend(const TUID& UID_, const TUID& FriendUID_, const EFriendState& FriendState_, const SKey& UserKey_) : UID(UID_), FriendUID(FriendUID_), FriendState(FriendState_), UserKey(UserKey_)
			{
			}
			SMsUnBlockFriend(TUID&& UID_, TUID&& FriendUID_, EFriendState&& FriendState_, SKey&& UserKey_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendState;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(EFriendState()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(EFriendState(), L"FriendState") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsUnBlockFriendFail : public SProto
		{
			EGameRet GameRet{};
			SKey UserKey{};
			SMsUnBlockFriendFail()
			{
			}
			SMsUnBlockFriendFail(const EGameRet& GameRet_, const SKey& UserKey_) : GameRet(GameRet_), UserKey(UserKey_)
			{
			}
			SMsUnBlockFriendFail(EGameRet&& GameRet_, SKey&& UserKey_) : GameRet(std::move(GameRet_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SScUnBlockFriend : public SProto
		{
			TUID FriendUID{};
			SScUnBlockFriend()
			{
			}
			SScUnBlockFriend(const TUID& FriendUID_) : FriendUID(FriendUID_)
			{
			}
			SScUnBlockFriend(TUID&& FriendUID_) : FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SCsMessageSend : public SProto
		{
			TUID ToUID{};
			TMessage Message{};
			SCsMessageSend()
			{
			}
			SCsMessageSend(const TUID& ToUID_, const TMessage& Message_) : ToUID(ToUID_), Message(Message_)
			{
			}
			SCsMessageSend(TUID&& ToUID_, TMessage&& Message_) : ToUID(std::move(ToUID_)), Message(std::move(Message_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ToUID;
				Stream_ >> Message;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ToUID"] >> ToUID;
				Value_["Message"] >> Message;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ToUID;
				Stream_ << Message;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ToUID"] = ToUID;
				Value_["Message"] = Message;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TMessage());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TMessage(), L"Message");
			}
		};
		struct SSsMessageSend : public SProto
		{
			TUID FromUID{};
			TUID ToUID{};
			TMessage Message{};
			SSsMessageSend()
			{
			}
			SSsMessageSend(const TUID& FromUID_, const TUID& ToUID_, const TMessage& Message_) : FromUID(FromUID_), ToUID(ToUID_), Message(Message_)
			{
			}
			SSsMessageSend(TUID&& FromUID_, TUID&& ToUID_, TMessage&& Message_) : FromUID(std::move(FromUID_)), ToUID(std::move(ToUID_)), Message(std::move(Message_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FromUID;
				Stream_ >> ToUID;
				Stream_ >> Message;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FromUID"] >> FromUID;
				Value_["ToUID"] >> ToUID;
				Value_["Message"] >> Message;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FromUID;
				Stream_ << ToUID;
				Stream_ << Message;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FromUID"] = FromUID;
				Value_["ToUID"] = ToUID;
				Value_["Message"] = Message;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TMessage());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TUID(), L"ToUID") + L"," + 
					GetMemberName(TMessage(), L"Message");
			}
		};
		struct SScMessageReceived : public SProto
		{
			TUID FromUID{};
			TMessage Message{};
			SScMessageReceived()
			{
			}
			SScMessageReceived(const TUID& FromUID_, const TMessage& Message_) : FromUID(FromUID_), Message(Message_)
			{
			}
			SScMessageReceived(TUID&& FromUID_, TMessage&& Message_) : FromUID(std::move(FromUID_)), Message(std::move(Message_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FromUID;
				Stream_ >> Message;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FromUID"] >> FromUID;
				Value_["Message"] >> Message;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FromUID;
				Stream_ << Message;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FromUID"] = FromUID;
				Value_["Message"] = Message;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TMessage());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FromUID") + L"," + 
					GetMemberName(TMessage(), L"Message");
			}
		};
		struct SCsChangeState : public SProto
		{
			TState State{};
			SCsChangeState()
			{
			}
			SCsChangeState(const TState& State_) : State(State_)
			{
			}
			SCsChangeState(TState&& State_) : State(std::move(State_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> State;
			}
			void operator << (const Value& Value_) override
			{
				Value_["State"] >> State;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << State;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["State"] = State;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TState(), L"State");
			}
		};
		struct SSmChangeState : public SProto
		{
			TUID UID{};
			TState State{};
			SKey UserKey{};
			SSmChangeState()
			{
			}
			SSmChangeState(const TUID& UID_, const TState& State_, const SKey& UserKey_) : UID(UID_), State(State_), UserKey(UserKey_)
			{
			}
			SSmChangeState(TUID&& UID_, TState&& State_, SKey&& UserKey_) : UID(std::move(UID_)), State(std::move(State_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> State;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["State"] >> State;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << State;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["State"] = State;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TState()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TState(), L"State") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsChangeState : public SProto
		{
			TState State{};
			SKey UserKey{};
			SMsChangeState()
			{
			}
			SMsChangeState(const TState& State_, const SKey& UserKey_) : State(State_), UserKey(UserKey_)
			{
			}
			SMsChangeState(TState&& State_, SKey&& UserKey_) : State(std::move(State_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> State;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["State"] >> State;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << State;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["State"] = State;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TState()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TState(), L"State") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SMsChangeStateFail : public SProto
		{
			EGameRet GameRet{};
			SKey UserKey{};
			SMsChangeStateFail()
			{
			}
			SMsChangeStateFail(const EGameRet& GameRet_, const SKey& UserKey_) : GameRet(GameRet_), UserKey(UserKey_)
			{
			}
			SMsChangeStateFail(EGameRet&& GameRet_, SKey&& UserKey_) : GameRet(std::move(GameRet_)), UserKey(std::move(UserKey_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> GameRet;
				Stream_ >> UserKey;
			}
			void operator << (const Value& Value_) override
			{
				Value_["GameRet"] >> GameRet;
				Value_["UserKey"] >> UserKey;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << GameRet;
				Stream_ << UserKey;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["GameRet"] = GameRet;
				Value_["UserKey"] = UserKey;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(EGameRet()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EGameRet(), L"GameRet") + L"," + 
					GetMemberName(SKey(), L"UserKey");
			}
		};
		struct SScChangeState : public SProto
		{
			TState State{};
			SScChangeState()
			{
			}
			SScChangeState(const TState& State_) : State(State_)
			{
			}
			SScChangeState(TState&& State_) : State(std::move(State_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> State;
			}
			void operator << (const Value& Value_) override
			{
				Value_["State"] >> State;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << State;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["State"] = State;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TState(), L"State");
			}
		};
		struct SUIDFriendInfo : public SProto
		{
			TUID UID{};
			SFriendInfo FriendInfo{};
			SUIDFriendInfo()
			{
			}
			SUIDFriendInfo(const TUID& UID_, const SFriendInfo& FriendInfo_) : UID(UID_), FriendInfo(FriendInfo_)
			{
			}
			SUIDFriendInfo(TUID&& UID_, SFriendInfo&& FriendInfo_) : UID(std::move(UID_)), FriendInfo(std::move(FriendInfo_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendInfo;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendInfo"] >> FriendInfo;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendInfo;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendInfo"] = FriendInfo;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SFriendInfo());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SFriendInfo(), L"FriendInfo");
			}
		};
		using TUIDFriendInfos = list<SUIDFriendInfo>;
		using TFriendInfos = map<TUID,SFriendInfo>;
		struct SMmFriendStateChanged : public SProto
		{
			TUIDFriendInfos UIDFriendInfos{};
			TUID FriendUID{};
			SMmFriendStateChanged()
			{
			}
			SMmFriendStateChanged(const TUIDFriendInfos& UIDFriendInfos_, const TUID& FriendUID_) : UIDFriendInfos(UIDFriendInfos_), FriendUID(FriendUID_)
			{
			}
			SMmFriendStateChanged(TUIDFriendInfos&& UIDFriendInfos_, TUID&& FriendUID_) : UIDFriendInfos(std::move(UIDFriendInfos_)), FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UIDFriendInfos;
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UIDFriendInfos"] >> UIDFriendInfos;
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UIDFriendInfos;
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UIDFriendInfos"] = UIDFriendInfos;
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUIDFriendInfos()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUIDFriendInfos(), L"UIDFriendInfos") + L"," + 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SMsFriendStateChanged : public SProto
		{
			TUIDFriendInfos UIDFriendInfos{};
			TUID FriendUID{};
			SMsFriendStateChanged()
			{
			}
			SMsFriendStateChanged(const TUIDFriendInfos& UIDFriendInfos_, const TUID& FriendUID_) : UIDFriendInfos(UIDFriendInfos_), FriendUID(FriendUID_)
			{
			}
			SMsFriendStateChanged(TUIDFriendInfos&& UIDFriendInfos_, TUID&& FriendUID_) : UIDFriendInfos(std::move(UIDFriendInfos_)), FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UIDFriendInfos;
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UIDFriendInfos"] >> UIDFriendInfos;
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UIDFriendInfos;
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UIDFriendInfos"] = UIDFriendInfos;
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUIDFriendInfos()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUIDFriendInfos(), L"UIDFriendInfos") + L"," + 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SScFriendStateChanged : public SProto
		{
			TUID FriendUID{};
			TState FriendState{};
			SScFriendStateChanged()
			{
			}
			SScFriendStateChanged(const TUID& FriendUID_, const TState& FriendState_) : FriendUID(FriendUID_), FriendState(FriendState_)
			{
			}
			SScFriendStateChanged(TUID&& FriendUID_, TState&& FriendState_) : FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << FriendUID;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(TState(), L"FriendState");
			}
		};
		struct SMmFriendStateChangedRenew : public SProto
		{
			list<SMmFriendStateChanged> MmFriendStateChangeds{};
			SMmFriendStateChangedRenew()
			{
			}
			SMmFriendStateChangedRenew(const list<SMmFriendStateChanged>& MmFriendStateChangeds_) : MmFriendStateChangeds(MmFriendStateChangeds_)
			{
			}
			SMmFriendStateChangedRenew(list<SMmFriendStateChanged>&& MmFriendStateChangeds_) : MmFriendStateChangeds(std::move(MmFriendStateChangeds_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MmFriendStateChangeds;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MmFriendStateChangeds"] >> MmFriendStateChangeds;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MmFriendStateChangeds;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MmFriendStateChangeds"] = MmFriendStateChangeds;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SMmFriendStateChanged>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SMmFriendStateChanged>(), L"MmFriendStateChangeds");
			}
		};
		struct SMsFriendStateChangedOffline : public SProto
		{
			map<TUID,list<TUID>> Friends{};
			SMsFriendStateChangedOffline()
			{
			}
			SMsFriendStateChangedOffline(const map<TUID,list<TUID>>& Friends_) : Friends(Friends_)
			{
			}
			SMsFriendStateChangedOffline(map<TUID,list<TUID>>&& Friends_) : Friends(std::move(Friends_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Friends;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Friends"] >> Friends;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Friends;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Friends"] = Friends;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(map<TUID,list<TUID>>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(map<TUID,list<TUID>>(), L"Friends");
			}
		};
		struct SScFriendStateChangedOffline : public SProto
		{
			list<TUID> Friends{};
			SScFriendStateChangedOffline()
			{
			}
			SScFriendStateChangedOffline(const list<TUID>& Friends_) : Friends(Friends_)
			{
			}
			SScFriendStateChangedOffline(list<TUID>&& Friends_) : Friends(std::move(Friends_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Friends;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Friends"] >> Friends;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Friends;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Friends"] = Friends;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<TUID>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<TUID>(), L"Friends");
			}
		};
		struct SAmPunish : public SProto
		{
			TUID UID{};
			system_clock::time_point EndTime{};
			SAmPunish()
			{
			}
			SAmPunish(const TUID& UID_, const system_clock::time_point& EndTime_) : UID(UID_), EndTime(EndTime_)
			{
			}
			SAmPunish(TUID&& UID_, system_clock::time_point&& EndTime_) : UID(std::move(UID_)), EndTime(std::move(EndTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> EndTime;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["EndTime"] >> EndTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << EndTime;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["EndTime"] = EndTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(system_clock::time_point());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(system_clock::time_point(), L"EndTime");
			}
		};
		struct SMsPunish : public SProto
		{
			TUID UID{};
			system_clock::time_point EndTime{};
			SMsPunish()
			{
			}
			SMsPunish(const TUID& UID_, const system_clock::time_point& EndTime_) : UID(UID_), EndTime(EndTime_)
			{
			}
			SMsPunish(TUID&& UID_, system_clock::time_point&& EndTime_) : UID(std::move(UID_)), EndTime(std::move(EndTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> EndTime;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["EndTime"] >> EndTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << EndTime;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["EndTime"] = EndTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(system_clock::time_point());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(system_clock::time_point(), L"EndTime");
			}
		};
		enum EDataBaseError
		{
			InvalidID=50003,
		};
		enum class EAuthProtoDB
		{
			Create,
			Login,
			AddFriendGetUID,
			ChangeNick,
			Check,
		};
		struct SDummyOutDb : public SProto
		{
			void operator << (CStream&) override
			{
			}
			void operator << (const Value&) override
			{
			}
			void operator >> (CStream&) const override
			{
			}
			void operator >> (Value&) const override
			{
			}
			static wstring StdName(void)
			{
				return L"";
			}
			static wstring MemberName(void)
			{
				return L"";
			}
		};
		struct SAuthLoginInDb : public SProto
		{
			TID ID{};
			SAuthLoginInDb()
			{
			}
			SAuthLoginInDb(const TID& ID_) : ID(ID_)
			{
			}
			SAuthLoginInDb(TID&& ID_) : ID(std::move(ID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ID"] >> ID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ID"] = ID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TID(), L"ID");
			}
		};
		struct SAuthCreateInDb : public SAuthLoginInDb
		{
			TNick Nick{};
			SAuthCreateInDb()
			{
			}
			SAuthCreateInDb(const SAuthLoginInDb& Super_, const TNick& Nick_) : SAuthLoginInDb(Super_), Nick(Nick_)
			{
			}
			SAuthCreateInDb(SAuthLoginInDb&& Super_, TNick&& Nick_) : SAuthLoginInDb(std::move(Super_)), Nick(std::move(Nick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SAuthLoginInDb::operator << (Stream_);
				Stream_ >> Nick;
			}
			void operator << (const Value& Value_) override
			{
				SAuthLoginInDb::operator << (Value_);
				Value_["Nick"] >> Nick;
			}
			void operator >> (CStream& Stream_) const override
			{
				SAuthLoginInDb::operator >> (Stream_);
				Stream_ << Nick;
			}
			void operator >> (Value& Value_) const override
			{
				SAuthLoginInDb::operator >> (Value_);
				Value_["Nick"] = Nick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SAuthLoginInDb()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SAuthLoginInDb(), L"") + L"," + 
					GetMemberName(TNick(), L"Nick");
			}
		};
		struct SAuthAddFriendGetUIDInDb : public SProto
		{
			TNick Nick{};
			SAuthAddFriendGetUIDInDb()
			{
			}
			SAuthAddFriendGetUIDInDb(const TNick& Nick_) : Nick(Nick_)
			{
			}
			SAuthAddFriendGetUIDInDb(TNick&& Nick_) : Nick(std::move(Nick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Nick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Nick"] >> Nick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Nick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Nick"] = Nick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"Nick");
			}
		};
		struct SAuthAddFriendGetUIDOutDb : public SProto
		{
			list<TUID> UIDs{};
			SAuthAddFriendGetUIDOutDb()
			{
			}
			SAuthAddFriendGetUIDOutDb(const list<TUID>& UIDs_) : UIDs(UIDs_)
			{
			}
			SAuthAddFriendGetUIDOutDb(list<TUID>&& UIDs_) : UIDs(std::move(UIDs_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UIDs;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UIDs"] >> UIDs;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UIDs;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UIDs"] = UIDs;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<TUID>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<TUID>(), L"UIDs");
			}
		};
		struct SAuthChangeNickInDb : public SProto
		{
			TUID UID{};
			TNick Nick{};
			SAuthChangeNickInDb()
			{
			}
			SAuthChangeNickInDb(const TUID& UID_, const TNick& Nick_) : UID(UID_), Nick(Nick_)
			{
			}
			SAuthChangeNickInDb(TUID&& UID_, TNick&& Nick_) : UID(std::move(UID_)), Nick(std::move(Nick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Nick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Nick"] >> Nick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Nick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Nick"] = Nick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TNick(), L"Nick");
			}
		};
		struct SAuthCheckInDb : public SAuthLoginInDb
		{
			SAuthCheckInDb()
			{
			}
			SAuthCheckInDb(const SAuthLoginInDb& Super_) : SAuthLoginInDb(Super_)
			{
			}
			SAuthCheckInDb(SAuthLoginInDb&& Super_) : SAuthLoginInDb(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SAuthLoginInDb::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SAuthLoginInDb::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SAuthLoginInDb::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SAuthLoginInDb::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SAuthLoginInDb());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SAuthLoginInDb(), L"");
			}
		};
		struct SAuthUser : public SProto
		{
			TUID UID{};
			TNick Nick{};
			SAuthUser()
			{
			}
			SAuthUser(const TUID& UID_, const TNick& Nick_) : UID(UID_), Nick(Nick_)
			{
			}
			SAuthUser(TUID&& UID_, TNick&& Nick_) : UID(std::move(UID_)), Nick(std::move(Nick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Nick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Nick"] >> Nick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Nick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Nick"] = Nick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TNick(), L"Nick");
			}
		};
		struct SAuthLoginOutDb : public SProto
		{
			list<SAuthUser> Users{};
			SAuthLoginOutDb()
			{
			}
			SAuthLoginOutDb(const list<SAuthUser>& Users_) : Users(Users_)
			{
			}
			SAuthLoginOutDb(list<SAuthUser>&& Users_) : Users(std::move(Users_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Users;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Users"] >> Users;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Users;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Users"] = Users;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SAuthUser>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SAuthUser>(), L"Users");
			}
		};
		enum class EMasterProtoDB
		{
			Create,
			Login,
			ChangeNick,
			Check,
			Punish,
			AddFriendBegin,
			AddFriendRequest,
			AddFriendEnd,
			AddFriendFail,
			AllowFriend,
			DenyFriend,
			BlockFriend,
			UnBlockFriend,
			ChangeState,
		};
		struct SMasterCreateInDb : public SProto
		{
			TUID UID{};
			SAccount Account{};
			TState State{};
			SMasterCreateInDb()
			{
			}
			SMasterCreateInDb(const TUID& UID_, const SAccount& Account_, const TState& State_) : UID(UID_), Account(Account_), State(State_)
			{
			}
			SMasterCreateInDb(TUID&& UID_, SAccount&& Account_, TState&& State_) : UID(std::move(UID_)), Account(std::move(Account_)), State(std::move(State_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Account;
				Stream_ >> State;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Account"] >> Account;
				Value_["State"] >> State;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Account;
				Stream_ << State;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Account"] = Account;
				Value_["State"] = State;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(SAccount()) + L"," + 
					GetStdName(TState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(SAccount(), L"Account") + L"," + 
					GetMemberName(TState(), L"State");
			}
		};
		struct SMasterCreateOutDb : public SProto
		{
			void operator << (CStream&) override
			{
			}
			void operator << (const Value&) override
			{
			}
			void operator >> (CStream&) const override
			{
			}
			void operator >> (Value&) const override
			{
			}
			static wstring StdName(void)
			{
				return L"";
			}
			static wstring MemberName(void)
			{
				return L"";
			}
		};
		struct SMasterLoginInDb : public SProto
		{
			TUID UID{};
			TID ID{};
			SMasterLoginInDb()
			{
			}
			SMasterLoginInDb(const TUID& UID_, const TID& ID_) : UID(UID_), ID(ID_)
			{
			}
			SMasterLoginInDb(TUID&& UID_, TID&& ID_) : UID(std::move(UID_)), ID(std::move(ID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> ID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["ID"] >> ID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << ID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["ID"] = ID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TID(), L"ID");
			}
		};
		struct SMasterUser : public SProto
		{
			TNick Nick{};
			TState State{};
			system_clock::time_point PunishEndTime{};
			SMasterUser()
			{
			}
			SMasterUser(const TNick& Nick_, const TState& State_, const system_clock::time_point& PunishEndTime_) : Nick(Nick_), State(State_), PunishEndTime(PunishEndTime_)
			{
			}
			SMasterUser(TNick&& Nick_, TState&& State_, system_clock::time_point&& PunishEndTime_) : Nick(std::move(Nick_)), State(std::move(State_)), PunishEndTime(std::move(PunishEndTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Nick;
				Stream_ >> State;
				Stream_ >> PunishEndTime;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Nick"] >> Nick;
				Value_["State"] >> State;
				Value_["PunishEndTime"] >> PunishEndTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Nick;
				Stream_ << State;
				Stream_ << PunishEndTime;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Nick"] = Nick;
				Value_["State"] = State;
				Value_["PunishEndTime"] = PunishEndTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TNick()) + L"," + 
					GetStdName(TState()) + L"," + 
					GetStdName(system_clock::time_point());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TNick(), L"Nick") + L"," + 
					GetMemberName(TState(), L"State") + L"," + 
					GetMemberName(system_clock::time_point(), L"PunishEndTime");
			}
		};
		struct SMasterLoginOutDb : public SProto
		{
			list<SMasterUser> Users{};
			TFriendDBs Friends{};
			SMasterLoginOutDb()
			{
			}
			SMasterLoginOutDb(const list<SMasterUser>& Users_, const TFriendDBs& Friends_) : Users(Users_), Friends(Friends_)
			{
			}
			SMasterLoginOutDb(list<SMasterUser>&& Users_, TFriendDBs&& Friends_) : Users(std::move(Users_)), Friends(std::move(Friends_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Users;
				Stream_ >> Friends;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Users"] >> Users;
				Value_["Friends"] >> Friends;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Users;
				Stream_ << Friends;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Users"] = Users;
				Value_["Friends"] = Friends;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SMasterUser>()) + L"," + 
					GetStdName(TFriendDBs());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SMasterUser>(), L"Users") + L"," + 
					GetMemberName(TFriendDBs(), L"Friends");
			}
		};
		struct SMasterChangeNickInDb : public SProto
		{
			TUID UID{};
			TNick Nick{};
			SMasterChangeNickInDb()
			{
			}
			SMasterChangeNickInDb(const TUID& UID_, const TNick& Nick_) : UID(UID_), Nick(Nick_)
			{
			}
			SMasterChangeNickInDb(TUID&& UID_, TNick&& Nick_) : UID(std::move(UID_)), Nick(std::move(Nick_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Nick;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Nick"] >> Nick;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Nick;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Nick"] = Nick;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TNick(), L"Nick");
			}
		};
		struct SMasterAddFriendBeginInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			TNick FriendNick{};
			EFriendState FriendState{};
			SMasterAddFriendBeginInDb()
			{
			}
			SMasterAddFriendBeginInDb(const TUID& UID_, const TUID& FriendUID_, const TNick& FriendNick_, const EFriendState& FriendState_) : UID(UID_), FriendUID(FriendUID_), FriendNick(FriendNick_), FriendState(FriendState_)
			{
			}
			SMasterAddFriendBeginInDb(TUID&& UID_, TUID&& FriendUID_, TNick&& FriendNick_, EFriendState&& FriendState_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendNick(std::move(FriendNick_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendNick;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendNick"] >> FriendNick;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendNick;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendNick"] = FriendNick;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(TNick(), L"FriendNick") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SMasterAddFriendRequestInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			TNick FriendNick{};
			EFriendState FriendState{};
			SMasterAddFriendRequestInDb()
			{
			}
			SMasterAddFriendRequestInDb(const TUID& UID_, const TUID& FriendUID_, const TNick& FriendNick_, const EFriendState& FriendState_) : UID(UID_), FriendUID(FriendUID_), FriendNick(FriendNick_), FriendState(FriendState_)
			{
			}
			SMasterAddFriendRequestInDb(TUID&& UID_, TUID&& FriendUID_, TNick&& FriendNick_, EFriendState&& FriendState_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendNick(std::move(FriendNick_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendNick;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendNick"] >> FriendNick;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendNick;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendNick"] = FriendNick;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(TNick()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(TNick(), L"FriendNick") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SMasterAddFriendRequestOutDb : public SProto
		{
			list<int32> RowCounts{};
			SMasterAddFriendRequestOutDb()
			{
			}
			SMasterAddFriendRequestOutDb(const list<int32>& RowCounts_) : RowCounts(RowCounts_)
			{
			}
			SMasterAddFriendRequestOutDb(list<int32>&& RowCounts_) : RowCounts(std::move(RowCounts_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> RowCounts;
			}
			void operator << (const Value& Value_) override
			{
				Value_["RowCounts"] >> RowCounts;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << RowCounts;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["RowCounts"] = RowCounts;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<int32>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<int32>(), L"RowCounts");
			}
		};
		struct SMasterAddFriendEndInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			EFriendState FriendState{};
			SMasterAddFriendEndInDb()
			{
			}
			SMasterAddFriendEndInDb(const TUID& UID_, const TUID& FriendUID_, const EFriendState& FriendState_) : UID(UID_), FriendUID(FriendUID_), FriendState(FriendState_)
			{
			}
			SMasterAddFriendEndInDb(TUID&& UID_, TUID&& FriendUID_, EFriendState&& FriendState_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SMasterAddFriendFailInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			SMasterAddFriendFailInDb()
			{
			}
			SMasterAddFriendFailInDb(const TUID& UID_, const TUID& FriendUID_) : UID(UID_), FriendUID(FriendUID_)
			{
			}
			SMasterAddFriendFailInDb(TUID&& UID_, TUID&& FriendUID_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID");
			}
		};
		struct SMasterAllowFriendInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			EFriendState FriendState{};
			SMasterAllowFriendInDb()
			{
			}
			SMasterAllowFriendInDb(const TUID& UID_, const TUID& FriendUID_, const EFriendState& FriendState_) : UID(UID_), FriendUID(FriendUID_), FriendState(FriendState_)
			{
			}
			SMasterAllowFriendInDb(TUID&& UID_, TUID&& FriendUID_, EFriendState&& FriendState_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SMasterDenyFriendInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			EFriendState FriendState{};
			SMasterDenyFriendInDb()
			{
			}
			SMasterDenyFriendInDb(const TUID& UID_, const TUID& FriendUID_, const EFriendState& FriendState_) : UID(UID_), FriendUID(FriendUID_), FriendState(FriendState_)
			{
			}
			SMasterDenyFriendInDb(TUID&& UID_, TUID&& FriendUID_, EFriendState&& FriendState_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SMasterBlockFriendInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			EFriendState FriendState{};
			SMasterBlockFriendInDb()
			{
			}
			SMasterBlockFriendInDb(const TUID& UID_, const TUID& FriendUID_, const EFriendState& FriendState_) : UID(UID_), FriendUID(FriendUID_), FriendState(FriendState_)
			{
			}
			SMasterBlockFriendInDb(TUID&& UID_, TUID&& FriendUID_, EFriendState&& FriendState_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SMasterUnBlockFriendInDb : public SProto
		{
			TUID UID{};
			TUID FriendUID{};
			EFriendState FriendState{};
			SMasterUnBlockFriendInDb()
			{
			}
			SMasterUnBlockFriendInDb(const TUID& UID_, const TUID& FriendUID_, const EFriendState& FriendState_) : UID(UID_), FriendUID(FriendUID_), FriendState(FriendState_)
			{
			}
			SMasterUnBlockFriendInDb(TUID&& UID_, TUID&& FriendUID_, EFriendState&& FriendState_) : UID(std::move(UID_)), FriendUID(std::move(FriendUID_)), FriendState(std::move(FriendState_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> FriendUID;
				Stream_ >> FriendState;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["FriendUID"] >> FriendUID;
				Value_["FriendState"] >> FriendState;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << FriendUID;
				Stream_ << FriendState;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["FriendUID"] = FriendUID;
				Value_["FriendState"] = FriendState;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(EFriendState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TUID(), L"FriendUID") + L"," + 
					GetMemberName(EFriendState(), L"FriendState");
			}
		};
		struct SMasterChangeStateInDb : public SProto
		{
			TUID UID{};
			TState State{};
			SMasterChangeStateInDb()
			{
			}
			SMasterChangeStateInDb(const TUID& UID_, const TState& State_) : UID(UID_), State(State_)
			{
			}
			SMasterChangeStateInDb(TUID&& UID_, TState&& State_) : UID(std::move(UID_)), State(std::move(State_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> State;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["State"] >> State;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << State;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["State"] = State;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TState());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TState(), L"State");
			}
		};
		struct SMasterCheckInDb : public SProto
		{
			TUID UID{};
			SMasterCheckInDb()
			{
			}
			SMasterCheckInDb(const TUID& UID_) : UID(UID_)
			{
			}
			SMasterCheckInDb(TUID&& UID_) : UID(std::move(UID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID");
			}
		};
		struct SMasterCheckOutDb : public SProto
		{
			list<system_clock::time_point> PunishEndTimes{};
			SMasterCheckOutDb()
			{
			}
			SMasterCheckOutDb(const list<system_clock::time_point>& PunishEndTimes_) : PunishEndTimes(PunishEndTimes_)
			{
			}
			SMasterCheckOutDb(list<system_clock::time_point>&& PunishEndTimes_) : PunishEndTimes(std::move(PunishEndTimes_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> PunishEndTimes;
			}
			void operator << (const Value& Value_) override
			{
				Value_["PunishEndTimes"] >> PunishEndTimes;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << PunishEndTimes;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["PunishEndTimes"] = PunishEndTimes;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<system_clock::time_point>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<system_clock::time_point>(), L"PunishEndTimes");
			}
		};
		struct SMasterPunishInDb : public SProto
		{
			TUID UID{};
			system_clock::time_point EndTime{};
			SMasterPunishInDb()
			{
			}
			SMasterPunishInDb(const TUID& UID_, const system_clock::time_point& EndTime_) : UID(UID_), EndTime(EndTime_)
			{
			}
			SMasterPunishInDb(TUID&& UID_, system_clock::time_point&& EndTime_) : UID(std::move(UID_)), EndTime(std::move(EndTime_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> EndTime;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["EndTime"] >> EndTime;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << EndTime;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["EndTime"] = EndTime;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(system_clock::time_point());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(system_clock::time_point(), L"EndTime");
			}
		};
		struct SMasterPunishOutDb : public SProto
		{
			void operator << (CStream&) override
			{
			}
			void operator << (const Value&) override
			{
			}
			void operator >> (CStream&) const override
			{
			}
			void operator >> (Value&) const override
			{
			}
			static wstring StdName(void)
			{
				return L"";
			}
			static wstring MemberName(void)
			{
				return L"";
			}
		};
		struct SLoginInfo : public SProto
		{
			TUID UID{};
			TID ID{};
			SLoginInfo()
			{
			}
			SLoginInfo(const TUID& UID_, const TID& ID_) : UID(UID_), ID(ID_)
			{
			}
			SLoginInfo(TUID&& UID_, TID&& ID_) : UID(std::move(UID_)), ID(std::move(ID_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> ID;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["ID"] >> ID;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << ID;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["ID"] = ID;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUID()) + L"," + 
					GetStdName(TID());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(TID(), L"ID");
			}
		};
	}
}
