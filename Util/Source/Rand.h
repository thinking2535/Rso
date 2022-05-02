#pragma once


#include "Base.h"
#include <Rso/Win/Win.h>


namespace rso
{
	namespace util
	{
		using namespace win;

		class CRand
		{
		private:
			uint32 _Seed;
			uint32 _Square;
			uint32 _Max;
			uint32 _Counter;
			uint32 _XorBit;
			uint32 _aFact[sizeof(uint32) << 3];

			uint32 _GetSquare(uint32 uiNum_)
			{
				uint32 Square = 0;
				for (; (uint32)(1 << Square) < uiNum_; ++Square);
				return Square;
			}

			void _Clear(void);

		public:
			CRand();
			~CRand();

			void Clear(void);
			uint32 RandSq(uint32 Square_);
			uint32 RandMx(uint32 Max_); // Divide Mode
			uint32 RandMxP(uint32 Max_); // Repeat Mode
		};

		template<typename _TType>
		class CRand64
		{
			static_assert(
				is_same<_TType, char>::value ||
				is_same<_TType, unsigned char>::value ||
				is_same<_TType, int8>::value ||
				is_same<_TType, uint8>::value ||
				is_same<_TType, int16>::value ||
				is_same<_TType, uint16>::value ||
				is_same<_TType, int32>::value ||
				is_same<_TType, uint32>::value ||
				is_same<_TType, int64>::value ||
				is_same<_TType, uint64>::value ||
				is_same<_TType, const char>::value ||
				is_same<_TType, const unsigned char>::value ||
				is_same<_TType, const int8>::value ||
				is_same<_TType, const uint8>::value ||
				is_same<_TType, const int16>::value ||
				is_same<_TType, const uint16>::value ||
				is_same<_TType, const int32>::value ||
				is_same<_TType, const uint32>::value ||
				is_same<_TType, const int64>::value ||
				is_same<_TType, const uint64>::value
				, "Invalid Type");

		private:
			_TType _Seed;
			_TType _Square;
			_TType _Max;
			_TType _Counter;
			_TType _XorBit;
			_TType _aFact[sizeof(_TType) << 3];

			_TType _GetSquare(_TType Num_)
			{
				_TType Square = 0;
				for (; (_TType)(1 << Square) < Num_; ++Square);
				return Square;
			}

			void _Clear(void);

		public:
			CRand64();
			~CRand64();

			void Clear(void);
			_TType RandSq(_TType Square_);
			_TType RandMx(_TType Max_); // Divide Mode
			_TType RandMxP(_TType Max_); // Repeat Mode
		};

		template<typename _TType>
		void CRand64<_TType>::_Clear(void)
		{
			_Seed = Rand((_TType)(1 << _Square));

			// Init Fact
			_aFact[0] = 0x0000000000000001;
			for (_TType n = 1; n < _Square; ++n)
				_aFact[n] = (_TType)(_aFact[n - 1] << 1);

			// Randomize Fact
			_TType SwapIndex = 0;
			for (_TType n = 0; n + 1 < _Square; ++n)
			{
				SwapIndex = Rand(_Square);
				if (SwapIndex == n)
					continue;

				Swap(_aFact[n], _aFact[SwapIndex]);
			}
		}

		template<typename _TType>
		CRand64<_TType>::CRand64() :
			_Seed(),
			_Square(),
			_Max(),
			_Counter(),
			_XorBit()
		{
		}

		template<typename _TType>
		CRand64<_TType>::~CRand64()
		{
		}

		template<typename _TType>
		void CRand64<_TType>::Clear(void)
		{
			_Counter = 0;
			_Clear();
		}

		template<typename _TType>
		_TType CRand64<_TType>::RandSq(_TType Square_)
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
			for (_TType Bit = 1, FactNum = 0; FactNum < _Square; Bit <<= 1, ++FactNum)
			{
				if (Bit&_XorBit)
					_Seed ^= _aFact[FactNum];
			}

			if (_Counter + 1 == (_TType)(1 << _Square))
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

		template<typename _TType>
		_TType CRand64<_TType>::RandMx(_TType Max_)
		{
			if (Max_ == 0)
				return 0;

			if (Max_ != _Max)
			{
				_Counter = 0;
				_Max = Max_;
			}

			_TType Square = _GetSquare(Max_);

			return RandSq(Square) * Max_ / (_TType)(1 << Square);
		}

		template<typename _TType>
		_TType CRand64<_TType>::RandMxP(_TType Max_)
		{
			if (Max_ == 0)
				return 0;

			if (Max_ != _Max)
			{
				_Counter = 0;
				_Max = Max_;
			}

			_TType Square = _GetSquare(Max_);

			_TType Ret = 0;
			for (Ret = RandSq(Square);
				Ret >= Max_;
				Ret = RandSq(Square));

			return Ret;
		}
	}
}