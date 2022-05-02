#pragma once

#include "Base.h"

namespace rso::physics
{
	// Line, Circle  Equation /////////////////////////////

	// ( 단, 직선은 y축에 평행하지 않음 )
	// m = (y1-y0)/(x1-x0)
	// A = (1+m*m)
	// B = -2*(a+m*(m*x0-y0+b))
	// C = (b+m*x0-y0)*(b+m*x0-y0) + a*a - r*r

	// ( 단, 직선은 x축에 평행하지 않음 )
	// n = (x1-x0)/(y1-y0)
	// A = (1+n*n)
	// B = -2*(b+n*(n*y0-x0+a))
	// C = (a+n*y0-x0)*(a+n*y0-x0) + b*b - r*r

	///////////////////////////////////////////////////////

	struct SShootInfo
	{
		float X;
		float Y;
		float Time;
	};

	// 역 탄도 계산 ( 발사속도를 알고, 탄착점의 좌표를 알때 X,Y 벡터와 시간을 얻는 함수 )
	bool GetShootInfo(float G_, float V_, float X_, float Y_, SShootInfo& ShootInfo_);

	inline float GetYFromParabola(float G_, float Y0_, float dY0_, float T_)
	{
		return ((G_ / 2.0f) * T_ * T_ + dY0_ * T_ + Y0_);
	}
	inline float GetXFromParabola(float X0_, float dX0_, float T_)
	{
		return ((dX0_ * T_) + X0_);
	}

	// SPoint operator ////////////////////////////////////////////////////////////
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

	// Functions //////////////////////////////////////////////////////////////////////
	inline SPoint Vector(float Theta_, float Radius_) { return SPoint(cos(Theta_) * Radius_, sin(Theta_) * Radius_); }
	inline float DistanceSquare(const SPoint& Point0_, const SPoint& Point1_) { return ((Point0_.X - Point1_.X) * (Point0_.X - Point1_.X) + (Point0_.Y - Point1_.Y) * (Point0_.Y - Point1_.Y)); }
	inline float Distance(const SPoint& Point0_, const SPoint& Point1_) { return sqrt(DistanceSquare(Point0_, Point1_)); }
	inline float GetScalar(const SPoint& Vector_) { return std::sqrt(Vector_.X * Vector_.X + Vector_.Y * Vector_.Y); }
	inline float Atan2(const SPoint& Vector_) { return atan2(Vector_.Y, Vector_.X); }
	float ThetaOfTwoThetas(float Theta0_, float Theta1_);
	float ThetaOfTwoVectors(const SPoint& Vector0_, const SPoint& Vector1_);	// +0 : Vector0_ 를 기준으로 Vector1_ 이 우측
	bool IsCenterThetaBetweenTwoThetas(float Theta_, float LeftTheta_, float RightTheta_);
	SPoint SymmetryPoint(const SPoint& Point_, const SLine& Line_);
	inline float vRadianToCWvDegree(float vRadian_) { return -(180.0f * vRadian_ / c_PI_F); }
	inline float CWvDegreeTovRadian(float vCWDegree_) { return -((vCWDegree_ * c_PI_F) / 180.0f); }
	float RadianToCWDegree(float Radian_);   // X좌표의 +방향이 0 Radian, 90도, Radian은 CCW
	float CWDegreeToRadian(float CWDegree_);   // X좌표의 +방향이 0 Radian, 90도, Radian은 CCW
	SPointRect RectThetaToPointRect(const SRectTheta& RectTheta_);
	SPoint GetCurPos(SObjectStraight& ObjectStraight_, float Time_);
	inline SPosTheta GetCurPosTheta(SObjectStraight& ObjectStraight_, float Time_) { return SPosTheta(GetCurPos(ObjectStraight_, Time_), ObjectStraight_.PosTheta.Theta); }
	inline SRect GetRect(const SPoint& Size_) { return SRect(-Size_.X * 0.5f, Size_.X * 0.5f, -Size_.Y * 0.5f, Size_.Y * 0.5f); }
	SRect GetRect(const SEngineRect& EngineRect_);

	bool IsOverlappedPointRect(const SPoint& Point_, const SRect& Rect_);
	bool IsOverlappedRectRect(const SRect& Rect0_, const SRect& Rect1_);
	bool IsOverlappedPointCircle(const SPoint& Point_, const SCircle& Circle_);
	bool IsOverlappedPointSector(const SPoint& Point_, const SSector& Sector_);
	bool IsOverlappedVLineCircle(const SVLine& Line_, const SCircle& Circle_);
	bool IsOverlappedVSegmentCircle(const SVSegment& Line_, const SCircle& Circle_);
	bool IsOverlappedHLineCircle(const SHLine& Line_, const SCircle& Circle_);
	bool IsOverlappedHSegmentCircle(const SHSegment& Line_, const SCircle& Circle_);
	bool IsOverlappedLineCircle(const SLine& Line_, const SCircle& Circle_);
	bool IsOverlappedSegmentCircle(const SSegment& Segment_, const SCircle& Circle_);
	bool IsOverlappedPointPointRect(const SPoint& Point_, const SPointRect& PointRect_);
	bool IsOverlappedPointRectTheta(const SPoint& Point_, const SRectTheta& RectTheta_);
	bool IsOverlappedCircleRectTheta(const SCircle& Circle_, const SRectTheta& RectTheta_);
	bool IsOverlappedCircleRect(const SCircle& Circle_, const SRect& Rect_);
	bool IsOverlappedCircleCircle(const SCircle& Circle0_, const SCircle& Circle1_);
	bool IsOverlappedCircleSector(const SCircle& Circle_, const SSector& Sector_);
	bool IsOverlappedCirclePointRect(const SCircle& Circle_, const SPointRect& PointRect_);
	bool IsOverlappedPointRectPointRect(const SPointRect& PointRect0_, const SPointRect& PointRect1_);
	bool IsCollidedPointVLine(const SPoint& Point_, const SVLine& VLine_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedPointHLine(const SPoint& Point_, const SHLine& HLine_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedPointVSegment(const SPoint& Point_, const SVSegment& VSegment_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedPointHSegment(const SPoint& Point_, const SHSegment& HSegment_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	//	 bool	IsCollidedPointSegment(const SPoint& Point_, const SSegment& Segment_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedVSegmentVSegment(const SVSegment& VSegment0_, const SVSegment& VSegment1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedHSegmentHSegment(const SHSegment& HSegment0_, const SHSegment& HSegment1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedPointRect(const SPoint& Point_, const SRect& Rect_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedRectRect(const SRect& Rect0_, const SRect& Rect1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedRectRect2(const SRect& Rect0_, const SRect& Rect1_, const SPoint& Dir0_, const SPoint& Dir1_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedCircleRect(const SCircle& Circle_, const SRect& Rect_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedPointCircle(const SPoint& Point_, const SCircle& Circle_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedCircleCircle(const SCircle& Circle0_, const SCircle& Circle1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedPointCircle2(const SPoint& Point_, const SCircle& Circle_, const SPoint& Dir0_, const SPoint& Dir1_, SCollisionInfo& CollisionInfo_);
	bool IsCollidedCircleCircle2(const SCircle& Circle0_, const SCircle& Circle1_, const SPoint& Dir0_, const SPoint& Dir1_, SCollisionInfo& CollisionInfo_);
	bool MoveTowards(SPoint& Current_, const SPoint& Target_, float DistanceDelta_);
}