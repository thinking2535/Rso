#include "Calculator.h"

namespace rso
{
	namespace core
	{
		double CCalculator::_Do(size_t& Index_, const string& Formula_)
		{
			double Sum = 0.0;
			double BufferValue = 0.0;
			double Value = 0.0;
			char CurOp = '+';
			char AddOp = '+';

			while (Index_ < Formula_.size())
			{
				Index_ = Formula_.find_first_not_of(" \t", Index_);
				if (Index_ == string::npos)
					break;

				if (Formula_[Index_] == ')')
				{
					++Index_;
					break;
				}

				if (CurOp != ' ') // 수 또는 괄호를 기대하는가?
				{
					if (Formula_[Index_] == '(')
					{
						++Index_;
						Value = _Do(Index_, Formula_);
					}
					else
					{
						auto EndIndex = Formula_.find_first_of(" ()+-*/", Index_);
						Value = stod(Formula_.substr(Index_, EndIndex - Index_));
						Index_ = EndIndex;
					}

					switch (CurOp)
					{
					case '+':
						Sum += BufferValue;
						BufferValue = Value;
						AddOp = CurOp;
						break;

					case '-':
						Sum -= BufferValue;
						BufferValue = Value;
						AddOp = CurOp;
						break;

					case '*':
						BufferValue *= Value;
						break;

					case '/':
						BufferValue /= Value;
						break;

					default:
						THROWEX();
					}

					CurOp = ' ';
				}
				else // 연산자를 기대하는가?
				{
					CurOp = Formula_[Index_];
					++Index_;
				}
			}

			if (CurOp != ' ')
				THROWEX();

			switch (AddOp)
			{
			case '+':
				return Sum += BufferValue;

			default:
				return Sum -= BufferValue;
			}
		}
		double CCalculator::Do(const string& Formula_)
		{
			size_t Index = 0;
			return _Do(Index, Formula_);
		}
	}
}
