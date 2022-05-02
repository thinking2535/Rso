#pragma once

#include "Base.h"
#include "MsgQue.h"
#include <list>
#include <Rso/Core/Core.h>
#include <Rso/Base/CycleMap.h>
#include <Rso/Json/Json.h>
#include <Rso/Http/Http.h>
#include <Rso/MSSQL/StoredProcedure.h>
#include <Rso/MSSQL/BulkSelect.h>
#include "Protocol.h"

namespace rso
{
	namespace mobileutil
	{
		using namespace std;
		using namespace core;
		using namespace base;
		using namespace json;
		using namespace http;
		using namespace mssql;

		class CFCMServer
		{
			struct _SUser
			{
				string Token;
				bool CanPushAtNight = false;
				minutes MinuteOffset{};

				_SUser(const string& Token_) :
					Token(Token_)
				{
				}
				_SUser(const string& Token_, bool CanPushAtNight_, const minutes& MinuteOffset_) :
					Token(Token_), CanPushAtNight(CanPushAtNight_), MinuteOffset(MinuteOffset_)
				{
				}
				bool CanPush(int64 NightStartTime_, int64 NightEndTime_) const
				{
					auto TimePart = GetTimePart(CDateTime(steady_now(), MinuteOffset).ToTimePoint());
					bool InTime = (NightStartTime_ < NightEndTime_ ? (TimePart >= NightStartTime_ && TimePart < NightEndTime_) : (TimePart >= NightStartTime_ || TimePart < NightEndTime_));
					return !(!CanPushAtNight && InTime);
				}
			};
			using _TSP = CStoredProcedure<TUID>;
			using _TOutObject = typename _TSP::SOutObject;
			using _TUsers = CCycleMap<TUID, _SUser>;
			using _TTokens = map<string, _TUsers::iterator>;

			static const int32 c_TokenLen_Max = 200;
			static const int32 c_SendRIDCnt_Max = 1000; // 동시에 보낼 수 있는 유저수
			static const int32 c_CCT_Max = 100; // FCM 매뉴얼에 따르면 HTTP 콜백이 없이 동시에 보낼 수 있는 메시지 수는 100 개라 함.
			static_assert(c_SendRIDCnt_Max > 0 && c_CCT_Max > 0, "Invalid c_SendRIDCnt_Max or c_CCT_Max");

			int64 _NightStartTime = GetTimePart(20, 0, 0);
			int64 _NightEndTime = GetTimePart(9, 0, 0);
			CHttp<> _Http;
			string _ServerKey;
			string _UserHeader;
			Value _JsonMsgAll;
			Value _JsonMsgOne;
			_TUsers _Users;
			_TTokens _Tokens;
			
			CMsgQue _MsgQue;
			int32 _CCT = 0;
			_TSP _DB;

			void _DBCallback(_TOutObject& OutObject_);
			void _SetUser(const _TUsers::key_type& UserKey_, const _SUser& User_); // throw
			void _SetUserHeader(void);
			void _ChangedCallback(size_t SessionIndex_, const string& ObjectName_, DWORD Received_, DWORD Total_);
			void _CompletedCallback(size_t SessionIndex_, EHttpRet Ret_, const string& ObjectName_, const TBuffer& Buffer_);
			void _PushHttp(const string& Params_);
			void _PushMany(void);
			inline bool _Full(void) const { return (_CCT >= c_CCT_Max); }
			inline bool _CanPush(_TUsers::iterator itUser_) const { return itUser_->second.CanPush(_NightStartTime, _NightEndTime); }

		public:
			CFCMServer(const SDBOption& DBOption_, size_t HttpSessionCnt_, size_t MsgQueSize_, const string& ServerKey_);
			void SetServerKey(const string& ServerKey_);
			void SetUser(const _TUsers::key_type& UserKey_, const string& Token_, const minutes& MinuteOffset_); // throw
			void DelUser(const _TUsers::key_type& UserKey_);
			void ChangeUser(const _TUsers::key_type& UserKey_, bool CanPushAtNight_);
			void PushOne(TUID UID_, const string& Title_, const string& Msg_); // throw
			void PushOne(const string& RID_, const string& Title_, const string& Msg_); // throw
			void PushAll(const string& Title_, const string& Msg_); // throw
			void Proc(void);
			inline int32 GetCCT(void) const { return _CCT; }
		};
	}
}
