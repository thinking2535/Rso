#include "stdafx.h"
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/MobileUtil/ReceiptToken.h>
#include <Rso/MobileUtil/ReceiptCheck.h>
#include <Rso/MobileUtil/Billing.h>
#include <Rso/MobileUtil/FCMServer.h>
#include <Rso/Base/Period.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace rso;
using namespace base;
using namespace gameutil;
using namespace mobileutil;
using namespace chrono;

unique_ptr<CReceiptCheck> g_ReceiptCheck;

void ReceiptCheckCallback(const CReceiptCheck::SOrder& Order_, const string& OrderID_, int64 PurchaseTime_, int32 PurchaseType_, int32 ConsumptionState_)
{
	cout << "OrderID:" << OrderID_ << " PurchaseTime:" << PurchaseTime_ << " ConsumptionState:" << ConsumptionState_ << endl;
}
void ReceiptErrorCallback(const CReceiptCheck::SOrder& Order_, int32 PurchaseState_)
{
	cout << "Error PurchaseState:" << PurchaseState_ << endl;
}

void ReceiptAccessTokenCallback(const string& AccessToken_)
{
	g_ReceiptCheck->SetAccessToken(AccessToken_);
	cout << "ReceiptAccessTokenCallback : " << AccessToken_ << endl;
}
void ReceiptAccessTokenFailCallback(const string& Msg_)
{
	cout << "ReceiptAccessTokenFailCallback : " << Msg_ << endl;
}

CPeriod<milliseconds> g_Period{ milliseconds(1000) };

int _tmain(int argc, _TCHAR* argv[])
{
#if true // Receipt

	try
	{
		CReceiptToken Token(1, L"OptionReceipt.ini", ReceiptAccessTokenCallback, ReceiptAccessTokenFailCallback);
		g_ReceiptCheck.reset(new CReceiptCheck(4, ReceiptCheckCallback, ReceiptErrorCallback, "com.stairgames.balloonstars", Token.GetAccessToken()));
		cout << "Get access_token : " << Token.GetAccessToken() << endl;

		while (true)
		{
			Token.Proc();

			if (g_ReceiptCheck)
			{
				if (g_Period.CheckAndNextLoose())
					g_ReceiptCheck->Check(EOS::Android, CReceiptCheck::SOrder(0, "aniballoon_ruby_1", "ccnolmpnpbgmpamllceejnae.AO-J1Oz5DfKZmydvDm7E_AHalYz2zt-kX-zjIzsDiKHeYnbvRLOwWBZVx3HJwWg4M0MyRoa_5IxHqxUoGYf4xhMILVFfjYDdoYjQ_dol5jSNWQJ2mFJTtwXSG7gryRtZU7o-z5sygrM6"));

				g_ReceiptCheck->Proc();
			}

			this_thread::sleep_for(milliseconds(100));
		}
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;

#elif false // verify

	const int c_i = 10000;
	int sum = 0;

	// CBilling g("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgoEbfIU5XEDkNQwptasLTmp1pZH58ndCY3kXJyRQIV4oDGukk2xhNuEy1RBaiKEPGLejYxclM+jB/6kWH6k7Sbm6iAK/ky19thJb3DpzfgjGWQGVuPvXbIpx5h7HxSCjIhBhFI54J8xTvcGNpO/zpkQmuaNboGUpcne9QdjxhPbVPUJu/YlrVOCvwvUKt40hy9cz1t6QwHuHnR0knzLxrX6qoDMHsUnUFWHHnfipNiWgAiRrcmBHE77FAWQghLjuqwora1Vqp1/Tax4VbJGLaqrPv/AC9nQ7yKndXSBonrspEFPbw+jF7MxLW8FGUMP2qZsWbD0ngx8inlqju8ccgQIDAQAB");
	CBilling g("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmwPh8J+rKCpNI9u3rfvxfo2w3C8OwGAA29xENq1SJ86ctmALYcojXmrLl5PuI1jsqjiJc3j9MzSqj/r3v9gQhasDqu39rXw8tqZ6YdH5vGNuppKdJyIHEl5qr9tv4tgO0EFO3EsapL/2cR+NHALNBarGFIEAbf2r1FmXgp5UZDSMLdqKZaTLFYCgBp1DV8XWwT5E9wGE5I3m2ic0V75A+EFAG+RJ9Lwtu0hDztr9DWKfwQhgVXIsHGlwzGJqMbPWl07Wyk4UTeqhraiX01qUHF2JCxu8lgDf6ugMlq+qt+4zqQ4dGoM7pAmj7AzWiXSgrlWstu1hszkv2AZ3TzqoKwIDAQAB");

	auto s = system_clock::now();
	for (int i = 0; i < c_i; ++i)
	{
		auto Ret = g.Verify_GoogleInappBilling_Signature("{\"orderId\":\"12999763169054705758.1315160756665859\",\"packageName\":\"com.actoz.skyko\",\"productId\":\"gg_diamond_10\",\"purchaseTime\":1378894906000,\"purchaseState\":0,\"purchaseToken\":\"nyvikghcqkxhzkgyrohpnxjb.AO-J1OzyV1G_BCA9vQmJCnh4evjLem1_jVdjvkVzeLwsBP9RSNqZ57qsM1KqCwQ2JDSplCHdzTDQ9HurTf5xQU5ud9rX2o9RysMYLD1bT1fHtAbcV7ZRERc\"}",
			"BpMJUSzo0QZ5ELekWayYGl2Xn8dnP1NUCQ1u3gbGxOPFvESfOR3jpXk/d+YL8WiNuidyzN+8ZbSdk7H0Jeq6Vwo63jPSOAvzsjglqBU6ojTs6FqJn3Th9QsSA6H/wGJb84U2OpNavge1VsExbwpa6/gEsUxtwoEJi3BUsnwff48uEesZbutiKw7qMSD7wO13psElCBrzCLuE1ql0Dy0NPq2Dib/5jc/bjgG2dkHG9rHXA8gkyhjdQlwKsYCTtlfjLK9T+XAtPycA8h9OrmOyj7QunvsuQ2g+opWMHYWyAXAefxcy2PV99aw0fixlK6vZSix3n/5e6JKhdcKGXUz8Ew==");

		//auto Ret = g.Verify_GoogleInappBilling_Signature(
		//	//"{\"orderId\":\"GPA.3349-2211-4013-85747\",\"packageName\":\"com.sharp.madplanets\",\"productId\":\"test_topaz\",\"purchaseTime\":1566898283721,\"purchaseState\":0,\"developerPayload\":\"{\"developerPayload\":\"\",\"is_free_trial\":false,\"has_introductory_price_trial\":false,\"is_updated\":false,\"accountId\":\"\"}\",\"purchaseToken\":\"idphlkbicciajjodgdicbmof.AO-J1OwfVLEaYB7SNdEnQvobSkInHpoLDwxCFNqG891OWpuhcWga2dPj1MUDsGiE7AVpHS_m6gkNPYr1yy1Za9JgkUVNa4FaaoZDlqUVmXb6mVbM1ERRaPFzlQF_NJBLoX398PNdw9u1\"}",
		//	"{\"orderId\":\"GPA.3349-2211-4013-85747\",\"packageName\":\"com.sharp.madplanets\",\"productId\":\"test_topaz\",\"purchaseTime\":1566898283721,\"purchaseState\":0,\"purchaseToken\":\"idphlkbicciajjodgdicbmof.AO-J1OwfVLEaYB7SNdEnQvobSkInHpoLDwxCFNqG891OWpuhcWga2dPj1MUDsGiE7AVpHS_m6gkNPYr1yy1Za9JgkUVNa4FaaoZDlqUVmXb6mVbM1ERRaPFzlQF_NJBLoX398PNdw9u1\"}",
		//	"MjqerDw2MGrRyfc705YKu6D3obJkEb/odsouTzHH/P1e01GXRk53nEzcgT/dvSzsanI36+9oSjE0YvjGOrBzyPcdNxrsd0KJQqFweLPwWSQ4FkWMPVWMAkt3lhkiNS52ZtRFfNtWLoY1YCb+im9CrEeEpDniBdhY3cFG4gp/nciU5cLmFdjIpWAIOR7DYLwopgAq/OiTrWHg31U2VLIJ51iRxpYPs4e0T2YWfjWXIb5iy4tREO5fcivTvx0sh83X6KXYRJK9NFOGdh6mq+V6rvchqaL8wqM7T3WUdRB/pAF19pP37w/em+MBWwTuTt0MXFvZV75uNLsP7CjLUIqIiQ==");

		if (Ret == 1)
			++sum;
	}
	auto e = system_clock::now();
	cout << duration_cast<milliseconds>(e - s).count() << " " << sum << endl;

#else

	COptionJson<SDBOption> DBOption(L"DBOption.ini", false);
	string UserToken = "c6x3oM1rQ7k:APA91bG5IbsS1byyW9y4c0fbjRfXpduvsjNEgjJ_cBywquuJRk31qnRlbocilIvaRBr11w2mo7pKRe1hVoCU4Cok1Ijvcq79u2oI_Mq2NbmL7qOjhi1pUdVixWEB7bguyiQsGQENeT1K"; // sone app
	CFCMServer fcm(*DBOption, 1, 1000, "AAAAJ4HPOQo:APA91bHls-_pJsz5We8jUCa3glFB_E4Qr0DNyGkS2i4H7ZA7wEFGUisFvpIgNzdUmTvwSLVFD1KSQg6M44G4BSMsBbFIOhjMQTZJ_WSScS_kCnuQd3pnLz-o2rULPtN3tLrp45ve-NNT"); // sone app

//	fcm.SetUser(1, "epnmy99vZbo:APA91bFW8adv1-AuhfREJf-PnGFP1qwfIDO-98vDr7gCg0LlKfj09yhAIng4fXjzDWThLjFL0bozWkp0KD3w9_Be5sgaqyLqNbQ_LsOLu7jGw5Ftx-4QstgHTNm3enTokENsPB_RxQmK");
	fcm.SetUser(2, "a", minutes(0));

	wstring wstr = L"1234567890..........";
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	string strUTF8;
	strUTF8.resize(nLen);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), (LPSTR)strUTF8.data(), nLen, NULL, NULL);


//	fcm.PushAll(strUTF8, "1");
	fcm.PushAll("ok", "1");
		//	fcm.PushOne(UserToken, "Msg");

		//FCM client
		//	클라는 무조건 받고, 서버가 보내준 값이 다르면 갱신?
		//	아니면 클라가 무조건 전송?

	while (true)
	{
		fcm.Proc();
		this_thread::sleep_for(milliseconds(100));
	}

#endif

	return 0;
	}
