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
	namespace mssql
	{
		using namespace std;
		using namespace net;
		struct SConnInfo : public SProto
		{
			wstring ServerName{};
			wstring UserName{};
			wstring PassWord{};
			wstring DataBase{};
			TPort Port{};
			uint32 QueryTimeOut{};
			SConnInfo()
			{
			}
			SConnInfo(const wstring& ServerName_, const wstring& UserName_, const wstring& PassWord_, const wstring& DataBase_, const TPort& Port_, const uint32& QueryTimeOut_) : ServerName(ServerName_), UserName(UserName_), PassWord(PassWord_), DataBase(DataBase_), Port(Port_), QueryTimeOut(QueryTimeOut_)
			{
			}
			SConnInfo(wstring&& ServerName_, wstring&& UserName_, wstring&& PassWord_, wstring&& DataBase_, TPort&& Port_, uint32&& QueryTimeOut_) : ServerName(std::move(ServerName_)), UserName(std::move(UserName_)), PassWord(std::move(PassWord_)), DataBase(std::move(DataBase_)), Port(std::move(Port_)), QueryTimeOut(std::move(QueryTimeOut_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> ServerName;
				Stream_ >> UserName;
				Stream_ >> PassWord;
				Stream_ >> DataBase;
				Stream_ >> Port;
				Stream_ >> QueryTimeOut;
			}
			void operator << (const Value& Value_) override
			{
				Value_["ServerName"] >> ServerName;
				Value_["UserName"] >> UserName;
				Value_["PassWord"] >> PassWord;
				Value_["DataBase"] >> DataBase;
				Value_["Port"] >> Port;
				Value_["QueryTimeOut"] >> QueryTimeOut;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << ServerName;
				Stream_ << UserName;
				Stream_ << PassWord;
				Stream_ << DataBase;
				Stream_ << Port;
				Stream_ << QueryTimeOut;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["ServerName"] = ServerName;
				Value_["UserName"] = UserName;
				Value_["PassWord"] = PassWord;
				Value_["DataBase"] = DataBase;
				Value_["Port"] = Port;
				Value_["QueryTimeOut"] = QueryTimeOut;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring()) + L"," + 
					GetStdName(wstring()) + L"," + 
					GetStdName(TPort()) + L"," + 
					GetStdName(uint32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(wstring(), L"ServerName") + L"," + 
					GetMemberName(wstring(), L"UserName") + L"," + 
					GetMemberName(wstring(), L"PassWord") + L"," + 
					GetMemberName(wstring(), L"DataBase") + L"," + 
					GetMemberName(TPort(), L"Port") + L"," + 
					GetMemberName(uint32(), L"QueryTimeOut");
			}
		};
		struct SDBOption : public SProto
		{
			int32 LoginTimeOut{};
			int32 ConnTimeOut{};
			SConnInfo ConnInfo{};
			SDBOption()
			{
			}
			SDBOption(const int32& LoginTimeOut_, const int32& ConnTimeOut_, const SConnInfo& ConnInfo_) : LoginTimeOut(LoginTimeOut_), ConnTimeOut(ConnTimeOut_), ConnInfo(ConnInfo_)
			{
			}
			SDBOption(int32&& LoginTimeOut_, int32&& ConnTimeOut_, SConnInfo&& ConnInfo_) : LoginTimeOut(std::move(LoginTimeOut_)), ConnTimeOut(std::move(ConnTimeOut_)), ConnInfo(std::move(ConnInfo_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> LoginTimeOut;
				Stream_ >> ConnTimeOut;
				Stream_ >> ConnInfo;
			}
			void operator << (const Value& Value_) override
			{
				Value_["LoginTimeOut"] >> LoginTimeOut;
				Value_["ConnTimeOut"] >> ConnTimeOut;
				Value_["ConnInfo"] >> ConnInfo;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << LoginTimeOut;
				Stream_ << ConnTimeOut;
				Stream_ << ConnInfo;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["LoginTimeOut"] = LoginTimeOut;
				Value_["ConnTimeOut"] = ConnTimeOut;
				Value_["ConnInfo"] = ConnInfo;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int32()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(SConnInfo());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int32(), L"LoginTimeOut") + L"," + 
					GetMemberName(int32(), L"ConnTimeOut") + L"," + 
					GetMemberName(SConnInfo(), L"ConnInfo");
			}
		};
	}
}
