#include "Asset.h"


namespace rso
{
	namespace stock
	{
		CAsset::TData	CAsset::_Deposit(void) const
		{
			TData Deposit = 0;

			for (auto& i : _Offers)
				Deposit += _SellDeposit(i.first, i.second);

			for (auto& i : _Bids)
				Deposit += _BuyDeposit(i.first, i.second);

			for (auto& i : _Opens)
			{
				if (i.second < 0)
					Deposit += _SellDeposit(i.first, -i.second);
			}

			return Deposit;
		}
		CAsset::TData	CAsset::_UsableAsset(void) const
		{
			return _Asset - _Deposit();
		}
		void	CAsset::_Buy(TData Data_, TData Cnt_)
		{
			auto it = _Opens.begin();
			if (it != _Opens.end())
			{
				if (it->second < 0)
				{
					TData Cnt = 0;

					for (; it != _Opens.end() && Cnt_ > 0; ++it)
					{
						if (Cnt_ >= -it->second)
						{
							Cnt = -it->second;
							Cnt_ += it->second;
						}
						else
						{
							Cnt = Cnt_;
							Cnt_ = 0;
						}

						_Asset += ((it->first - Data_) * Cnt);

						if (Cnt < -it->second)
						{
							it->second += Cnt;
							break;
						}
					}

					_Opens.erase(_Opens.begin(), it);
				}
			}

			if (Cnt_ > 0)
			{
				auto ib = _Opens.emplace(Data_, Cnt_);
				if (!ib.second)
					ib.first->second += Cnt_;
			}
		}
		void	CAsset::_Sell(TData Data_, TData Cnt_)
		{
			auto it = _Opens.begin();
			if (it != _Opens.end())
			{
				if (it->second > 0)
				{
					TData Cnt = 0;

					for (; it != _Opens.end() && Cnt_ > 0; ++it)
					{
						if (Cnt_ >= it->second)
						{
							Cnt = it->second;
							Cnt_ -= it->second;
						}
						else
						{
							Cnt = Cnt_;
							Cnt_ = 0;
						}

						_Asset += ((Data_ - it->first) * Cnt);

						if (Cnt < it->second)
						{
							it->second -= Cnt;
							break;
						}
					}

					_Opens.erase(_Opens.begin(), it);
				}
			}

			if (Cnt_ > 0)
			{
				auto ib = _Opens.emplace(Data_, -Cnt_);
				if (!ib.second)
					ib.first->second -= Cnt_;
			}
		}
		void	CAsset::OtherBuy(TData Data_, TData Offer_, TData Bid_)
		{
			_Cur = Data_;
			_Offer = Offer_;
			_Bid = Bid_;

			auto itEnd = _Offers.lower_bound(Data_);
			for (auto it = _Offers.begin(); it != itEnd; ++it)
				_Sell(it->first, it->second);

			_Offers.erase(_Offers.begin(), itEnd);
		}
		void	CAsset::OtherSell(TData Data_, TData Offer_, TData Bid_)
		{
			_Cur = Data_;
			_Offer = Offer_;
			_Bid = Bid_;

			auto itBegin = _Bids.upper_bound(Data_);
			for (auto it = itBegin; it != _Bids.end(); ++it)
				_Buy(it->first, it->second);

			_Bids.erase(itBegin, _Bids.end());
		}
		bool	CAsset::Buy(TData Data_, TData Cnt_)
		{
			if (Cnt_ <= 0)
				return false;

			if (_UsableAsset() < _BuyDeposit(Data_, Cnt_))
				return false;

			if (Data_ >= _Offer)
			{
				_Buy(_Offer, Cnt_);
			}
			else
			{
				auto ib = _Bids.emplace(Data_, Cnt_);
				if (!ib.second)
					ib.first->second += Cnt_;
			}

			return true;
		}
		bool	CAsset::Sell(TData Data_, TData Cnt_)
		{
			if (Cnt_ <= 0)
				return false;

			if (_UsableAsset() < _SellDeposit(Data_, Cnt_))
				return false;

			if (Data_ <= _Bid)
			{
				_Sell(_Bid, Cnt_);
			}
			else
			{
				auto ib = _Offers.emplace(Data_, Cnt_);
				if (!ib.second)
					ib.first->second += Cnt_;
			}

			return true;
		}
		CAsset::TData	CAsset::Gain(void) const
		{
			TData Sum = 0;
			TData Cnt = 0;

			for (auto& it : _Opens)
			{
				Sum += (it.first * it.second);
				Cnt += it.second;
			}

			if (Cnt > 0)
			{
				return (_Cur * Cnt) - Sum;
			}
			else if (Cnt < 0)
			{
				return Sum + (_Cur * Cnt);
			}
			else
			{
				return 0;
			}
		}
		CAsset::TData	CAsset::Asset(void) const
		{
			return _Asset + Gain();
		}
		void	CAsset::Cancel(TData Data_, TData Cnt_)
		{
			auto it = _Offers.find(Data_);
			if (it != _Offers.end())
			{
				if (it->second > Cnt_)
					it->second -= Cnt_;
				else
					_Offers.erase(it);
			}
			else
			{
				it = _Bids.find(Data_);
				if (it != _Bids.end())
				{
					if (it->second > Cnt_)
						it->second -= Cnt_;
					else
						_Bids.erase(it);
				}
			}
		}
		void	CAsset::CancelAll(void)
		{
			_Offers.clear();
			_Bids.clear();
		}
	}
}
