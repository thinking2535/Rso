#include "EloController.h"

namespace rso::gameutil
{
	EloController::EloController() noexcept :
		_k(20.0),
		_diffRatio(10.0),
		_diffPoint(400.0)
	{
	}
	EloController::EloController(double k, double diffRatio, double diffPoint) noexcept :
		_k(k),
		_diffRatio(diffRatio),
		_diffPoint(diffPoint)
	{
	}
	double EloController::getAddedPoint(double point, double opponentPoint, double realWinRatio) noexcept
	{
		const auto winRatio = 1.0 / (pow(_diffRatio, (opponentPoint - point) / _diffPoint) + 1);
		return _k * (realWinRatio - winRatio);
	}
}
