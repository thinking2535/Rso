#pragma once


#include "Base.h"


namespace rso
{
	namespace util
	{
		using namespace std;

		template<typename TValue>
		class CRandomizer
		{
		private:
			using _TDatas = map<__int32, TValue>;

			__int32 _SumRate{ 0 };
			_TDatas _Datas;

		public:
			bool	insert(__int32 Ratio_, const TValue& Value_)
			{
				if (Ratio_ <= 0)
					return false;

				try
				{
					auto ib = _Datas.emplace(_SumRate + Ratio_, Value_);
					if (!ib.second)
						return false;

					_SumRate += Ratio_;

					return true;
				}
				catch (...)
				{
					return false;
				}
			}
			size_t size(void) const
			{
				return _Datas.size();
			}
			const TValue& get(void) const
			{
				// _SumRate == 0 인경우는 외부 책임
				return _Datas.upper_bound(rand() % _SumRate)->second;
			}
		};
	}
}