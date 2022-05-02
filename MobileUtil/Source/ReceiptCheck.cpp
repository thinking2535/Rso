#include "ReceiptCheck.h"

namespace rso
{
	namespace mobileutil
	{
		bool CReceiptCheck::_Check(EOS OS_, const SOrder& Order_, bool Test_)
		{
			try
			{
				auto it = _Orders.emplace(Order_);

				try
				{
					_OSInfos[size_t(OS_)].PushFunc(_PackageName, Order_, Test_, it.Index());
				}
				catch (...)
				{
					_Orders.pop_back();
					throw;
				}
			}
			catch (...)
			{
				return false;
			}

			return true;
		}
		void CReceiptCheck::_ChangedCallback(size_t /*SessionIndex_*/, const string& /*ObjectName_*/, DWORD /*Received_*/, DWORD /*Total_*/, const EOS& /*OS_*/, const size_t& /*OrderIndex_*/)
		{
		}
		void CReceiptCheck::_CompletedCallback(size_t /*SessionIndex_*/, EHttpRet Ret_, const string& /*ObjectName_*/, const TBuffer& Buffer_, const EOS& OS_, const size_t& OrderIndex_)
		{
			auto itOrder = _Orders.get(OrderIndex_);

			try
			{
				if (Ret_ != EHttpRet::Ok)
					THROWEX();

				auto [OrderID, PurchaseTime, PurchaseType, ConsumptionState] = _OSInfos[size_t(OS_)].ParseFunc(CJson(string(Buffer_.begin(), Buffer_.end())), *itOrder);
				_CheckFunc(*itOrder, OrderID, PurchaseTime, PurchaseType, ConsumptionState);
			}
			catch (const _STestPush&)
			{
			}
			catch (int32 PurchaseState_)
			{
				_ErrorFunc(*itOrder, PurchaseState_);
			}
			catch (...)
			{
				_ErrorFunc(*itOrder, -1);
			}

			_Orders.erase(OrderIndex_);
		}
		CReceiptCheck::CReceiptCheck(size_t SessionCount_, TCheckFunc CheckFunc_, TErrorFunc ErrorFunc_, const string& PackageName_, const string& AccessToken_) :
			_OSInfos((size_t)EOS::Max),
			_Http(SessionCount_, std::bind(&CReceiptCheck::_ChangedCallback, this, _1, _2, _3, _4, _5, _6), std::bind(&CReceiptCheck::_CompletedCallback, this, _1, _2, _3, _4, _5, _6)),
			_CheckFunc(CheckFunc_),
			_ErrorFunc(ErrorFunc_),
			_PackageName(PackageName_),
			_AccessToken(AccessToken_)
		{
			_OSInfos[(size_t)EOS::Android] = _SOSInfo(
				[&](const string& PackageName_, const SOrder& Order_, bool /*Test_*/, size_t OrderIndex_)
				{
					string Object;
					Object += "/androidpublisher/v3";
					Object += ("/applications/" + PackageName_);
					Object += ("/purchases/products/" + Order_.ProductID);
					Object += ("/tokens/" + Order_.PurchaseToken);

					string Params;
					Params += ("access_token=" + _AccessToken);

					_Http.Push(SInObj(c_UserHeader, "www.googleapis.com", 443, "", "", EMethod::Get, Object, Params, true), EOS::Android, OrderIndex_);
				},
				[&](const CJson& Json_, const SOrder& /*Order_*/)
				{
					string OrderID;
					int64 PurchaseTime = 0;
					int32 PurchaseType = -1;
					int32 ConsumptionState = 0;

					//{
					// "kind": "androidpublisher#inappPurchase",
					// "purchaseTimeMillis": "1566895297219",
					// "purchaseState": 0,
					// "consumptionState": 1,
					// "developerPayload": "{\"developerPayload\":\"\",\"is_free_trial\":false,\"has_introductory_price_trial\":false,\"is_updated\":false,\"accountId\":\"\"}",
					// "orderId": "GPA.3327-0176-6561-33555",
					// "purchaseType": 0  // 이거 테스트 계정에서만 날라옴.
					//}

					// purchaseType : 0 Test, 1 Promo, 2 Rewarded
					// purchaseState : 0 Purchased, 1 Canceled, 2 Pending
					// consumptionState : 0 Yet to be consumed, 1 Consumed

					auto& PurchaseStateValue = Json_["purchaseState"];
					if (PurchaseStateValue.isNull())
						throw (-2);

					int32 PurchaseState = PurchaseStateValue.asInt();
					if (PurchaseState != 0)
						throw PurchaseState;

					auto& OrderIDValue = Json_["orderId"];
					if (OrderIDValue.isNull())
						throw (-3);

					auto& PurchaseTimeValue = Json_["purchaseTimeMillis"];
					if (PurchaseTimeValue.isNull())
						throw (-4);

					try
					{
						auto& PurchaseTypeValue = Json_["purchaseType"];
						if (!PurchaseTypeValue.isNull())
							PurchaseType = PurchaseTypeValue.asInt();
					}
					catch (...)
					{
					}

					auto& ConsumptionStateValue = Json_["consumptionState"];
					if (ConsumptionStateValue.isNull())
						throw (-5);

					OrderID = OrderIDValue.asString();
					PurchaseTime = stoll(PurchaseTimeValue.asString());
					ConsumptionState = ConsumptionStateValue.asInt();

					return _TParseResult(OrderID, PurchaseTime, PurchaseType, ConsumptionState);
				}
			);

			_OSInfos[(size_t)EOS::iOS] = _SOSInfo(
				[&](const string& /*PackageName_*/, const SOrder& Order_, bool Test_, size_t OrderIndex_)
				{
					_Http.Push(SInObj(c_UserHeader, Test_ ? "sandbox.itunes.apple.com" : "buy.itunes.apple.com", 443, "", "", EMethod::Post, "/verifyReceipt", Order_.PurchaseToken, true), EOS::iOS, OrderIndex_);
				},
				[&](const CJson& Json_, const SOrder& Order_)
				{
					string OrderID;
					int64 PurchaseTime = 0;
					int32 PurchaseType = 100; // 100 : Standard Bill
					int32 ConsumptionState = 0;

					//Status Code	Description
					//21000			The App Store could not read the JSON object you provided.
					//21002			The data in the receipt-data property was malformed or missing.
					//21003			The receipt could not be authenticated.
					//21004			The shared secret you provided does not match the shared secret on file for your account.
					//21005			The receipt server is not currently available.
					//21006			This receipt is valid but the subscription has expired. When this status code is returned to your server, the receipt data is also decoded and returned as part of the response.
					//				Only returned for iOS 6 style transaction receipts for auto-renewable subscriptions.
					//21007			This receipt is from the test environment, but it was sent to the production environment for verification. Send it to the test environment instead.
					//21008			This receipt is from the production environment, but it was sent to the test environment for verification. Send it to the production environment instead.
					//21010			This receipt could not be authorized. Treat this the same as if a purchase was never made.
					//21100-21199	Internal data access error.

					try // new type receipt
					{
						// new type receipt
						//{
						//	"receipt":
						//		{
						//			"receipt_type":"ProductionSandbox",
						//			"adam_id":0,
						//			"app_item_id":0,
						//			"bundle_id":"com.sharp.madplanets",
						//			"application_version":"10",
						//			"download_id":0,
						//			"version_external_identifier":0,
						//			"receipt_creation_date":"2019-10-16 12:58:12 Etc/GMT",
						//			"receipt_creation_date_ms":"1571230692000",
						//			"receipt_creation_date_pst":"2019-10-16 05:58:12 America/Los_Angeles",
						//			"request_date":"2019-10-16 15:38:46 Etc/GMT",
						//			"request_date_ms":"1571240326729",
						//			"request_date_pst":"2019-10-16 08:38:46 America/Los_Angeles",
						//			"original_purchase_date":"2013-08-01 07:00:00 Etc/GMT",
						//			"original_purchase_date_ms":"1375340400000",
						//			"original_purchase_date_pst":"2013-08-01 00:00:00 America/Los_Angeles",
						//			"original_application_version":"1.0",
						//			"in_app":[
						//				{
						//					"quantity":"1",
						//					"product_id":"topaz_pack_1",
						//					"transaction_id":"1000000579960838",
						//					"original_transaction_id":"1000000579960838",
						//					"purchase_date":"2019-10-16 12:58:12 Etc/GMT",
						//					"purchase_date_ms":"1571230692000",
						//					"purchase_date_pst":"2019-10-16 05:58:12 America/Los_Angeles",
						//					"original_purchase_date":"2019-10-16 12:58:12 Etc/GMT",
						//					"original_purchase_date_ms":"1571230692000",
						//					"original_purchase_date_pst":"2019-10-16 05:58:12 America/Los_Angeles",
						//					"is_trial_period":"false"
						//				}
						//			]
						//		},
						//	"status":0,
						//	"environment":"Sandbox"
						//}

						auto& PurchaseStateValue = Json_["status"];
						if (PurchaseStateValue.isNull())
							throw (-101);

						int32 PurchaseState = PurchaseStateValue.asInt();
						if (PurchaseState != 0)
						{
							if (PurchaseState == 21007) // sendbox 용 영수증인데 buy 용 주소로 검증한 경우
							{
								if (!_Check(EOS::iOS, Order_, true))
									throw (-102); // 이후 old type receipt 루틴을 타게되지만 status 가 0 이 아니므로 에러 처리됨.

								throw _STestPush();
							}
							else
							{
								throw PurchaseState;
							}
						}

						auto& InApp = Json_["receipt"]["in_app"];
						if (InApp.isNull())
							throw (-103);

						if (InApp.size() != 1)
							throw (-104);

						auto& QuantityValue = InApp[0]["quantity"];
						if (QuantityValue.isNull())
							throw (-105);

						if (stoi(QuantityValue.asString()) != 1)
							throw (-106);

						auto& ProductIDValue = InApp[0]["product_id"];
						if (ProductIDValue.isNull())
							throw (-107);

						if (ProductIDValue.asString() != Order_.ProductID)
							throw (-108);

						auto& OrderIDValue = InApp[0]["original_transaction_id"];
						if (OrderIDValue.isNull())
							throw (-109);

						auto& PurchaseTimeValue = InApp[0]["purchase_date_ms"];
						if (PurchaseTimeValue.isNull())
							throw (-110);

						OrderID = OrderIDValue.asString();
						PurchaseTime = stoll(PurchaseTimeValue.asString());
					}
					catch (const _STestPush&)
					{
						throw;
					}
					catch (...) // old type receipt
					{
						//{
						//	"receipt":{
						//		"original_purchase_date_pst":"2013-08-23 20:57:50 America/Los_Angeles",
						//		"purchase_date_ms":"1377316670812",
						//		"unique_identifier":"5a4aba0532efa9c3332cc5e531ed20b10ef69a1d",
						//		"original_transaction_id":"1000000085070613",
						//		"bvrs":"1.05.02",
						//		"transaction_id":"1000000085070613",
						//		"quantity":"1",
						//		"unique_vendor_identifier":"9E78D7FC-E7CD-4A5E-A47E-7231BA6C45F",
						//		"item_id":"677520105",
						//		"product_id":"ap_diamond_125",
						//		"purchase_date":"2013-08-24 03:57:50 Etc/GMT",
						//		"original_purchase_date":"2013-08-24 03:57:50 Etc/GMT",
						//		"purchase_date_pst":"2013-08-23 20:57:50 America/Los_Angeles",
						//		"bid":"com.atoz.skyko",
						//		"original_purchase_date_ms":"1377316670812"
						//	},
						//	"status":0
						//}

						auto& PurchaseStateValue = Json_["status"];
						if (PurchaseStateValue.isNull())
							throw (-202);

						int32 PurchaseState = PurchaseStateValue.asInt();
						if (PurchaseState != 0)
							throw PurchaseState;

						auto& ProductIDValue = Json_["receipt"]["product_id"];
						if (ProductIDValue.isNull())
							throw (-203);

						if (ProductIDValue.asString() != Order_.ProductID)
							throw (-204);

						auto& OrderIDValue = Json_["receipt"]["original_transaction_id"];
						if (OrderIDValue.isNull())
							throw (-205);

						auto& PurchaseTimeValue = Json_["receipt"]["purchase_date_ms"];
						if (PurchaseTimeValue.isNull())
							throw (-206);

						OrderID = OrderIDValue.asString();
						PurchaseTime = stoll(PurchaseTimeValue.asString());
					}

					return _TParseResult(OrderID, PurchaseTime, PurchaseType, ConsumptionState);
				}
			);

			for (auto& i : _OSInfos)
				if (!i.ParseFunc)
					THROWEX();
		}
		bool CReceiptCheck::Check(EOS OS_, const SOrder& Order_)
		{
			return _Check(OS_, Order_, false);
		}
		void CReceiptCheck::Proc(void)
		{
			_Http.Proc();
		}
	}
}
