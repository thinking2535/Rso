#pragma once


#include <map>
#include <functional>


namespace rso
{
	namespace gameutil
	{
		using namespace std;

		template<typename TCount, typename TValue>
		class CDevider
		{
			struct _SValueCnt
			{
				const TValue& Value;
				TCount	Count = 0;

				_SValueCnt(const TValue& Value_, TCount Count_) :
					Value(Value_), Count(Count_)
				{}
			};

			map<TCount, TValue, std::greater<TCount>> _Datas;

		public:
			bool	insert(const TCount& Count_, const TValue& Value_)
			{
				return _Datas.emplace(Count_, Value_).second;
			}
			list<_SValueCnt>	get(TCount Count_) const
			{
				list<_SValueCnt> l;

				try
				{
					for (auto& i : _Datas)
					{
						if (Count_ == 0)
							break;

						if (Count_ / i.first > TCount{})
						{
							l.emplace_back(i.second, Count_ / i.first);
							Count_ %= i.first;
						}
					}
				}
				catch (...)
				{
				}

				return l;
			}
		};
	}
}
