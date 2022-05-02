#include "Base.h"

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
	SRect GetRect(const SEngineRect& EngineRect_)
	{
		auto Rect = GetRect(EngineRect_.Size);
		Rect += EngineRect_.Offset;
		Rect *= EngineRect_.Scale;
		return Rect;
	}
}