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
//					// 여러 전략으로 처리 가능하도록
//					//  1. 한개의 전략으로 진입하고 아직 청산하지 못한 경우에 다시 동일한 전략으로 진입 가능, 불가능 하도록 처리할것.
//					// 여러건의 거래를 별개로 처리할 수 있도록 할것.(정정(Change)와 관련있음. 정정은 기존OrderNo에서 수량을 뺀 후 신규 order를 넣는 방식)
//					//	1. COrder클래스 만들것
//					//		개별적으로Busy가질것.
//					//	2. CStgTick::_RemindCallback 함수에서 각 주문건 별로 for 문돌리면 될듯.
//					//		if (OpenInterest() == 0)  하고 if (GetOrder().IsNull()) 등은 정리 필요
//
//					//한개의 큰 volume의 틱으로 진입하지않도록 조건 넣을것.(거래 VOlume을 1 또는 -1로 입력받는 RangeSum을 두어 처리가능할듯)
//
//					//장시작 후 지난시간에 반비례해서 진입조건을 강화할것?
//
//					// 최적의 인덱스를 찾는 클래스 만들것(2개 이상의 인덱스도, 범위를 지정해주면? 2분법으로 최적값을 찾도록.)
//					//  위 클래스를 Brain에 적용할 수 있을까?
//
//
//					if (Tick_.Trade > 0)
//					{
//						11시 이전 전고점, 전저점을 뚫고 가는 시점 한번만 거래할것 ? ?
//							전고, 저 점과의 시간차와 고저폭 과 이후 추세강도는 비례 ? (시간차 는 좀 생각해볼것.)
//							전고, 저 점을 돌파하다 말면 곧바로 청산하고 이후 거래 금지(대략 횡보장)
//							다른날 확인할것.
//							지표 정리할것.
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
//							OrderBegin(SOrder(EOrderType::지정가, Tick_.Price, _TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::지정가_FOK, Tick_.Price, _TradeUnitCnt));
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
//							OrderBegin(SOrder(EOrderType::지정가, Tick_.Price, -_TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::지정가_FOK, Tick_.Price, -_TradeUnitCnt));
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
//							OrderBegin(SOrder(EOrderType::지정가, Tick_.Price, _TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::시장가, Tick_.Price, _TradeUnitCnt));
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
//							OrderBegin(SOrder(EOrderType::지정가, Tick_.Price, -_TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::시장가, Tick_.Price, -_TradeUnitCnt));
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
//							OrderBegin(SOrder(EOrderType::지정가, Tick_.Price, _TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::시장가, Tick_.Price, _TradeUnitCnt));
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
//							OrderBegin(SOrder(EOrderType::지정가, Tick_.Price, -_TradeUnitCnt));
//						else
//							OrderBegin(SOrder(EOrderType::시장가, Tick_.Price, -_TradeUnitCnt));
//
//						++_SellCnt;
//					}
//#endif
//				}
//				else
//				{
//					if (OrderVolume() > 0)
//					{
//						//	1. 일정 시간후 청산되지 않으면?
//						//	2. 일정 이상 가격이 변동하면??
//
//
//						// 매수진입했는데 매도타이밍이면 취소;
//						//취소시 Volume 은 양수, 음수 가능
//					}
//					else
//					{
//						// 매도 진입했는데 매수 타이밍이면 취소
//						//취소시 Volume 은 양수, 음수 가능
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
//					//	ChangeBegin(OrderNo(), SOrder(EOrderType::지정가, Price, OrderVolume()));
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
//					if (OrderType() != EOrderType::시장가)
//					{
//						if (duration_cast<milliseconds>(CurTime() - OrderBeginTime()) > milliseconds(3600000) ||
//							Tick_.InsertedTime > CDateTime(Tick_.InsertedTime.year, Tick_.InsertedTime.month, Tick_.InsertedTime.day, 14, 25, 0))
//						ChangeBegin(OrderNo(), SOrder(EOrderType::시장가, 0, OrderVolume()));
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
//						//	1. 일정 시간후 청산되지 않으면?
//						//	2. 일정 이상 가격이 변동하면??
//
//
//						// 매수진입했는데 매도타이밍이면 취소;
//						//취소시 Volume 은 양수, 음수 가능
//					}
//					else
//					{
//						CancelBegin(OrderNo(), OrderVolume());
//
//						// 매도 진입했는데 매수 타이밍이면 취소
//						//취소시 Volume 은 양수, 음수 가능
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
//				//매수매도세에 맞게 청산폭 정할것. 시간이 아니라.
//				//	2016-12-13 에 매도갭이 커서 청산 못함.
//				//	매수,매도세는 어떻게 파악할 것인가?
//				//		1. 그래프의 기울기?? 이는 어떻게 지표로 나타낼까?
//
//				OrderBegin(SOrder(EOrderType::지정가, _ExitPrice, -Volume_));
//			}
//		}
//	}
//}