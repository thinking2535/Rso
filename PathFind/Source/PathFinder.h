#pragma once

#include "Base.h"
#include <Rso/Base/List.h>

namespace rso
{
	namespace pathfind
	{
		using namespace base;

		class CPathFinder
		{
		private:
			static const size_t c_ObjectIndex = size_t(~0);
			struct _SCell
			{
				size_t ObjectIndex = c_ObjectIndex;
			};
			using _TMap = vector<vector<_SCell>>;

			struct _SObstacle
			{
				TIndex Index = 0;

				//장애물은 서로 병합, 분리 가능하도록

				~_SObstacle()
				{
					// 현 객체에 연결된 연결 제거
				}
			};
			using _TObstacles = CList<_SObstacle>;

			TIndex _Index = 0;
			_TMap _Map;
			_TObstacles _Obstacles;
			TPath _Path;

			void _ConnectPointToPoint(const SPoint& From_, const SPoint& To_);
			void _ConnectToObstacle(const SPoint& From_, _SObstacle& Obstacle_);
			inline bool _ConnectToCell(const SPoint& From_, __int32 X_, __int32 Y_)
			{
				if (_Map[X_][Y_].ObjectIndex == c_ObjectIndex)
					return false;

				_ConnectToObstacle(From_, _Obstacles[_Map[X_][Y_].ObjectIndex]);

				return true;
			}

		public:
			CPathFinder(__int32 Width_, __int32 Height_);
			size_t AddObstacle(__int32 X_, __int32 Y_, __int32 Width_, __int32 Height_);
			void DelObstacle(size_t Index_);
			const TPath& Find(const SPoint& From_, const SPoint& To_);
		};
	}
}
