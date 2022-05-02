#pragma once


#include "Base.h"
#include <map>
#include <vector>


namespace rso
{
	namespace math
	{
		using namespace std;

		template<typename _TType>
		class CDataBound
		{
		public:
			typedef	multimap<_TType, std::size_t>	TBoundary;

		private:
			typedef	vector<_TType>		TDatas;

			TDatas		_Datas;
			TBoundary	_Boundary;		// 데이터와 데이터의인덱스를 키과 값으로 갖는 맵.

		public:
			CDataBound() {}
			~CDataBound() {}

			bool		Resize(std::size_t Size_);
			void		Push(_TType Data_);
			bool		CalcBoundary(std::size_t BoundCnt_);	// BoundCnt_는 크고 데이터의 편차가 작은 경우 구분불가하여 가장 작은 인덱스가 반환됨
			std::size_t	GetOrder(_TType Value_) const;
			TBoundary	GetBoundary(void) const { return _Boundary; }
		};

		template<typename _TType>
		bool		CDataBound<_TType>::Resize(std::size_t Size_)
		{
			try
			{
				_Datas.resize(Size_);
			}
			catch (...)
			{
				return false;
			}

			return true;
		}

		template<typename _TType>
		void		CDataBound<_TType>::Push(_TType Data_)
		{
			try
			{
				_Datas.push_back(Data_);
			}
			catch (...)
			{
			}
		}

		template<typename _TType>
		bool		CDataBound<_TType>::CalcBoundary(std::size_t BoundCnt_)
		{
			if (BoundCnt_ <= 0)
				return false;

			if (_Datas.size() == 0)
				return false;

			try
			{
				std::sort(_Datas.begin(), _Datas.end());
			}
			catch (...)
			{
				return false;
			}

			_Boundary.clear();


			std::size_t Index = 0;
			TBoundary::iterator	it;
			for (std::size_t Order = 0; Order < BoundCnt_; ++Order)
			{
				Index = _Datas.size() * Order / BoundCnt_;

				try
				{
					it = _Boundary.insert(TBoundary::value_type(_Datas[Index], Order));
					if (it == _Boundary.end()) return false;
				}
				catch (...)
				{
					_Boundary.clear();
					return false;
				}
			}

			return true;
		}

		template<typename _TType>
		std::size_t		CDataBound<_TType>::GetOrder(_TType Value_) const
		{
			TBoundary::const_iterator it = _Boundary.upper_bound(Value_);
			if (it == _Boundary.end())
			{
				TBoundary::const_reverse_iterator rit = _Boundary.rbegin();
				if (rit == _Boundary.rend()) return 0;

				return _Boundary.size();
			}

			return it->second;
		}
	}
}
