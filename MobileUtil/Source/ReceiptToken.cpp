#include "ReceiptToken.h"

namespace rso
{
	namespace mobileutil
	{
		void CReceiptToken::_SetNextGetAccessTokenTime(int64 TokenLifeSeconds_)
		{
			_NextGetAccessTokenTime = system_clock::now() + seconds(TokenLifeSeconds_ - c_TokenPeriodErrorSeconds);
		}
		void CReceiptToken::_GetAccessToken(size_t CmdIndex_)
		{
			try
			{
				_Http.Push(SInObj(c_UserHeader, "accounts.google.com", 443, "", "", EMethod::Post, "/o/oauth2/token", _ParamsGetAccessToken, true), CmdIndex_);
			}
			catch (...)
			{
			}
		}
		void CReceiptToken::_GetRefreshTokenCallback(EHttpRet Ret_, const TBuffer& Buffer_)
		{
			if (Ret_ != EHttpRet::Ok)
				THROWEXA(L"_GetRefreshTokenCallback Fail Ret[%d]", (int32)Ret_);

			//{
			//  "access_token": "ya29.Glt0B-KUH5XyvRt3An46Dn0SROXzWEfA17Rgff7ecKMTCfythQIcyQ6XLrwwJXqWMsOr0HmbKZQ5_vYkE0aHoNX8i_mxmG_AxVl9UZvroUabOx6gcJyU49npb9ys",
			//  "expires_in" : 3600,
			//  "refresh_token" : "1/ZTLi5RcFVJ5TcQgnE6F4ZJ59B7AkqiiXFD_PSCfADlw",
			//  "scope" : "https://www.googleapis.com/auth/androidpublisher",
			//  "token_type" : "Bearer"
			//}

			string JsonString(Buffer_.begin(), Buffer_.end());
			CJson Json(JsonString);

			auto& RefreshToken = Json["refresh_token"];
			if (RefreshToken.isNull())
				throw SException(JsonString);

			auto& AccessToken = Json["access_token"];
			if (AccessToken.isNull())
				throw SException(JsonString);

			auto& ExpiresIn = Json["expires_in"];
			if (ExpiresIn.isNull())
				throw SException(JsonString);

			_Option->RefreshToken = RefreshToken.asString();
			_Option.Save();
			_AccessToken = AccessToken.asString();
			_SetNextGetAccessTokenTime(ExpiresIn.asInt64());
		}
		void CReceiptToken::_GetAccessTokenCallback(EHttpRet Ret_, const TBuffer& Buffer_)
		{
			//{
			//  "access_token": "ya29.Glt1B0xJU30TUr2WkRZTtfT3G1_jo1Z46SWjG8G-Clbx2F6U01nbfXS3kXpCZlM4DGwkCQHnhgHGj6YtRotP9MjlfssXYC8xsir_X-yswE5ySsM70BIP7w_FjGAJ",
			//  "expires_in": 3600,
			//  "scope": "https://www.googleapis.com/auth/androidpublisher",
			//  "token_type": "Bearer"
			//}

			if (Ret_ != EHttpRet::Ok)
				THROWEXA(L"_GetAccessTokenCallback Fail Ret[%d]", (int32)Ret_);

			string JsonString(Buffer_.begin(), Buffer_.end());
			CJson Json(JsonString);

			auto& AccessToken = Json["access_token"];
			if (AccessToken.isNull())
				throw SException(JsonString);

			auto& ExpiresIn = Json["expires_in"];
			if (ExpiresIn.isNull())
				throw SException(JsonString);

			_AccessToken = AccessToken.asString();
			_SetNextGetAccessTokenTime(ExpiresIn.asInt64());
		}
		void CReceiptToken::_ChangedCallback(size_t /*SessionIndex_*/, const string& /*ObjectName_*/, DWORD /*Received_*/, DWORD /*Total_*/, size_t /*CmdIndex_*/)
		{
		}
		void CReceiptToken::_CompletedCallback(size_t /*SessionIndex_*/, EHttpRet Ret_, const string& /*ObjectName_*/, const TBuffer& Buffer_, size_t CmdIndex_)
		{
			switch (CmdIndex_)
			{
			case 0:
			{
				_GetRefreshTokenCallback(Ret_, Buffer_);
				break;
			}

			case 1: // 생성자에서 호출한 Get AccesToken 쿼리 
			{
				_GetAccessTokenCallback(Ret_, Buffer_);
				break;
			}

			case 2: // Proc 에서 호출한 Get AccesToken 쿼리 
			{
				try
				{
					_GetAccessTokenCallback(Ret_, Buffer_);
					_ReceiveAccessTokenFunc(_AccessToken);
				}
				catch (const string& Msg_)
				{
					_NextGetAccessTokenTime = system_clock::now() + c_RefreshErrorSleepDuration;
					_ReceiveAccessTokenFailFunc(Msg_);
				}
				catch (...)
				{
					_NextGetAccessTokenTime = system_clock::now() + c_RefreshErrorSleepDuration;
					_ReceiveAccessTokenFailFunc("ReceiveAccessTokenFail");
				}
				_WaitingAccessToken = false;
				break;
			}
			}
		}
		CReceiptToken::CReceiptToken(size_t SessionCount_, const wstring& OptionFileName_, TReceiveAccessTokenFunc ReceiveAccessTokenFunc_, TReceiveAccessTokenFailFunc ReceiveAccessTokenFailFunc_) :
			_Option(OptionFileName_, true),
			_Http(SessionCount_, std::bind(&CReceiptToken::_ChangedCallback, this, _1, _2, _3, _4, _5), std::bind(&CReceiptToken::_CompletedCallback, this, _1, _2, _3, _4, _5)),
			_ReceiveAccessTokenFunc(ReceiveAccessTokenFunc_),
			_ReceiveAccessTokenFailFunc(ReceiveAccessTokenFailFunc_)
		{
			if (_Option->RefreshToken.empty())
			{
				string Token;
				Token += ("code=" + _Option->OAuthCode);
				Token += ("&client_id=" + _Option->ClientID);
				Token += ("&client_secret=" + _Option->ClientSecret);
				Token += ("&redirect_uri=" + c_RedirectURI);
				Token += "&grant_type=authorization_code";

				_Http.Push(SInObj(c_UserHeader, "accounts.google.com", 443, "", "", EMethod::Post, "/o/oauth2/token", Token, true), 0);

				while (_Option->RefreshToken.empty())
				{
					_Http.Proc();
					this_thread::sleep_for(milliseconds(500));
				}
			}

			_ParamsGetAccessToken += ("&client_id=" + _Option->ClientID);
			_ParamsGetAccessToken += ("&client_secret=" + _Option->ClientSecret);
			_ParamsGetAccessToken += ("&refresh_token=" + _Option->RefreshToken);
			_ParamsGetAccessToken += "&grant_type=refresh_token";

			_GetAccessToken(1);

			while (_AccessToken.empty())
			{
				_Http.Proc();
				this_thread::sleep_for(milliseconds(500));
			}
		}
		void CReceiptToken::Proc(void)
		{
			_Http.Proc();

			if (!_WaitingAccessToken && system_clock::now() > _NextGetAccessTokenTime)
			{
				_GetAccessToken(2);
				_WaitingAccessToken = true;
			}
		}
	}
}
