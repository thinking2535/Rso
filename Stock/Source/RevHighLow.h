#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		using namespace std;

		class CRevHighLow
		{
			vector<SHighLow> _Datas;
			size_t	_Size = 0;

		public:
			size_t	size(void) const
			{
				return _Size;
			}
			void set(const vector<SDataDb>& Datas_)
			{
				_Datas.clear();
				_Size = Datas_.size();

				_Datas.emplace_back(
					SHighLow(
						STimePrice(DateTimeToElapsedSecFromMidNight(Datas_.back().Time), Datas_.back().High),
						STimePrice(DateTimeToElapsedSecFromMidNight(Datas_.back().Time), Datas_.back().Low)
						)
					);

				for (size_t i = Datas_.size() - 1; i > 0; --i)
				{
					_Datas.push_back(_Datas.back());

					if (_Datas.back().High.Price <= Datas_[i - 1].High)
					{
						_Datas.back().High.Time = DateTimeToElapsedSecFromMidNight(Datas_[i - 1].Time);
						_Datas.back().High.Price = Datas_[i - 1].High;
					}
					if (_Datas.back().Low.Price >= Datas_[i - 1].Low)
					{
						_Datas.back().Low.Time = DateTimeToElapsedSecFromMidNight(Datas_[i - 1].Time);
						_Datas.back().Low.Price = Datas_[i - 1].Low;
					}
				}
			}
			const SHighLow&	pop(void)
			{
				return _Datas[--_Size];
			}
		};
	}
}