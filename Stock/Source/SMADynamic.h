#pragma once


#include "Base.h"
#include <Rso/Base/Ring.h>


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename TType>
		class CSMADynamic
		{
			using _TSizes = vector<size_t>;

			CRing<TType>	_Datas;
			_TSizes			_Indice;
			size_t			_Size = 0;

		public:
			CSMADynamic(const initializer_list<size_t>& Sizes_)
			{
				if(Sizes_.size() == 0)
					THROWEX();

				size_t DataCnt = 0;
				for (auto& i : Sizes_)
				{
					if (i <= 0)
						THROWEX();

					_Indice.emplace_back(DataCnt);
					DataCnt += i;
				}
				_Indice.emplace_back(DataCnt);
				_Datas.reserve(DataCnt);
			}
			inline const size_t data_cnt(void) const
			{
				return _Indice.back();
			}
			inline const size_t size(void) const
			{
				return _Size;
			}
			void	push_back(const TType& Data_)
			{
				auto OldSize = _Datas.size();
				_Datas.emplace(Data_);

				if (_Datas.size() == OldSize)
					return;

				if (_Datas.size() >= (_Indice.back() - _Indice[_Indice.size() - 2 - _Size]))
					++_Size;
			}
			//bool	push_back_pop_front(const TType& Data_, TType& PoppedData_)	// return true : Popped
			//{
			//	auto Ret = _Datas.full();
			//	if (Ret)
			//		PoppedData_ = _Datas.front();

			//	push_back(Data_);
			//	return Ret;
			//}
			void	insert(const vector<TType>& Datas_)
			{
				clear();

				auto itData = Datas_.begin();
				auto itIndex = _Indice.begin();
				size_t Index = 0;
				for (++itIndex; itIndex != _Indice.end(); ++itIndex)
				{
					for (; Index < *itIndex; ++Index)
					{
						if (itData == Datas_.end())
							return;

						_Datas.emplace(*itData);
						++itData;
					}
					++_Size;
				}
			}
			bool	pop_front(TType& PoppedData_)
			{
				if (_Datas.empty())
					return false;

				PoppedData_ = _Datas.front();
				_Datas.pop();

				if (_Datas.size() < _Indice[_Size])
					--_Size;

				return true;
			}
			TType	at(size_t Index_)
			{
				TType Sum(_Datas[_Indice[Index_]]);

				for (auto i = _Indice[Index_] + 1; i < _Indice[Index_ + 1]; ++i)
					Sum += _Datas[i];

				return (Sum / (_Indice[Index_ + 1] - _Indice[Index_]));
			}
			TType	reverse_at(size_t Index_)
			{
				auto From = _Indice.back() - _Indice[_Indice.size() - 1 - Index_];
				auto To = _Indice.back() - _Indice[_Indice.size() - 2 - Index_];

				TType Sum(_Datas.reverse_at(From));
				for (auto i = From + 1; i < To; ++i)
					Sum += _Datas.reverse_at(i);

				return (Sum / (To - From));
			}
			void	clear(void)
			{
				_Datas.clear();
				_Size = 0;
			}
			bool full(void) const
			{
				return _Datas.full();
			}
		};
	}
}
