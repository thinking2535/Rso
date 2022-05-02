// 정해진 주기이상이거나, 정해진 진폭 이상을 추출

#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		class CZigZag
		{
		private:
		//	struct SData
		//	{
		//		time_t	TimeT{ 0 };
		//		TValue	Value{ 0 };

		//		SData()
		//		{
		//		}
		//		SData(time_t TimeT_, TValue Value_) :
		//			TimeT(TimeT_), Value(Value_)
		//		{
		//		}

		//		TValue	operator -= (const SData& Data_)
		//		{
		//			return (Value -= Data_.Value);
		//		}
		//		TValue	operator - (const SData& Data_) const
		//		{
		//			auto Me = *this;
		//			return (Me -= Data_);
		//		}
		//		bool	operator == (const SData& Data_) const
		//		{
		//			return (Value == Data_.Value);
		//		}
		//		bool	operator != (const SData& Data_) const
		//		{
		//			return (Value != Data_.Value);
		//		}
		//		bool	operator < (const SData& Data_) const
		//		{
		//			return (Value < Data_.Value);
		//		}
		//		bool	operator >(const SData& Data_) const
		//		{
		//			return (Value > Data_.Value);
		//		}
		//		bool	operator <= (const SData& Data_) const
		//		{
		//			return (Value <= Data_.Value);
		//		}
		//		bool	operator >= (const SData& Data_) const
		//		{
		//			return (Value >= Data_.Value);
		//		}
		//	};

		//	using _TDatas = vector<SData>;

		//	_TDatas		_Datas;
		//	time_t		_Range{ 0 };
		//	TValue		_MinAmp{ 0 };
		//	SData		_PrevData;

		//public:
		//	CZigZag(time_t Range_, TValue MinAmp_) :
		//		_Range(Range_), _MinAmp(MinAmp_)
		//	{
		//	}

		//	void emplace(const SData& Data_);
		//	inline void	clear(void) { _Datas.clear(); }
		//	inline size_t size(void) const { return _Datas.size(); }
		//	inline time_t Range(void) const{ return _Range; }
		//	inline TValue MinAmp(void) const { return _MinAmp; }
		//	inline const SData&	operator[] (size_t Index_) const { return _Datas[Index_]; }
		//	TValue get(void) const;
		};
	}
}
