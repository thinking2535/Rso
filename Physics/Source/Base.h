#pragma once

#include <Rso/Core/Base.h>

#pragma comment( lib, "Rso/Physics_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include <memory>
#include "Protocol.h"
#include <Rso/Math/Base.h>

namespace rso::physics
{
	using namespace std;
	using namespace math;

	struct SShootInfo
	{
		float X;
		float Y;
		float Time;
	};

	// SPoint operator ////////////////////////////////////////////////////////////
	inline SPoint operator - (const SPoint& Point_)
	{
		return SPoint(-Point_.X, -Point_.Y);
	}
	inline SPoint& operator += (SPoint& Point0_, const SPoint& Point1_)
	{
		Point0_.X += Point1_.X;
		Point0_.Y += Point1_.Y;
		return Point0_;
	}
	inline SPoint operator + (const SPoint& Point0_, const SPoint& Point1_)
	{
		SPoint Point0(Point0_);
		return (Point0 += Point1_);
	}
	inline SPoint& operator -= (SPoint& Point0_, const SPoint& Point1_)
	{
		Point0_.X -= Point1_.X;
		Point0_.Y -= Point1_.Y;
		return Point0_;
	}
	inline SPoint operator - (const SPoint& Point0_, const SPoint& Point1_)
	{
		SPoint Point0(Point0_);
		return (Point0 -= Point1_);
	}
	inline SPoint& operator += (SPoint& Point_, float Value_)
	{
		Point_.X += Value_;
		Point_.Y += Value_;
		return Point_;
	}
	inline SPoint operator + (const SPoint& Point_, float Value_)
	{
		SPoint Point(Point_);
		return (Point += Value_);
	}
	inline SPoint& operator -= (SPoint& Point_, float Value_)
	{
		Point_.X -= Value_;
		Point_.Y -= Value_;
		return Point_;
	}
	inline SPoint operator - (const SPoint& Point_, float Value_)
	{
		SPoint Point(Point_);
		return (Point -= Value_);
	}
	inline SPoint& operator *= (SPoint& Point_, float Value_)
	{
		Point_.X *= Value_;
		Point_.Y *= Value_;
		return Point_;
	}
	inline SPoint operator * (const SPoint& Point_, float Value_)
	{
		SPoint Point(Point_);
		return (Point *= Value_);
	}
	inline SPoint& operator /= (SPoint& Point_, float Value_)
	{
		Point_.X /= Value_;
		Point_.Y /= Value_;
		return Point_;
	}
	inline SPoint operator / (const SPoint& Point_, float Value_)
	{
		SPoint Point(Point_);
		return (Point /= Value_);
	}

	// STPoint operator ////////////////////////////////////////////////////////////
	template<typename TType>
	struct STPoint
	{
		TType X{};
		TType Y{};

		STPoint()
		{
		}
		STPoint(const TType& X_, const TType& Y_) :
			X(X_), Y(Y_)
		{
		}
		STPoint& operator += (const STPoint& Point_)
		{
			X += Point_.X;
			Y += Point_.Y;
			return *this;
		}
		STPoint operator + (const STPoint& Point_) const
		{
			auto Point(*this);
			return (Point += Point_);
		}
		STPoint& operator -= (const STPoint& Point_)
		{
			X -= Point_.X;
			Y -= Point_.Y;
			return *this;
		}
		STPoint operator - (const STPoint& Point_) const
		{
			auto Point(*this);
			return (Point -= Point_);
		}
		STPoint& operator += (const TType& Value_)
		{
			X += Value_;
			Y += Value_;
			return *this;
		}
		STPoint operator + (const TType& Value_) const
		{
			auto Point(*this);
			return (Point += Value_);
		}
		STPoint& operator -= (const TType& Value_)
		{
			X -= Value_;
			Y -= Value_;
			return *this;
		}
		STPoint operator - (const TType& Value_) const
		{
			auto Point(*this);
			return (Point -= Value_);
		}
		STPoint& operator *= (const TType& Value_)
		{
			X *= Value_;
			Y *= Value_;
			return *this;
		}
		STPoint operator * (const TType& Value_) const
		{
			auto Point(*this);
			return (Point *= Value_);
		}
		STPoint& operator /= (const TType& Value_)
		{
			X /= Value_;
			Y /= Value_;
			return *this;
		}
		STPoint operator / (const TType& Value_) const
		{
			auto Point(*this);
			return (Point /= Value_);
		}
	};

	template<typename TType>
	bool operator < (const STPoint<TType>& lhs_, const STPoint<TType>& rhs_)
	{
		if (lhs_.Y < rhs_.Y)
			return true;
		else if (lhs_.Y > rhs_.Y)
			return false;

		return (lhs_.X < rhs_.X);
	}

	// SRect operator ////////////////////////////////////////////////////////////
	inline SRect& operator *= (SRect& Rect_, float Value_)
	{
		Rect_.Left *= Value_;
		Rect_.Right *= Value_;
		Rect_.Bottom *= Value_;
		Rect_.Top *= Value_;
		return Rect_;
	}
	inline SRect operator * (const SRect& Rect_, float Value_)
	{
		SRect Rect(Rect_);
		return (Rect *= Value_);
	}
	inline SRect& operator /= (SRect& Rect_, float Value_)
	{
		Rect_.Left /= Value_;
		Rect_.Right /= Value_;
		Rect_.Bottom /= Value_;
		Rect_.Top /= Value_;
		return Rect_;
	}
	inline SRect operator / (const SRect& Rect_, float Value_)
	{
		SRect Rect(Rect_);
		return (Rect /= Value_);
	}
	inline SRect& operator += (SRect& Rect_, const SPoint& Vector_)
	{
		Rect_.Left += Vector_.X;
		Rect_.Right += Vector_.X;
		Rect_.Bottom += Vector_.Y;
		Rect_.Top += Vector_.Y;
		return Rect_;
	}
	inline SRect operator + (const SRect& Rect_, const SPoint& Vector_)
	{
		SRect Rect(Rect_);
		return (Rect += Vector_);
	}
	inline SRect& operator -= (SRect& Rect_, const SPoint& Vector_)
	{
		Rect_.Left -= Vector_.X;
		Rect_.Right -= Vector_.X;
		Rect_.Bottom -= Vector_.Y;
		Rect_.Top -= Vector_.Y;
		return Rect_;
	}
	inline SRect operator - (const SRect& Rect_, const SPoint& Vector_)
	{
		SRect Rect(Rect_);
		return (Rect -= Vector_);
	}
	inline SRect& operator *= (SRect& Rect_, const SPoint& Vector_)
	{
		Rect_.Left *= Vector_.X;
		Rect_.Right *= Vector_.X;
		Rect_.Bottom *= Vector_.Y;
		Rect_.Top *= Vector_.Y;
		return Rect_;
	}
	inline SRect operator * (const SRect& Rect_, const SPoint& Vector_)
	{
		SRect Rect(Rect_);
		return (Rect *= Vector_);
	}
	inline SRect& operator /= (SRect& Rect_, const SPoint& Vector_)
	{
		Rect_.Left /= Vector_.X;
		Rect_.Right /= Vector_.X;
		Rect_.Bottom /= Vector_.Y;
		Rect_.Top /= Vector_.Y;
		return Rect_;
	}
	inline SRect operator / (const SRect& Rect_, const SPoint& Vector_)
	{
		SRect Rect(Rect_);
		return (Rect /= Vector_);
	}
	inline SPoint GetLeftTop(const SRect& Rect_)
	{
		return SPoint(Rect_.Left, Rect_.Top);
	}
	inline SPoint GetRightTop(const SRect& Rect_)
	{
		return SPoint(Rect_.Right, Rect_.Top);
	}
	inline SPoint GetLeftBottom(const SRect& Rect_)
	{
		return SPoint(Rect_.Left, Rect_.Bottom);
	}
	inline SPoint GetRightBottom(const SRect& Rect_)
	{
		return SPoint(Rect_.Right, Rect_.Bottom);
	}

	// STRect operator ////////////////////////////////////////////////////////////
	template<typename TType>
	struct STRect
	{
		TType Left{};
		TType Right{};
		TType Bottom{};
		TType Top{};
		STRect()
		{
		}
		STRect(const TType& Left_, const TType& Right_, const TType& Bottom_, const TType& Top_) :
			Left(Left_), Right(Right_), Bottom(Bottom_), Top(Top_)
		{
		}
		TType Width(void) const
		{
			return (Right - Left);
		}
		TType Height(void) const
		{
			return (Top - Bottom);
		}
		STRect& operator *= (const TType& Value_)
		{
			Left *= Value_;
			Right *= Value_;
			Bottom *= Value_;
			Top *= Value_;
			return *this;
		}
		STRect operator * (const TType& Value_) const
		{
			auto Rect(*this);
			return (Rect *= Value_);
		}
		STRect& operator /= (const TType& Value_)
		{
			Left /= Value_;
			Right /= Value_;
			Bottom /= Value_;
			Top /= Value_;
			return *this;
		}
		STRect operator / (const TType& Value_) const
		{
			auto Rect(*this);
			return (Rect /= Value_);
		}
		STRect& operator += (const STPoint<TType>& Vector_)
		{
			Left += Vector_.X;
			Right += Vector_.X;
			Bottom += Vector_.Y;
			Top += Vector_.Y;
			return *this;
		}
		STRect operator + (const STPoint<TType>& Vector_) const
		{
			auto Rect(*this);
			return (Rect += Vector_);
		}
		STRect& operator -= (const STPoint<TType>& Vector_)
		{
			Left -= Vector_.X;
			Right -= Vector_.X;
			Bottom -= Vector_.Y;
			Top -= Vector_.Y;
			return *this;
		}
		STRect operator - (const STPoint<TType>& Vector_) const
		{
			auto Rect(*this);
			return (Rect -= Vector_);
		}
	};
	inline SRect RectCollider2DToRect(const SRectCollider2D& RectCollider_)
	{
		return SRect(-RectCollider_.Size.X * 0.5f, RectCollider_.Size.X * 0.5f, -RectCollider_.Size.Y * 0.5f, RectCollider_.Size.Y * 0.5f) + RectCollider_.Offset;
	}

	inline SRectCollider2D RectCollider2DTo90Rotated(const SRectCollider2D& Self_) // CCW
	{
		return SRectCollider2D(SPoint(Self_.Size.Y, Self_.Size.X), SPoint(-Self_.Offset.Y, Self_.Offset.X));
	}
	inline SRectCollider2D RectCollider2DTo180Rotated(const SRectCollider2D& Self_)
	{
		return SRectCollider2D(Self_.Size, SPoint(-Self_.Offset.X, -Self_.Offset.Y));
	}
	inline SRectCollider2D RectCollider2DTo270Rotated(const SRectCollider2D& Self_) // CCW
	{
		return SRectCollider2D(SPoint(Self_.Size.Y, Self_.Size.X), SPoint(Self_.Offset.Y, -Self_.Offset.X));
	}
}
