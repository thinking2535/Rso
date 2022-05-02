#pragma once


#include "Dice.h"


namespace rso
{
	namespace util
	{
		template< typename TDiceCnt, TDiceCnt DiceCnt_ >
		class CDices
		{
		public:
			static const	TDiceCnt	c_DiceCnt = DiceCnt_;

		private:
			CDice	m_aDice[c_DiceCnt];
			TDiceCnt	m_ActiveDiceCnt;

			void		Clear(void)
			{
				for (TDiceCnt IDX = 0; IDX < c_DiceCnt; ++IDX)
				{
					m_aDice[IDX].Clear();
				}
			}

		public:
			CDices()
			{
				m_ActiveDiceCnt = c_DiceCnt;
			}

			TDiceCnt	GetActiveDiceCnt(void){ return m_ActiveDiceCnt; }
			void		Throw(TDiceCnt DiceCnt_ = c_DiceCnt)
			{
				Throw(DiceCnt_, 0, 0);
			}
			void		Throw(CDice::TDiceNum Min_, CDice::TDiceNum Max_)
			{
				Throw(c_DiceCnt, Min_, Max_);
			}
			void		Throw(TDiceCnt  DiceCnt_, CDice::TDiceNum Min_, CDice::TDiceNum Max_)
			{
				Clear();

				if (DiceCnt_ < 1 ||
					DiceCnt_ > c_DiceCnt) DiceCnt_ = c_DiceCnt;

				if (Min_ > Max_ ||
					Min_ < DiceCnt_ ||
					Max_ >(CDice::c_DiceNum_Max*DiceCnt_))
				{
					Min_ = DiceCnt_;
					Max_ = (CDice::c_DiceNum_Max*DiceCnt_);
				}

				m_ActiveDiceCnt = DiceCnt_;

				CDice::TDiceNum	DiceMin = CDice::c_DiceNum_Null;
				CDice::TDiceNum	DiceMax = CDice::c_DiceNum_Null;

				CDice::TDiceNum	ValueSum = 0;

				for (TDiceCnt IDX = 0; IDX < DiceCnt_; ++IDX)
				{
					TDiceCnt LC = DiceCnt_ - 1 - IDX;
					DiceMin = Min_ - ValueSum - CDice::c_DiceNum_Max*LC;
					DiceMax = Max_ - ValueSum - LC;

					if (DiceMin < 1) DiceMin = 1;
					if (DiceMax >= CDice::c_DiceNum_Max) DiceMax = CDice::c_DiceNum_Max;

					m_aDice[IDX].Throw(DiceMin, DiceMax);

					ValueSum += m_aDice[IDX].Get();
				}
			}

			CDice::TDiceNum	Get(void)
			{
				CDice::TDiceNum	DiceNum = 0;
				for (TDiceCnt Cnt = 0; Cnt < m_ActiveDiceCnt; ++Cnt)
				{
					DiceNum += m_aDice[Cnt].Get();
				}
				return DiceNum;
			}
			CDice::TDiceNum	Get(TDiceCnt DiceIDX_)
			{
				if (DiceIDX_ < 0 ||
					DiceIDX_ >= m_ActiveDiceCnt) return CDice::c_DiceNum_Null;

				return m_aDice[DiceIDX_].Get();
			}
			void		Set(TDiceCnt DiceCnt_, CDice::TDiceNum aDiceNum_[])
			{
				if (DiceCnt_ < 1 ||
					DiceCnt_ > c_DiceCnt) return;


				for (TDiceCnt Cnt = 0; Cnt < DiceCnt_; ++Cnt)
				{
					m_aDice[Cnt].Set(aDiceNum_[Cnt]);
				}

				m_ActiveDiceCnt = DiceCnt_;
			}
			bool		IsNull(void)
			{
				for (TDiceCnt IDX = 0; IDX < m_ActiveDiceCnt; ++IDX)
				{
					if (m_aDice[IDX].IsNull() == true) return true;
				}

				return false;
			}
			bool		IsValid(void)
			{
				for (TDiceCnt IDX = 0; IDX < m_ActiveDiceCnt; ++IDX)
				{
					if (m_aDice[IDX].IsValid() == false) return false;
				}

				return true;
			}
			bool		IsSame(void)
			{
				CDice::TDiceNum	BaseNum = CDice::c_DiceNum_Null;

				// Set BaseNum
				TDiceCnt	IDX = 0;
				for (; IDX < m_ActiveDiceCnt; ++IDX)
				{
					if (m_aDice[IDX].IsNull() == true) continue;
					BaseNum = m_aDice[IDX].Get();
					++IDX;
					break;
				}

				if (BaseNum == CDice::c_DiceNum_Null) return false;

				// Compare
				bool bChecked = false;
				for (; IDX < m_ActiveDiceCnt; ++IDX)
				{
					if (m_aDice[IDX].IsNull() == true) continue;

					if (m_aDice[IDX].Get() != BaseNum) return false;
					bChecked = true;
				}

				if (bChecked == true) return true;

				return false;
			}
		};
	}
}
