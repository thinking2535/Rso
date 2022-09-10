#pragma once

#include <cmath>

namespace rso::gameutil
{
	class EloController
	{
		const double _k;
		const double _diffRatio;
		const double _diffPoint;

	public:
		EloController() noexcept;
		EloController(double k, double diffRatio, double diffPoint) noexcept;
		void play(double& point, double& opponentPoint, double realWinRatio) noexcept;
	};
}
