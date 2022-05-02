#include "CurData.h"


namespace rso
{
	namespace stock
	{
		void	CCurData::set(const SDataDb& Data_)
		{
			_OldData = _CurData;
			_CurData = Data_;

			if (_CurData.Close > _OldData.Close)
			{
				_OfferPrice = _CurData.Close;
				_BidPrice = _CurData.Close - 5;
			}
			else if (_CurData.Close < _OldData.Close)
			{
				_OfferPrice = _CurData.Close + 5;
				_BidPrice = _CurData.Close;
			}
		}
		const SDataDb& CCurData::get(void) const
		{
			return _CurData;
		}
		TValue	CCurData::OfferPrice(void) const
		{
			return _OfferPrice;
		}
		TValue	CCurData::BidPrice(void) const
		{
			return _BidPrice;
		}
		TValue	CCurData::Price(void) const
		{
			return _CurData.Close;
		}
		TValue	CCurData::TradePrice(int Direction_) const
		{
			return (Direction_ > 0 ? _OfferPrice : _BidPrice);
		}
	}
}