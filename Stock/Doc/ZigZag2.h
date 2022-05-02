#pragma once


#include "Base.h"


namespace rso
{
	namespace stock
	{
		using namespace base;

		template<typename _TType>
		class CZigZag
		{
		public:
			using TData = SData<_TType>;
		private:
			using _TDatas = vector<TData>;
			using _TDatasIt = typename _TDatas::iterator;
			using _TDatasCIt = typename _TDatas::const_iterator;

			_TDatas	_Datas;
			TValue	_MinAmp = 0;
			TData	_Last;

		public:
			CZigZag(TValue MinAmp_) :
				_MinAmp(MinAmp_)
			{
			}
			inline void	clear(void)
			{
				_Datas.clear();
			}
			inline TData operator[] (size_t Index_) const
			{
				return _Datas[_Datas.size() - Index_ - 1];
			}
			inline TData diff(size_t Index_) const	// 2이상부터 유효, 1이하일때 예외상황발생은 외부 책임.
			{
				return (_Datas[_Datas.size() - Index_ - 1] - _Datas[_Datas.size() - Index_ - 2]);
			}
			inline size_t	size(void) const
			{
				return _Datas.size();
			}
			inline _TDatasIt begin(void)
			{
				return _Datas.begin();
			}
			inline _TDatasIt end(void)
			{
				return _Datas.end();
			}
			inline _TDatasCIt begin(void) const
			{
				return _Datas.begin();
			}
			inline _TDatasCIt end(void) const
			{
				return _Datas.end();
			}
			inline TValue	MinAmp(void) const
			{
				return _MinAmp;
			}
			void insert(const TData& Data_)
			{
				if (_Datas.size() == 0)
				{
					_Datas.emplace_back(Data_);
					_Last = Data_;
				}
				else if (_Datas.size() == 1)
				{
					if (_Last.Data > _Datas.back().Data)
					{
						if (Data_.Data > _Last.Data)
						{
							_Last = Data_;
						}
						else if (Data_.Data < _Last.Data)
						{
							if (_Last.Data - _Datas.back().Data < _MinAmp)
							{
								if (Data_.Data < _Datas.back().Data)
								{
									_Datas.back() = _Last;
									_Last = Data_;
								}
							}
							else
							{
								if (_Last.Data - Data_.Data >= _MinAmp)
								{
									_Datas.emplace_back(_Last);
									_Last = Data_;
								}
							}
						}
					}
					else if (_Last.Data < _Datas.back().Data)
					{
						if (Data_.Data < _Last.Data)
						{
							_Last = Data_;
						}
						else if (Data_.Data > _Last.Data)
						{
							if (_Datas.back().Data - _Last.Data < _MinAmp)
							{
								if (Data_.Data > _Datas.back().Data)
								{
									_Datas.back() = _Last;
									_Last = Data_;
								}
							}
							else
							{
								if (Data_.Data - _Last.Data >= _MinAmp)
								{
									_Datas.emplace_back(_Last);
									_Last = Data_;
								}
							}
						}
					}
					else
					{
						if (Data_.Data != _Last.Data)
							_Last = Data_;
					}
				}
				else
				{
					if (_Last.Data > _Datas.back().Data)
					{
						if (Data_.Data > _Last.Data)
						{
							_Last = Data_;
						}
						if (_Last.Data - Data_.Data >= _MinAmp)
						{
							_Datas.emplace_back(_Last);
							_Last = Data_;
						}
					}
					if (_Last.Data < _Datas.back().Data)
					{
						if (Data_.Data < _Last.Data)
						{
							_Last = Data_;
						}
						if (Data_.Data - _Last.Data >= _MinAmp)
						{
							_Datas.emplace_back(_Last);
							_Last = Data_;
						}
					}
				}
			}
		};
	}
}
