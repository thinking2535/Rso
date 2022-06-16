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

	extern const SPoint UnitPoint;
	extern const STransform ZeroTransform;
	inline STransform GetDefaultTransform(const SPoint& LocalPosition_)
	{
		return STransform(LocalPosition_, SPoint3(), UnitPoint);
	}
}