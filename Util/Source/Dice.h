#pragma once


#include "Base.h"


namespace rso
{
	namespace util
	{
		class CDice
		{
		public:
			typedef	int		TDiceNum;
			static const	TDiceNum	c_DiceNum_Null = (TDiceNum)(~0);
			static const	TDiceNum	c_DiceNum_Max = 6;


		private:
			TDiceNum	m_DiceNum;

		public:
			CDice();
			~CDice();

			bool		operator == (CDice& rDice_);

			void		Throw(void);
			void		Throw(TDiceNum Min_, TDiceNum Max_);
			TDiceNum	Get(void){ return m_DiceNum; }
			void		Set(TDiceNum Dice_);
			bool		IsNull(void){ return (m_DiceNum == c_DiceNum_Null ? true : false); }
			bool		IsValid(void);
			void		Clear(void){ m_DiceNum = c_DiceNum_Null; }
		};
	}
}
