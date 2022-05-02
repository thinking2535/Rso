#pragma once

#include <functional>
#include "Base.h"

namespace rso
{
	namespace pathfind
	{
		using namespace std;
		using namespace placeholders;

		class CPathFind
		{
		public:
			struct SCoord
			{
				__int32 X = 0;
				__int32 Y = 0;

				SCoord()
				{
				}
				SCoord(__int32 X_, __int32 Y_) :
					X(X_), Y(Y_)
				{
				}
				explicit SCoord(const SPoint& Point_) :
					SCoord(Point_.X, Point_.Y)
				{
				}
				//inline double GetDistance(const SCoord& Coord_) const
				//{
				//	return Distance(SPoint(X, Y), SPoint(Point_.X, Point_.Y));
				//}
				inline bool operator == (const SCoord& Coord_) const
				{
					return (X == Coord_.X && Y == Coord_.Y);
				}
				inline bool operator != (const SCoord& Coord_) const
				{
					return !operator == (Coord_);
				}
			};
			struct SRect : public SCoord
			{
				__int32 Width;
				__int32 Height;

				SRect(__int32 X_, __int32 Y_, __int32 Width_, __int32 Height_) :
					SCoord(X_, Y_), Width(Width_), Height(Height_)
				{
				}
			};
			using TPath = vector<SCoord>;

		private:
			using _TCanProceed = function<bool(const SCoord&, __int8&, SCoord&)>;
			struct _SNode
			{
				TIndex Index = 0;
				SPoint Point; // 굴절 노드 체크 위함
				SCoord PrevCoord; // for returning path
				bool Obstacle = false;
			};
			struct _STraceCore
			{
				double WeightSum = 0.0; // 최초(To) 부터 Prev 까지 거리의 합 (Prev 부터 현재 까지의 거리 제외)
				SCoord Coord;
			};
			struct _STrace : public _STraceCore
			{
				__int8 Dir = 0; // 현재 노드에 진입할 때의 방향

				_STrace(double WeightSum_, const SCoord& Coord_, __int8 Dir_) :
					WeightSum(WeightSum_), Coord(Coord_), Dir(Dir_)
				{
				}
				__int8 GetTurnDirRight(void) const
				{
					return (Dir + 1 < 4 ? Dir + 1 : 0);
				}
				__int8 GetTurnDirLeft(void) const
				{
					return (Dir > 0 ? Dir - 1 : 3);
				}
			};
			using _TTraces = multimap<double, _STrace>;
			using _TTracesIt = _TTraces::iterator;

			_TCanProceed _CanProceeds[4] = // n, e, s, w
			{
				std::bind(&CPathFind::_CanProceedNorth, this, _1,_2,_3),
				std::bind(&CPathFind::_CanProceedEast, this, _1,_2,_3),
				std::bind(&CPathFind::_CanProceedSouth, this, _1,_2,_3),
				std::bind(&CPathFind::_CanProceedWest, this, _1,_2,_3)
			};
			vector<vector<_SNode>> _Map;
			TIndex _Index = 0;
			SCoord _From;
			SCoord _To;
			_TTraces _Traces; // first : total weight + last shortest weight
			TPath _Path;
			__int8 _CurDir;
			SCoord _CurCoord;

			bool _CanProceedNorth(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const;
			bool _CanProceedEast(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const;
			bool _CanProceedSouth(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const;
			bool _CanProceedWest(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const;
			bool _Trace(const _STrace& Trace_);
			inline _SNode& _GetNode(const SCoord& Point_)
			{
				return _Map[Point_.X][Point_.Y];
			}

		public:
			CPathFind(__int32 Width_, __int32 Height_);
			inline void SetObstacle(__int32 X_, __int32 Y_, bool Obstacle_)
			{
				_Map[X_][Y_].Obstacle = Obstacle_;
			}
			inline void SetObstacle(const SCoord& Point_, bool Obstacle_)
			{
				SetObstacle(Point_.X, Point_.Y, Obstacle_);
			}
			void SetObstacle(__int32 X_, __int32 Y_, __int32 Width_, __int32 Height_, bool Obstacle_);
			inline void SetObstacle(const SRect& Rect_, bool Obstacle_)
			{
				SetObstacle(Rect_.X, Rect_.Y, Rect_.Width, Rect_.Height, Obstacle_);
			}
			TPath&& Find(const SPoint& From_, const SPoint& To_);

			void Print(void) const;
		};
	}
}
