// MinAmp 에 의한 최근 추세
#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		template<typename TData>
		class CTrend
		{
			TData _MinAmp{};
			TData _LastData0{};
			TData _LastData1{};
			TData _LastData{};

		public:
			CTrend(TData MinAmp_) :
				_MinAmp(MinAmp_)
			{
			}
			void init(const TData& Data_)
			{
				_LastData0 = _LastData1 = _LastData = Data_;
			}
			void push(const TData& Data_)
			{
				if (_LastData1 - _LastData0 > TData{})
				{
					if (Data_ > _LastData1)
					{
						_LastData1 = Data_;
					}
					else if(_LastData1 - Data_ > _MinAmp)
					{
						_LastData0 = _LastData1;
						_LastData1 = Data_;
					}
					else if (Data_ == _LastData)
					{
						return;
					}
				}
				else if (_LastData1 - _LastData0 < TData{})
				{
					if (Data_ < _LastData1)
					{
						_LastData1 = Data_;
					}
					else if (Data_ - _LastData1 > _MinAmp)
					{
						_LastData0 = _LastData1;
						_LastData1 = Data_;
					}
					else if (Data_ == _LastData)
					{
						return;
					}
				}
				else
				{
					if (Data_ - _LastData1 > _MinAmp ||
						_LastData1 - Data_ > _MinAmp)
					{
						_LastData0 = _LastData1;
						_LastData1 = Data_;
					}
					else if (Data_ == _LastData)
					{
						return;
					}
				}

				_LastData = Data_;
			}
			TData MinAmp(void) const
			{
				return _MinAmp;
			}
			TData Trend(void) const
			{
				return (_LastData - _LastData0);
			}
		};
	}
}
