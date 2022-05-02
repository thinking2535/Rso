#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

namespace rso
{
	namespace mobileutil
	{
		struct SUserLoadAllDBIn : public SProto
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
		struct SUserDB : public SProto
		{
			int64 UID{};
			string Token{};
			bool CanPushAtNight{};
			int32 MinuteOffset{};
			SUserDB()
			{
			}
			SUserDB(const int64& UID_, const string& Token_, const bool& CanPushAtNight_, const int32& MinuteOffset_) : UID(UID_), Token(Token_), CanPushAtNight(CanPushAtNight_), MinuteOffset(MinuteOffset_)
			{
			}
			SUserDB(int64&& UID_, string&& Token_, bool&& CanPushAtNight_, int32&& MinuteOffset_) : UID(std::move(UID_)), Token(std::move(Token_)), CanPushAtNight(std::move(CanPushAtNight_)), MinuteOffset(std::move(MinuteOffset_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Token;
				Stream_ >> CanPushAtNight;
				Stream_ >> MinuteOffset;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Token"] >> Token;
				Value_["CanPushAtNight"] >> CanPushAtNight;
				Value_["MinuteOffset"] >> MinuteOffset;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Token;
				Stream_ << CanPushAtNight;
				Stream_ << MinuteOffset;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Token"] = Token;
				Value_["CanPushAtNight"] = CanPushAtNight;
				Value_["MinuteOffset"] = MinuteOffset;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(bool()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"UID") + L"," + 
					GetMemberName(string(), L"Token") + L"," + 
					GetMemberName(bool(), L"CanPushAtNight") + L"," + 
					GetMemberName(int32(), L"MinuteOffset");
			}
		};
		struct SUserLoadAllDBOut : public SProto
		{
			list<SUserDB> Users{};
			SUserLoadAllDBOut()
			{
			}
			SUserLoadAllDBOut(const list<SUserDB>& Users_) : Users(Users_)
			{
			}
			SUserLoadAllDBOut(list<SUserDB>&& Users_) : Users(std::move(Users_))
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
					GetStdName(list<SUserDB>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SUserDB>(), L"Users");
			}
		};
		struct SUserInsertDBIn : public SUserDB
		{
			SUserInsertDBIn()
			{
			}
			SUserInsertDBIn(const SUserDB& Super_) : SUserDB(Super_)
			{
			}
			SUserInsertDBIn(SUserDB&& Super_) : SUserDB(std::move(Super_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SUserDB::operator << (Stream_);
			}
			void operator << (const Value& Value_) override
			{
				SUserDB::operator << (Value_);
			}
			void operator >> (CStream& Stream_) const override
			{
				SUserDB::operator >> (Stream_);
			}
			void operator >> (Value& Value_) const override
			{
				SUserDB::operator >> (Value_);
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SUserDB());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SUserDB(), L"");
			}
		};
		struct SUserChangeTokenDBIn : public SProto
		{
			int64 UID{};
			string Token{};
			SUserChangeTokenDBIn()
			{
			}
			SUserChangeTokenDBIn(const int64& UID_, const string& Token_) : UID(UID_), Token(Token_)
			{
			}
			SUserChangeTokenDBIn(int64&& UID_, string&& Token_) : UID(std::move(UID_)), Token(std::move(Token_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Token;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Token"] >> Token;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Token;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Token"] = Token;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(string());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"UID") + L"," + 
					GetMemberName(string(), L"Token");
			}
		};
		struct SUserChangeUIDDBIn : public SProto
		{
			int64 UID{};
			string Token{};
			SUserChangeUIDDBIn()
			{
			}
			SUserChangeUIDDBIn(const int64& UID_, const string& Token_) : UID(UID_), Token(Token_)
			{
			}
			SUserChangeUIDDBIn(int64&& UID_, string&& Token_) : UID(std::move(UID_)), Token(std::move(Token_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> Token;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["Token"] >> Token;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << Token;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["Token"] = Token;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(string());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"UID") + L"," + 
					GetMemberName(string(), L"Token");
			}
		};
		struct SUserDelUIDChangeTokenDBIn : public SProto
		{
			int64 DelUID{};
			int64 UID{};
			string Token{};
			int32 MinuteOffset{};
			SUserDelUIDChangeTokenDBIn()
			{
			}
			SUserDelUIDChangeTokenDBIn(const int64& DelUID_, const int64& UID_, const string& Token_, const int32& MinuteOffset_) : DelUID(DelUID_), UID(UID_), Token(Token_), MinuteOffset(MinuteOffset_)
			{
			}
			SUserDelUIDChangeTokenDBIn(int64&& DelUID_, int64&& UID_, string&& Token_, int32&& MinuteOffset_) : DelUID(std::move(DelUID_)), UID(std::move(UID_)), Token(std::move(Token_)), MinuteOffset(std::move(MinuteOffset_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> DelUID;
				Stream_ >> UID;
				Stream_ >> Token;
				Stream_ >> MinuteOffset;
			}
			void operator << (const Value& Value_) override
			{
				Value_["DelUID"] >> DelUID;
				Value_["UID"] >> UID;
				Value_["Token"] >> Token;
				Value_["MinuteOffset"] >> MinuteOffset;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << DelUID;
				Stream_ << UID;
				Stream_ << Token;
				Stream_ << MinuteOffset;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["DelUID"] = DelUID;
				Value_["UID"] = UID;
				Value_["Token"] = Token;
				Value_["MinuteOffset"] = MinuteOffset;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(int64()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(int32());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"DelUID") + L"," + 
					GetMemberName(int64(), L"UID") + L"," + 
					GetMemberName(string(), L"Token") + L"," + 
					GetMemberName(int32(), L"MinuteOffset");
			}
		};
		struct SUserDelDBIn : public SProto
		{
			int64 UID{};
			SUserDelDBIn()
			{
			}
			SUserDelDBIn(const int64& UID_) : UID(UID_)
			{
			}
			SUserDelDBIn(int64&& UID_) : UID(std::move(UID_))
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
					GetStdName(int64());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"UID");
			}
		};
		struct SUserChangeDBIn : public SProto
		{
			int64 UID{};
			bool CanPushAtNight{};
			SUserChangeDBIn()
			{
			}
			SUserChangeDBIn(const int64& UID_, const bool& CanPushAtNight_) : UID(UID_), CanPushAtNight(CanPushAtNight_)
			{
			}
			SUserChangeDBIn(int64&& UID_, bool&& CanPushAtNight_) : UID(std::move(UID_)), CanPushAtNight(std::move(CanPushAtNight_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> UID;
				Stream_ >> CanPushAtNight;
			}
			void operator << (const Value& Value_) override
			{
				Value_["UID"] >> UID;
				Value_["CanPushAtNight"] >> CanPushAtNight;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << UID;
				Stream_ << CanPushAtNight;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["UID"] = UID;
				Value_["CanPushAtNight"] = CanPushAtNight;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(int64()) + L"," + 
					GetStdName(bool());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(int64(), L"UID") + L"," + 
					GetMemberName(bool(), L"CanPushAtNight");
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
		struct SReceiptOption : public SProto
		{
			string OAuthCode{};
			string PackageName{};
			string ClientID{};
			string ClientSecret{};
			string RefreshToken{};
			SReceiptOption()
			{
			}
			SReceiptOption(const string& OAuthCode_, const string& PackageName_, const string& ClientID_, const string& ClientSecret_, const string& RefreshToken_) : OAuthCode(OAuthCode_), PackageName(PackageName_), ClientID(ClientID_), ClientSecret(ClientSecret_), RefreshToken(RefreshToken_)
			{
			}
			SReceiptOption(string&& OAuthCode_, string&& PackageName_, string&& ClientID_, string&& ClientSecret_, string&& RefreshToken_) : OAuthCode(std::move(OAuthCode_)), PackageName(std::move(PackageName_)), ClientID(std::move(ClientID_)), ClientSecret(std::move(ClientSecret_)), RefreshToken(std::move(RefreshToken_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> OAuthCode;
				Stream_ >> PackageName;
				Stream_ >> ClientID;
				Stream_ >> ClientSecret;
				Stream_ >> RefreshToken;
			}
			void operator << (const Value& Value_) override
			{
				Value_["OAuthCode"] >> OAuthCode;
				Value_["PackageName"] >> PackageName;
				Value_["ClientID"] >> ClientID;
				Value_["ClientSecret"] >> ClientSecret;
				Value_["RefreshToken"] >> RefreshToken;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << OAuthCode;
				Stream_ << PackageName;
				Stream_ << ClientID;
				Stream_ << ClientSecret;
				Stream_ << RefreshToken;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["OAuthCode"] = OAuthCode;
				Value_["PackageName"] = PackageName;
				Value_["ClientID"] = ClientID;
				Value_["ClientSecret"] = ClientSecret;
				Value_["RefreshToken"] = RefreshToken;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(string()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(string()) + L"," + 
					GetStdName(string());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(string(), L"OAuthCode") + L"," + 
					GetMemberName(string(), L"PackageName") + L"," + 
					GetMemberName(string(), L"ClientID") + L"," + 
					GetMemberName(string(), L"ClientSecret") + L"," + 
					GetMemberName(string(), L"RefreshToken");
			}
		};
	}
}
