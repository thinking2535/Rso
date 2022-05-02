#pragma once

#include "Base.h"
#include <map>
#include <Rso/Base/Memory.h>
#include <algorithm>
#include <cmath>

namespace rso
{
	namespace util
	{
		using namespace std;
		using namespace base;


		class CSpace
		{
		public:
			using TIDX = size_t;

			struct SIDXPoint
			{
				TIDX IDX;
				SPoint3 Point;
			};

			struct SDistIDX
			{
				TDist Dist;
				TIDX IDX;

				SDistIDX() : Dist(0), IDX(c_IDX_Null) {}
				SDistIDX(const TDist Dist_, const TIDX IDX_) : Dist(Dist_), IDX(IDX_) {}

				bool operator < (const SDistIDX& DistIDX_) { return (Dist < DistIDX_.Dist); }
			};

		private:
			typedef CMemory< SIDXPoint, TCharIDX > TPnts; // IDX : SubIDX 주 컨테이너
			typedef CMemory< SIDXPoint, TCharIDX > TSpcPnts; // SubIDX : IDX 블럭 단위 컨테이너

			static const TIDX c_IDX_Null;
			static const TDist c_Width_Min = 100;
			static const TDist c_Height_Min = 100;
			static const TDist c_Width_Max = 1000000;
			static const TDist c_Height_Max = 1000000;

		public:
			typedef CMemory<SDistIDX, TCharIDX> TIDXs;

		private:
			TSpcPnts** _2Space = nullptr;
			TPnts _Pnts;
			TIDXs _IDXs; // 기준포인트 범위내 데이터 출력을 위한 컨테이너
			TDist _Width = 0;
			TDist _Height = 0;
			TDist _UnitWidth = 0;
			TDist _UnitHeight = 0;
			TDist _UnitSz = 1;

			SPoint3 GetUnitPoint(const SPoint3& Point_) const
			{
				return SPoint3(Point_.X / _UnitSz, Point_.Y / _UnitSz, 0);
			}
			inline bool _IsValidPoint(const SPoint3& Point_) const;
			inline void _InsertIDXs(const SPoint3& Source_, const _SArea& Area_, const TSpcPnts& SpcPnts_);
			bool _IsInArea(const _SArea& Area_, const SVector3& Target_, TDist& Dist_o);

		public:
			CSpace(const TDist Width_, const TDist Height_, const TIDX ReserveCnt_);
			virtual ~CSpace();
			bool New(const TIDX IDX_, const SPoint3& Point_);
			void Del(const TIDX IDX_);
			void Move(const TIDX IDX_, const SPoint3& Point_);
			bool GetDist(const TIDX IDX_, const STarget& Target_, TDist& Dist_o) const;
			const TIDXs* GetIDXsInArea(const STarget& Target_, const SArea& Area_, const SVector3& Dir_);
		};

		const CSpace::TIDX CSpace::c_IDX_Null = numeric_limits<CSpace::TIDX>::max();
	}
}
