#pragma once

#include "Base.h"

namespace rso
{
	namespace pathfind
	{
		class CPathFind
		{
			struct _SNode
			{
				TIndex Index = 0;
				SCoord PrevCoord; // for returning path
				bool Obstacle = false;
			};
			struct _STrace
			{
				double WeightSum = 0.0;
				SCoord Coord;

				_STrace(double WeightSum_, const SCoord& Coord_) :
					WeightSum(WeightSum_), Coord(Coord_)
				{
				}
			};
			using _TTraces = multimap<double, _STrace>;
			using _TTracesIt = _TTraces::iterator;

			vector<vector<_SNode>> _Map;
			TIndex _Index = 0;
			SCoord _From;
			SCoord _To;
			_TTraces _Traces; // first : total weight + last shortest weight
			TPath _Path;

			bool _Trace(_TTracesIt It_, const SCoord& Coord_);
			inline _SNode& _GetNode(const SCoord& Coord_)
			{
				return _Map[Coord_.X][Coord_.Y];
			}

		public:
			CPathFind(__int32 Width_, __int32 Height_);
			inline void SetObstacle(__int32 X_, __int32 Y_, bool Obstacle_)
			{
				_Map[X_][Y_].Obstacle = Obstacle_;
			}
			inline void SetObstacle(const SCoord& Coord_, bool Obstacle_)
			{
				SetObstacle(Coord_.X, Coord_.Y, Obstacle_);
			}
			void SetObstacle(__int32 X_, __int32 Y_, __int32 Width_, __int32 Height_, bool Obstacle_);
			inline void SetObstacle(const SRect& Rect_, bool Obstacle_)
			{
				SetObstacle(Rect_.X, Rect_.Y, Rect_.Width, Rect_.Height, Obstacle_);
			}
			const TPath& Find(const SCoord& From_, const SCoord& To_);
		};
	}
}
