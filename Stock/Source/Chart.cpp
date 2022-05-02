#include "Chart.h"

namespace rso
{
	namespace stock
	{
		size_t CChart::_Index(size_t Index_) const
		{
			return (_StartIndex + Index_) % _Datas.size();
		}
		optional<SChartTime> CChart::_Add(const SChartTime& Data_)
		{
			for (size_t i = 0; i < _Indices.size(); ++i)
				_Chart[i] -= _Datas[_Index(_Indices[i] - 1)];

			for (size_t i = 0; i < _Indices.size() - 1; ++i)
				_Chart[i + 1] += _Datas[_Index(_Indices[i] - 1)];

			if (_StartIndex > 0)
				--_StartIndex;
			else
				_StartIndex = _Datas.size() - 1;

			optional<SChartTime> PoppedData;

			if (Full())
				PoppedData = _Datas[_StartIndex];
			else
				++_Size;

			_Datas[_StartIndex] = Data_;
			_Chart[0] += Data_;

			return PoppedData;
		}
		CChart::CChart(const TSizes& Sizes_) :
			_Sizes(Sizes_),
			_Chart(Sizes_.size())
		{
			size_t Total = 0;

			for (auto& i : Sizes_)
			{
				Total += i;
				_Indices.emplace_back(Total);
			}

			_Datas.resize(Total);
		}
		optional<SChartTime> CChart::Insert(const SChartTime& Data_)
		{
			auto Ret = CTickMerge::Insert(Data_);
			if (Ret)
				return _Add(*Ret);
			else
				return Ret;
		}
	}
}
