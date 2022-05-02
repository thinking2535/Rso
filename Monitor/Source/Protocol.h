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
	namespace monitor
	{
		using namespace net;
		enum class ECsProto
		{
			Login,
			UserProto,
			ToAgent,
			Max,
			Nul,
		};
		struct SCsHeader : public SProto
		{
			ECsProto Proto{};
			list<SKey> AgentKeys{};
			SCsHeader()
			{
			}
			SCsHeader(const ECsProto& Proto_, const list<SKey>& AgentKeys_) : Proto(Proto_), AgentKeys(AgentKeys_)
			{
			}
			SCsHeader(ECsProto&& Proto_, list<SKey>&& AgentKeys_) : Proto(std::move(Proto_)), AgentKeys(std::move(AgentKeys_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Proto;
				Stream_ >> AgentKeys;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Proto"] >> Proto;
				Value_["AgentKeys"] >> AgentKeys;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Proto;
				Stream_ << AgentKeys;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Proto"] = Proto;
				Value_["AgentKeys"] = AgentKeys;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(ECsProto()) + L"," + 
					GetStdName(list<SKey>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(ECsProto(), L"Proto") + L"," + 
					GetMemberName(list<SKey>(), L"AgentKeys");
			}
		};
		struct SCsLogin : public SProto
		{
			wstring ID{};
			wstring PW{};
			SCsLogin()
			{
			}
			SCsLogin(const wstring& ID_, const wstring& PW_) : ID(ID_), PW(PW_)
			{
			}
			SCsLogin(wstring&& ID_, wstring&& PW_) : ID(std::move(ID_)), PW(std::move(PW_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ID;
				Stream_ >> PW;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ID"] >> ID;
				Value_["PW"] >> PW;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ID;
				Stream_ << PW;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ID"] = ID;
				Value_["PW"] = PW;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"ID") + L"," + 
					GetMemberName(wstring(), L"PW");
			}
		};
		enum class EScProto
		{
			AgentOn,
			AgentOff,
			ProcOn,
			ProcOff,
			AgentInfos,
			AgentOption,
			AgentStat,
			ProcStat,
			Notify,
			Max,
			Null,
		};
		struct SAgentOption : public SProto
		{
			bool KeepAlive{};
			SAgentOption()
			{
			}
			SAgentOption(const bool& KeepAlive_) : KeepAlive(KeepAlive_)
			{
			}
			SAgentOption(bool&& KeepAlive_) : KeepAlive(std::move(KeepAlive_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> KeepAlive;
			}
			void operator << (const Value& Value_) override
			{
				Value_["KeepAlive"] >> KeepAlive;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << KeepAlive;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["KeepAlive"] = KeepAlive;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(bool());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(bool(), L"KeepAlive");
			}
		};
		struct SAgent : public SProto
		{
			wstring Name{};
			SAgentOption Option{};
			wstring Stat{};
			SAgent()
			{
			}
			SAgent(const wstring& Name_, const SAgentOption& Option_, const wstring& Stat_) : Name(Name_), Option(Option_), Stat(Stat_)
			{
			}
			SAgent(wstring&& Name_, SAgentOption&& Option_, wstring&& Stat_) : Name(std::move(Name_)), Option(std::move(Option_)), Stat(std::move(Stat_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Name;
				Stream_ >> Option;
				Stream_ >> Stat;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Name"] >> Name;
				Value_["Option"] >> Option;
				Value_["Stat"] >> Stat;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Name;
				Stream_ << Option;
				Stream_ << Stat;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Name"] = Name;
				Value_["Option"] = Option;
				Value_["Stat"] = Stat;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(SAgentOption()) + L"," + 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Name") + L"," + 
					GetMemberName(SAgentOption(), L"Option") + L"," + 
					GetMemberName(wstring(), L"Stat");
			}
		};
		struct SProc : public SProto
		{
			wstring Name{};
			wstring Stat{};
			SProc()
			{
			}
			SProc(const wstring& Name_, const wstring& Stat_) : Name(Name_), Stat(Stat_)
			{
			}
			SProc(wstring&& Name_, wstring&& Stat_) : Name(std::move(Name_)), Stat(std::move(Stat_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Name;
				Stream_ >> Stat;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Name"] >> Name;
				Value_["Stat"] >> Stat;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Name;
				Stream_ << Stat;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Name"] = Name;
				Value_["Stat"] = Stat;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Name") + L"," + 
					GetMemberName(wstring(), L"Stat");
			}
		};
		struct SAgentInfo : public SProto
		{
			SKey Key{};
			SAgent Agent{};
			SProc Proc{};
			SAgentInfo()
			{
			}
			SAgentInfo(const SKey& Key_, const SAgent& Agent_, const SProc& Proc_) : Key(Key_), Agent(Agent_), Proc(Proc_)
			{
			}
			SAgentInfo(SKey&& Key_, SAgent&& Agent_, SProc&& Proc_) : Key(std::move(Key_)), Agent(std::move(Agent_)), Proc(std::move(Proc_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Key;
				Stream_ >> Agent;
				Stream_ >> Proc;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Key"] >> Key;
				Value_["Agent"] >> Agent;
				Value_["Proc"] >> Proc;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Key;
				Stream_ << Agent;
				Stream_ << Proc;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Key"] = Key;
				Value_["Agent"] = Agent;
				Value_["Proc"] = Proc;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(SAgent()) + L"," + 
					GetStdName(SProc());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"Key") + L"," + 
					GetMemberName(SAgent(), L"Agent") + L"," + 
					GetMemberName(SProc(), L"Proc");
			}
		};
		struct SScHeader : public SProto
		{
			EScProto Proto{};
			SScHeader()
			{
			}
			SScHeader(const EScProto& Proto_) : Proto(Proto_)
			{
			}
			SScHeader(EScProto&& Proto_) : Proto(std::move(Proto_))
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
					GetStdName(EScProto());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EScProto(), L"Proto");
			}
		};
		struct SScAgentOn : public SAgent
		{
			SKey Key{};
			SScAgentOn()
			{
			}
			SScAgentOn(const SAgent& Super_, const SKey& Key_) : SAgent(Super_), Key(Key_)
			{
			}
			SScAgentOn(SAgent&& Super_, SKey&& Key_) : SAgent(std::move(Super_)), Key(std::move(Key_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SAgent::operator << (Stream_);
				Stream_ >> Key;
			}
			void operator << (const Value& Value_) override
			{
				SAgent::operator << (Value_);
				Value_["Key"] >> Key;
			}
			void operator >> (CStream& Stream_) const override
			{
				SAgent::operator >> (Stream_);
				Stream_ << Key;
			}
			void operator >> (Value& Value_) const override
			{
				SAgent::operator >> (Value_);
				Value_["Key"] = Key;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SAgent()) + L"," + 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SAgent(), L"") + L"," + 
					GetMemberName(SKey(), L"Key");
			}
		};
		struct SScAgentOff : public SProto
		{
			SKey Key{};
			SScAgentOff()
			{
			}
			SScAgentOff(const SKey& Key_) : Key(Key_)
			{
			}
			SScAgentOff(SKey&& Key_) : Key(std::move(Key_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Key;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Key"] >> Key;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Key;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Key"] = Key;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"Key");
			}
		};
		struct SScProcOn : public SProto
		{
			SKey Key{};
			SProc Proc{};
			SScProcOn()
			{
			}
			SScProcOn(const SKey& Key_, const SProc& Proc_) : Key(Key_), Proc(Proc_)
			{
			}
			SScProcOn(SKey&& Key_, SProc&& Proc_) : Key(std::move(Key_)), Proc(std::move(Proc_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Key;
				Stream_ >> Proc;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Key"] >> Key;
				Value_["Proc"] >> Proc;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Key;
				Stream_ << Proc;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Key"] = Key;
				Value_["Proc"] = Proc;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(SProc());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"Key") + L"," + 
					GetMemberName(SProc(), L"Proc");
			}
		};
		struct SScProcOff : public SProto
		{
			SKey Key{};
			SScProcOff()
			{
			}
			SScProcOff(const SKey& Key_) : Key(Key_)
			{
			}
			SScProcOff(SKey&& Key_) : Key(std::move(Key_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Key;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Key"] >> Key;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Key;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Key"] = Key;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"Key");
			}
		};
		struct SScAgentInfos : public SProto
		{
			list<SAgentInfo> Infos{};
			SScAgentInfos()
			{
			}
			SScAgentInfos(const list<SAgentInfo>& Infos_) : Infos(Infos_)
			{
			}
			SScAgentInfos(list<SAgentInfo>&& Infos_) : Infos(std::move(Infos_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Infos;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Infos"] >> Infos;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Infos;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Infos"] = Infos;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SAgentInfo>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SAgentInfo>(), L"Infos");
			}
		};
		struct SKeyData : public SProto
		{
			wstring Key{};
			wstring Data{};
			SKeyData()
			{
			}
			SKeyData(const wstring& Key_, const wstring& Data_) : Key(Key_), Data(Data_)
			{
			}
			SKeyData(wstring&& Key_, wstring&& Data_) : Key(std::move(Key_)), Data(std::move(Data_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Key;
				Stream_ >> Data;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Key"] >> Key;
				Value_["Data"] >> Data;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Key;
				Stream_ << Data;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Key"] = Key;
				Value_["Data"] = Data;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Key") + L"," + 
					GetMemberName(wstring(), L"Data");
			}
		};
		struct SScAgentOption : public SProto
		{
			SKey AgentKey{};
			SAgentOption Option{};
			SScAgentOption()
			{
			}
			SScAgentOption(const SKey& AgentKey_, const SAgentOption& Option_) : AgentKey(AgentKey_), Option(Option_)
			{
			}
			SScAgentOption(SKey&& AgentKey_, SAgentOption&& Option_) : AgentKey(std::move(AgentKey_)), Option(std::move(Option_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> AgentKey;
				Stream_ >> Option;
			}
			void operator << (const Value& Value_) override
			{
				Value_["AgentKey"] >> AgentKey;
				Value_["Option"] >> Option;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << AgentKey;
				Stream_ << Option;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["AgentKey"] = AgentKey;
				Value_["Option"] = Option;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(SAgentOption());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"AgentKey") + L"," + 
					GetMemberName(SAgentOption(), L"Option");
			}
		};
		struct SScAgentStat : public SProto
		{
			SKey AgentKey{};
			SKeyData KeyData{};
			SScAgentStat()
			{
			}
			SScAgentStat(const SKey& AgentKey_, const SKeyData& KeyData_) : AgentKey(AgentKey_), KeyData(KeyData_)
			{
			}
			SScAgentStat(SKey&& AgentKey_, SKeyData&& KeyData_) : AgentKey(std::move(AgentKey_)), KeyData(std::move(KeyData_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> AgentKey;
				Stream_ >> KeyData;
			}
			void operator << (const Value& Value_) override
			{
				Value_["AgentKey"] >> AgentKey;
				Value_["KeyData"] >> KeyData;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << AgentKey;
				Stream_ << KeyData;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["AgentKey"] = AgentKey;
				Value_["KeyData"] = KeyData;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(SKeyData());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"AgentKey") + L"," + 
					GetMemberName(SKeyData(), L"KeyData");
			}
		};
		struct SScProcStat : public SProto
		{
			SKey AgentKey{};
			SKeyData KeyData{};
			SScProcStat()
			{
			}
			SScProcStat(const SKey& AgentKey_, const SKeyData& KeyData_) : AgentKey(AgentKey_), KeyData(KeyData_)
			{
			}
			SScProcStat(SKey&& AgentKey_, SKeyData&& KeyData_) : AgentKey(std::move(AgentKey_)), KeyData(std::move(KeyData_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> AgentKey;
				Stream_ >> KeyData;
			}
			void operator << (const Value& Value_) override
			{
				Value_["AgentKey"] >> AgentKey;
				Value_["KeyData"] >> KeyData;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << AgentKey;
				Stream_ << KeyData;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["AgentKey"] = AgentKey;
				Value_["KeyData"] = KeyData;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(SKeyData());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"AgentKey") + L"," + 
					GetMemberName(SKeyData(), L"KeyData");
			}
		};
		struct SScNotify : public SProto
		{
			wstring Msg{};
			SScNotify()
			{
			}
			SScNotify(const wstring& Msg_) : Msg(Msg_)
			{
			}
			SScNotify(wstring&& Msg_) : Msg(std::move(Msg_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Msg;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Msg"] >> Msg;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Msg;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Msg"] = Msg;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Msg");
			}
		};
		enum class ESaProto
		{
			FileSend,
			KeepAlive,
			RunProcess,
			KillProcess,
			ShellCommand,
			UserProto,
			ToApp,
			Max,
			Null,
		};
		struct SSaHeader : public SProto
		{
			ESaProto Proto{};
			SSaHeader()
			{
			}
			SSaHeader(const ESaProto& Proto_) : Proto(Proto_)
			{
			}
			SSaHeader(ESaProto&& Proto_) : Proto(std::move(Proto_))
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
					GetStdName(ESaProto());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(ESaProto(), L"Proto");
			}
		};
		struct SFileInfo : public SProto
		{
			wstring PathName{};
			CStream Stream{};
			SFileInfo()
			{
			}
			SFileInfo(const wstring& PathName_, const CStream& Stream_) : PathName(PathName_), Stream(Stream_)
			{
			}
			SFileInfo(wstring&& PathName_, CStream&& Stream_) : PathName(std::move(PathName_)), Stream(std::move(Stream_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> PathName;
				Stream_ >> Stream;
			}
			void operator << (const Value& Value_) override
			{
				Value_["PathName"] >> PathName;
				Value_["Stream"] >> Stream;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << PathName;
				Stream_ << Stream;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["PathName"] = PathName;
				Value_["Stream"] = Stream;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(CStream());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"PathName") + L"," + 
					GetMemberName(CStream(), L"Stream");
			}
		};
		struct SSaFileSend : public SProto
		{
			list<SFileInfo> Files{};
			SSaFileSend()
			{
			}
			SSaFileSend(const list<SFileInfo>& Files_) : Files(Files_)
			{
			}
			SSaFileSend(list<SFileInfo>&& Files_) : Files(std::move(Files_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Files;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Files"] >> Files;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Files;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Files"] = Files;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(list<SFileInfo>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SFileInfo>(), L"Files");
			}
		};
		struct SSaKeepAlive : public SProto
		{
			bool On{};
			SSaKeepAlive()
			{
			}
			SSaKeepAlive(const bool& On_) : On(On_)
			{
			}
			SSaKeepAlive(bool&& On_) : On(std::move(On_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> On;
			}
			void operator << (const Value& Value_) override
			{
				Value_["On"] >> On;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << On;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["On"] = On;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(bool());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(bool(), L"On");
			}
		};
		struct SSaRunProcess : public SProto
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
		struct SSaKillProcess : public SProto
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
		struct SSaShellCommand : public SProto
		{
			wstring Command{};
			SSaShellCommand()
			{
			}
			SSaShellCommand(const wstring& Command_) : Command(Command_)
			{
			}
			SSaShellCommand(wstring&& Command_) : Command(std::move(Command_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Command;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Command"] >> Command;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Command;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Command"] = Command;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Command");
			}
		};
		enum class EAsProto
		{
			AgentOn,
			ProcOn,
			ProcOff,
			AgentOption,
			AgentStat,
			ProcStat,
			NotifyToClient,
			Max,
			Null,
		};
		struct SAsHeader : public SProto
		{
			EAsProto Proto{};
			SAsHeader()
			{
			}
			SAsHeader(const EAsProto& Proto_) : Proto(Proto_)
			{
			}
			SAsHeader(EAsProto&& Proto_) : Proto(std::move(Proto_))
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
					GetStdName(EAsProto());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EAsProto(), L"Proto");
			}
		};
		struct SAsAgentOn : public SAgent
		{
			SAsAgentOn()
			{
			}
			SAsAgentOn(const SAgent& Super_) : SAgent(Super_)
			{
			}
			SAsAgentOn(SAgent&& Super_) : SAgent(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SAgent::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SAgent::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SAgent::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SAgent::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SAgent());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SAgent(), L"");
			}
		};
		struct SAsProcOn : public SProc
		{
			SAsProcOn()
			{
			}
			SAsProcOn(const SProc& Super_) : SProc(Super_)
			{
			}
			SAsProcOn(SProc&& Super_) : SProc(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SProc::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SProc::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SProc::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SProc::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SProc());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SProc(), L"");
			}
		};
		struct SAsProcOff : public SProto
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
		struct SAsAgentOption : public SProto
		{
			SAgentOption Option{};
			SAsAgentOption()
			{
			}
			SAsAgentOption(const SAgentOption& Option_) : Option(Option_)
			{
			}
			SAsAgentOption(SAgentOption&& Option_) : Option(std::move(Option_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Option;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Option"] >> Option;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Option;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Option"] = Option;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SAgentOption());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SAgentOption(), L"Option");
			}
		};
		struct SAsAgentStat : public SKeyData
		{
			SAsAgentStat()
			{
			}
			SAsAgentStat(const SKeyData& Super_) : SKeyData(Super_)
			{
			}
			SAsAgentStat(SKeyData&& Super_) : SKeyData(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SKeyData::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SKeyData::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SKeyData::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SKeyData::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKeyData());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKeyData(), L"");
			}
		};
		struct SAsProcStat : public SKeyData
		{
			SAsProcStat()
			{
			}
			SAsProcStat(const SKeyData& Super_) : SKeyData(Super_)
			{
			}
			SAsProcStat(SKeyData&& Super_) : SKeyData(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SKeyData::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SKeyData::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SKeyData::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SKeyData::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKeyData());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKeyData(), L"");
			}
		};
		enum class EApProto
		{
			Stop,
			Message,
			UserProto,
			Max,
			Null,
		};
		struct SApHeader : public SProto
		{
			EApProto Proto{};
			SApHeader()
			{
			}
			SApHeader(const EApProto& Proto_) : Proto(Proto_)
			{
			}
			SApHeader(EApProto&& Proto_) : Proto(std::move(Proto_))
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
					GetStdName(EApProto());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EApProto(), L"Proto");
			}
		};
		struct SApStop : public SProto
		{
			wstring Message{};
			int32 SecondLeft{};
			SApStop()
			{
			}
			SApStop(const wstring& Message_, const int32& SecondLeft_) : Message(Message_), SecondLeft(SecondLeft_)
			{
			}
			SApStop(wstring&& Message_, int32&& SecondLeft_) : Message(std::move(Message_)), SecondLeft(std::move(SecondLeft_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Message;
				Stream_ >> SecondLeft;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Message"] >> Message;
				Value_["SecondLeft"] >> SecondLeft;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Message;
				Stream_ << SecondLeft;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Message"] = Message;
				Value_["SecondLeft"] = SecondLeft;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Message") + L"," + 
					GetMemberName(int32(), L"SecondLeft");
			}
		};
		struct SApMessage : public SProto
		{
			wstring Message{};
			SApMessage()
			{
			}
			SApMessage(const wstring& Message_) : Message(Message_)
			{
			}
			SApMessage(wstring&& Message_) : Message(std::move(Message_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Message;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Message"] >> Message;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Message;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Message"] = Message;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"Message");
			}
		};
		enum class EPaProto
		{
			ProcOn,
			Stat,
			NotifyToClient,
			Max,
			Null,
		};
		struct SPaHeader : public SProto
		{
			EPaProto Proto{};
			SPaHeader()
			{
			}
			SPaHeader(const EPaProto& Proto_) : Proto(Proto_)
			{
			}
			SPaHeader(EPaProto&& Proto_) : Proto(std::move(Proto_))
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
					GetStdName(EPaProto());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(EPaProto(), L"Proto");
			}
		};
		struct SPaProcOn : public SProc
		{
			SPaProcOn()
			{
			}
			SPaProcOn(const SProc& Super_) : SProc(Super_)
			{
			}
			SPaProcOn(SProc&& Super_) : SProc(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SProc::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SProc::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SProc::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SProc::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SProc());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SProc(), L"");
			}
		};
		struct SPaStat : public SKeyData
		{
			SPaStat()
			{
			}
			SPaStat(const SKeyData& Super_) : SKeyData(Super_)
			{
			}
			SPaStat(SKeyData&& Super_) : SKeyData(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SKeyData::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SKeyData::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SKeyData::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SKeyData::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKeyData());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKeyData(), L"");
			}
		};
		struct SNotifyToClient : public SProto
		{
			SKey Key{};
			wstring Msg{};
			SNotifyToClient()
			{
			}
			SNotifyToClient(const SKey& Key_, const wstring& Msg_) : Key(Key_), Msg(Msg_)
			{
			}
			SNotifyToClient(SKey&& Key_, wstring&& Msg_) : Key(std::move(Key_)), Msg(std::move(Msg_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Key;
				Stream_ >> Msg;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Key"] >> Key;
				Value_["Msg"] >> Msg;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Key;
				Stream_ << Msg;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Key"] = Key;
				Value_["Msg"] = Msg;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SKey()) + L"," + 
					GetStdName(wstring());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SKey(), L"Key") + L"," + 
					GetMemberName(wstring(), L"Msg");
			}
		};
	}
}
