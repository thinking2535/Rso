#include "Asset.h"


namespace rso
{
	namespace stock
	{
		void CAsset::Order(__int64 OrderNo_, TValue Price_, TValue Volume_)
		{
			_OrderNo = OrderNo_;
			_OrderPrice = Price_;
			_OrderVolume = Volume_;
		}
		void CAsset::Change(__int64 OrgOrderNo_, __int64 OrderNo_, TValue Price_, TValue Volume_)
		{
			if (_OrderNo != OrgOrderNo_)
				return;

			_OrderNo = OrderNo_;
			_OrderPrice = Price_;
			_OrderVolume = Volume_;
		}
		void CAsset::Cancel(__int64 OrderNo_, TValue Volume_)
		{
			if (_OrderNo != OrderNo_)
				return;

			if (_OrderVolume == 0)
				return;

			if (_OrderVolume > 0)
			{
				if (_OrderVolume > Volume_)
				{
					_OrderVolume -= Volume_;
				}
				else
				{
					_OrderNo = 0;
					_OrderPrice = 0;
					_OrderVolume = 0;
				}
			}
			else
			{
				if (_OrderVolume < Volume_)
				{
					_OrderVolume -= Volume_;
				}
				else
				{
					_OrderNo = 0;
					_OrderPrice = 0;
					_OrderVolume = 0;
				}
			}
		}
		void CAsset::Trade(__int64 OrderNo_, TValue Price_, TValue Volume_)
		{
			if (_OrderNo != OrderNo_)
				return;

			_OpenInterest += Volume_;
			_Asset -= (Price_ * Volume_);
			_OrderVolume -= Volume_;

			if (_OrderVolume == 0)
			{
				_OrderPrice = 0;
				++_TradeCnt;
			}
		}
		int CAsset::TradeCnt(void) const
		{
			return _TradeCnt;
		}
		void CAsset::ClearTradeCnt(void)
		{
			_TradeCnt = 0;
		}
		void CAsset::clear(void)
		{
			_Asset = 0;
			_OrderNo = 0;
			_OrderVolume = 0;
			_OrderPrice = 0;
			_OpenInterest = 0;
		}
	}
}
