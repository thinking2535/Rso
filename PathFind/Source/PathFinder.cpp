#include "PathFinder.h"
#include <iostream>
using namespace std;

namespace rso
{
	namespace pathfind
	{
		void CPathFinder::_ConnectPointToPoint(const SPoint& From_, const SPoint& To_)
		{
			// ���� Point ���� 1Cell ��ŭ���� �ƴϰ�, ���� Cell�� ���� ��ǥ���� �����ϰ� ó���� �� �ֵ���.

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
			// ��ֹ��� 2���� ���� ����
			// _ConnectPointToPoint(From_, );
		}
		CPathFinder::CPathFinder(__int32 Width_, __int32 Height_) :
			_Map(Width_, vector<_SCell>(Height_))
		{
		}
		size_t CPathFinder::AddObstacle(__int32 X_, __int32 Y_, __int32 Width_, __int32 Height_)
		{
			// �������� �� Obstacle �� ����
			// _ConnectPointToPoint() �� ȣ��

			return _Obstacles.emplace(_SObstacle()).Index();

			// _Obstacles Ž���Ͽ� ���� üũ, �� ����
		}
		void CPathFinder::DelObstacle(size_t Index_)
		{
			_Obstacles[Index_]

			//_Obstacles ���� �����ϵ� ���յ� ��ֹ� �и��Ұ�.
			//���ᵵ �����ϰ�,
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

			// ��ϵ� ��ֹ� clear?
			// ���������� ����� ���� Ž���ϵ�
			// ���� ���� ��ο� ��ֹ��� ������?
			//		��ֹ��� ����Ʈ�� ������ ����ó��
			// �ƴϸ�

			return _Path;
		}
	}
}
