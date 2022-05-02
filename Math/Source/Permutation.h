#pragma once


#include <functional>
#include <vector>
#include <iterator>
#include <algorithm>


namespace rso
{
	namespace math
	{
		using namespace std;


		template<typename _Ty>
		class CPermutation
		{
		public:
			using TDatas = vector<_Ty>;
			using TCallback = function<void(const TDatas&)>;

		private:
			TDatas& _Datas;
			TCallback _Callback;

			void _Calc(size_t Index_)
			{
				if (Index_ == _Datas.size())
				{
					_Callback(_Datas);
					return;
				}

				for (size_t i = Index_; i < _Datas.size(); ++i)
				{
					swap(_Datas[Index_], _Datas[i]);
					_Calc(Index_ + 1);
					swap(_Datas[Index_], _Datas[i]);
				}
			}

		public:
			CPermutation(TDatas& Datas_, TCallback Callback_) :
				_Datas(Datas_),
				_Callback(Callback_)
			{
				_Calc(0);
			}
		};

		template<typename _Ty>
		void Permutation(const vector<_Ty>& Datas_, function<void(const vector<const _Ty*>&)> Callback_)
		{
			vector<const _Ty*> Datas;
			transform(Datas_.begin(), Datas_.end(), back_inserter(Datas), [](const _Ty& Data_) { return &Data_; });
			CPermutation<const _Ty*>(Datas, Callback_);
		}
	}
}