#include "Strategy.h"

namespace rso
{
	namespace stock
	{
		CStrategy::CStrategy(int32 MaxTradeVolume_, TNewCallback NewCallback_, TModifyCallback ModifyCallback_, TCancelCallback CancelCallback_, double TradeIntensity_) :
			_MaxTradeVolume(MaxTradeVolume_), _NewCallback(NewCallback_), _ModifyCallback(ModifyCallback_), _CancelCallback(CancelCallback_), _TradeIntensity(TradeIntensity_)
		{
		}
		bool CStrategy::NewCs(const TTime& Time_, EOrderType OrderType_, const SOrder& Order_)
		{
			auto Order = Order_;

			if (_HaveVolume == 0)
			{
				if (_Orders.size() > 0)
					return false; // 추가 주문 제한

				// 주문 수량 설정 값으로
				Order.Volume = _MaxTradeVolume * (Order.Volume / abs(Order.Volume));
			}
			else
			{
				if (_HaveVolume * Order_.Volume > 0) // 추가 진입은 금지
					return false;

				// 청산 주문 수량
				int32 LiquidationOrderVolume = 0;
				for (auto& i : _Orders)
				{
					if (_HaveVolume * i.second.Volume > 0) // 진입방향 주문은 합산하지 않음.
						continue;

					LiquidationOrderVolume += i.second.Volume;
				}

				if (LiquidationOrderVolume + _HaveVolume == 0) // 이미 보유 수량만큼 청산주문 완료
					return false;

				Order.Volume = -(LiquidationOrderVolume + _HaveVolume);
			}

			_LastNewTime = Time_;
			_NewCallback(OrderType_, Order);
			return true;
		}
		void CStrategy::NewSc(int32 OrderNo_, const SOrder& Order_)
		{
			if (Order_.Volume == 0) // 키움은 정정시 여기서 기존 주문 삭제 및 갱신이 됨(정정시 "접수" 프로토콜이 중간에 날라옴)
				_Orders.erase(OrderNo_);
			else
				_Orders.insert_or_assign(OrderNo_, Order_);
		}
		void CStrategy::ModifyUpCs(void)
		{
			for (auto& i : _Orders)
			{
				_ModifyCallback(i.first, EOrderType::지정가, SOrder(i.second.Price + c_PriceGap, i.second.Volume));
				break; // 한번에 하나씩만 처리되도록.
			}
		}
		void CStrategy::ModifyDownCs(void)
		{
			for (auto& i : _Orders)
			{
				_ModifyCallback(i.first, EOrderType::지정가, SOrder(i.second.Price - c_PriceGap, i.second.Volume));
				break; // 한번에 하나씩만 처리되도록.
			}
		}
		void CStrategy::ModifyMarketCs(void)
		{
			for (auto& i : _Orders)
			{
				_ModifyCallback(i.first, EOrderType::시장가, SOrder(0, i.second.Volume));
				break; // 한번에 하나씩만 처리되도록.
			}
		}
		void CStrategy::CancelCs(void)
		{
			for (auto& i : _Orders)
			{
				_CancelCallback(i.first, i.second.Volume);
				break; // 한번에 하나씩만 처리되도록.
			}
		}
		void CStrategy::TradeSc(int32 OriginOrderNo_, const SOrder& Order_)
		{
			auto it = _Orders.find(OriginOrderNo_);
			if (it == _Orders.end())
				return;

			it->second.Volume -= Order_.Volume;

			if (it->second.Volume == 0)
				_Orders.erase(it);

			_HaveVolume += Order_.Volume;
			_Asset -= ((__int64)Order_.Price * (__int64)Order_.Volume * c_PointPrice);

			//	보유	지금	청산
			//	0		+		0
			//	0		-		0
			//	+		+		-
			//	+		-		0
			//	-		+		0
			//	-		-		+

			//// 바로 청산 주문
			//if (_HaveVolume * Order_.Volume > 0)
			//	_NewCallback(EOrderType::지정가, SOrder(Order_.Price + c_PriceGap * (Order_.Volume / abs(Order_.Volume)), -Order_.Volume));
			//else if (_HaveVolume == 0 && _Orders.empty())
			//	_StrategyPosition = 0; // 다시 전략 실행 대기 상태로 전환
		}
		void CStrategy::Proc(const TTime& /*Time_*/)
		{
			// dddd
			return;
			//for (auto it = _Orders.begin(); it != _Orders.end(); )
			//{
			//	auto itCheck = it;
			//	++it;

			//	if (itCheck->second.Locked)
			//		continue;

			//	if (itCheck->second.Volume * _StrategyPosition > 0) // 진입 주문이면
			//	{
			//		// IOC, FOK 로 신규 주문했으면 여기 오지 않음.
			//		if (duration_cast<seconds>(Time_ - _LastNewTime).count() >= 2) // 신규주문후 해당 초만큼 지났으면
			//		{
			//			itCheck->second.Locked = true;
			//			_CancelCallback(itCheck->first, itCheck->second.Volume);
			//		}
			//	}
			//	else // 청산 주문이면
			//	{
			//		if (_StrategyPosition / abs(_StrategyPosition) * -TradeIntensity > 5.0 || // 진입반대방향의 매매강도가 초당 거래량이 해당 값을 넘으면
			//			duration_cast<seconds>(Time_ - _LastNewTime).count() >= 5) // 진입후 해당 초만큼 지났으면
			//		{
			//			itCheck->second.Locked = true;
			//			_ModifyCallback(itCheck->first, EOrderType::시장가, SOrder(itCheck->second.Price, itCheck->second.Volume));
			//		}
			//		else if (duration_cast<seconds>(Time_ - _LastNewTime).count() >= 3) // 진입후 해당 초만큼 지났으면
			//		{
			//			itCheck->second.Locked = true;

			//			if (itCheck->second.Volume > 0) // 매수 청산 미체결 약정
			//				_ModifyCallback(itCheck->first, EOrderType::지정가, SOrder(itCheck->second.Price < _BuyPrice1 ? _BuyPrice1 : _BuyPrice1 + c_PriceGap, itCheck->second.Volume));
			//			else // 매도 청산 미체결 약정
			//				_ModifyCallback(itCheck->first, EOrderType::지정가, SOrder(itCheck->second.Price > _SellPrice1 ? _SellPrice1 : _SellPrice1 - c_PriceGap, itCheck->second.Volume));
			//		}
			//	}
			//}
		}
		double CStrategy::Tick(const SChartTime& Tick_)
		{
			if (Tick_.Volume > 0)
				_SellPrice1 = Tick_.Price;
			else
				_BuyPrice1 = Tick_.Price;

			_CurPrice = Tick_.Price;
			Proc(Tick_.TimePoint);
			return _TradeIntensity.Tick(Tick_.TimePoint, static_cast<double>(Tick_.Volume));
		}
	}
}
