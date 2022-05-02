#include "PathFinder.h"
#include <iostream>
using namespace std;

namespace rso
{
	namespace pathfind
	{
		void CPathFinder::_ConnectPointToPoint(const SPoint& From_, const SPoint& To_)
		{
			// 시작 Point 부터 1Cell 만큼씩이 아니고, 다음 Cell의 시작 좌표부터 균일하게 처리할 수 있도록.

			auto Vector = (To_ - From_);
			auto iFromX = __int32(From_.X);
			auto iFromY = __int32(From_.Y);

			if (Vector.X >= Vector.Y)
			{
				if (_ConnectToCell(From_, iFromX, iFromY))
					return;

				auto iToX = __int32(To_.X);
				for (auto iX = iFromX + 1; iX <= iToX; ++iX)
				{
					auto Y = (double)(iX - iFromX) / Vector.X * Vector.Y + From_.Y;

					if (Y > double(iFromY + 1))
						if (_ConnectToCell(From_, iX - 1, iFromY + 1))
							return;

					auto iY = __int32(Y);
					if (_ConnectToCell(From_, iX, iY))
						return;

					iFromY = iY;
				}
			}
			else
			{
				if (_ConnectToCell(From_, iFromX, iFromY))
					return;

				auto iToY = __int32(To_.Y);
				for (auto iY = iFromY + 1; iY <= iToY; ++iY)
				{
					auto X = (double)(iY - iFromY) / Vector.Y * Vector.X + From_.X;

					if (X > double(iFromX + 1))
						if (_ConnectToCell(From_, iFromX + 1, iY - 1))
							return;

					auto iX = __int32(X);
					if (_ConnectToCell(From_, iX, iY))
						return;

					iFromX = iX;
				}
			}
		}
		void CPathFinder::_ConnectToObstacle(const SPoint& From_, _SObstacle& Obstacle_)
		{
			// 장애물의 2개의 점과 연결
			// _ConnectPointToPoint(From_, );
		}
		CPathFinder::CPathFinder(__int32 Width_, __int32 Height_) :
			_Map(Width_, vector<_SCell>(Height_))
		{
		}
		size_t CPathFinder::AddObstacle(__int32 X_, __int32 Y_, __int32 Width_, __int32 Height_)
		{
			// 꼭지점과 각 Obstacle 에 연결
			// _ConnectPointToPoint() 로 호출

			return _Obstacles.emplace(_SObstacle()).Index();

			// _Obstacles 탐색하여 연결 체크, 및 병합
		}
		void CPathFinder::DelObstacle(size_t Index_)
		{
			_Obstacles[Index_]

			//_Obstacles 에서 삭제하되 병합된 장애물 분리할것.
			//연결도 해제하고,
		}
		const TPath& CPathFinder::Find(const SPoint& From_, const SPoint& To_)
		{
			if (_Index + 1 < _Index)
			{
				for (auto& i : _Obstacles)
					i.Index = 0;
			}

			++_Index;

			_ConnectPointToPoint(From_, To_);

			// 등록된 장애물 clear?
			// 도착점에서 출발점 까지 탐색하되
			// 직접 연결 경로에 장애물이 있으면?
			//		장애물의 포인트와 가능한 연결처리
			// 아니면

			return _Path;
		}
	}
}
