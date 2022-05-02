#pragma once

#include <Rso/Core/Core.h>

#pragma comment( lib, "Rso/Util_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <Rso/Math/Base.h>

namespace rso
{
	namespace util
	{
		using namespace core;
		using namespace base;
		using namespace math;

		using TDist = int64;
		using TDegree = double;
		using TDirect = double;
		using TRadian = double;
		using TCharIDX = std::size_t;
		const TCharIDX c_CharIDX_Null = (numeric_limits<TCharIDX>::max)();

		struct _SPoint3
		{
			TDist X;
			TDist Y;
			TDist Z;
		};

		struct SVector3
		{
			TDirect X;
			TDirect Y;
			TDirect Z;

			SVector3() : X(0), Y(0), Z(0) {}
			SVector3(TDirect X_, TDirect Y_, TDirect Z_) : X(X_), Y(Y_), Z(Z_) {}
		};

		struct SPoint3 : public _SPoint3
		{
			SPoint3()
			{
				X = 0;
				Y = 0;
				Z = 0;
			}
			SPoint3(const _SPoint3& Point_) : _SPoint3(Point_) {}
			SPoint3(const TDist X_, const TDist Y_, const TDist Z_)
			{
				X = X_;
				Y = Y_;
				Z = Z_;
			}
			bool operator == (const SPoint3& Point_) const
			{
				if (Point_.X == X &&
					Point_.Y == Y &&
					Point_.Z == Z) return true;

				return false;
			}

			SVector3 operator - (const SPoint3& Point_) const
			{
				SVector3 Vector;

				Vector.X = static_cast<double>(X - Point_.X);
				Vector.Y = static_cast<double>(Y - Point_.Y);
				Vector.Z = static_cast<double>(Z - Point_.Z);

				return Vector;
			}
		};

		struct SArea
		{
			TDegree Degree; // 0 : Sphere 360 : Circle else : Pie
			TDist Radius;
			TDist Height;

			SArea() : Degree(0), Radius(0), Height(0) {}
			SArea(TDegree Degree_, TDist Radius_, TDist Height_) : Degree(Degree_), Radius(Radius_), Height(Height_) {}
		};

		struct _SArea
		{
			// Start == End == 0 : Sphere
			enum EType
			{
				Type_Sphere,
				Type_Circle,
				Type_Pie,
				Type_Max,
				Type_Null,
			};

			EType Type;
			SVector3 VecStart;
			SVector3 VecEnd;
			TDist Height;
			TDist Radius;

			_SArea() : Type(Type_Null), Radius(0) {}
			_SArea(const SArea& Area_, const SVector3& Dir_)
			{
				if (Area_.Degree == 0)
				{
					Type = Type_Sphere;
				}
				else if (Area_.Degree == 360)
				{
					Type = Type_Circle;
					Height = Area_.Height;
				}
				else
				{
					Type = Type_Pie;

					double cs1 = std::cos(-c_PI / 360.0*(Area_.Degree));
					double sn1 = std::sin(-c_PI / 360.0*(Area_.Degree));
					double cs2 = std::cos(c_PI / 360.0*(Area_.Degree));
					double sn2 = std::sin(c_PI / 360.0*(Area_.Degree));

					VecStart.X = Dir_.X * cs1 - Dir_.Y * sn1;
					VecStart.Y = Dir_.X * sn1 + Dir_.Y * cs1;
					VecStart.Z = 0.0;

					VecEnd.X = Dir_.X * cs2 - Dir_.Y * sn2;
					VecEnd.Y = Dir_.X * sn2 + Dir_.Y * cs2;
					VecEnd.Z = 0.0;
					Height = Area_.Height;
				}
				Radius = Area_.Radius;
			}
		};

		struct STarget
		{
			enum EType
			{
				Type_IDX,
				Type_Point,
				Type_Max,
				Type_Null
			};

			EType Type;
			union
			{
				TCharIDX IDX;
				_SPoint3 Point;
			};

			STarget() : Type(Type_Null) {} // _SSpellInfo::union 때문에 기본 초기화 제거
			STarget(const TCharIDX IDX_) : Type(Type_IDX), IDX(IDX_) {}
			STarget(const SPoint3& Point_) : Type(Type_Point), Point(Point_) {}

			void SetNull(void) { Type = Type_Null; }
		};

		bool IsEmail(const wstring& Str_);

		template<typename TType>
		string NumberToBinaryStr(TType Value_)
		{
			if (!numeric_limits<TType>::is_integer)
				return string();

			string Str(sizeof(TType) * 8, '0');

			for (int i = 0; i < sizeof(TType) * 8; ++i)
			{
				if (Value_ & 1)
					Str[sizeof(TType) * 8 - i - 1] = '1';

				Value_ >>= 1;
			}

			return Str;
		}
	}
}
