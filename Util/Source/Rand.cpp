#include "Rand.h"


namespace rso
{
	namespace util
	{
		void CRand::_Clear(void)
		{
			_Seed = Rand((uint32)(1 << _Square));

			// Init Fact
			_aFact[0] = 0x00000001;
			for (uint32 ui = 1; ui < _Square; ++ui)
				_aFact[ui] = (uint32)(_aFact[ui - 1] << 1);

			// Randomize Fact
			uint32 SwapIndex = 0;
			for (uint32 ui = 0; ui + 1 < _Square; ++ui)
			{
				SwapIndex = Rand(_Square);
				if (SwapIndex == ui)
					continue;

				swap(_aFact[ui], _aFact[SwapIndex]);
			}
		}

		CRand::CRand() :
			_Seed(),
			_Square(),
			_Max(),
			_Counter(),
			_XorBit()
		{
		}

		CRand::~CRand()
		{
		}

		void CRand::Clear(void)
		{
			_Counter = 0;
			_Clear();
		}

		uint32 CRand::RandSq(uint32 Square_)
		{
			if (Square_ >= 32)
				Square_ = 31;

			if (Square_ != _Square)
			{
				_Counter = _XorBit = 0;
				_Square = Square_;
			}

			if (_Counter == 0)
				_Clear();


			// _Counter 의 각 비트값에 따라 _Seed의 값을 _aFact값과 XOR 연산
			for (uint32 Bit = 1, FactNum = 0; FactNum < _Square; Bit <<= 1, ++FactNum)
			{
				if (Bit&_XorBit)
					_Seed ^= _aFact[FactNum];
			}

			if (_Counter + 1 == (uint32)(1 << _Square))
			{
				_Counter = _XorBit = 0;
			}
			else
			{
				_XorBit = (_Counter ^ (_Counter + 1));
				++_Counter;
			}

			return _Seed;
		}

		uint32 CRand::RandMx(uint32 Max_)
		{
			if (Max_ == 0)
				return 0;

			if (Max_ != _Max)
			{
				_Counter = 0;
				_Max = Max_;
			}

			uint32 Square = _GetSquare(Max_);

			return RandSq(Square) * Max_ / (uint32)(1 << Square);
		}

		uint32 CRand::RandMxP(uint32 Max_)
		{
			if (Max_ == 0)
				return 0;

			if (Max_ != _Max)
			{
				_Counter = 0;
				_Max = Max_;
			}

			uint32 Square = _GetSquare(Max_);

			uint32 uiRet = 0;
			for (uiRet = RandSq(Square);
				uiRet >= Max_;
				uiRet = RandSq(Square));

			return uiRet;
		}
	}
}
