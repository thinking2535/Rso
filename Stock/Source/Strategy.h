#pragma once

#include "TradeIntensity.h"

namespace rso
{
	namespace stock
	{
		using namespace std;

		class CStrategy
		{
		public:
			using TNewCallback = function<void(EOrderType OrderType_, const SOrder& Order_)>; // 외부로 신규주문할 것을 요청
			using TModifyCallback = function<void(int32 OriginOrderNo_, EOrderType OrderType_, const SOrder& Order_)>; // 외부로 정정주문할 것을 요청
			using TCancelCallback = function<void(int32 OriginOrderNo_, int32 Volume_)>; // 외부로 취소주문할 것을 요청
			using TOrders = map<int32, SOrder>;
		private:
			int32 _HaveVolume = 0; // 보유한 수량
			__int64 _Asset = 0;
			TNewCallback _NewCallback;
			TModifyCallback _ModifyCallback;
			TCancelCallback _CancelCallback;

			CTradeIntensity _TradeIntensity;
			TOrders _Orders; // Volume 은 음수 가능
			int32 _SellPrice1 = 0;
			int32 _BuyPrice1 = MaxValue<int32>();
			int32 _CurPrice = -1;
			TTime _LastNewTime;
			int32 _MaxTradeVolume = 0;

			inline __int64 _CurHavePrice(void) const noexcept { return ((__int64)_HaveVolume * (__int64)_CurPrice * c_PointPrice); }

		public:
			CStrategy(int32 MaxTradeVolume_, TNewCallback NewCallback_, TModifyCallback ModifyCallback_, TCancelCallback CancelCallback_, double TradeIntensity_);
			bool NewCs(const TTime& Time_, EOrderType OrderType_, const SOrder& Order_); // 주문 요청
			void NewSc(int32 OrderNo_, const SOrder& Order_); // 주문 접수 됨.
			void ModifyUpCs(void);
			void ModifyDownCs(void);
			void ModifyMarketCs(void);
			void CancelCs(void); // 모든 주문수량 취소
			void TradeSc(int32 OriginOrderNo_, const SOrder& Order_); // Volume 은 음수일 수 있음.
			void Proc(const TTime& Time_);
			double Tick(const SChartTime& Tick_);
			inline int32 GetHaveVolume(void) const noexcept { return _HaveVolume; }
			inline __int64 GetProfit(void) const noexcept { return _Asset + _CurHavePrice(); }
			inline const TOrders& GetOrders(void) const { return _Orders; }
			inline void SetMaxTradeVolume(int32 MaxTradeVolume_) { _MaxTradeVolume = MaxTradeVolume_; }
		};
	}
}
