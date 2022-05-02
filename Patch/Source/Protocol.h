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
	using namespace net;
	namespace patch
	{
		enum class EProto
		{
			UdData,
			UdUpdate,
			CsLogin,
			ScPatchData,
			AmLogin,
			MaPatchData,
			AmUpdate,
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
		using TVersion = int32;
		using TUpdateFiles = map<wstring,bool>;
		struct SFile : public SProto
		{
			bool IsAdded{};
			TVersion SubVersion{};
			SFile()
			{
			}
			SFile(const bool& IsAdded_, const TVersion& SubVersion_) : IsAdded(IsAdded_), SubVersion(SubVersion_)
			{
			}
			SFile(bool&& IsAdded_, TVersion&& SubVersion_) : IsAdded(std::move(IsAdded_)), SubVersion(std::move(SubVersion_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> IsAdded;
				Stream_ >> SubVersion;
			}
			void operator << (const Value& Value_) override
			{
				Value_["IsAdded"] >> IsAdded;
				Value_["SubVersion"] >> SubVersion;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << IsAdded;
				Stream_ << SubVersion;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["IsAdded"] = IsAdded;
				Value_["SubVersion"] = SubVersion;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(bool()) + L"," + 
					GetStdName(TVersion());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(bool(), L"IsAdded") + L"," + 
					GetMemberName(TVersion(), L"SubVersion");
			}
		};
		using TFiles = map<wstring,SFile>;
		struct SFiles : public SProto
		{
			TFiles Files{};
			SFiles()
			{
			}
			SFiles(const TFiles& Files_) : Files(Files_)
			{
			}
			SFiles(TFiles&& Files_) : Files(std::move(Files_))
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
					GetStdName(TFiles());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TFiles(), L"Files");
			}
		};
		struct SServerData : public SProto
		{
			TVersion MainVersion{};
			vector<SFiles> Patches{};
			SServerData()
			{
			}
			SServerData(const TVersion& MainVersion_, const vector<SFiles>& Patches_) : MainVersion(MainVersion_), Patches(Patches_)
			{
			}
			SServerData(TVersion&& MainVersion_, vector<SFiles>&& Patches_) : MainVersion(std::move(MainVersion_)), Patches(std::move(Patches_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> MainVersion;
				Stream_ >> Patches;
			}
			void operator << (const Value& Value_) override
			{
				Value_["MainVersion"] >> MainVersion;
				Value_["Patches"] >> Patches;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << MainVersion;
				Stream_ << Patches;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["MainVersion"] = MainVersion;
				Value_["Patches"] = Patches;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TVersion()) + L"," + 
					GetStdName(vector<SFiles>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TVersion(), L"MainVersion") + L"," + 
					GetMemberName(vector<SFiles>(), L"Patches");
			}
		};
		struct SVersion : public SProto
		{
			TVersion Main{};
			TVersion Sub{};
			SVersion()
			{
			}
			SVersion(const TVersion& Main_, const TVersion& Sub_) : Main(Main_), Sub(Sub_)
			{
			}
			SVersion(TVersion&& Main_, TVersion&& Sub_) : Main(std::move(Main_)), Sub(std::move(Sub_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Main;
				Stream_ >> Sub;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Main"] >> Main;
				Value_["Sub"] >> Sub;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Main;
				Stream_ << Sub;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Main"] = Main;
				Value_["Sub"] = Sub;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(TVersion()) + L"," + 
					GetStdName(TVersion());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(TVersion(), L"Main") + L"," + 
					GetMemberName(TVersion(), L"Sub");
			}
		};
		struct SUpdateData : public SProto
		{
			bool IsReset{};
			TUpdateFiles Files{};
			SUpdateData()
			{
			}
			SUpdateData(const bool& IsReset_, const TUpdateFiles& Files_) : IsReset(IsReset_), Files(Files_)
			{
			}
			SUpdateData(bool&& IsReset_, TUpdateFiles&& Files_) : IsReset(std::move(IsReset_)), Files(std::move(Files_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> IsReset;
				Stream_ >> Files;
			}
			void operator << (const Value& Value_) override
			{
				Value_["IsReset"] >> IsReset;
				Value_["Files"] >> Files;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << IsReset;
				Stream_ << Files;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["IsReset"] = IsReset;
				Value_["Files"] = Files;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(bool()) + L"," + 
					GetStdName(TUpdateFiles());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(bool(), L"IsReset") + L"," + 
					GetMemberName(TUpdateFiles(), L"Files");
			}
		};
		struct SPatchData : public SProto
		{
			SVersion Version{};
			TFiles Files{};
			SPatchData()
			{
			}
			SPatchData(const SVersion& Version_, const TFiles& Files_) : Version(Version_), Files(Files_)
			{
			}
			SPatchData(SVersion&& Version_, TFiles&& Files_) : Version(std::move(Version_)), Files(std::move(Files_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Version;
				Stream_ >> Files;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Version"] >> Version;
				Value_["Files"] >> Files;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Version;
				Stream_ << Files;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Version"] = Version;
				Value_["Files"] = Files;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SVersion()) + L"," + 
					GetStdName(TFiles());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SVersion(), L"Version") + L"," + 
					GetMemberName(TFiles(), L"Files");
			}
		};
		struct SCsLogin : public SProto
		{
			SVersion Version{};
			SCsLogin()
			{
			}
			SCsLogin(const SVersion& Version_) : Version(Version_)
			{
			}
			SCsLogin(SVersion&& Version_) : Version(std::move(Version_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Version;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Version"] >> Version;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Version;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Version"] = Version;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SVersion());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SVersion(), L"Version");
			}
		};
		struct SAmLogin : public SProto
		{
			wstring ID{};
			wstring PW{};
			SVersion Version{};
			SAmLogin()
			{
			}
			SAmLogin(const wstring& ID_, const wstring& PW_, const SVersion& Version_) : ID(ID_), PW(PW_), Version(Version_)
			{
			}
			SAmLogin(wstring&& ID_, wstring&& PW_, SVersion&& Version_) : ID(std::move(ID_)), PW(std::move(PW_)), Version(std::move(Version_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ID;
				Stream_ >> PW;
				Stream_ >> Version;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ID"] >> ID;
				Value_["PW"] >> PW;
				Value_["Version"] >> Version;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ID;
				Stream_ << PW;
				Stream_ << Version;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ID"] = ID;
				Value_["PW"] = PW;
				Value_["Version"] = Version;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring()) + L"," + 
					GetStdName(SVersion());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"ID") + L"," + 
					GetMemberName(wstring(), L"PW") + L"," + 
					GetMemberName(SVersion(), L"Version");
			}
		};
	}
}
