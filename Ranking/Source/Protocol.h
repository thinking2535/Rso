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
	namespace ranking
	{
		using namespace net;
		enum class EProtoDB
		{
			ConfigLoad,
			Refresh,
			Update,
		};
		struct SOption : public SProto
		{
			SNamePort MasterNamePort{};
			int32 NetCThreadCnt{};
			TPort ChildBindPort{};
			TPort ClientBindPort{};
			SNamePort ClientBindNamePortPub{};
			SOption()
			{
			}
			SOption(const SNamePort& MasterNamePort_, const int32& NetCThreadCnt_, const TPort& ChildBindPort_, const TPort& ClientBindPort_, const SNamePort& ClientBindNamePortPub_) : MasterNamePort(MasterNamePort_), NetCThreadCnt(NetCThreadCnt_), ChildBindPort(ChildBindPort_), ClientBindPort(ClientBindPort_), ClientBindNamePortPub(ClientBindNamePortPub_)
			{
			}
			SOption(SNamePort&& MasterNamePort_, int32&& NetCThreadCnt_, TPort&& ChildBindPort_, TPort&& ClientBindPort_, SNamePort&& ClientBindNamePortPub_) : MasterNamePort(std::move(MasterNamePort_)), NetCThreadCnt(std::move(NetCThreadCnt_)), ChildBindPort(std::move(ChildBindPort_)), ClientBindPort(std::move(ClientBindPort_)), ClientBindNamePortPub(std::move(ClientBindNamePortPub_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MasterNamePort;
				Stream_ >> NetCThreadCnt;
				Stream_ >> ChildBindPort;
				Stream_ >> ClientBindPort;
				Stream_ >> ClientBindNamePortPub;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MasterNamePort"] >> MasterNamePort;
				Value_["NetCThreadCnt"] >> NetCThreadCnt;
				Value_["ChildBindPort"] >> ChildBindPort;
				Value_["ClientBindPort"] >> ClientBindPort;
				Value_["ClientBindNamePortPub"] >> ClientBindNamePortPub;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MasterNamePort;
				Stream_ << NetCThreadCnt;
				Stream_ << ChildBindPort;
				Stream_ << ClientBindPort;
				Stream_ << ClientBindNamePortPub;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MasterNamePort"] = MasterNamePort;
				Value_["NetCThreadCnt"] = NetCThreadCnt;
				Value_["ChildBindPort"] = ChildBindPort;
				Value_["ClientBindPort"] = ClientBindPort;
				Value_["ClientBindNamePortPub"] = ClientBindNamePortPub;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SNamePort()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(TPort()) + L"," + 
					GetStdName(TPort()) + L"," + 
					GetStdName(SNamePort());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SNamePort(), L"MasterNamePort") + L"," + 
					GetMemberName(int32(), L"NetCThreadCnt") + L"," + 
					GetMemberName(TPort(), L"ChildBindPort") + L"," + 
					GetMemberName(TPort(), L"ClientBindPort") + L"," + 
					GetMemberName(SNamePort(), L"ClientBindNamePortPub");
			}
		};
		struct SMasterOption : public SProto
		{
			TPort AgentBindPort{};
			int64 MaxRankingPoint{};
			int32 MaxDataCount{};
			string PeriodBaseTime{};
			int32 PeriodMinutes{};
			SMasterOption()
			{
			}
			SMasterOption(const TPort& AgentBindPort_, const int64& MaxRankingPoint_, const int32& MaxDataCount_, const string& PeriodBaseTime_, const int32& PeriodMinutes_) : AgentBindPort(AgentBindPort_), MaxRankingPoint(MaxRankingPoint_), MaxDataCount(MaxDataCount_), PeriodBaseTime(PeriodBaseTime_), PeriodMinutes(PeriodMinutes_)
			{
			}
			SMasterOption(TPort&& AgentBindPort_, int64&& MaxRankingPoint_, int32&& MaxDataCount_, string&& PeriodBaseTime_, int32&& PeriodMinutes_) : AgentBindPort(std::move(AgentBindPort_)), MaxRankingPoint(std::move(MaxRankingPoint_)), MaxDataCount(std::move(MaxDataCount_)), PeriodBaseTime(std::move(PeriodBaseTime_)), PeriodMinutes(std::move(PeriodMinutes_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> AgentBindPort;
				Stream_ >> MaxRankingPoint;
				Stream_ >> MaxDataCount;
				Stream_ >> PeriodBaseTime;
				Stream_ >> PeriodMinutes;
			}
			void operator << (const Value& Value_) override
			{
				Value_["AgentBindPort"] >> AgentBindPort;
				Value_["MaxRankingPoint"] >> MaxRankingPoint;
				Value_["MaxDataCount"] >> MaxDataCount;
				Value_["PeriodBaseTime"] >> PeriodBaseTime;
				Value_["PeriodMinutes"] >> PeriodMinutes;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << AgentBindPort;
				Stream_ << MaxRankingPoint;
				Stream_ << MaxDataCount;
				Stream_ << PeriodBaseTime;
				Stream_ << PeriodMinutes;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["AgentBindPort"] = AgentBindPort;
				Value_["MaxRankingPoint"] = MaxRankingPoint;
				Value_["MaxDataCount"] = MaxDataCount;
				Value_["PeriodBaseTime"] = PeriodBaseTime;
				Value_["PeriodMinutes"] = PeriodMinutes;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TPort()) + L"," + 
					GetStdName(int64()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TPort(), L"AgentBindPort") + L"," + 
					GetMemberName(int64(), L"MaxRankingPoint") + L"," + 
					GetMemberName(int32(), L"MaxDataCount") + L"," + 
					GetMemberName(string(), L"PeriodBaseTime") + L"," + 
					GetMemberName(int32(), L"PeriodMinutes");
			}
		};
		enum class EProto
		{
			InfoSa,
			UserMinPointSa,
			UpdateAs,
			RequestCs,
			RequestSc,
		};
		using TRewards = map<TUID,int64>;
		struct SUserMinPointSa : public SProto
		{
			int64 Point{};
			SUserMinPointSa()
			{
			}
			SUserMinPointSa(const int64& Point_) : Point(Point_)
			{
			}
			SUserMinPointSa(int64&& Point_) : Point(std::move(Point_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Point;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Point"] >> Point;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Point;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Point"] = Point;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"Point");
			}
		};
		struct SInfoSa : public SProto
		{
			int64 Counter{};
			SUserMinPointSa UserMinPoint{};
			TRewards Rewards{};
			SInfoSa()
			{
			}
			SInfoSa(const int64& Counter_, const SUserMinPointSa& UserMinPoint_, const TRewards& Rewards_) : Counter(Counter_), UserMinPoint(UserMinPoint_), Rewards(Rewards_)
			{
			}
			SInfoSa(int64&& Counter_, SUserMinPointSa&& UserMinPoint_, TRewards&& Rewards_) : Counter(std::move(Counter_)), UserMinPoint(std::move(UserMinPoint_)), Rewards(std::move(Rewards_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Counter;
				Stream_ >> UserMinPoint;
				Stream_ >> Rewards;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Counter"] >> Counter;
				Value_["UserMinPoint"] >> UserMinPoint;
				Value_["Rewards"] >> Rewards;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Counter;
				Stream_ << UserMinPoint;
				Stream_ << Rewards;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Counter"] = Counter;
				Value_["UserMinPoint"] = UserMinPoint;
				Value_["Rewards"] = Rewards;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(SUserMinPointSa()) + L"," + 
					GetStdName(TRewards());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"Counter") + L"," + 
					GetMemberName(SUserMinPointSa(), L"UserMinPoint") + L"," + 
					GetMemberName(TRewards(), L"Rewards");
			}
		};
		struct SUserCore : public SProto
		{
			wstring Nick{};
			int32 CharCode{};
			wstring CountryCode{};
			SUserCore()
			{
			}
			SUserCore(const wstring& Nick_, const int32& CharCode_, const wstring& CountryCode_) : Nick(Nick_), CharCode(CharCode_), CountryCode(CountryCode_)
			{
			}
			SUserCore(wstring&& Nick_, int32&& CharCode_, wstring&& CountryCode_) : Nick(std::move(Nick_)), CharCode(std::move(CharCode_)), CountryCode(std::move(CountryCode_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Nick;
				Stream_ >> CharCode;
				Stream_ >> CountryCode;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Nick"] >> Nick;
				Value_["CharCode"] >> CharCode;
				Value_["CountryCode"] >> CountryCode;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Nick;
				Stream_ << CharCode;
				Stream_ << CountryCode;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Nick"] = Nick;
				Value_["CharCode"] = CharCode;
				Value_["CountryCode"] = CountryCode;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Nick") + L"," + 
					GetMemberName(int32(), L"CharCode") + L"," + 
					GetMemberName(wstring(), L"CountryCode");
			}
		};
		struct SUser : public SUserCore
		{
			TUID UID{};
			int32 Point{};
			SUser()
			{
			}
			SUser(const SUserCore& Super_, const TUID& UID_, const int32& Point_) : SUserCore(Super_), UID(UID_), Point(Point_)
			{
			}
			SUser(SUserCore&& Super_, TUID&& UID_, int32&& Point_) : SUserCore(std::move(Super_)), UID(std::move(UID_)), Point(std::move(Point_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SUserCore::operator << (Stream_);
				Stream_ >> UID;
				Stream_ >> Point;
			}
			void operator << (const Value& Value_) override
			{
				SUserCore::operator << (Value_);
				Value_["UID"] >> UID;
				Value_["Point"] >> Point;
			}
			void operator >> (CStream& Stream_) const override
			{
				SUserCore::operator >> (Stream_);
				Stream_ << UID;
				Stream_ << Point;
			}
			void operator >> (Value& Value_) const override
			{
				SUserCore::operator >> (Value_);
				Value_["UID"] = UID;
				Value_["Point"] = Point;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SUserCore()) + L"," + 
					GetStdName(TUID()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SUserCore(), L"") + L"," + 
					GetMemberName(TUID(), L"UID") + L"," + 
					GetMemberName(int32(), L"Point");
			}
		};
		using TUsers = list<SUser>;
		struct SUpdateAs : public SProto
		{
			TUsers Changed{};
			SUpdateAs()
			{
			}
			SUpdateAs(const TUsers& Changed_) : Changed(Changed_)
			{
			}
			SUpdateAs(TUsers&& Changed_) : Changed(std::move(Changed_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Changed;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Changed"] >> Changed;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Changed;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Changed"] = Changed;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUsers());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUsers(), L"Changed");
			}
		};
		struct SRequestCs : public SProto
		{
			TUID UID{};
			SRequestCs()
			{
			}
			SRequestCs(const TUID& UID_) : UID(UID_)
			{
			}
			SRequestCs(TUID&& UID_) : UID(std::move(UID_))
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
		struct SRanking : public SProto
		{
			TUsers Users{};
			int32 RewardedRanking{};
			SRanking()
			{
			}
			SRanking(const TUsers& Users_, const int32& RewardedRanking_) : Users(Users_), RewardedRanking(RewardedRanking_)
			{
			}
			SRanking(TUsers&& Users_, int32&& RewardedRanking_) : Users(std::move(Users_)), RewardedRanking(std::move(RewardedRanking_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Users;
				Stream_ >> RewardedRanking;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Users"] >> Users;
				Value_["RewardedRanking"] >> RewardedRanking;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Users;
				Stream_ << RewardedRanking;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Users"] = Users;
				Value_["RewardedRanking"] = RewardedRanking;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUsers()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUsers(), L"Users") + L"," + 
					GetMemberName(int32(), L"RewardedRanking");
			}
		};
		struct SDummyDBOut : public SProto
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
		struct SConfigLoadDBIn : public SProto
		{
			system_clock::time_point StartTimePoint{};
			SConfigLoadDBIn()
			{
			}
			SConfigLoadDBIn(const system_clock::time_point& StartTimePoint_) : StartTimePoint(StartTimePoint_)
			{
			}
			SConfigLoadDBIn(system_clock::time_point&& StartTimePoint_) : StartTimePoint(std::move(StartTimePoint_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> StartTimePoint;
			}
			void operator << (const Value& Value_) override
			{
				Value_["StartTimePoint"] >> StartTimePoint;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << StartTimePoint;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["StartTimePoint"] = StartTimePoint;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(system_clock::time_point());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(system_clock::time_point(), L"StartTimePoint");
			}
		};
		struct SConfigDB : public SProto
		{
			int64 Counter{};
			system_clock::time_point ExpireTimePoint{};
			SConfigDB()
			{
			}
			SConfigDB(const int64& Counter_, const system_clock::time_point& ExpireTimePoint_) : Counter(Counter_), ExpireTimePoint(ExpireTimePoint_)
			{
			}
			SConfigDB(int64&& Counter_, system_clock::time_point&& ExpireTimePoint_) : Counter(std::move(Counter_)), ExpireTimePoint(std::move(ExpireTimePoint_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Counter;
				Stream_ >> ExpireTimePoint;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Counter"] >> Counter;
				Value_["ExpireTimePoint"] >> ExpireTimePoint;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Counter;
				Stream_ << ExpireTimePoint;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Counter"] = Counter;
				Value_["ExpireTimePoint"] = ExpireTimePoint;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(system_clock::time_point());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"Counter") + L"," + 
					GetMemberName(system_clock::time_point(), L"ExpireTimePoint");
			}
		};
		struct SConfigLoadDBOut : public SProto
		{
			list<SConfigDB> Configs{};
			SConfigLoadDBOut()
			{
			}
			SConfigLoadDBOut(const list<SConfigDB>& Configs_) : Configs(Configs_)
			{
			}
			SConfigLoadDBOut(list<SConfigDB>&& Configs_) : Configs(std::move(Configs_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Configs;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Configs"] >> Configs;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Configs;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Configs"] = Configs;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SConfigDB>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SConfigDB>(), L"Configs");
			}
		};
		struct SLoadDBIn : public SProto
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
		struct SLoadDBOut : public SProto
		{
			list<SUser> Users{};
			SLoadDBOut()
			{
			}
			SLoadDBOut(const list<SUser>& Users_) : Users(Users_)
			{
			}
			SLoadDBOut(list<SUser>&& Users_) : Users(std::move(Users_))
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
					GetStdName(list<SUser>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SUser>(), L"Users");
			}
		};
		struct SRefreshDBIn : public SProto
		{
			SConfigDB Config{};
			TRewards Rewards{};
			SRefreshDBIn()
			{
			}
			SRefreshDBIn(const SConfigDB& Config_, const TRewards& Rewards_) : Config(Config_), Rewards(Rewards_)
			{
			}
			SRefreshDBIn(SConfigDB&& Config_, TRewards&& Rewards_) : Config(std::move(Config_)), Rewards(std::move(Rewards_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Config;
				Stream_ >> Rewards;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Config"] >> Config;
				Value_["Rewards"] >> Rewards;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Config;
				Stream_ << Rewards;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Config"] = Config;
				Value_["Rewards"] = Rewards;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SConfigDB()) + L"," + 
					GetStdName(TRewards());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SConfigDB(), L"Config") + L"," + 
					GetMemberName(TRewards(), L"Rewards");
			}
		};
		struct SRewardLoadDBIn : public SProto
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
		struct SRewardLoadDBOut : public SProto
		{
			TRewards Rewards{};
			SRewardLoadDBOut()
			{
			}
			SRewardLoadDBOut(const TRewards& Rewards_) : Rewards(Rewards_)
			{
			}
			SRewardLoadDBOut(TRewards&& Rewards_) : Rewards(std::move(Rewards_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Rewards;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Rewards"] >> Rewards;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Rewards;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Rewards"] = Rewards;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TRewards());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TRewards(), L"Rewards");
			}
		};
		struct SUpdateDBIn : public SProto
		{
			TUsers Merged{};
			list<TUID> Deleted{};
			SUpdateDBIn()
			{
			}
			SUpdateDBIn(const TUsers& Merged_, const list<TUID>& Deleted_) : Merged(Merged_), Deleted(Deleted_)
			{
			}
			SUpdateDBIn(TUsers&& Merged_, list<TUID>&& Deleted_) : Merged(std::move(Merged_)), Deleted(std::move(Deleted_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Merged;
				Stream_ >> Deleted;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Merged"] >> Merged;
				Value_["Deleted"] >> Deleted;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Merged;
				Stream_ << Deleted;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Merged"] = Merged;
				Value_["Deleted"] = Deleted;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TUsers()) + L"," + 
					GetStdName(list<TUID>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TUsers(), L"Merged") + L"," + 
					GetMemberName(list<TUID>(), L"Deleted");
			}
		};
	}
}
