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
					return false; // �߰� �ֹ� ����

				// �ֹ� ���� ���� ������
				Order.Volume = _MaxTradeVolume * (Order.Volume / abs(Order.Volume));
			}
			else
			{
				if (_HaveVolume * Order_.Volume > 0) // �߰� ������ ����
					return false;

				// û�� �ֹ� ����
				int32 LiquidationOrderVolume = 0;
				for (auto& i : _Orders)
				{
					if (_HaveVolume * i.second.Volume > 0) // ���Թ��� �ֹ��� �ջ����� ����.
						continue;

					LiquidationOrderVolume += i.second.Volume;
				}

				if (LiquidationOrderVolume + _HaveVolume == 0) // �̹� ���� ������ŭ û���ֹ� �Ϸ�
					return false;

				Order.Volume = -(LiquidationOrderVolume + _HaveVolume);
			}

			_LastNewTime = Time_;
			_NewCallback(OrderType_, Order);
			return true;
		}
		void CStrategy::NewSc(int32 OrderNo_, const SOrder& Order_)
		{
			if (Order_.Volume == 0) // Ű���� ������ ���⼭ ���� �ֹ� ���� �� ������ ��(������ "����" ���������� �߰��� �����)
				_Orders.erase(OrderNo_);
			else
				_Orders.insert_or_assign(OrderNo_, Order_);
		}
		void CStrategy::ModifyUpCs(void)
		{
			for (auto& i : _Orders)
			{
				_ModifyCallback(i.first, EOrderType::������, SOrder(i.second.Price + c_PriceGap, i.second.Volume));
				break; // �ѹ��� �ϳ����� ó���ǵ���.
			}
		}
		void CStrategy::ModifyDownCs(void)
		{
			for (auto& i : _Orders)
			{
				_ModifyCallback(i.first, EOrderType::������, SOrder(i.second.Price - c_PriceGap, i.second.Volume));
				break; // �ѹ��� �ϳ����� ó���ǵ���.
			}
		}
		void CStrategy::ModifyMarketCs(void)
		{
			for (auto& i : _Orders)
			{
				_ModifyCallback(i.first, EOrderType::���尡, SOrder(0, i.second.Volume));
				break; // �ѹ��� �ϳ����� ó���ǵ���.
			}
		}
		void CStrategy::CancelCs(void)
		{
			for (auto& i : _Orders)
			{
				_CancelCallback(i.first, i.second.Volume);
				break; // �ѹ��� �ϳ����� ó���ǵ���.
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

			//	����	����	û��
			//	0		+		0
			//	0		-		0
			//	+		+		-
			//	+		-		0
			//	-		+		0
			//	-		-		+

			//// �ٷ� û�� �ֹ�
			//if (_HaveVolume * Order_.Volume > 0)
			//	_NewCallback(EOrderType::������, SOrder(Order_.Price + c_PriceGap * (Order_.Volume / abs(Order_.Volume)), -Order_.Volume));
			//else if (_HaveVolume == 0 && _Orders.empty())
			//	_StrategyPosition = 0; // �ٽ� ���� ���� ��� ���·� ��ȯ
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

			//	if (itCheck->second.Volume * _StrategyPosition > 0) // ���� �ֹ��̸�
			//	{
			//		// IOC, FOK �� �ű� �ֹ������� ���� ���� ����.
			//		if (duration_cast<seconds>(Time_ - _LastNewTime).count() >= 2) // �ű��ֹ��� �ش� �ʸ�ŭ ��������
			//		{
			//			itCheck->second.Locked = true;
			//			_CancelCallback(itCheck->first, itCheck->second.Volume);
			//		}
			//	}
			//	else // û�� �ֹ��̸�
			//	{
			//		if (_StrategyPosition / abs(_StrategyPosition) * -TradeIntensity > 5.0 || // ���Թݴ������ �ŸŰ����� �ʴ� �ŷ����� �ش� ���� ������
			//			duration_cast<seconds>(Time_ - _LastNewTime).count() >= 5) // ������ �ش� �ʸ�ŭ ��������
			//		{
			//			itCheck->second.Locked = true;
			//			_ModifyCallback(itCheck->first, EOrderType::���尡, SOrder(itCheck->second.Price, itCheck->second.Volume));
			//		}
			//		else if (duration_cast<seconds>(Time_ - _LastNewTime).count() >= 3) // ������ �ش� �ʸ�ŭ ��������
			//		{
			//			itCheck->second.Locked = true;

			//			if (itCheck->second.Volume > 0) // �ż� û�� ��ü�� ����
			//				_ModifyCallback(itCheck->first, EOrderType::������, SOrder(itCheck->second.Price < _BuyPrice1 ? _BuyPrice1 : _BuyPrice1 + c_PriceGap, itCheck->second.Volume));
			//			else // �ŵ� û�� ��ü�� ����
			//				_ModifyCallback(itCheck->first, EOrderType::������, SOrder(itCheck->second.Price > _SellPrice1 ? _SellPrice1 : _SellPrice1 - c_PriceGap, itCheck->second.Volume));
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
