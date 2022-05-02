//#include "StgTick.h"
//
//
//namespace rso
//{
//	namespace stock
//	{
//		bool CStgTick::_InputCallback(const STick& Tick_, const SDataInfo& DataInfo_, TIn& In_)
//		{
//			if (DataInfo_.IsBegin)
//			{
//				_Trend.init(Tick_.Price);
//				_HighPenetrate.reset(Tick_.Price, Tick_.Volume);
//				_LowPenetrate.reset(Tick_.Price, Tick_.Volume);
//				_RangeSums0.clear();
//				_RangeSums1.clear();
//				_RangeSums2.clear();
//				_RangeSums10.clear();
//				_RangeSums11.clear();
//				_RangeSums12.clear();
//				_RangeSumsAll.clear();
//				ClearTradeCnt();
//				_BuyCnt = 0;
//				_SellCnt = 0;
//			}
//			else
//			{
//				_Trend.push(Tick_.Price);
//				_HighPenetrate.set(Tick_.Price, Tick_.Volume);
//				_LowPenetrate.set(Tick_.Price, Tick_.Volume);
//			}
//
//			if (Tick_.Trade == 0)
//				return false;
//
//			auto CurTime = Tick_.InsertedTime.ToTimePoint();
//			auto SignedVolume = (Tick_.Trade > 0 ? Tick_.Volume : -Tick_.Volume);
//			//_RangeSumsAll.push(CurTime, Tick_.Volume);
//
//
//			//if (Tick_.Trade > 0)
//			//{
//			//	_RangeSumsBuy0.push(CurTime, SignedVolume);
//			//}
//			//else
//			//{
//			//	_RangeSumsSell0.push(CurTime, SignedVolume);
//			//}
//
//
//			//_RangeSums0.push(CurTime, Tick_.Trade);
//			//_RangeSums1.push(CurTime, Tick_.Trade);
//			//_RangeSums2.push(CurTime, Tick_.Trade);
//			_RangeSums10.push(CurTime, SignedVolume);
//			//_RangeSums11.push(CurTime, SignedVolume);
//			//_RangeSums12.push(CurTime, SignedVolume);
//
//
//			size_t Index = 0;
//			//In_[Index++] = _RangeSums0.get();
//			//In_[Index++] = _RangeSums1.get();
//			//In_[Index++] = _RangeSums2.get();
//			In_[Index++] = _RangeSums10.get();
//			//In_[Index++] = _RangeSums11.get();
//			//In_[Index++] = _RangeSums12.get();
//			//In_[Index++] = _RangeSumsAll.get();
//
//			return true;
//		}
//		void CStgTick::_RemindCallback(const STick& Tick_, const TRmns& Offers_, const TRmns& Bids_, const SDataInfo& DataInfo_, const TIn& In_, const TOut& OutMin_, const TOut& OutMax_)
//		{
//			if (OpenInterest() == 0)
//			{
//				if (OrderVolume() == 0)
//				{
//#if 0
//					if (duration_cast<milliseconds>(CurTime() - OrderBeginTime()) < milliseconds(10000) ||
//						duration_cast<milliseconds>(CurTime() - TradedTime()) < milliseconds(10000))
//						return;
//
//					if (Tick_.InsertedTime > CDateTime(Tick_.InsertedTime.year, Tick_.InsertedTime.month, Tick_.InsertedTime.day, c_TradeEndHour, c_TradeEndMinute, 0))
//						return;
//
//					// ���� �������� ó�� �����ϵ���
//					//  1. �Ѱ��� �������� �����ϰ� ���� û������ ���� ��쿡 �ٽ� ������ �������� ���� ����, �Ұ��� �ϵ��� ó���Ұ�.
//					// �������� �ŷ��� ������ ó���� �� �ֵ��� �Ұ�.(����(Change)�� ��������. ������ ����OrderNo���� ������ �� �� �ű� order�� �ִ� ���)
//					//	1. COrderŬ���� �����
//					//		����������Busy������.
//					//	2. CStgTick::_RemindCallback �Լ����� �� �ֹ��� ���� for �������� �ɵ�.
//					//		if (OpenInterest() == 0)  �ϰ� if (GetOrder().IsNull()) ���� ���� �ʿ�
//
//					//�Ѱ��� ū volume�� ƽ���� ���������ʵ��� ���� ������.(�ŷ� VOlume�� 1 �Ǵ� -1�� �Է¹޴� RangeSum�� �ξ� ó�������ҵ�)
//
//					//����� �� �����ð��� �ݺ���ؼ� ���������� ��ȭ�Ұ�?
//
//					// ������ �ε����� ã�� Ŭ���� �����(2�� �̻��� �ε�����, ������ �������ָ�? 2�й����� �������� ã����.)
//					//  �� Ŭ������ Brain�� ������ �� ������?
//
//
//					if (Tick_.Trade > 0)
//					{
//						11�� ���� ������, �������� �հ� ���� ���� �ѹ��� �ŷ��Ұ� ? ?
//							����, �� ������ �ð����� ������ �� ���� �߼������� ��� ? (�ð��� �� �� �����غ���.)
//							����, �� ���� �����ϴ� ���� ��ٷ� û���ϰ� ���� �ŷ� ����(�뷫 Ⱦ����)
//							�ٸ��� Ȯ���Ұ�.
//							��ǥ �����Ұ�.
//
//							//if (In_[0] < -In_[1] * 2.0)
//							//	return;
//
//							//if (Offers_[0].Volume > 50)
//							//	return;
//
//							//if (_Trend.Trend() > 40)
//							//	return;
//
//							if (In_[2] < 400.0)
//								return;
//
//						//if (In_[3] < 400.0)
//						//	return;
//
//						if (IsSimulateServer())
//							OrderBegin(SOrder(EOrderType::������, Tick_.Price, _TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::������_FOK, Tick_.Price, _TradeUnitCnt));
//					}
//					else if (Tick_.Trade < 0)
//					{
//						//if (In_[0] * 2.0 > -In_[1])
//						//	return;
//
//						//if (Bids_[0].Volume > 50)
//						//	return;
//
//						//if (_Trend.Trend() < -40)
//						//	return;
//
//						if (-In_[2] < 400.0)
//							return;
//
//						//if (-In_[3] < 400.0)
//						//	return;
//
//						if (IsSimulateServer())
//							OrderBegin(SOrder(EOrderType::������, Tick_.Price, -_TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::������_FOK, Tick_.Price, -_TradeUnitCnt));
//					}
//#elif 0
//					if (duration_cast<milliseconds>(CurTime() - OrderBeginTime()) < milliseconds(10000) ||
//						duration_cast<milliseconds>(CurTime() - TradedTime()) < milliseconds(10000))
//						return;
//
//					if (Tick_.InsertedTime > CDateTime(Tick_.InsertedTime.year, Tick_.InsertedTime.month, Tick_.InsertedTime.day, 15, 20, 0))
//						return;
//
//					if (Tick_.Trade > 0)
//					{
//						if (In_[0] < 5.0 ||
//							In_[1] < 10.0 ||
//							In_[2] < 0.0 ||
//							In_[3] < 50.0 ||
//							In_[4] < 100.0 ||
//							In_[5] < 0.0 ||
//							In_[6] < 500.0)
//							return;
//
//						if (IsSimulateServer())
//							OrderBegin(SOrder(EOrderType::������, Tick_.Price, _TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::���尡, Tick_.Price, _TradeUnitCnt));
//					}
//					else if (Tick_.Trade < 0)
//					{
//						if (-In_[0] < 5.0 ||
//							-In_[1] < 10.0 ||
//							-In_[2] < 0.0 ||
//							-In_[3] < 50.0 ||
//							-In_[4] < 100.0 ||
//							-In_[5] < 0.0 ||
//							In_[6] < 500.0)
//							return;
//
//						if (IsSimulateServer())
//							OrderBegin(SOrder(EOrderType::������, Tick_.Price, -_TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::���尡, Tick_.Price, -_TradeUnitCnt));
//					}
//#else
//					if (duration_cast<milliseconds>(CurTime() - OrderBeginTime()) < milliseconds(10000) ||
//						duration_cast<milliseconds>(CurTime() - TradedTime()) < milliseconds(10000))
//						return;
//
//					if (Tick_.InsertedTime > CDateTime(Tick_.InsertedTime.year, Tick_.InsertedTime.month, Tick_.InsertedTime.day, 13, 0, 0))
//						return;
//
//					if (Tick_.Trade > 0)
//					{
//						if (_BuyCnt > 0)
//							return;
//
//						//if (In_[0] < 5.0 ||
//						//	In_[1] < 10.0 ||
//						//	In_[2] < 0.0 ||
//						//	In_[3] < 50.0 ||
//						//	In_[4] < 100.0 ||
//						//	In_[5] < 0.0 ||
//						//	In_[6] < 100.0)
//						//	return;
//
//						if (In_[0] < 40.0)
//							return;
//
//						if (_HighPenetrate.gap() < 70)
//							return;
//
//						if (_HighPenetrate.volume() < 10000)
//							return;
//
//						_ExitPrice = Tick_.Price + ((_HighPenetrate.gap() / 20) * 5);
//
//						if (IsSimulateServer())
//							OrderBegin(SOrder(EOrderType::������, Tick_.Price, _TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::���尡, Tick_.Price, _TradeUnitCnt));
//
//						++_BuyCnt;
//					}
//					else if (Tick_.Trade < 0)
//					{
//						if (_SellCnt > 0)
//							return;
//
//						//if (-In_[0] < 5.0 ||
//						//	-In_[1] < 10.0 ||
//						//	-In_[2] < 0.0 ||
//						//	-In_[3] < 50.0 ||
//						//	-In_[4] < 100.0 ||
//						//	-In_[5] < 0.0 ||
//						//	In_[6] < 100.0)
//						//	return;
//
//						if (-In_[0] < 40.0)
//							return;
//
//						if (_LowPenetrate.gap() > -70)
//							return;
//
//						if (_LowPenetrate.volume() < 10000)
//							return;
//
//						_ExitPrice = Tick_.Price + ((_LowPenetrate.gap() / 20) * 5);
//
//						if (IsSimulateServer())
//							OrderBegin(SOrder(EOrderType::������, Tick_.Price, -_TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::���尡, Tick_.Price, -_TradeUnitCnt));
//
//						++_SellCnt;
//					}
//#endif
//				}
//				else
//				{
//					if (OrderVolume() > 0)
//					{
//						//	1. ���� �ð��� û����� ������?
//						//	2. ���� �̻� ������ �����ϸ�??
//
//
//						// �ż������ߴµ� �ŵ�Ÿ�̹��̸� ���;
//						//��ҽ� Volume �� ���, ���� ����
//					}
//					else
//					{
//						// �ŵ� �����ߴµ� �ż� Ÿ�̹��̸� ���
//						//��ҽ� Volume �� ���, ���� ����
//					}
//				}
//			}
//			else
//			{
//				if (OrderVolume() == 0)
//				{
//					if (OpenInterest() > 0)
//					{
//					}
//					else
//					{
//					}
//				}
//				else
//				{
//					//if (ChangedCount() == 0 && duration_cast<milliseconds>(CurTime() - OrderBeginTime()) > milliseconds(60000))
//					//{
//					//	auto Price = 0;
//					//	if (OrderVolume() > 0)
//					//		Price = OrderPrice() + 5;
//					//	else
//					//		Price = OrderPrice() - 5;
//
//					//	ChangeBegin(OrderNo(), SOrder(EOrderType::������, Price, OrderVolume()));
//					//	return;
//					//}
//
//					//if (Tick_.InsertedTime < CDateTime(Tick_.InsertedTime.year, Tick_.InsertedTime.month, Tick_.InsertedTime.day, 14, 25, 0))
//					//{
//					//	if (OrderVolume() > 0)
//					//	{
//					//		if (In_[0] < 0.0)
//					//			return;
//					//	}
//					//	else
//					//	{
//					//		if (In_[0] > 0.0)
//					//			return;
//					//	}
//					//}
//
//					if (OrderType() != EOrderType::���尡)
//					{
//						if (duration_cast<milliseconds>(CurTime() - OrderBeginTime()) > milliseconds(3600000) ||
//							Tick_.InsertedTime > CDateTime(Tick_.InsertedTime.year, Tick_.InsertedTime.month, Tick_.InsertedTime.day, 14, 25, 0))
//						ChangeBegin(OrderNo(), SOrder(EOrderType::���尡, 0, OrderVolume()));
//						return;
//					}
//				}
//			}
//		}
//		bool CStgTick::_MemoryCallback(const STick& Tick_, TOut& Out_)
//		{
//			//_OutDatas.emplace(Tick_.Price);
//			//if (_OutDatas.size() < c_NeuronTimeGap)
//			//	return false;
//			//else if (_OutDatas.size() > c_NeuronTimeGap)
//			//	_OutDatas.pop();
//
//			//for (size_t i = 0; i < _OutDatas.size(); ++i)
//			//	Out_[i] = (_OutDatas[i] - FrontData().Close);
//
//			return true;
//		}
//		void CStgTick::_Proc(void)
//		{
//			if (!IsSimulateServer())
//				return;
//
//			if (duration_cast<milliseconds>(system_clock::now() - OrderBeginTime()) < milliseconds(500))
//				return;
//
//			if (OpenInterest() == 0)
//			{
//				if (OrderVolume() != 0)
//				{
//					if (OrderVolume() > 0)
//					{
//						CancelBegin(OrderNo(), OrderVolume());
//
//						//	1. ���� �ð��� û����� ������?
//						//	2. ���� �̻� ������ �����ϸ�??
//
//
//						// �ż������ߴµ� �ŵ�Ÿ�̹��̸� ���;
//						//��ҽ� Volume �� ���, ���� ����
//					}
//					else
//					{
//						CancelBegin(OrderNo(), OrderVolume());
//
//						// �ŵ� �����ߴµ� �ż� Ÿ�̹��̸� ���
//						//��ҽ� Volume �� ���, ���� ����
//					}
//				}
//			}
//		}
//		CStgTick::CStgTick(bool SimulateServer_, size_t SampleCnt_, const CDateTime& TradeStartTimeStamp_,
//			TOrderCallback OrderCallback_,
//			TChangeCallback ChangeCallback_,
//			TCancelCallback CancelCallback_,
//			TValue TradeUnitCnt_) :
//			CStg(SimulateServer_, SampleCnt_, c_NeuronTimeGap, TradeStartTimeStamp_,
//				OrderCallback_, ChangeCallback_, CancelCallback_),
//			_TradeUnitCnt(TradeUnitCnt_)
//		{
//		}
//		wstring CStgTick::StatisticsString(void) const
//		{
//			return _Statistics.ToString();
//		}
//		void CStgTick::Trade(__int64 OrderNo_, TValue Price_, TValue Volume_)
//		{
//			CStg::Trade(OrderNo_, Price_, Volume_);
//
//			if (OpenInterest() != 0)
//			{
//				//TValue PriceGap = 5;
//				//if (CurTick().InsertedTime.hour < 10)
//				//	PriceGap = 15;
//				//else if (CurTick().InsertedTime.hour < 11)
//				//	PriceGap = 10;
//
//
//				//�ż��ŵ����� �°� û���� ���Ұ�. �ð��� �ƴ϶�.
//				//	2016-12-13 �� �ŵ����� Ŀ�� û�� ����.
//				//	�ż�,�ŵ����� ��� �ľ��� ���ΰ�?
//				//		1. �׷����� ����?? �̴� ��� ��ǥ�� ��Ÿ����?
//
//				OrderBegin(SOrder(EOrderType::������, _ExitPrice, -Volume_));
//			}
//		}
//	}
//}