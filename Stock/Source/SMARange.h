#pragma once


#include "SMA.h"


namespace rso
{
	namespace stock
	{
		class CSMARange
		{
		private:
			CSMA _SMA;
			CRing<double> _Datas;

		public:
			CSMARange(size_t SMACount_, size_t Count_) :
				_SMA(SMACount_),
				_Datas(Count_)
			{
			}
			inline const size_t size(void) const
			{
				return _Datas.size();
			}
			inline const size_t capacity(void) const
			{
				return _Datas.capacity();
			}
			void insert(TValue Value_)
			{
				_SMA.insert(Value_);
				_Datas.emplace(_SMA.avg());
			}
			bool full(void) const
			{
				return (_SMA.full() && _Datas.full());
			}
			double operator[](size_t Index_) const
			{
				return _Datas.reverse_at(Index_);
			}
		};
	}
}
