#include "StockExchange.h"


namespace rso
{
	namespace stock
	{
		void CStockExchange::_Clear(void)
		{
			_CurTime = TTime();
			_Offer0 = 5;
			_Bid0 = 5;
			_Offers = TRmns();
			_Bids = TRmns();
			_PreOrder = SOrder();
			_Type = _EType::Null;
		}
		void CStockExchange::_Trade(__int64 OrderNo_, TValue Price_, TValue Volume_)
		{
			Trade(OrderNo_, Price_, Volume_);
			_PreRemain = 0;
			_TradeCallback(OrderNo(), Price_, Volume_);
		}
		void CStockExchange::_Order(void)
		{
			_Type = _EType::Null;
			Order(++_OrderCounter, _PreOrder.Price, _PreOrder.Volume);
			_PreOrder.Reset();
			_ResetPreRemain();
		}
		void CStockExchange::_Change(void)
		{
			_Type = _EType::Null;
			Change(_OrderedNo, ++_OrderCounter, _PreOrder.Price, _PreOrder.Volume);
			_PreOrder.Reset();
			_ResetPreRemain();
		}
		void CStockExchange::_Cancel(void)
		{
			_Type = _EType::Null;
			auto Volume = _PreOrder.Volume;
			_PreOrder.Reset();
			Cancel(_OrderedNo, Volume);// 원칙은 주문번호로 취소 해야하고, 1단위로 취소 할 수 있어야 함. 현재는 _Order의 Volume이 2 이상이고 1개 취소 해도 모두 취소됨.
			_CancelCallback(_OrderedNo, Volume);
		}
		void CStockExchange::_ResetPreRemain(void)
		{
			if (_OrderType != EOrderType::시장가)
			{
				if (OrderVolume() < 0)
				{
					for (auto& Remain : _Offers)
					{
						if (Remain.Price == OrderPrice())
							_PreRemain = Remain.Volume;
					}
				}
				else if (OrderVolume() > 0)
				{
					for (auto& Remain : _Bids)
					{
						if (Remain.Price == OrderPrice())
							_PreRemain = Remain.Volume;
					}
				}
			}
		}
		CStockExchange::CStockExchange(
			TOrderCallback OrderCallback_,
			TChangeCallback ChangeCallback_,
			TCancelCallback CancelCallback_,
			TTradeCallback TradeCallback_,
			milliseconds Delay_) :
			_OrderCallback(OrderCallback_),
			_ChangeCallback(ChangeCallback_),
			_CancelCallback(CancelCallback_),
			_TradeCallback(TradeCallback_),
			_Delay(Delay_)
		{
		}
		void CStockExchange::Set(const STick& Tick_)
		{
			if (_OldDay != Tick_.InsertedTime.day)
			{
				_OldDay = Tick_.InsertedTime.day;
				_Clear();
			}

			_CurTime = Tick_.InsertedTime.ToTimePoint();

			if (_EmptyRemain())
				return;

			for (_Offer0 = 0; _Offer0 < _Offers.size(); ++_Offer0)
			{
				if (Tick_.Offer0 < _Offers[_Offer0].Price)
				{
					break;
				}
				else if (Tick_.Offer0 == _Offers[_Offer0].Price)
				{
					if (Tick_.Price == Tick_.Offer0)
					{
						if (_Offers[_Offer0].Volume > Tick_.Volume)
							_Offers[_Offer0].Volume -= Tick_.Volume;
						else
						{
							_Offers[_Offer0].Volume = 0;
							_Offers[_Offer0].Cnt = 0;
							++_Offer0;
						}
					}

					break;
				}

				_Offers[_Offer0].Volume = 0;
				_Offers[_Offer0].Cnt = 0;
			}

			for (_Bid0 = 0; _Bid0 < _Bids.size(); ++_Bid0)
			{
				if (Tick_.Bid0 > _Bids[_Bid0].Price)
				{
					break;
				}
				else if (Tick_.Bid0 == _Bids[_Bid0].Price)
				{
					if (Tick_.Price == Tick_.Bid0)
					{
						if (_Bids[_Bid0].Volume > Tick_.Volume)
							_Bids[_Bid0].Volume -= Tick_.Volume;
						else
						{
							_Bids[_Bid0].Volume = 0;
							_Bids[_Bid0].Cnt = 0;
							++_Bid0;
						}
					}

					break;
				}

				_Bids[_Bid0].Volume = 0;
				_Bids[_Bid0].Cnt = 0;
			}

			if (Tick_.Trade > 0)
			{
				if (OrderVolume() < 0)
				{
					if (Tick_.Price == OrderPrice())
					{
						if (_PreRemain > Tick_.Volume)
							_PreRemain -= Tick_.Volume;
						else
							_PreRemain = 0;
					}
					else if (Tick_.Price > OrderPrice())
					{
						_PreRemain = 0;
					}

					if (!_Busy)
					{
						if (OrderPrice() < Tick_.Price ||
							(OrderPrice() == Tick_.Price && _PreRemain == 0))
							_Trade(OrderNo(), OrderPrice(), OrderVolume());
					}
				}
				else if (OrderVolume() > 0)
				{
					if (Tick_.Price <= OrderPrice())
						_PreRemain = 0;
				}
			}
			else
			{
				if (OrderVolume() > 0)
				{
					if (Tick_.Price == OrderPrice())
					{
						if (_PreRemain > Tick_.Volume)
							_PreRemain -= Tick_.Volume;
						else
							_PreRemain = 0;
					}
					else if (Tick_.Price < OrderPrice())
					{
						_PreRemain = 0;
					}

					if (!_Busy)
					{
						if (OrderPrice() > Tick_.Price ||
							(OrderPrice() == Tick_.Price && _PreRemain == 0))
							_Trade(OrderNo(), OrderPrice(), OrderVolume());
					}
				}
				else if (OrderVolume() < 0)
				{
					if (Tick_.Price >= OrderPrice())
						_PreRemain = 0;
				}
			}
		}
		void CStockExchange::SetRemain(const SRemain& Remain_)
		{
			_Offers[0] = SRmn(Remain_.OfferHo1, Remain_.OfferRem1, Remain_.OfferCnt1);
			_Offers[1] = SRmn(Remain_.OfferHo2, Remain_.OfferRem2, Remain_.OfferCnt2);
			_Offers[2] = SRmn(Remain_.OfferHo3, Remain_.OfferRem3, Remain_.OfferCnt3);
			_Offers[3] = SRmn(Remain_.OfferHo4, Remain_.OfferRem4, Remain_.OfferCnt4);
			_Offers[4] = SRmn(Remain_.OfferHo5, Remain_.OfferRem5, Remain_.OfferCnt5);

			_Bids[0] = SRmn(Remain_.BidHo1, Remain_.BidRem1, Remain_.BidCnt1);
			_Bids[1] = SRmn(Remain_.BidHo2, Remain_.BidRem2, Remain_.BidCnt2);
			_Bids[2] = SRmn(Remain_.BidHo3, Remain_.BidRem3, Remain_.BidCnt3);
			_Bids[3] = SRmn(Remain_.BidHo4, Remain_.BidRem4, Remain_.BidCnt4);
			_Bids[4] = SRmn(Remain_.BidHo5, Remain_.BidRem5, Remain_.BidCnt5);

			_Offer0 = 0;
			for (auto& i : _Offers)
			{
				if (i.Volume > 0)
					break;

				++_Offer0;
			}

			_Bid0 = 0;
			for (auto& i : _Bids)
			{
				if (i.Volume > 0)
					break;

				++_Bid0;
			}

			// Set _PreRemain ////////////////////////
			if (OrderVolume() < 0)
			{
				for (auto& Remain : _Offers)
				{
					if (Remain.Price == OrderPrice() &&
						Remain.Volume < _PreRemain)
						_PreRemain = Remain.Volume;
				}
			}
			else if (OrderVolume() > 0)
			{
				for (auto& Remain : _Bids)
				{
					if (Remain.Price == OrderPrice() &&
						Remain.Volume < _PreRemain)
						_PreRemain = Remain.Volume;
				}
			}
		}
		bool CStockExchange::CanOrder(void) const
		{
			if (!_PreOrder.IsNull() ||
				_EmptyRemain())
				return false;

			return true;
		}
		bool CStockExchange::OrderBegin(const SOrder& Order_)
		{
			if (!_PreOrder.IsNull() ||
				OrderVolume() != 0 ||
				_EmptyRemain())
				return false;

			if (Order_.Volume == 0)
				return false;

			_OrderProcessTime = _CurTime + _Delay;
			_PreOrder = Order_;
			_Type = _EType::Order;
			_OrderType = Order_.Type;
			_Busy = true;

			Proc();

			return true;
		}
		bool CStockExchange::ChangeBegin(__int64 OrgOrderNo_, const SOrder& Order_)
		{
			if (!_PreOrder.IsNull() ||
				OrderVolume() == 0 ||
				_EmptyRemain())
				return false;

			if (OrderNo() != OrgOrderNo_)
				return false;

			if (Order_.Volume == 0)
				return false;

			if (Order_.Price == OrderPrice())
				return false;

			_OrderedNo = OrgOrderNo_;
			_OrderProcessTime = _CurTime + _Delay;
			_PreOrder = Order_;
			_Type = _EType::Change;
			_OrderType = Order_.Type;
			_Busy = true;

			Proc();

			return true;
		}
		bool CStockExchange::CancelBegin(__int64 OrgOrderNo_, TValue Volume_)
		{
			if (!_PreOrder.IsNull() ||
				OrderVolume() == 0)
				return false;

			_OrderedNo = OrgOrderNo_;
			_OrderProcessTime = _CurTime + _Delay;
			_PreOrder.Volume = Volume_;
			_Type = _EType::Cancel;
			_Busy = true;

			Proc();

			return true;
		}
		void CStockExchange::Proc(void)
		{
			if (_PreOrder.IsNull() ||
				_CurTime < _OrderProcessTime)
				return;

			if (_Busy)
				_Busy = false;

			switch (_Type)
			{
			case _EType::Order:
				_Order();
				_OrderCallback(OrderNo(), OrderPrice(), OrderVolume());
				break;

			case _EType::Change:
				_Change(); // 수량이 2개이상일 경우 수정요.
				_ChangeCallback(_OrderedNo, OrderNo(), OrderPrice(), OrderVolume());
				break;

			case _EType::Cancel:
				_Cancel();
				return;
			}

			if (OrderVolume() > 0 &&
				(_OrderType == EOrderType::시장가 || OrderPrice() >= Offers0().Price)) // rrr 지정가_FOK 에 주문수량의 일부만 거래된 경우 실패 나야하는데 이 로직에서는 거래됨
			{
				_Trade(OrderNo(), Offers0().Price, OrderVolume());
			}
			else if (OrderVolume() < 0 &&
				(_OrderType == EOrderType::시장가 || OrderPrice() <= Bids0().Price)) // rrr 지정가_FOK 에 주문수량의 일부만 거래된 경우 실패 나야하는데 이 로직에서는 거래됨
			{
				_Trade(OrderNo(), Bids0().Price, OrderVolume());
			}
			else if (_OrderType == EOrderType::지정가_FOK)
			{
				auto Volume = OrderVolume();
				auto OrgOrderNo = OrderNo();
				Cancel(OrgOrderNo, Volume);
				_CancelCallback(OrgOrderNo, Volume);
			}
		}
	}
}
