#include "TrendLine.h"


namespace rso
{
	namespace stock
	{
		void		CTrendLine::_TrendUp(void)
		{
			if (_Trend < 0)
				_Trend = 1;
			else
				++_Trend;
		}
		void		CTrendLine::_TrendDown(void)
		{
			if (_Trend > 0)
				_Trend = -1;
			else
				--_Trend;
		}
		void	CTrendLine::emplace(const SData& Data_)
		{
			if (_Datas.size() == 0)
			{
				_Datas.emplace(Data_);
			}
			else if (_Datas.size() == 1)
			{
				if (_Datas.back().Value != Data_.Value)
					_Datas.emplace(Data_);
			}
			else if (_Datas.size() == 2 &&
				_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value <= _MinAmp &&
				_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value >= -_MinAmp)
			{
				// �� ���� ������̸�
				if (_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value > 0)
				{
					// �� ���� �վ�����
					if (_Datas[_Datas.size() - 1].Value < Data_.Value)
					{
						_Datas[_Datas.size() - 1] = Data_;
					}
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value < -_MinAmp)
					{
						_Datas.emplace(Data_);
					}
					else if (_Datas[_Datas.size() - 2].Value >= Data_.Value)
					{
						_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
						_Datas[_Datas.size() - 1] = Data_;
					}
				}
				else
				{
					// �Ʒ� ���� �վ�����
					if (_Datas[_Datas.size() - 1].Value > Data_.Value)
					{
						_Datas[_Datas.size() - 1] = Data_;
					}
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value > _MinAmp)
					{
						_Datas.emplace(Data_);
					}
					else if (_Datas[_Datas.size() - 2].Value <= Data_.Value)
					{
						_Datas[_Datas.size() - 2] = _Datas[_Datas.size() - 1];
						_Datas[_Datas.size() - 1] = Data_;
					}
				}
			}
			else
			{
				// �� ���� ������̸�
				if (_Datas[_Datas.size() - 1].Value - _Datas[_Datas.size() - 2].Value > 0)
				{
					// ���� �վ�����
					if (_Datas[_Datas.size() - 1].Value < Data_.Value)
					{
						if (_Datas.size() >= 3)
						{
							// �� ������ �վ�����
							if (_Datas[_Datas.size() - 3].Value >= _Datas[_Datas.size() - 1].Value &&
								_Datas[_Datas.size() - 3].Value < Data_.Value)
								_TrendUp();
						}
							
						_Datas[_Datas.size() - 1] = Data_;
					}
					// Data_ - ������ ���� -���� �ʰ��̸�
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value < -_MinAmp)
					{
						if (_Datas.size() >= 3)
						{
							// �� ������ �϶������� ( ����� ���� ������ ó�������Ƿ� �϶��� ��츸 ó�� )
							if (_Datas[_Datas.size() - 3].Value > _Datas[_Datas.size() - 1].Value)
								_TrendDown();
						}

						// �� ������ �վ�����
						if (_Datas[_Datas.size() - 2].Value > Data_.Value)
							_TrendDown();

						_Datas.emplace(Data_);
					}
				}
				else
				{
					// �Ʒ��� �վ�����
					if (_Datas[_Datas.size() - 1].Value > Data_.Value)
					{
						if (_Datas.size() >= 3)
						{
							// �� ������ �վ�����
							if (_Datas[_Datas.size() - 3].Value <= _Datas[_Datas.size() - 1].Value &&
								_Datas[_Datas.size() - 3].Value > Data_.Value)
								_TrendDown();
						}

						_Datas[_Datas.size() - 1] = Data_;
					}
					// Data_ - ������ ���� ���� �ʰ���
					else if (Data_.Value - _Datas[_Datas.size() - 1].Value > _MinAmp)
					{
						if (_Datas.size() >= 3)
						{
							// �� ������ ��������� ( �϶��� ���� ������ ó�������Ƿ� ����� ��츸 ó�� )
							if (_Datas[_Datas.size() - 3].Value < _Datas[_Datas.size() - 1].Value)
								_TrendUp();
						}

						// �� ������ �վ�����
						if (_Datas[_Datas.size() - 2].Value < Data_.Value)
							_TrendUp();

						_Datas.emplace(Data_);
					}
				}
			}

			if (_Datas.size() > 3)
				_Datas.pop();
		}
		TValue		CTrendLine::Trend(void) const
		{
			if (_Datas.size() < 2)
				return 0;

			return _Trend;
		}
	}
}
