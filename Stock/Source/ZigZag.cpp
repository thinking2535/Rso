#include "ZigZag.h"

namespace rso
{
	namespace stock
	{
		bool CZigZag::insert(size_t Index_, const SCandle& Data_)
		{
			if (_Direction == 1) // ������̸�
			{
				if (Data_.High > _Datas.back().Price) // ���ӻ�����ΰ�
				{
					new (&_Datas.back()) SZigZag(Index_, Data_.High);
					return true;
				}
				else if (double(_Datas.back().Price - Data_.Low) / double(_Datas.back().Price) >= _Gap) // �϶��߼��� �������?
				{
					_Datas.emplace_back(Index_, Data_.Low);
					_Direction = -1;
					return true;
				}
			}
			else if (_Direction == -1) // �϶����̸�
			{
				if (Data_.Low < _Datas.back().Price) // �����϶����ΰ�
				{
					new (&_Datas.back()) SZigZag(Index_, Data_.Low);
					return true;
				}
				else if (double(Data_.High - _Datas.back().Price) / double(Data_.High) >= _Gap) // ����߼��� �������?
				{
					_Datas.emplace_back(Index_, Data_.High);
					_Direction = 1;
					return true;
				}
			}
			else if (_FirstHigh.Price != -1) // ���� �߼��� �ƴ����� ���� ������ ������ �ƴϸ�
			{
				if (Data_.High > _FirstHigh.Price)
				{
					new (&_FirstHigh) SZigZag(Index_, Data_.High);
					if (double(_FirstHigh.Price - _FirstLow.Price) / double(_FirstHigh.Price) >= _Gap)
					{
						_Direction = 1;
						_Datas.emplace_back(_FirstLow);
						_Datas.emplace_back(_FirstHigh);
						return true;
					}
				}
				else if (Data_.Low < _FirstLow.Price)
				{
					new (&_FirstLow) SZigZag(Index_, Data_.Low);
					if (double(_FirstHigh.Price - _FirstLow.Price) / double(_FirstHigh.Price) >= _Gap)
					{
						_Direction = -1;
						_Datas.emplace_back(_FirstHigh);
						_Datas.emplace_back(_FirstLow);
						return true;
					}
				}
			}
			else // ���� ������ �����̸�
			{
				_FirstHigh = SZigZag(Index_, Data_.High);
				_FirstLow = SZigZag(Index_, Data_.Low);
			}

			return false;
		}
		void CZigZag::clear(void)
		{
			_Direction = 0;
			new (&_FirstHigh) SZigZag();
			new (&_FirstLow) SZigZag();
			_Datas.clear();
		}
	}
}
