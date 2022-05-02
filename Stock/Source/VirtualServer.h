#pragma once

#include "Base.h"
#include <Rso/Base/MultiIndexMap.h>

namespace rso
{
	namespace stock
	{
		using namespace std;
		using namespace base;

		class CVirtualServer
		{
		public:
			enum class EState
			{
				Stopped,
				Started,
				Paused,
				Max
			};

			using TNewCallback = function<void(int32 OrderNo_, const SOrder & Order_)>; // 외부로 신규주문할 것을 요청
			using TTradeCallback = function<void(int32 OriginOrderNo_, const SOrder & Order_)>;
			using TTickCallback = function<void(const SChartTime& Data_)>;
			using TRemainCallback = function<void(const SRemainDB & Data_)>;
			using TErrorCallback = function<void(const string & Msg_)>;

		private:
			using _TDatas = vector<SChartTime>;
			using _TRemainDatas = vector<SRemainDB>;
			using _TOrders = CMultiIndexMap<int32, unique_index<int32>, non_unique_index<int32>>;

			TNewCallback _NewCallback;
			TTradeCallback _TradeCallback;
			TTickCallback _TickCallback;
			TRemainCallback _RemainCallback;
			TErrorCallback _ErrorCallback;
			_TDatas _AllDatas;
			_TRemainDatas _AllRemainDatas;
			size_t _Size = 0;
			size_t _RemainSize = 0;
			EState _State = EState::Stopped;

			int32 _SellPrice1 = MaxValue<int32>(); 
			int32 _BuyPrice1 = 0;
			int32 _OrderCounter = 0;
			_TOrders _Orders; // Value : [Volume], Key : [OrderNo, Price]

			bool _CheckPrice(EOrderType OrderType_, int32 Price_);
			bool _CheckVolume(int32 Volume_);
			void _CheckTrade(_TOrders::iterator it_);
			bool _IsMarketPriceOrder(EOrderType OrderType_) const;
			int32 _GetOrderPrice(EOrderType OrderType_, const SOrder& Order_);
			void _Tick(void);
			void _Remain(void);

		public:
			CVirtualServer(TNewCallback NewCallback_, TTradeCallback TradeCallback_, TTickCallback TickCallback_, TRemainCallback RemainCallback_, TErrorCallback ErrorCallback_);
			inline void insert(const SChartTime& Data_) { _AllDatas.emplace_back(Data_); }
			inline void insert_remain(const SRemainDB& Data_) { _AllRemainDatas.emplace_back(Data_); }
			TTime Start(void);
			void Stop(void) noexcept;
			bool Pause(void);
			bool Resume(void);
			inline EState GetState(void) const noexcept { return _State; }
			void Proc(const TTime& Time_);
			bool New(EOrderType OrderType_, const SOrder& Order_);
			bool Modify(int32 OriginOrderNo_, EOrderType OrderType_, int32 Price_);
			bool Cancel(int32 OriginOrderNo_);
		};
	}
}
