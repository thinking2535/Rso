#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		class CCurData
		{
			TValue	_OfferPrice = 0;
			TValue	_BidPrice = 0;
			SDataDb	_OldData;
			SDataDb	_CurData;

		public:
			void	set(const SDataDb& Data_);
			const SDataDb& get(void) const;
			TValue	OfferPrice(void) const;
			TValue	BidPrice(void) const;
			TValue	Price(void) const;
			TValue	TradePrice(int Direction_) const;
		};
	}
}