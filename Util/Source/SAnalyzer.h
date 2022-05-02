#pragma once


#include "Base.h"
#include <map>
#include <functional>


namespace rso
{
	namespace util
	{
		using namespace std;
		using namespace base;

		template< typename _TData, typename _TSize, _TSize Size >
		class CSAnalyzer
		{
		public:
			typedef int32 TValue;
			typedef int32 TCnt;
			typedef _TSize TSize;
			static const TSize c_Size = Size;
			static const _TData c_Null;

			struct TData
			{
				_TData aData[Size];

				TData()
				{
					for (_TSize cnt = 0; cnt < Size; ++cnt)
						aData[cnt] = 0;
				}

				bool operator < (const TData& RVal_) const
				{
					for (_TSize cnt = 0; cnt < Size; ++cnt)
					{
						if (aData[cnt] < RVal_.aData[cnt]) return true;
						if (aData[cnt] > RVal_.aData[cnt]) return false;
					}

					return false;
				}
			};
			typedef function< void(const TData&, double, TValue, TCnt, bool) > TResultCallback;

		private:
			struct SResult
			{
				TValue Sum;
				TValue Cnt;
				double Ratio;

				SResult() : Sum(0), Cnt(0), Ratio(0.0) {}
				SResult(TValue Gap_) : Sum(0), Cnt(0), Ratio(0.0)
				{
					Sum = Gap_;

					if (Gap_ > 50)
					{
						Ratio = 1.0;
					}
					else if (Gap_ < -50)
					{
						Ratio = -1.0;
					}

					Cnt = 1;
				}
				void Add(TValue Gap_)
				{
					Sum += Gap_;

					if (Gap_ > 50)
					{
						Ratio += 1.0;
					}
					else if (Gap_ < -50)
					{
						Ratio += -1.0;
					}

					++Cnt;
				}
				TValue GetAvg(void) const
				{
					return Sum / Cnt;
				}
				double GetRatio(void) const
				{
					return Ratio * 100.0 / (double)Cnt;
				}
			};

			typedef map< const TData, SResult > TMap;
			typedef typename TMap::iterator TMapIt;
			TMap m_mapData;
			double m_Ratio;
			TValue m_GapABS;
			TCnt m_MinDataCnt;
			TResultCallback m_ResultCallback;

		public:
			CSAnalyzer(double Ratio_, TValue GapABS_, const TCnt MinDataCnt_, TResultCallback ResultCallback_) : m_Ratio(Ratio_), m_GapABS(GapABS_), m_MinDataCnt(MinDataCnt_), m_ResultCallback(ResultCallback_)
			{
			}
			~CSAnalyzer() { Clear(); }

			void Clear(void) { m_mapData.clear(); }
			bool SetData(_TSize Mask_, const TData& Data_, TValue Gap_);
			void GetResult(void);
			TValue Predict(_TSize Mask_, const TData& Data_);
		};

		template< typename _TData, typename _TSize, _TSize Size >
		const _TData CSAnalyzer<_TData, _TSize>::c_Null = numeric_limits<_TData>::max();

		template< typename _TData, typename _TSize, _TSize Size >
		bool CSAnalyzer<_TData, _TSize, Size>::SetData(_TSize Mask_, const TData& Data_, TValue Gap_)
		{
			// Mask ////
			TData Data = Data_;
			for (_TSize bit = 0; bit < Size; ++bit)
			{
				if ((Mask_ & (1 << bit)) == 0)
					Data.aData[bit] = c_Null;
			}


			TMap::iterator it = m_mapData.find(Data);
			if (it == m_mapData.end())
			{
				try
				{
					TMap::_Pairib ib = m_mapData.insert(TMap::value_type(Data, SResult(Gap_)));
					if (ib.second == false)
						return false;
				}
				catch (...)
				{
					return false;
				}
			}
			else
			{
				it->second.Add(Gap_);
			}

			return true;
		}

		template< typename _TData, typename _TSize, _TSize Size >
		void CSAnalyzer<_TData, _TSize, Size>::GetResult(void)
		{
			for (TMap::iterator It = m_mapData.begin();
				It != m_mapData.end();
				++It)
			{
				if (It->second.Cnt >= m_MinDataCnt &&
					(It->second.GetRatio() >= m_Ratio || It->second.GetRatio() <= -m_Ratio) &&
					(It->second.GetAvg() >= m_GapABS || It->second.GetAvg() <= -m_GapABS))
				{
					m_ResultCallback(It->first, It->second.GetRatio(), It->second.GetAvg(), It->second.Cnt, false);
				}
			}
		}

		template< typename _TData, typename _TSize, _TSize Size >
		typename CSAnalyzer<_TData, _TSize, Size>::TValue CSAnalyzer<_TData, _TSize, Size>::Predict(_TSize Mask_, const TData& Data_)
		{
			// Mask ////
			TData Data = Data_;
			for (_TSize bit = 0; bit < Size; ++bit)
			{
				if ((Mask_ & (1 << bit)) == 0)
					Data.aData[bit] = c_Null;
			}

			TMap::iterator It = m_mapData.find(Data);
			if (It == m_mapData.end())
				return 0;

			if (It->second.Cnt >= m_MinDataCnt &&
				(It->second.GetRatio() >= m_Ratio || It->second.GetRatio() <= -m_Ratio) &&
				(It->second.GetAvg() >= m_GapABS || It->second.GetAvg() <= -m_GapABS))
			{
				m_ResultCallback(It->first, It->second.GetRatio(), It->second.GetAvg(), It->second.Cnt, true);

				return It->second.GetAvg();
			}

			return 0;
		}
	}
}
