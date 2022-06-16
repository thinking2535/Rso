#include "Physics.h"

namespace rso::physics
{
	bool GetShootInfo(float G_, float V_, float X_, float Y_, SShootInfo& ShootInfo_)
	{
		auto A = G_ * G_ / 4.0f;
		auto B = -(V_ * V_ + Y_ * G_);
		auto C = X_ * X_ + Y_ * Y_;
		auto D = B * B - 4.0f * A * C;

		float T = 0.0f;

		if (D < 0.0f)
			return false;

		else if (D == 0.0f)
		{
			T = -B / (2.0f * A);

			if (T <= 0.0f)
				return false;
		}
		else
		{
			auto T0 = (-B + sqrt(D)) / (2.0f * A);
			auto T1 = (-B - sqrt(D)) / (2.0f * A);

			if (T0 > 0.0f)
			{
				if (T1 > 0.0f)
					T = (T0 < T1 ? T0 : T1);
				else
					T = T0;
			}
			else
			{
				if (T1 > 0.0f)
					T = T1;
				else
					return false;
			}
		}

		ShootInfo_.Time = sqrt(T);
		ShootInfo_.X = X_ / ShootInfo_.Time;
		ShootInfo_.Y = sqrt(V_ * V_ - ShootInfo_.X * ShootInfo_.X);

		return true;
	}
	float ThetaOfTwoThetas(float Theta0_, float Theta1_)
	{
		auto Theta = Theta0_ - Theta1_;
		if (Theta > c_PI_F)
			Theta -= c_2_PI_F;
		else if (Theta < -c_PI_F)
			Theta += c_2_PI_F;

		return Theta;
	}
	// return  - : left side  + : right side
	float ThetaOfTwoVectors(const SPoint& Vector0_, const SPoint& Vector1_)
	{
		return ThetaOfTwoThetas(atan2(Vector0_.Y, Vector0_.X), atan2(Vector1_.Y, Vector1_.X));
	}
	bool IsCenterThetaBetweenTwoThetas(float Theta_, float LeftTheta_, float RightTheta_)
	{
		auto RangeAngle = LeftTheta_ - RightTheta_;
		if (RangeAngle < 0.0f)
			RangeAngle += c_2_PI_F;

		auto Angle = LeftTheta_ - Theta_;
		if (Angle < 0.0f)
			Angle += c_2_PI_F;

		if (Angle > RangeAngle)
			return false;
		else
			return true;
	}
	SPoint SymmetryPoint(const SPoint& Point_, const SLine& Line_)
	{
		SPoint PointVector(Point_ - Line_.Point0);
		auto NewTheta = (atan2(PointVector.Y, PointVector.X) - 2.0f * ThetaOfTwoVectors(PointVector, Line_.Point1 - Line_.Point0));
		auto Radius = sqrt(PointVector.X * PointVector.X + PointVector.Y * PointVector.Y);
		return SPoint(cos(NewTheta) * Radius, sin(NewTheta) * Radius) + Line_.Point0;
	}
	float RadianToCWDegree(float Radian_)   // X좌표의 +방향이 0 Radian, 90도, Radian은 CCW
	{
		//rad   deg
		//0     90
		//p/2   0
		//p     270
		//p*3/2 180
		auto CWDegree = (90.0f + vRadianToCWvDegree(Radian_));
		auto Div = CWDegree / 360.0f;
		return CWDegree - (360.0f * int(Div));
	}
	float CWDegreeToRadian(float CWDegree_)   // X좌표의 +방향이 0 Radian, 90도, Radian은 CCW
	{
		//rad   deg
		//0     90
		//p/2   0
		//p     270
		//p*3/2 180
		auto Radian = (c_PI_F_2 + CWvDegreeTovRadian(CWDegree_));
		auto Div = Radian / c_2_PI_F;
		return Radian - (c_2_PI_F * int(Div));
	}
	SPointRect RectThetaToPointRect(const SRectTheta& RectTheta_)
	{
		SPoint Center((RectTheta_.Left + RectTheta_.Right) * 0.5f, (RectTheta_.Bottom + RectTheta_.Top) * 0.5f);
		auto Slope = sqrt((RectTheta_.Right - RectTheta_.Left) * (RectTheta_.Right - RectTheta_.Left) + (RectTheta_.Top - RectTheta_.Bottom) * (RectTheta_.Top - RectTheta_.Bottom)) * 0.5f;
		SPoint TopRight(SPoint(RectTheta_.Right, RectTheta_.Top) - Center);
		auto BaseTheta = atan2(TopRight.Y, TopRight.X);
		return SPointRect(
			Center + SPoint(cos((BaseTheta + RectTheta_.Theta)) * Slope, sin((BaseTheta + RectTheta_.Theta)) * Slope),
			Center + SPoint(cos(c_PI_F - BaseTheta + RectTheta_.Theta) * Slope, sin(c_PI_F - BaseTheta + RectTheta_.Theta) * Slope),
			Center + SPoint(cos(c_PI_F + BaseTheta + RectTheta_.Theta) * Slope, sin(c_PI_F + BaseTheta + RectTheta_.Theta) * Slope),
			Center + SPoint(cos(-BaseTheta + RectTheta_.Theta) * Slope, sin(-BaseTheta + RectTheta_.Theta) * Slope)
		);
	}
	SPoint GetCurPos(SObjectStraight& ObjectStraight_, float Time_)
	{
		if (ObjectStraight_.Dist == 0.0f)
			return ObjectStraight_.PosTheta.Pos;

		auto CurDist = ObjectStraight_.Speed * (Time_ - ObjectStraight_.Time);
		if (CurDist > ObjectStraight_.Dist)
		{
			CurDist = ObjectStraight_.Dist;
			ObjectStraight_.Dist = 0.0f;
			ObjectStraight_.Time = Time_;
			return ObjectStraight_.PosTheta.Pos += (Vector(ObjectStraight_.PosTheta.Theta, CurDist));
		}
		else
		{
			return ObjectStraight_.PosTheta.Pos + (Vector(ObjectStraight_.PosTheta.Theta, CurDist));
		}
	}

	bool IsOverlappedPointRect(const SPoint& Point_, const SRect& Rect_)
	{
		if (
			Point_.X >= Rect_.Left &&
			Point_.X <= Rect_.Right &&
			Point_.Y >= Rect_.Bottom &&
			Point_.Y <= Rect_.Top)
			return true;

		return false;
	}
	bool IsOverlappedRectRect(const SRect& Rect0_, const SRect& Rect1_)
	{
		return (
			Rect0_.Left <= Rect1_.Right &&
			Rect0_.Right >= Rect1_.Left &&
			Rect0_.Bottom <= Rect1_.Top &&
			Rect0_.Top >= Rect1_.Bottom);
	}
	bool IsOverlappedPointCircle(const SPoint& Point_, const SCircle& Circle_)
	{
		if ((Point_.X - Circle_.X) * (Point_.X - Circle_.X) + (Point_.Y - Circle_.Y) * (Point_.Y - Circle_.Y) <= Circle_.Radius * Circle_.Radius)
			return true;

		return false;
	}
	bool IsOverlappedPointSector(const SPoint& Point_, const SSector& Sector_)
	{
		return (IsCenterThetaBetweenTwoThetas(Atan2(Point_ - Sector_), Sector_.LeftTheta, Sector_.RightTheta) && IsOverlappedPointCircle(Point_, Sector_));
	}
	bool IsOverlappedVLineCircle(const SVLine& Line_, const SCircle& Circle_)
	{
		if (Line_.X < Circle_.X - Circle_.Radius ||
			Line_.X > Circle_.X + Circle_.Radius)
			return false;

		return true;
	}
	bool IsOverlappedVSegmentCircle(const SVSegment& Line_, const SCircle& Circle_)
	{
		if (!IsOverlappedVLineCircle(Line_, Circle_))
			return false;

		if (Circle_.Y <= Line_.Top &&
			Circle_.Y >= Line_.Bottom)
			return true;

		return (IsOverlappedPointCircle(SPoint(Line_.Top, Line_.X), Circle_) ||
			IsOverlappedPointCircle(SPoint(Line_.Bottom, Line_.X), Circle_));
	}
	bool IsOverlappedHLineCircle(const SHLine& Line_, const SCircle& Circle_)
	{
		if (Line_.Y < Circle_.Y - Circle_.Radius ||
			Line_.Y > Circle_.Y + Circle_.Radius)
			return false;

		return true;
	}
	bool IsOverlappedHSegmentCircle(const SHSegment& Line_, const SCircle& Circle_)
	{
		if (!IsOverlappedHLineCircle(Line_, Circle_))
			return false;

		if (Circle_.X <= Line_.Right &&
			Circle_.X >= Line_.Left)
			return true;

		return (IsOverlappedPointCircle(SPoint(Line_.Right, Line_.Y), Circle_) ||
			IsOverlappedPointCircle(SPoint(Line_.Left, Line_.Y), Circle_));
	}
	bool IsOverlappedLineCircle(const SLine& Line_, const SCircle& Circle_)
	{
		if (Line_.Point0.X == Line_.Point1.X)
		{
			if (Line_.Point0.Y == Line_.Point1.Y)
				return IsOverlappedPointCircle(Line_.Point0, Circle_);
			else
				return IsOverlappedVLineCircle(SVLine(Line_.Point0.X), Circle_);
		}
		else if (Line_.Point0.Y == Line_.Point1.Y)
		{
			return IsOverlappedHLineCircle(SHLine(Line_.Point0.Y), Circle_);
		}
		else
		{
			auto S = (Line_.Point1.Y - Line_.Point0.Y) / (Line_.Point1.X - Line_.Point0.X);
			auto T = -S * Line_.Point0.X + Line_.Point0.Y;
			auto D2 = (S * (T - Circle_.Y) - Circle_.X) * (S * (T - Circle_.Y) - Circle_.X) - (S * S + 1.0) * ((T - Circle_.Y) * (T - Circle_.Y) + Circle_.X * Circle_.X - Circle_.Radius * Circle_.Radius);
			if (D2 < 0)
				return false;

			return true;
		}
	}
	bool IsOverlappedSegmentCircle(const SSegment& Segment_, const SCircle& Circle_)
	{
		if (Segment_.Point0.X == Segment_.Point1.X)
		{
			if (Segment_.Point0.Y == Segment_.Point1.Y)
				return IsOverlappedPointCircle(Segment_.Point0, Circle_);
			else if (Segment_.Point0.Y < Segment_.Point1.Y)
				return IsOverlappedVLineCircle(SVSegment(SVLine(Segment_.Point0.X), Segment_.Point0.Y, Segment_.Point1.Y), Circle_);
			else
				return IsOverlappedVLineCircle(SVSegment(SVLine(Segment_.Point0.X), Segment_.Point1.Y, Segment_.Point0.Y), Circle_);
		}
		else if (Segment_.Point0.Y == Segment_.Point1.Y)
		{
			if (Segment_.Point0.X < Segment_.Point1.X)
				return IsOverlappedHLineCircle(SHSegment(SHLine(Segment_.Point0.Y), Segment_.Point0.X, Segment_.Point1.X), Circle_);
			else
				return IsOverlappedHLineCircle(SHSegment(SHLine(Segment_.Point0.Y), Segment_.Point1.X, Segment_.Point0.X), Circle_);
		}
		else
		{
			auto S = (Segment_.Point1.Y - Segment_.Point0.Y) / (Segment_.Point1.X - Segment_.Point0.X);
			auto T = -S * Segment_.Point0.X + Segment_.Point0.Y;
			auto A = (S * S + 1.0f);
			auto B = 2.0f * (S * (T - Circle_.Y) - Circle_.X);
			auto C = ((T - Circle_.Y) * (T - Circle_.Y) + Circle_.X * Circle_.X - Circle_.Radius * Circle_.Radius);
			auto D = B * B - 4.0f * A * C;
			if (D < 0)
				return false;

			auto X0 = (-B - sqrt(D)) / (2.0f * A);
			auto X1 = (-B + sqrt(D)) / (2.0f * A);
			if (Segment_.Point0.X < Segment_.Point1.X)
			{
				if (X1 < Segment_.Point0.X || X0 > Segment_.Point1.X)
					return false;
			}
			else
			{
				if (X1 < Segment_.Point1.X || X0 > Segment_.Point0.X)
					return false;
			}

			return true;
		}
	}
	bool IsOverlappedPointPointRect(const SPoint& Point_, const SPointRect& PointRect_)
	{
		return (
			ThetaOfTwoVectors(PointRect_.TopLeft - PointRect_.TopRight, Point_ - PointRect_.TopRight) < 0.0f &&
			ThetaOfTwoVectors(PointRect_.BottomLeft - PointRect_.TopLeft, Point_ - PointRect_.TopLeft) < 0.0f &&
			ThetaOfTwoVectors(PointRect_.BottomRight - PointRect_.BottomLeft, Point_ - PointRect_.BottomLeft) < 0.0f &&
			ThetaOfTwoVectors(PointRect_.TopRight - PointRect_.BottomRight, Point_ - PointRect_.BottomRight) < 0.0f);
	}
	bool IsOverlappedPointRectTheta(const SPoint& Point_, const SRectTheta& RectTheta_)
	{
		return IsOverlappedPointPointRect(Point_, RectThetaToPointRect(RectTheta_));
	}
	bool IsOverlappedCircleRectTheta(const SCircle& Circle_, const SRectTheta& RectTheta_)
	{
		// Circle_.Radius 만큼 확장한 사각형에 포함되지 않으면
		auto RectTheta = RectTheta_;
		RectTheta.Left -= Circle_.Radius;
		RectTheta.Right += Circle_.Radius;
		RectTheta.Bottom -= Circle_.Radius;
		RectTheta.Top += Circle_.Radius;
		if (!IsOverlappedPointRectTheta(Circle_, RectTheta))
			return false;

		// 세로로 긴 사각형에 포함되나?
		RectTheta = RectTheta_;
		RectTheta.Bottom -= Circle_.Radius;
		RectTheta.Top += Circle_.Radius;

		if (IsOverlappedPointRectTheta(Circle_, RectTheta))
			return true;

		// 가로로 긴 사각형에 포함되나?
		RectTheta.Bottom = RectTheta_.Bottom;
		RectTheta.Top = RectTheta_.Top;
		RectTheta.Left -= Circle_.Radius;
		RectTheta.Right += Circle_.Radius;

		if (IsOverlappedPointRectTheta(Circle_, RectTheta))
			return true;

		// 각 꼭지점에 포함되나?
		auto PointRect = RectThetaToPointRect(RectTheta_);
		if (IsOverlappedPointCircle(PointRect.TopRight, Circle_) ||
			IsOverlappedPointCircle(PointRect.TopLeft, Circle_) ||
			IsOverlappedPointCircle(PointRect.BottomRight, Circle_) ||
			IsOverlappedPointCircle(PointRect.BottomLeft, Circle_))
			return true;

		return false;
	}
	bool IsOverlappedCircleRect(const SCircle& Circle_, const SRect& Rect_)
	{
		if (
			// 세로로 긴 직사각형에 포함
			IsOverlappedPointRect(Circle_, SRect(Rect_.Left, Rect_.Right, Rect_.Bottom - Circle_.Radius, Rect_.Top + Circle_.Radius)) ||
			// 가로로 긴 직사각형에 포함
			IsOverlappedPointRect(Circle_, SRect(Rect_.Left - Circle_.Radius, Rect_.Right + Circle_.Radius, Rect_.Bottom, Rect_.Top)) ||
			// 네 모서리 1/4 원에 포함
			IsOverlappedPointCircle(SPoint(Rect_.Left, Rect_.Bottom), Circle_) ||
			IsOverlappedPointCircle(SPoint(Rect_.Right, Rect_.Bottom), Circle_) ||
			IsOverlappedPointCircle(SPoint(Rect_.Left, Rect_.Top), Circle_) ||
			IsOverlappedPointCircle(SPoint(Rect_.Right, Rect_.Top), Circle_)
			)
			return true;

		return false;
	}
	bool IsOverlappedCircleCircle(const SCircle& Circle0_, const SCircle& Circle1_)
	{
		if ((Circle0_.X - Circle1_.X) * (Circle0_.X - Circle1_.X) + (Circle0_.Y - Circle1_.Y) * (Circle0_.Y - Circle1_.Y) <=
			(Circle0_.Radius + Circle1_.Radius) * (Circle0_.Radius + Circle1_.Radius))
			return true;

		return false;
	}
	bool IsOverlappedCircleSector(const SCircle& Circle_, const SSector& Sector_)
	{
		if (
			IsOverlappedPointSector(Circle_, SSector(SCircle(Sector_, Sector_.Radius + Circle_.Radius), Sector_.LeftTheta, Sector_.RightTheta)) ||
			IsOverlappedPointCircle(Sector_, Circle_) ||
			IsOverlappedPointCircle(SPoint(cos(Sector_.LeftTheta) * Sector_.Radius, sin(Sector_.LeftTheta) * Sector_.Radius), Circle_) ||
			IsOverlappedPointCircle(SPoint(cos(Sector_.RightTheta) * Sector_.Radius, sin(Sector_.RightTheta) * Sector_.Radius), Circle_))
			return true;

		SPoint LeftVector(cos(Sector_.LeftTheta) * Sector_.Radius, sin(Sector_.LeftTheta) * Sector_.Radius);
		SPoint LeftNormal(cos(Sector_.LeftTheta + c_PI_F_2) * Circle_.Radius, sin(Sector_.LeftTheta + c_PI_F_2) * Circle_.Radius);
		SPoint LeftBR(Sector_ + LeftVector);
		SPoint LeftTL(Sector_ + LeftNormal);
		SPoint LeftTR(LeftTL + LeftVector);
		if (IsOverlappedPointPointRect(Circle_, SPointRect(LeftTR, LeftTL, Sector_, LeftBR)))
			return true;

		SPoint RightVector(cos(Sector_.RightTheta) * Sector_.Radius, sin(Sector_.RightTheta) * Sector_.Radius);
		SPoint RightNormal(cos(Sector_.RightTheta - c_PI_F_2) * Circle_.Radius, sin(Sector_.RightTheta - c_PI_F_2) * Circle_.Radius);
		SPoint RightTR(Sector_ + RightVector);
		SPoint RightBL(Sector_ + RightNormal);
		SPoint RightBR(RightBL + RightVector);
		if (IsOverlappedPointPointRect(Circle_, SPointRect(RightTR, Sector_, RightBL, RightBR)))
			return true;

		return false;
	}
	bool IsOverlappedCirclePointRect(const SCircle& Circle_, const SPointRect& PointRect_)
	{
		return (
			IsOverlappedPointPointRect(Circle_, SPointRect(SPoint(PointRect_.TopRight.X, PointRect_.TopRight.Y + Circle_.Radius), SPoint(PointRect_.TopLeft.X, PointRect_.TopLeft.Y + Circle_.Radius), SPoint(PointRect_.BottomLeft.X, PointRect_.BottomLeft.Y - Circle_.Radius), SPoint(PointRect_.BottomRight.X, PointRect_.BottomRight.Y - Circle_.Radius))) ||
			IsOverlappedPointPointRect(Circle_, SPointRect(SPoint(PointRect_.TopRight.X + Circle_.Radius, PointRect_.TopRight.Y), SPoint(PointRect_.TopLeft.X - Circle_.Radius, PointRect_.TopLeft.Y), SPoint(PointRect_.BottomLeft.X - Circle_.Radius, PointRect_.BottomLeft.Y), SPoint(PointRect_.BottomRight.X + Circle_.Radius, PointRect_.BottomRight.Y))) ||
			IsOverlappedPointCircle(Circle_, SCircle(PointRect_.TopRight, Circle_.Radius)) ||
			IsOverlappedPointCircle(Circle_, SCircle(PointRect_.TopLeft, Circle_.Radius)) ||
			IsOverlappedPointCircle(Circle_, SCircle(PointRect_.BottomLeft, Circle_.Radius)) ||
			IsOverlappedPointCircle(Circle_, SCircle(PointRect_.BottomRight, Circle_.Radius))
			);
	}
	bool IsOverlappedPointRectPointRect(const SPointRect& PointRect0_, const SPointRect& PointRect1_)
	{
		return (
			IsOverlappedPointPointRect(PointRect0_.TopLeft, PointRect1_) ||
			IsOverlappedPointPointRect(PointRect0_.TopRight, PointRect1_) ||
			IsOverlappedPointPointRect(PointRect0_.BottomLeft, PointRect1_) ||
			IsOverlappedPointPointRect(PointRect0_.BottomRight, PointRect1_) ||
			IsOverlappedPointPointRect(PointRect1_.TopLeft, PointRect0_) ||
			IsOverlappedPointPointRect(PointRect1_.TopRight, PointRect0_) ||
			IsOverlappedPointPointRect(PointRect1_.BottomLeft, PointRect0_) ||
			IsOverlappedPointPointRect(PointRect1_.BottomRight, PointRect0_));
	}
	bool IsCollidedPointVLine(const SPoint& Point_, const SVLine& VLine_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (Dir_.X == 0.0f)
			return false;

		if (Dir_.X < 0.0f &&
			Point_.X < VLine_.X)
			return false;

		if (Dir_.X > 0.0f &&
			Point_.X > VLine_.X)
			return false;

		auto ContactY = (Dir_.Y / Dir_.X) * (VLine_.X - Point_.X) + Point_.Y;
		CollisionInfo_.Time = (VLine_.X - Point_.X) / Dir_.X;

		if (CollisionInfo_.Time < 0.0f)
			return false;

		CollisionInfo_.Point = SPoint(VLine_.X, ContactY);
		CollisionInfo_.Normal = SPoint(-Dir_.X, 0.0f);
		return true;
	}
	bool IsCollidedPointHLine(const SPoint& Point_, const SHLine& HLine_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (Dir_.Y == 0.0f)
			return false;

		if (Dir_.Y < 0.0f &&
			Point_.Y < HLine_.Y)
			return false;

		if (Dir_.Y > 0.0f &&
			Point_.Y > HLine_.Y)
			return false;

		auto ContactX = (Dir_.X / Dir_.Y) * (HLine_.Y - Point_.Y) + Point_.X;
		CollisionInfo_.Time = (HLine_.Y - Point_.Y) / Dir_.Y;

		if (CollisionInfo_.Time < 0.0f)
			return false;

		CollisionInfo_.Point = SPoint(ContactX, HLine_.Y);
		CollisionInfo_.Normal = SPoint(0.0f, -Dir_.Y);
		return true;
	}
	bool IsCollidedPointVSegment(const SPoint& Point_, const SVSegment& VSegment_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (!IsCollidedPointVLine(Point_, VSegment_, Dir_, CollisionInfo_))
			return false;

		if (CollisionInfo_.Point.Y < VSegment_.Bottom ||
			CollisionInfo_.Point.Y > VSegment_.Top)
			return false;

		return true;
	}
	bool IsCollidedPointHSegment(const SPoint& Point_, const SHSegment& HSegment_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (!IsCollidedPointHLine(Point_, HSegment_, Dir_, CollisionInfo_))
			return false;

		if (CollisionInfo_.Point.X < HSegment_.Left ||
			CollisionInfo_.Point.X > HSegment_.Right)
			return false;

		return true;
	}
	//bool IsCollidedPointSegment(const SPoint& Point_, const SSegment& Segment_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	//{
	//	return false;
	//}

	bool IsCollidedVSegmentVSegment(const SVSegment& VSegment0_, const SVSegment& VSegment1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (!IsCollidedPointVLine(SPoint(VSegment0_.X, VSegment0_.Bottom), VSegment1_, Dir_, CollisionInfo_))
			return false;

		// 라인의 중첩영역을 임시로 CollisionInfo의 Point에 저장
		CollisionInfo_.Point.X = CollisionInfo_.Point.Y + (VSegment0_.Top - VSegment0_.Bottom);

		if (CollisionInfo_.Point.Y < VSegment1_.Bottom)	// Point.Y : ContactBottom
			CollisionInfo_.Point.Y = VSegment1_.Bottom;
		if (CollisionInfo_.Point.X > VSegment1_.Top)	// Point.X : ContactTop
			CollisionInfo_.Point.X = VSegment1_.Top;

		if (CollisionInfo_.Point.X < CollisionInfo_.Point.Y)	// 중첩영역이 없으면
			return false;

		CollisionInfo_.Point.Y = ((CollisionInfo_.Point.X + CollisionInfo_.Point.Y) / 2.0f);
		CollisionInfo_.Point.X = VSegment1_.X;
		CollisionInfo_.Normal = SPoint(-Dir_.X, 0.0f);
		return true;
	}
	bool IsCollidedHSegmentHSegment(const SHSegment& HSegment0_, const SHSegment& HSegment1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (!IsCollidedPointHLine(SPoint(HSegment0_.Left, HSegment0_.Y), HSegment1_, Dir_, CollisionInfo_))
			return false;

		// 라인의 중첩영역을 임시로 CollisionInfo의 Point에 저장
		CollisionInfo_.Point.Y = CollisionInfo_.Point.X + (HSegment0_.Right - HSegment0_.Left);

		if (CollisionInfo_.Point.X < HSegment1_.Left)	// Point.Y : ContactLeft
			CollisionInfo_.Point.X = HSegment1_.Left;
		if (CollisionInfo_.Point.Y > HSegment1_.Right)	// Point.X : ContactRight
			CollisionInfo_.Point.Y = HSegment1_.Right;

		if (CollisionInfo_.Point.Y < CollisionInfo_.Point.X)	// 중첩영역이 없으면
			return false;

		CollisionInfo_.Point.X = ((CollisionInfo_.Point.Y + CollisionInfo_.Point.X) / 2.0f);
		CollisionInfo_.Point.Y = HSegment1_.Y;
		CollisionInfo_.Normal = SPoint(0.0f, -Dir_.Y);
		return true;
	}
	bool IsCollidedPointRect(const SPoint& Point_, const SRect& Rect_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (Dir_.X > 0.0f)
		{
			if (IsCollidedPointVSegment(Point_, SVSegment(SVLine(Rect_.Left), Rect_.Bottom, Rect_.Top), Dir_, CollisionInfo_))
				return true;
		}
		else if (Dir_.X < 0.0f)
		{
			if (IsCollidedPointVSegment(Point_, SVSegment(SVLine(Rect_.Right), Rect_.Bottom, Rect_.Top), Dir_, CollisionInfo_))
				return true;
		}
		if (Dir_.Y > 0.0f)
		{
			if (IsCollidedPointHSegment(Point_, SHSegment(SHLine(Rect_.Bottom), Rect_.Left, Rect_.Right), Dir_, CollisionInfo_))
				return true;
		}
		else if (Dir_.Y < 0.0f)
		{
			if (IsCollidedPointHSegment(Point_, SHSegment(SHLine(Rect_.Top), Rect_.Left, Rect_.Right), Dir_, CollisionInfo_))
				return true;
		}

		return false;
	}
	bool IsCollidedRectRect(const SRect& Rect0_, const SRect& Rect1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (Dir_.X > 0.0f)
		{
			if (IsCollidedVSegmentVSegment(
				SVSegment(SVLine(Rect0_.Right), Rect0_.Bottom, Rect0_.Top),
				SVSegment(SVLine(Rect1_.Left), Rect1_.Bottom, Rect1_.Top),
				Dir_, CollisionInfo_))
				return true;
		}
		else if (Dir_.X < 0.0f)
		{
			if (IsCollidedVSegmentVSegment(
				SVSegment(SVLine(Rect0_.Left), Rect0_.Bottom, Rect0_.Top),
				SVSegment(SVLine(Rect1_.Right), Rect1_.Bottom, Rect1_.Top),
				Dir_, CollisionInfo_))
				return true;
		}

		if (Dir_.Y > 0.0f)
		{
			if (IsCollidedHSegmentHSegment(
				SHSegment(SHLine(Rect0_.Top), Rect0_.Left, Rect0_.Right),
				SHSegment(SHLine(Rect1_.Bottom), Rect1_.Left, Rect1_.Right),
				Dir_, CollisionInfo_))
				return true;
		}
		else if (Dir_.Y < 0.0f)
		{
			if (IsCollidedHSegmentHSegment(
				SHSegment(SHLine(Rect0_.Bottom), Rect0_.Left, Rect0_.Right),
				SHSegment(SHLine(Rect1_.Top), Rect1_.Left, Rect1_.Right),
				Dir_, CollisionInfo_))
				return true;
		}

		return false;
	}
	bool IsCollidedRectRect2(const SRect& Rect0_, const SRect& Rect1_, const SPoint& Dir0_, const SPoint& Dir1_, SCollisionInfo& CollisionInfo_)
	{
		if (!IsCollidedRectRect(Rect0_, Rect1_, Dir0_ - Dir1_, CollisionInfo_))
			return false;

		CollisionInfo_.Point += (Dir1_ * CollisionInfo_.Time);
		return true;
	}
	bool IsCollidedCircleRect(const SCircle& Circle_, const SRect& Rect_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (Dir_.X == 0.0f &&
			Dir_.Y == 0.0f)
			return false;

		if (IsOverlappedCircleRect(Circle_, Rect_))
		{
			CollisionInfo_.Time = 0.0f;
			return true;
		}

		// 우로 이동중이면
		if (Dir_.X > 0.0f)
		{
			// 좌측 선분에 충돌했나?
			if (IsCollidedPointVSegment(Circle_, SVSegment(SVLine(Rect_.Left - Circle_.Radius), Rect_.Bottom, Rect_.Top), Dir_, CollisionInfo_))
			{
				CollisionInfo_.Point.X += Circle_.Radius;
				CollisionInfo_.Normal = SPoint(-Dir_.X, 0.0f);
				return true;
			}
		}
		// 좌로 이동중이면
		else if (Dir_.X < 0.0f)
		{
			// 우측 선분에 충돌했나?
			if (IsCollidedPointVSegment(Circle_, SVSegment(SVLine(Rect_.Right + Circle_.Radius), Rect_.Bottom, Rect_.Top), Dir_, CollisionInfo_))
			{
				CollisionInfo_.Point.X -= Circle_.Radius;
				CollisionInfo_.Normal = SPoint(-Dir_.X, 0.0f);
				return true;
			}
		}

		// 위로 이동중이면
		if (Dir_.Y > 0.0f)
		{
			// 하단 선분에 충돌했나?
			if (IsCollidedPointHSegment(Circle_, SHSegment(SHLine(Rect_.Bottom - Circle_.Radius), Rect_.Left, Rect_.Right), Dir_, CollisionInfo_))
			{
				CollisionInfo_.Point.Y += Circle_.Radius;
				CollisionInfo_.Normal = SPoint(0.0f, -Dir_.Y);
				return true;
			}
		}
		// 아래로 이동중이면
		else if (Dir_.Y < 0.0f)
		{
			// 상단 선분에 충돌했나?
			if (IsCollidedPointHSegment(Circle_, SHSegment(SHLine(Rect_.Top + Circle_.Radius), Rect_.Left, Rect_.Right), Dir_, CollisionInfo_))
			{
				CollisionInfo_.Point.Y -= Circle_.Radius;
				CollisionInfo_.Normal = SPoint(0.0f, -Dir_.Y);
				return true;
			}
		}


		// 우로 이동중이면
		if (Dir_.X > 0.0f)
		{
			// 위로 이동중이면
			if (Dir_.Y > 0.0f)
			{
				// 좌하단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					// rrr
					//충돌점은 IsCollidedPointCircle 에서 계산을 하였으나 아래에서 조정을 하게되는데 최적화 방법은?
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Bottom;

					// rrr
					//충돌점과 원의 중점을 이은 선분에 수직인 직선에 Dir을 반사시켜 CollisionInfo_.Dir 을 설정
					//반사각;
					return true;
				}
				// 좌상단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
				//  우하단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
			}
			else if (Dir_.Y < 0.0f)
			{
				// 좌상단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
				//  우상단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
				// 좌하단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
			}
			else
			{
				// 좌상단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
				// 좌하단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
			}
		}
		// 좌로 이동중이면
		else if (Dir_.X < 0.0f)
		{
			// 위로 이동중이면
			if (Dir_.Y > 0.0f)
			{
				//  우하단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
				// 좌하단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
				//  우상단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
			}
			else if (Dir_.Y < 0.0f)
			{
				//  우상단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
				//  우하단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
				// 좌상단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
			}
			else
			{
				//  우하단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
				//  우상단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
			}
		}
		else
		{
			// 위로 이동중이면
			if (Dir_.Y > 0.0f)
			{
				// 좌하단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
				//  우하단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Bottom), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Bottom;
					// rrr
					//반사각;
					return true;
				}
			}
			else if (Dir_.Y < 0.0f)
			{
				//  우상단 원에 충돌했나?
				if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Right, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Right;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
				// 좌상단 원에 충돌했나?
				else if (IsCollidedPointCircle(Circle_, SCircle(SPoint(Rect_.Left, Rect_.Top), Circle_.Radius), Dir_, CollisionInfo_))
				{
					CollisionInfo_.Point.X = Rect_.Left;
					CollisionInfo_.Point.Y = Rect_.Top;
					// rrr
					//반사각;
					return true;
				}
			}
		}

		return false;
	}
	bool IsCollidedPointCircle(const SPoint& Point_, const SCircle& Circle_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (Dir_.X == 0.0f &&
			Dir_.Y == 0.0f)
			return false;

		if (IsOverlappedPointCircle(Point_, Circle_))
		{
			CollisionInfo_.Point = Point_;
			// rrr 현 Point와 원의 중점의 수직선에 반사되는 각으로;
			CollisionInfo_.Time = 0.0f;
			return true;
		}

		if (Dir_.Y * Dir_.Y < Dir_.X * Dir_.X) // 가로로 누운 직선이면( 분모 0 방지 및 자료형(현 float) 오차범위 줄이기 위함 )
		{
			auto m = Dir_.Y / Dir_.X;
			auto A = (1.0f + m * m);
			auto B = -2.0f * (Circle_.X + m * (m * Point_.X - Point_.Y + Circle_.Y));
			auto C = (Circle_.Y + m * Point_.X - Point_.Y) * (Circle_.Y + m * Point_.X - Point_.Y) + Circle_.X * Circle_.X - Circle_.Radius * Circle_.Radius;
			auto D = B * B - 4.0f * A * C;

			if (D < 0.0f)
				return false;

			if (D == 0.0f)
			{
				CollisionInfo_.Point.X = -B / (2.0f * A);
			}
			else
			{
				if (Dir_.X > 0.0f)
					CollisionInfo_.Point.X = (-B - sqrt(D)) / (2.0f * A);
				else
					CollisionInfo_.Point.X = (-B + sqrt(D)) / (2.0f * A);
			}
			CollisionInfo_.Point.Y = m * (CollisionInfo_.Point.X - Point_.X) + Point_.Y;
			CollisionInfo_.Time = (CollisionInfo_.Point.X - Point_.X) / Dir_.X;

			if (CollisionInfo_.Time < 0.0f)
				return false;

			// rrr 반사각;
		}
		else
		{
			auto n = Dir_.X / Dir_.Y;
			auto A = (1.0f + n * n);
			auto B = -2.0f * (Circle_.Y + n * (n * Point_.Y - Point_.X + Circle_.X));
			auto C = (Circle_.X + n * Point_.Y - Point_.X) * (Circle_.X + n * Point_.Y - Point_.X) + Circle_.Y * Circle_.Y - Circle_.Radius * Circle_.Radius;
			auto D = B * B - 4.0f * A * C;

			if (D < 0.0f)
				return false;

			if (D == 0.0f)
			{
				CollisionInfo_.Point.Y = -B / (2.0f * A);
			}
			else
			{
				if (Dir_.Y > 0.0f)
					CollisionInfo_.Point.Y = (-B - sqrt(D)) / (2.0f * A);
				else
					CollisionInfo_.Point.Y = (-B + sqrt(D)) / (2.0f * A);
			}
			CollisionInfo_.Point.X = n * (CollisionInfo_.Point.Y - Point_.Y) + Point_.X;
			CollisionInfo_.Time = (CollisionInfo_.Point.Y - Point_.Y) / Dir_.Y;

			if (CollisionInfo_.Time < 0.0f)
				return false;

			// rrr 반사각;
		}

		return true;
	}
	bool IsCollidedCircleCircle(const SCircle& Circle0_, const SCircle& Circle1_, const SPoint& Dir_, SCollisionInfo& CollisionInfo_)
	{
		if (IsOverlappedCircleCircle(Circle0_, Circle1_))
		{
			// rrr
			//충돌점은 중점;
			//반사각;
			CollisionInfo_.Time = 0.0f;
			return true;
		}

		if (!IsCollidedPointCircle(Circle0_, SCircle(Circle1_, Circle0_.Radius + Circle1_.Radius), Dir_, CollisionInfo_))
			return false;

		if (CollisionInfo_.Time < 0.0f)
			return false;

		CollisionInfo_.Point.X = (CollisionInfo_.Point.X - Circle1_.X) * (Circle1_.Radius / (Circle0_.Radius + Circle1_.Radius)) + Circle1_.X;
		CollisionInfo_.Point.Y = (CollisionInfo_.Point.Y - Circle1_.Y) * (Circle1_.Radius / (Circle0_.Radius + Circle1_.Radius)) + Circle1_.Y;
		// rrr
		//충돌점;
		//반사각;
		return true;
	}
	bool IsCollidedPointCircle2(const SPoint& Point_, const SCircle& Circle_, const SPoint& Dir0_, const SPoint& Dir1_, SCollisionInfo& CollisionInfo_)
	{
		if (!IsCollidedPointCircle(Point_, Circle_, Dir0_ - Dir1_, CollisionInfo_))
			return false;

		if (CollisionInfo_.Time < 0.0f)
			return false;

		CollisionInfo_.Point += (Dir1_ * CollisionInfo_.Time);

		// rrr
		//아래는 위에서 계산되었나?
		//충돌점;
		//반사각;
		return true;
	}
	bool IsCollidedCircleCircle2(const SCircle& Circle0_, const SCircle& Circle1_, const SPoint& Dir0_, const SPoint& Dir1_, SCollisionInfo& CollisionInfo_)
	{
		if (!IsCollidedCircleCircle(Circle0_, Circle1_, Dir0_ - Dir1_, CollisionInfo_))
			return false;

		if (CollisionInfo_.Time < 0.0f)
			return false;

		CollisionInfo_.Point += (Dir1_ * CollisionInfo_.Time);

		// rrr
		//아래는 위에서 계산되었나 ?
		//충돌점;
		//반사각;
		return true;
	}
	bool MoveTowards(SPoint& Current_, const SPoint& Target_, float DistanceDelta_) // return true : reached
	{
		auto Vector = Target_ - Current_;
		auto Scalar = GetScalar(Vector);
		if (Scalar > DistanceDelta_)
		{
			Current_ += SPoint(Vector.X * DistanceDelta_ / Scalar, Vector.Y * DistanceDelta_ / Scalar);
			return false;
		}
		else
		{
			Current_ = Target_;
			return true;
		}
	}

	const SPoint UnitPoint = SPoint(1.0f, 1.0f);
	const STransform ZeroTransform = STransform(SPoint(), SPoint3(), UnitPoint);
}
