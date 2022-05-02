#pragma once

#include "Base.h"
#include <Rso/Base/List.h>
#include <Rso/GameUtil/OptionJson.h>
#include <Rso/Http/Http.h>

namespace rso
{
	namespace mobileutil
	{
		using namespace base;
		using namespace gameutil;
		using namespace http;
		using namespace std;

		class CReceiptCheck // Hub와 통신하여 AccessToken 을 받음.
		{
			struct _STestPush
			{
			};

		public:
			struct SOrder
			{
				int64 UID = 0;
				string ProductID;
				string PurchaseToken;

				SOrder()
				{
				}
				SOrder(int64 UID_, const string& ProductID_, const string& PurchaseToken_) :
					UID(UID_), ProductID(ProductID_), PurchaseToken(PurchaseToken_)
				{
				}
			};
			using TCheckFunc = function<void(const SOrder& Order_, const string& OrderID_, int64 PurchaseTime_, int32 PurchaseType_, int32 ConsumptionState_)>;
			using TErrorFunc = function<void(const SOrder& Order_, int32 PurchaseState_)>;

		private:
			using _TPushFunc = function<void(const string & PackageName_, const SOrder & Order_, bool Test_, size_t OrderIndex_)>;
			using _TParseResult = tuple<string, int64, int32, int32>;
			using _TParseFunc = function<_TParseResult(const CJson & Json_, const SOrder & Order_)>;

			struct _SOSInfo
			{
				_TPushFunc PushFunc;
				_TParseFunc ParseFunc;

				_SOSInfo()
				{
				}
				_SOSInfo(_TPushFunc PushFunc_, _TParseFunc ParseFunc_) :
					PushFunc(PushFunc_), ParseFunc(ParseFunc_)
				{
				}
			};

			vector<_SOSInfo> _OSInfos;
			CHttp<EOS, size_t> _Http;
			TCheckFunc _CheckFunc;
			TErrorFunc _ErrorFunc;
			string _PackageName;
			string _AccessToken; // CReceiptToken 과 통신하여 받음
			CList<SOrder> _Orders;

			bool _Check(EOS OS_, const SOrder& Order_, bool Test_);
			void _ChangedCallback(size_t SessionIndex_, const string& ObjectName_, DWORD Received_, DWORD Total_, const EOS& OS_, const size_t& OrderIndex_);
			void _CompletedCallback(size_t SessionIndex_, EHttpRet Ret_, const string& ObjectName_, const TBuffer& Buffer_, const EOS& OS_, const size_t& OrderIndex_);

		public:
			CReceiptCheck(size_t SessionCount_, TCheckFunc CheckFunc_, TErrorFunc ErrorFunc_, const string& PackageName_, const string& AccessToken_);
			bool Check(EOS OS_, const SOrder& Order_);
			void Proc(void);
			void SetAccessToken(const string& AccessToken_) { _AccessToken = AccessToken_; }
		};
	}
}
