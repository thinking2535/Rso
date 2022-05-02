#include "Dice.h"
#include <stdlib.h>


namespace rso
{
	namespace util
	{
		CDice::CDice()
		{
			m_DiceNum = c_DiceNum_Null;
		}

		CDice::~CDice()
		{
		}

		bool	CDice::operator == (CDice& rDice_)
		{
			if (m_DiceNum == rDice_.Get()) return true;
			return false;
		}

		void	CDice::Throw(void)
		{
			m_DiceNum = (rand() % c_DiceNum_Max + 1);
		}

		void	CDice::Throw(TDiceNum Min_, TDiceNum Max_)
		{
			Throw();

			if (Min_ <= Max_  &&
				Min_ >= 1 &&
				Max_ <= c_DiceNum_Max)
			{
				m_DiceNum = (m_DiceNum - 1) * (Max_ - Min_ + 1) / c_DiceNum_Max + Min_;
			}
		}

		void		CDice::Set(TDiceNum Dice_)
		{
			if (Dice_ < 1 ||
				Dice_ > c_DiceNum_Max) return;

			m_DiceNum = Dice_;
		}

		bool		CDice::IsValid(void)
		{
			if (m_DiceNum >= 1 &&
				m_DiceNum <= c_DiceNum_Max) return true;

			return false;
		}
	}
}
