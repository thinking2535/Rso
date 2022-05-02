#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		template<typename _TOperator>
		class CExtremeValue
		{
			using _TType = typename _TOperator::first_argument_type;

			_TType _Extreme0;
			_TType _Extreme1;
			_TType _Gap;
			_TType _AccVolume;
			_TType _Volume;

		public:
			void reset(const _TType& Value_, const _TType& Volume_)
			{
				_Extreme0 = Value_;
				_Extreme1 = Value_;
				_Gap = _TType{};
				_AccVolume = Volume_;
			}
			void set(const _TType& Value_, const _TType& Volume_)
			{
				if (_TOperator()(Value_, _Extreme0))
				{
					_Gap = _Extreme0 - _Extreme1;
					_Volume = _AccVolume + Volume_;
					_Extreme0 = _Extreme1 = Value_;
					_AccVolume = Volume_;
				}
				else
				{
					_AccVolume += Volume_;

					if (_Gap != _TType{})
						_Gap = _TType{};
				}

				if (_TOperator()(_Extreme1, Value_))
					_Extreme1 = Value_;
			}
			const _TType& gap(void) const
			{
				return _Gap;
			}
			const _TType& volume(void) const
			{
				return _Volume;
			}
		};
	}
}
