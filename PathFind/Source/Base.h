#pragma once

#include <Rso/Base/List.h>
#include <Rso/Physics/Physics.h>

#pragma comment( lib, "Rso/PathFind_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

namespace rso
{
	namespace pathfind
	{
		using namespace base;
		using namespace physics;

		using TIndex = unsigned __int64; // do not change to signed type

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
			inline double GetDistance(const SCoord& Coord_) const
			{
				return Distance(SPoint(static_cast<float>(X), static_cast<float>(Y)), SPoint(static_cast<float>(Coord_.X), static_cast<float>(Coord_.Y)));
			}
			inline bool operator == (const SCoord& Coord_) const
			{
				return (X == Coord_.X && Y == Coord_.Y);
			}
			inline bool operator != (const SCoord& Coord_) const
			{
				return !operator == (Coord_);
			}
			inline SCoord GetRight(void) const { return SCoord(X + 1, Y); }
			inline SCoord GetLeft(void) const { return SCoord(X - 1, Y); }
			inline SCoord GetUp(void) const { return SCoord(X, Y + 1); }
			inline SCoord GetDown(void) const { return SCoord(X, Y - 1); }
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
	}
}
