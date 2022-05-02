#pragma once

#include <Rso/Core/Core.h>

#pragma comment( lib, "Rso/Math_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Base/Base.h>
#include <algorithm>
#include <vector>

namespace rso
{
	namespace math
	{
		using namespace core;
		using namespace std;

		const double c_PI = 3.14159265358979323846;
		const double c_2_PI = (c_PI * 2.0);
		const double c_PI_2 = (c_PI * 0.5);
		const float c_PI_F = 3.14159265358979323846f;
		const float c_2_PI_F = (c_PI_F * 2.0f);
		const float c_PI_F_2 = (c_PI_F * 0.5f);

		inline double NormalDistribution(double X_, double M_, double S_) noexcept
		{
			return (1.0 / (S_ * sqrt(2 * c_PI))) * exp(-0.5 * pow((X_ - M_) / S_, 2.0));
		}
		inline double SimpleNormalDistribution(double X_, double A_, double B_) noexcept
		{
			return (1.0 / 1.0 + pow(A_ * (X_ - B_), 2.0));
		}
		inline double Sigmoid(double X_) noexcept
		{
			return (1.0 / (1.0 + exp(-X_)));
		}
		inline double DerivativeSigmoid(double X_) noexcept
		{
			const auto s = Sigmoid(X_);
			return s * (1.0 - s);
		}
		inline double TanH(double Value_) noexcept
		{
			return tanh(Value_);
		}
		inline double DerivativeTanH(double Value_) noexcept
		{
			const auto t = tanh(Value_);
			return 1.0 - t * t;
		}
		inline double ELU(double Value_) noexcept
		{
			return (Value_ > 0.0 ? Value_ : (exp(Value_) - 1));
		}
		inline double DerivativeELU(double Value_) noexcept
		{
			return (Value_ > 0.0 ? 1.0 : exp(Value_));
		}
	}
}
