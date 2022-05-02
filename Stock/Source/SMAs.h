// 마지막 SMA 들만 얻을 수 있음.
#pragma once


#include "SMA.h"


namespace rso
{
	namespace stock
	{
		class CSMAs
		{
			vector<CSMA>	_SMAs;

		public:
			CSMAs(const initializer_list<size_t>& Datas_)
			{
				for (auto& it : Datas_)
					_SMAs.emplace_back(CSMA(it));
			}
			inline const size_t size(void) const
			{
				return _SMAs.size();
			}
			void	insert(TValue Data_)
			{
				for (size_t i = 0; i < _SMAs.size(); ++i)
					_SMAs[i].insert(Data_);
			}
			const CSMA& operator[](size_t Index_) const
			{
				return _SMAs[Index_];
			}
		};
	}
}
