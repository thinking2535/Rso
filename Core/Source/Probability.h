#pragma once


#include "Core.h"


namespace rso
{
	namespace core
	{
		using namespace std;

		class CProbability
		{
			double _Probability{ 0.0 };
			double _Precision{ 0.0 };
			int64 _HitCnt{ 0 };
			int64 _MissCnt{ 0 };
			bool _Overflow{ false };

		public:
			CProbability() {}
			CProbability(double Probability_, double Precision_) :
				_Probability(Probability_), _Precision(Precision_) {}
			CProbability(double Probability_, double Precision_, int64 HitCnt_, int64 MissCnt_) :
				_Probability(Probability_), _Precision(Precision_), _HitCnt(HitCnt_), _MissCnt(MissCnt_) {}

			inline int64& HitCnt(void) { return _HitCnt; }
			inline int64& MissCnt(void) { return _MissCnt; }
			inline double& Probability(void) { return _Probability; }

			bool Check(void);
			bool Proc(void);
			void Clear(void);
		};
	}
}
