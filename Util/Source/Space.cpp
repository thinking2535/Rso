#include "Space.h"


namespace rso
{
	namespace util
	{
#define _DEF_Dist2D( Pnt1_, Pnt2_ ) std::sqrt( (Pnt1_.X-Pnt2_.X)*(Pnt1_.X-Pnt2_.X) + (Pnt1_.Y-Pnt2_.Y)*(Pnt1_.Y-Pnt2_.Y) )
#define _DEF_Dist3D( Pnt1_, Pnt2_ ) std::sqrt( (Pnt1_.X-Pnt2_.X)*(Pnt1_.X-Pnt2_.X) + (Pnt1_.Y-Pnt2_.Y)*(Pnt1_.Y-Pnt2_.Y) + (Pnt1_.Z-Pnt2_.Z)*(Pnt1_.Z-Pnt2_.Z) )

		bool CSpace::_IsValidPoint(const SPoint3& Point_) const
		{
			if (Point_.X < 0 || Point_.X >= _Width ||
				Point_.Y < 0 || Point_.Y >= _Height)
				return false;

			return true;
		}

		void CSpace::_InsertIDXs(const SPoint3& Source_, const _SArea& Area_, const TSpcPnts& SpcPnts_)
		{
			TDist Dist = 0;

			for (const TSpcPnts::SLink* it = SpcPnts_.Head();
				it != 0;
				it = SpcPnts_.Next(it))
			{
				auto pIDXPoint = SpcPnts_.GetObj(it->ObjectIDX);
				if (pIDXPoint == nullptr)
					continue;

				if (_IsInArea(Area_, pIDXPoint->Point - Source_, Dist) == false)
					continue;

				auto pDistIDX = _IDXs.New();
				if (pDistIDX != nullptr)
				{
					pDistIDX->Dist = Dist;
					pDistIDX->IDX = pIDXPoint->IDX;
				}
			}
		}

		bool CSpace::_IsInArea(const _SArea& Area_, const SVector3& Target_, TDist& Dist_o)
		{
			Dist_o = (TDist)std::sqrt((double)((Target_.X)*(Target_.X) + (Target_.Y)*(Target_.Y) + (Target_.Z)*(Target_.Z)));

			if (Area_.Type == _SArea::Type_Sphere)
			{
				if (Dist_o > Area_.Radius)
					return false;

				return true;
			}

			if (std::abs(Target_.Z) > Area_.Height)
				return false;

			TDist Dist = (TDist)std::sqrt((double)((Target_.X)*(Target_.X) + (Target_.Y)*(Target_.Y)));
			if (Dist > Area_.Radius)
				return false;


			if (Area_.Type == _SArea::Type_Circle)
				return true;


			if (Area_.Type == _SArea::Type_Pie)
			{
				auto d1 = std::atan2(Area_.VecStart.Y, Area_.VecStart.X);
				auto d2 = std::atan2(Area_.VecEnd.Y, Area_.VecEnd.X);
				auto d = std::atan2(Target_.Y, Target_.X);

				if (d1 > d2)
				{
					if (d1 <= d || d <= d2)
						return true;
				}
				else if (d1 < d2)
				{
					if (d1 <= d && d <= d2)
						return true;
				}
				else
				{
					return true;
				}
			}

			return false;
		}
		CSpace::CSpace(const TDist Width_, const TDist Height_, const TIDX ReserveCnt_)
		{
			if (Width_ < c_Width_Min || Width_ > c_Width_Max ||
				Height_ < c_Height_Min || Height_ > c_Height_Max)
				THROWEX();

			_UnitSz = (TDist)sqrt((double)((Width_ * Height_) / (c_Width_Min * c_Height_Min)));
			_Width = Width_;
			_Height = Height_;
			_UnitWidth = (Width_ - 1) / _UnitSz + 1;
			_UnitHeight = (Height_ - 1) / _UnitSz + 1;

			_2Space = new TSpcPnts * [(uint32)_UnitHeight];

			try
			{
				if (!_Pnts.Reserve(ReserveCnt_))
					THROWEX();

				for (TDist h = 0; h < _UnitHeight; ++h)
				{
					try
					{
						_2Space[h] = new TSpcPnts[(uint32)_UnitWidth];
					}
					catch (...)
					{
						for (TDist i = 0; i < h; ++i)
							delete[] _2Space[i];

						throw;
					}
				}
			}
			catch (...)
			{
				delete[] _2Space;
				throw;
			}
		}
		CSpace::~CSpace()
		{
			for (TDist h = 0; h < _UnitHeight; ++h)
				delete[] _2Space[h];

			delete[] _2Space;
		}
		bool CSpace::New(const TIDX IDX_, const SPoint3& Point_)
		{
			if (_IsValidPoint(Point_) == false)
				return false;

			auto UnitPoint = GetUnitPoint(Point_);
			auto pSubIDXPoint = _Pnts.NewIndex(IDX_);
			if (pSubIDXPoint == nullptr) return false;

			pSubIDXPoint->Point = Point_;

			auto SubIDX = c_IDX_Null;

			auto pIDXPoint = _2Space[UnitPoint.Y][UnitPoint.X].New(&SubIDX);
			if (pIDXPoint == nullptr)
			{
				_Pnts.Del(IDX_);
				return false;
			}
			pIDXPoint->IDX = IDX_;
			pIDXPoint->Point = pSubIDXPoint->Point;
			pSubIDXPoint->IDX = SubIDX;

			return true;
		}

		void CSpace::Del(const TIDX IDX_)
		{
			auto pSubIDXPoint = _Pnts.GetObj(IDX_);
			if (pSubIDXPoint == nullptr)
				return;

			auto UnitPoint = GetUnitPoint(pSubIDXPoint->Point);
			_2Space[UnitPoint.Y][UnitPoint.X].Del(pSubIDXPoint->IDX);

			_Pnts.Del(IDX_);
		}

		void CSpace::Move(const TIDX IDX_, const SPoint3& Point_)
		{
			if (!_IsValidPoint(Point_))
				return;

			auto pSubIDXPoint = _Pnts.GetObj(IDX_);
			if (pSubIDXPoint == nullptr)
				return;

			if (pSubIDXPoint->Point == Point_)
				return;

			SPoint3 UnitPointOld = GetUnitPoint(pSubIDXPoint->Point);
			SPoint3 UnitPointNew = GetUnitPoint(Point_);

			pSubIDXPoint->Point = Point_;

			if (UnitPointOld == UnitPointNew)
				return;

			auto SubIDX = c_IDX_Null;
			auto pIDXPoint = _2Space[UnitPointNew.Y][UnitPointNew.X].New(&SubIDX);
			if (pIDXPoint == nullptr)
				return;

			pIDXPoint->IDX = IDX_;
			pIDXPoint->Point = pSubIDXPoint->Point;

			_2Space[UnitPointOld.Y][UnitPointOld.X].Del(pSubIDXPoint->IDX);

			pSubIDXPoint->IDX = SubIDX;
		}

		bool CSpace::GetDist(const TIDX IDX_, const STarget& Target_, TDist& Dist_o) const
		{
			auto pIDXPoint = _Pnts.GetObj(IDX_);
			if (pIDXPoint == nullptr) return false;


			const _SPoint3* pPoint = nullptr;
			if (Target_.Type == STarget::Type_IDX)
			{
				auto pIDXPoint2 = _Pnts.GetObj(Target_.IDX);
				if (pIDXPoint2 == nullptr)
					return false;

				pPoint = &pIDXPoint2->Point;
			}
			else if (Target_.Type == STarget::Type_Point)
			{
				pPoint = &Target_.Point;
			}
			else
			{
				return false;
			}

			Dist_o = static_cast<TDist>(_DEF_Dist3D(pIDXPoint->Point, (*pPoint)));

			return true;
		}

		/**
		return : 해당 영역에 있는 Client의 IDX 컨테이너
		Source_ : 기준 점
		Area_ : 각도와 반경을 가지는 영역
		Dir_ : Area_ 의 방향
		*/
		const CSpace::TIDXs* CSpace::GetIDXsInArea(const STarget& Target_, const SArea& Area_, const SVector3& Dir_)
		{
			_IDXs.Clear();

			SPoint3 Point;
			if (Target_.Type == STarget::Type_Point)
			{
				Point = Target_.Point;
			}
			else if (Target_.Type == STarget::Type_IDX)
			{
				auto pIDXPoint = _Pnts.GetObj(Target_.IDX);
				if (pIDXPoint == nullptr)
					return nullptr;

				if (Area_.Radius == 0.0)
				{
					auto pDistIDX = _IDXs.New();
					if (pDistIDX == nullptr)
						return nullptr;

					pDistIDX->Dist = 0;
					pDistIDX->IDX = Target_.IDX;

					return &_IDXs;
				}

				Point = pIDXPoint->Point;
			}
			else
			{
				return nullptr;
			}


			// Scan Space that include circle and Insert IDX is in Dist_ to IDXs container /////////////
			SPoint3 UnitPoint = GetUnitPoint(Point);
			SPoint3 ScanPoint;
			_SArea Area(Area_, Dir_);


			// center
			_InsertIDXs(Point, Area, _2Space[UnitPoint.Y][UnitPoint.X]);

			// right
			ScanPoint.Y = UnitPoint.Y;
			for (ScanPoint.X = UnitPoint.X + 1; ScanPoint.X < _UnitWidth; ++ScanPoint.X)
			{
				if ((ScanPoint.X*_UnitSz) - Point.X > Area_.Radius)
					break;

				_InsertIDXs(Point, Area, _2Space[ScanPoint.Y][ScanPoint.X]);
			}

			// left
			for (ScanPoint.X = UnitPoint.X; ScanPoint.X > 0; --ScanPoint.X)
			{
				if (Point.X - (ScanPoint.X*_UnitSz) > Area_.Radius)
					break;

				_InsertIDXs(Point, Area, _2Space[ScanPoint.Y][ScanPoint.X - 1]);
			}


			// top
			for (ScanPoint.Y = UnitPoint.Y; ScanPoint.Y > 0; --ScanPoint.Y)
			{
				ScanPoint.X = UnitPoint.X;

				if (Point.Y - (ScanPoint.Y*_UnitSz) > Area_.Radius)
					break;

				_InsertIDXs(Point, Area, _2Space[ScanPoint.Y - 1][ScanPoint.X]);


				// right
				for (ScanPoint.X = UnitPoint.X + 1; ScanPoint.X < _UnitWidth; ++ScanPoint.X)
				{
					if (_DEF_Dist2D(Point, SPoint3(ScanPoint.X*_UnitSz, ScanPoint.Y*_UnitSz, 0)) > Area_.Radius)
						break;

					_InsertIDXs(Point, Area, _2Space[ScanPoint.Y - 1][ScanPoint.X]);
				}

				// left
				for (ScanPoint.X = UnitPoint.X; ScanPoint.X > 0; --ScanPoint.X)
				{
					if (_DEF_Dist2D(Point, SPoint3(ScanPoint.X*_UnitSz, ScanPoint.Y*_UnitSz, 0)) > Area_.Radius)
						break;

					_InsertIDXs(Point, Area, _2Space[ScanPoint.Y - 1][ScanPoint.X - 1]);
				}
			}

			// bottom
			for (ScanPoint.Y = UnitPoint.Y + 1; ScanPoint.Y < _UnitHeight; ++ScanPoint.Y)
			{
				ScanPoint.X = UnitPoint.X;

				if ((ScanPoint.Y*_UnitSz) - Point.Y > Area_.Radius)
					break;

				_InsertIDXs(Point, Area, _2Space[ScanPoint.Y][ScanPoint.X]);


				// right
				for (ScanPoint.X = UnitPoint.X + 1; ScanPoint.X < _UnitWidth; ++ScanPoint.X)
				{
					if (_DEF_Dist2D(Point, SPoint3(ScanPoint.X*_UnitSz, ScanPoint.Y*_UnitSz, 0)) > Area_.Radius)
						break;

					_InsertIDXs(Point, Area, _2Space[ScanPoint.Y][ScanPoint.X]);
				}

				// left
				for (ScanPoint.X = UnitPoint.X; ScanPoint.X > 0; --ScanPoint.X)
				{
					if (_DEF_Dist2D(Point, SPoint3(ScanPoint.X*_UnitSz, ScanPoint.Y*_UnitSz, 0)) > Area_.Radius)
						break;

					_InsertIDXs(Point, Area, _2Space[ScanPoint.Y][ScanPoint.X - 1]);
				}
			}
			////////////////////////////////////////////////////////////////////////////////////

			// Sort IDXs container
			std::sort(_IDXs.Begin(), _IDXs.Begin() + _IDXs.GetObjCnt());

			return &_IDXs;
		}

#undef _DEF_Dist3D
#undef _DEF_Dist2D
	}
}
