#pragma once


#include "Base.h"
#include <functional>


namespace rso
{
	namespace gameutil
	{
		using namespace std;

		template<typename TPoint, typename TData>
		class CRanking : public multimap<TPoint, TData, greater<TPoint>>
		{
        public:
            struct ranking_point
            {
                size_t Ranking;
                TPoint Point;
            };

        private:
            using _TRankingValueType = pair<ranking_point, TData>;
            using _TRanking = vector<_TRankingValueType>;

            _TRanking _ranking(function<void(size_t&, size_t)> op_) const
			{
                auto it = begin();
                if (it == end())
                    return _TRanking();

                TPoint OldPoint = it->first;
                size_t n = 0;
                size_t r = 0;
                _TRanking Ranking;

                Ranking.emplace_back(_TRankingValueType(ranking_point{ r, it->first }, it->second));
                for (++it; it != end(); ++it)
                {
                    ++n;
                    if (it->first != OldPoint)
                        op_(r, n);

                    OldPoint = it->first;
                    Ranking.emplace_back(_TRankingValueType(ranking_point{ r, it->first }, it->second));
                }

                return Ranking;
			}

        public:
            _TRanking ranking(void) const
            {
                return _ranking([](size_t& r_, size_t n_) { r_ = n_; });
            }
            _TRanking dense_ranking(void) const
            {
                return _ranking([](size_t& r_, size_t n_) { ++r_; });
            }
        };
	}
}