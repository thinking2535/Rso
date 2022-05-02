#pragma once


#include "Random.h"
#include <Rso/Physics/Base.h>


namespace rso
{
	namespace util
	{
		using namespace physics;

		class CRandomRect
		{
			CRandom<std::mt19937_64, float> _RandomX;
			CRandom<std::mt19937_64, float> _RandomY;

		public:
			CRandomRect(const SRect& Rect_) :
				_RandomX(Rect_.Left, Rect_.Right),
				_RandomY(Rect_.Bottom, Rect_.Top)
			{
			}
			inline SPoint Get(void)
			{
				return SPoint(_RandomX.Get(), _RandomY.Get());
			}
		};
		class CRandomCircle
		{
			CRandom<std::mt19937_64, float> _RandomTheta{ 0.0f, c_2_PI_F };
			CRandom<std::mt19937_64, float> _RandomRadius;

		public:
			CRandomCircle(float Radius_) :
				_RandomRadius(0.0f, Radius_)
			{
			}
			SPoint Get(void)
			{
				auto Theta = _RandomTheta.Get();
				auto Radius = _RandomRadius.Get();

				return SPoint(cos(Theta) * Radius, sin(Theta) * Radius);
			}
		};
		class CRandomDonut
		{
			CRandom<std::mt19937_64, float> _RandomTheta{ 0.0, c_2_PI_F };
			float _Radius;
			CRandom<std::mt19937_64, float> _RandomDistance;

		public:
			CRandomDonut(float Radius_, float Distance_) :
				_Radius(Radius_),
				_RandomDistance(float{}, Distance_)
			{
			}
			SPoint Get(void)
			{
				auto Theta = _RandomTheta.Get();
				auto RadiusPlusDistance = _Radius + _RandomDistance.Get();
				return SPoint(cos(Theta) * RadiusPlusDistance, sin(Theta) * RadiusPlusDistance);
			}
		};
	}
}