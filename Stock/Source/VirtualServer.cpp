#include "VirtualServer.h"

namespace rso
{
	namespace stock
	{
		bool CVirtualServer::_CheckPrice(EOrderType OrderType_, int32 Price_)
		{
			if (!_IsMarketPriceOrder(OrderType_) &&
				(Price_ % c_PriceGap != 0 || Price_ <= 0))
			{
				_ErrorCallback(StringFormat("Invalid Price[%d]", Price_));
				return false;
			}

			return true;
		}
		bool CVirtualServer::_CheckVolume(int32 Volume_)
		{
			if (Volume_ == 0)
			{
				_ErrorCallback("Volume must not be 0");
				return false;
			}

			return true;
		}
		void CVirtualServer::_CheckTrade(_TOrders::iterator it_)
		{
			// 실제로는 잔량에 따라 거래가 될지 결정되지만 여기서는 매도, 매수 1호가에 무한대의 잔량이 존재한다고 가정
			auto OrderPrice = std::get<1>(it_->first)->first;
			if (it_->second > 0)
			{
				if (OrderPrice < _SellPrice1)
					return;

				if (OrderPrice > _SellPrice1)
					OrderPrice = _SellPrice1;
			}
			else
			{
				if (OrderPrice > _BuyPrice1)
					return;

				if (OrderPrice < _BuyPrice1)
					OrderPrice = _BuyPrice1;
			}

			auto OrderNo = std::get<0>(it_->first)->first;
			auto Volume = it_->second;
			_Orders.erase(it_);
			_TradeCallback(OrderNo, SOrder(OrderPrice, Volume));
		}
		bool CVirtualServer::_IsMarketPriceOrder(EOrderType OrderType_) const
		{
			return (OrderType_ == EOrderType::시장가 || OrderType_ == EOrderType::시장가_IOC || OrderType_ == EOrderType::시장가_FOK);
		}
		int32 CVirtualServer::_GetOrderPrice(EOrderType OrderType_, const SOrder& Order_)
		{
			if (_IsMarketPriceOrder(OrderType_))
			{
				if (Order_.Volume > 0)
					return _SellPrice1;
				else
					return _BuyPrice1;
			}
			else
			{
				return Order_.Price;
			}
		}
		void CVirtualServer::_Tick(void)
		{
			_TickCallback(_AllDatas.at(_Size));

			if (_AllDatas.at(_Size).Volume > 0)
			{
				_SellPrice1 = _AllDatas.at(_Size).Price;
				_BuyPrice1 = _SellPrice1 - c_PriceGap;

				auto itEnd = _Orders.operator()<1>().upper_bound(_SellPrice1);
				for (auto it = _Orders.operator()<1>().begin(); it != itEnd;)
				{
					auto itCheck = it;
					++it;
					auto itOrders = _Orders.get(itCheck->second);
					if (itOrders->second < 0)
						_CheckTrade(itOrders);
				}
			}
			else
			{
				_BuyPrice1 = _AllDatas.at(_Size).Price;
				_SellPrice1 = _BuyPrice1 + c_PriceGap;

				for (auto it = _Orders.operator() <1>().lower_bound(_BuyPrice1); it != _Orders.operator() <1>().end();)
				{
					auto itCheck = it;
					++it;
					auto itOrders = _Orders.get(itCheck->second);
					if (itOrders->second > 0)
						_CheckTrade(itOrders);
				}
			}

			++_Size;
		}
		void CVirtualServer::_Remain(void)
		{
			_RemainCallback(_AllRemainDatas.at(_RemainSize));
			++_RemainSize;
		}
		CVirtualServer::CVirtualServer(TNewCallback NewCallback_, TTradeCallback TradeCallback_, TTickCallback TickCallback_, TRemainCallback RemainCallback_, TErrorCallback ErrorCallback_) :
			_NewCallback(NewCallback_),
			_TradeCallback(TradeCallback_),
			_TickCallback(TickCallback_),
			_RemainCallback(RemainCallback_),
			_ErrorCallback(ErrorCallback_)
		{
		}
		TTime CVirtualServer::Start(void)
		{
			if (_State != EState::Stopped)
				return TTime();

			if (_AllDatas.empty())
				return TTime();

			// 1초단위로 뭉쳐있는 잔량 데이터를 분산
			if (!_AllRemainDatas.empty())
			{
				system_clock::time_point CheckTime(t_duration(_AllRemainDatas.front().TimePoint.time_since_epoch().count() - (_AllRemainDatas.front().TimePoint.time_since_epoch().count() % 10000000)));
				size_t FixedEndIndex = 0;
				for (size_t i = 0; i < _AllRemainDatas.size();)
				{
					if (_AllRemainDatas.at(i).TimePoint < (CheckTime + seconds(1)))
					{
						++i;
						continue;
					}

					for (auto c = FixedEndIndex; c < i; ++c)
						_AllRemainDatas.at(c).TimePoint = CheckTime + (microseconds(1000000) / (i - FixedEndIndex) * (c - FixedEndIndex));

					FixedEndIndex = i;
					CheckTime += seconds(1);
				}

				for (auto c = FixedEndIndex; c < _AllRemainDatas.size(); ++c)
					_AllRemainDatas.at(c).TimePoint = CheckTime + (microseconds(1000000) / (_AllRemainDatas.size() - FixedEndIndex) * (c - FixedEndIndex));
			}

			// 1초단위로 뭉쳐있는 데이터를 분산
			{
				system_clock::time_point CheckTime(t_duration(_AllDatas.front().TimePoint.time_since_epoch().count() - (_AllDatas.front().TimePoint.time_since_epoch().count() % 10000000)));
				size_t FixedEndIndex = 0;
				for (size_t i = 0; i < _AllDatas.size();)
				{
					if (_AllDatas.at(i).TimePoint < (CheckTime + seconds(1)))
					{
						++i;
						continue;
					}

					for (auto c = FixedEndIndex; c < i; ++c)
						_AllDatas.at(c).TimePoint = CheckTime + (microseconds(1000000) / (i - FixedEndIndex) * (c - FixedEndIndex));

					FixedEndIndex = i;
					CheckTime += seconds(1);
				}

				for (auto c = FixedEndIndex; c < _AllDatas.size(); ++c)
					_AllDatas.at(c).TimePoint = CheckTime + (microseconds(1000000) / (_AllDatas.size() - FixedEndIndex) * (c - FixedEndIndex));
			}
			//////////////////////////////////////////

			_State = EState::Started;
			return _AllDatas.front().TimePoint;
		}
		void CVirtualServer::Stop(void) noexcept
		{
			_AllDatas.clear();
			_Size = 0;
			_RemainSize = 0;
			_State = EState::Stopped;
		}
		bool CVirtualServer::Pause(void)
		{
			if (_State != EState::Started)
				return false;

			_State = EState::Paused;
			return true;
		}
		bool CVirtualServer::Resume(void)
		{
			if (_State != EState::Paused)
				return false;

			_State = EState::Started;
			return true;
		}
		void CVirtualServer::Proc(const TTime& Time_)
		{
			if (_State != EState::Started)
				return;

			// 아래 단순화 시킬것.
			while (true)
			{
				if (_Size < _AllDatas.size())
				{
					if (_RemainSize < _AllRemainDatas.size())
					{
						if (_AllDatas.at(_Size).TimePoint < _AllRemainDatas.at(_RemainSize).TimePoint)
						{
							if (_AllDatas.at(_Size).TimePoint <= Time_)
								_Tick();
							else
								break;
						}
						else
						{
							if (_AllRemainDatas.at(_RemainSize).TimePoint <= Time_)
								_Remain();
							else
								break;
						}
					}
					else
					{
						if (_AllDatas.at(_Size).TimePoint <= Time_)
							_Tick();
						else
							break;
					}
				}
				else if (_RemainSize < _AllRemainDatas.size())
				{
					if (_AllRemainDatas.at(_RemainSize).TimePoint <= Time_)
						_Remain();
					else
						break;
				}
				else
				{
					break;
				}
			}
		}
		bool CVirtualServer::New(EOrderType OrderType_, const SOrder& Order_)
		{
			if (!_CheckPrice(OrderType_, Order_.Price) ||
				!_CheckVolume(Order_.Volume))
				return false;

			++_OrderCounter;
			auto it = _Orders.emplace(_OrderCounter, Order_.Volume, _GetOrderPrice(OrderType_, Order_));
			_NewCallback(_OrderCounter, Order_);
			_CheckTrade(it);
			return true;
		}
		bool CVirtualServer::Modify(int32 OriginOrderNo_, EOrderType OrderType_, int32 Price_)
		{
			if (!_CheckPrice(OrderType_, Price_))
				return false;

			auto it = _Orders.find<0>(OriginOrderNo_);
			if (it == _Orders.end<0>())
			{
				_ErrorCallback("Invalid OriginOrderNo");
				return false;
			}

			// 조정된 가격이 기존 가격과 같으면 에러
			if (!_IsMarketPriceOrder(OrderType_) &&
				Price_ == std::get<1>(_Orders.get(it->second)->first)->first)
			{
				_ErrorCallback("Price is same with origin");
				return false;
			}

			auto Volume = _Orders[it->second];
			_Orders.erase(it->second);
			++_OrderCounter;
			SOrder Order(Price_, Volume.second);
			auto itNewOrder = _Orders.emplace(_OrderCounter, _GetOrderPrice(OrderType_, Order), Volume.second);
			_NewCallback(_OrderCounter, Order);
			_NewCallback(OriginOrderNo_, SOrder());
			_CheckTrade(itNewOrder);
			return true;
		}
		bool CVirtualServer::Cancel(int32 OriginOrderNo_)
		{
			auto it = _Orders.find<0>(OriginOrderNo_);
			if (it == _Orders.end<0>())
			{
				_ErrorCallback("Invalid OriginOrderNo");
				return false;
			}

			_Orders.erase(it->second);
			_NewCallback(OriginOrderNo_, SOrder());
			return true;
		}
	}
}
