#pragma once


#include "Base.h"
#include <map>


namespace rso
{
	namespace stock
	{
		using namespace base;
		using namespace std;

		class _CAsset
		{
		public:
			using TData = __int64;

		private:
			using _TDatas = map<TData, TData>;	// P_e, Cnt

			TData	_UpLimit{ 0 };
			TData	_Asset{ 0 };
			TData	_Cur{ 0 };
			TData	_Offer{ 0 };
			TData	_Bid{ 0 };
			_TDatas	_Offers;
			_TDatas	_Bids;
			_TDatas	_Opens;

			TData	_BuyDeposit(TData Data_, TData Cnt_) const { return Data_ * Cnt_; }
			TData	_SellDeposit(TData Data_, TData Cnt_) const { return (_UpLimit - Data_) * Cnt_; }
			TData	_Deposit(void) const;
			TData	_UsableAsset(void) const;
			void	_Buy(TData Data_, TData Cnt_);
			void	_Sell(TData Data_, TData Cnt_);

		public:
			CAsset(TData UpLimit_, TData Cur_, TData Offer_, TData Bid_, TData Asset_) :
				_UpLimit(UpLimit_), _Cur(Cur_), _Offer(Offer_), _Bid(Bid_), _Asset(Asset_)
			{
			}
			void	SetUpLimit(TData UpLimit_)
			{
				_UpLimit = UpLimit_;
			}
			void	OtherBuy(TData Data_, TData Offer_, TData Bid_);
			void	OtherSell(TData Data_, TData Offer_, TData Bid_);
			bool	Buy(TData Data_, TData Cnt_);
			bool	Sell(TData Data_, TData Cnt_);
			TData	Gain(void) const;
			TData	Asset(void) const;
			void	Cancel(TData Data_, TData Cnt_);
			void	CancelAll(void);
		};
	}
}
