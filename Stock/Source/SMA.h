#pragma once


#include "Base.h"
#include <Rso/Base/Ring.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		class CSMA
		{
		private:
			CRing<TValue>	_Datas;
			TValue			_Sum = 0;

		public:
			CSMA(size_t Count_) :
				_Datas(Count_)
			{
			}
			inline const size_t size(void) const
			{
				return _Datas.size();
			}
			inline bool full(void) const
			{
				return _Datas.full();
			}
			inline bool empty(void) const
			{
				return _Datas.empty();
			}
			void insert(TValue Value_)
			{
				if (_Datas.full())
					_Sum -= _Datas.front();

				_Datas.emplace(Value_);
				_Sum += _Datas.back();
			}
			inline TValue	sum(void) const
			{
				return _Sum;
			}
			inline double	avg(void) const
			{
				return (double(_Sum) / _Datas.size());
			}
		};
	}
}
