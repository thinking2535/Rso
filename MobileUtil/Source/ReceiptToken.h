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

		class CReceiptToken // Hub���� ���۰� ����Ͽ� RefreshToken, AccessToken �� ����
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
			string _AccessToken; // ���۰� ����Ͽ� ����
			bool _WaitingAccessToken = false; // �����ڰ� �ƴ� Proc���� _NextGetAccessTokenTime �� ���ŵ��� �ʾ� ������ Proc���� �� ���� ������ �ʵ��� �ϱ� ����
			TReceiveAccessTokenFunc _ReceiveAccessTokenFunc; // Proc �� ���ؼ� ȣ��
			TReceiveAccessTokenFailFunc _ReceiveAccessTokenFailFunc; // Proc �� ���ؼ� ȣ��
			string _ParamsGetAccessToken;
			system_clock::time_point _NextGetAccessTokenTime; // ExpireTime ���� �̸��ð�

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
