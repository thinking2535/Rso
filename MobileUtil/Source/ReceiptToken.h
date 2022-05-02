#pragma once

#include "Base.h"
#include "Protocol.h"
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Http/Http.h>

namespace rso
{
	namespace mobileutil
	{
		using namespace gameutil;
		using namespace http;

		class CReceiptToken // Hub에서 구글과 통신하여 RefreshToken, AccessToken 을 받음
		{
		public:
			using TReceiveAccessTokenFunc = function<void(const string& AccessToken_)>;
			using TReceiveAccessTokenFailFunc = function<void(const string& Msg_)>;

		protected:
			const int64 c_TokenPeriodErrorSeconds = 60;
			const seconds c_RefreshErrorSleepDuration{ 60 };
			const string c_RedirectURI = "urn:ietf:wg:oauth:2.0:oob";

			COptionJson<SReceiptOption> _Option;
			CHttp<size_t> _Http;
			string _AccessToken; // 구글과 통신하여 받음
			bool _WaitingAccessToken = false; // 생성자가 아닌 Proc에서 _NextGetAccessTokenTime 가 갱신되지 않아 다음번 Proc에서 또 쿼리 날리지 않도록 하기 위함
			TReceiveAccessTokenFunc _ReceiveAccessTokenFunc; // Proc 에 의해서 호출
			TReceiveAccessTokenFailFunc _ReceiveAccessTokenFailFunc; // Proc 에 의해서 호출
			string _ParamsGetAccessToken;
			system_clock::time_point _NextGetAccessTokenTime; // ExpireTime 보다 이른시각

			void _SetNextGetAccessTokenTime(int64 TokenLifeSeconds_);
			void _GetAccessToken(size_t CmdIndex_);
			void _GetRefreshTokenCallback(EHttpRet Ret_, const TBuffer& Buffer_);
			void _GetAccessTokenCallback(EHttpRet Ret_, const TBuffer& Buffer_);
			void _ChangedCallback(size_t SessionIndex_, const string& ObjectName_, DWORD Received_, DWORD Total_, size_t CmdIndex_);
			void _CompletedCallback(size_t SessionIndex_, EHttpRet Ret_, const string& ObjectName_, const TBuffer& Buffer_, size_t CmdIndex_);

		public:
			CReceiptToken(size_t SessionCount_, const wstring& OptionFileName_, TReceiveAccessTokenFunc ReceiveAccessTokenFunc_, TReceiveAccessTokenFailFunc ReceiveAccessTokenFailFunc_);
			void Proc(void);
			const SReceiptOption& GetOption(void) const { return *_Option; }
			const string& GetAccessToken(void) const { return _AccessToken; }
		};
	}
}
