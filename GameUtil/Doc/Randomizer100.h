#pragma once


#include "Base.h"


namespace rso
{
	namespace util
	{
		using namespace std;

		template<typename TValue>
		class CRandomizer100
		{
		private:
			struct _SData
			{
				__int32 Ratio = 0;
				TValue Value;

				_SData(_int32 Ratio_, const TValue& Value_) :
					Ratio(Ratio_), Value(Value_)
				{}
			};
			using _TDatas = list<_SData>;

			_TDatas _Datas;
			typename _TDatas::const_iterator _It{ _Datas.cend() };

		public:
			bool	insert(__int32 Ratio_, const TValue& Value_)
			{
				if (Ratio_ <= 0)
					return false;

				try
				{
					_Datas.emplace_back(Ratio_, Value_);
					return true;
				}
				catch (...)
				{
					return false;
				}
			}
			size_t size(void) const
			{
				return _Datas.size();
			}
			const TValue* get()
			{
				if (_It == _Datas.cend())
					_It = _Datas.cbegin();
				else
					++_It;

				for (; _It != _Datas.cend(); ++_It)
				{
					if (_It->Ratio > (rand() % 100))
						return &_It->Value;
				}

				return nullptr;
			}
		};
	}
}