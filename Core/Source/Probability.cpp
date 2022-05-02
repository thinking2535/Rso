#include "Probability.h"


namespace rso
{
	namespace core
	{
		using namespace std;

		bool CProbability::Check(void)
		{
			if (_Overflow ||
				(_HitCnt == 0 && _MissCnt == 0))
			{
				return (double(rand() % 10000) / 100.0 < _Probability);
			}
			else
			{
				auto Diff = double(_HitCnt) * 100.0 / double(_HitCnt + _MissCnt) - _Probability;
				if (Diff > _Precision)
					return false;
				else if (Diff < -_Precision)
					return true;
				else
					return (double(rand() % 10000) / 100.0 < _Probability);
			}
		}

		bool CProbability::Proc(void)
		{
			if (Check())
			{
				if (_HitCnt + 1 > _HitCnt)
					++_HitCnt;
				else
					_Overflow = true;

				return true;
			}
			else
			{
				if (_MissCnt + 1 > _MissCnt)
					++_MissCnt;
				else
					_Overflow = true;

				return false;
			}
		}

		void CProbability::Clear(void)
		{
			_Overflow = false;
			_HitCnt = 0;
			_MissCnt = 0;
		}
	}
}
