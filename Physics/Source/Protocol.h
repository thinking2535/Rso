#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

namespace rso
{
	namespace physics
	{
		using namespace core;
		using namespace json;
		struct SPoint : public SProto
		{
			float X{};
			float Y{};
			SPoint()
			{
			}
			SPoint(const float& X_, const float& Y_) : X(X_), Y(Y_)
			{
			}
			SPoint(float&& X_, float&& Y_) : X(std::move(X_)), Y(std::move(Y_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> X;
				Stream_ >> Y;
			}
			void operator << (const Value& Value_) override
			{
				Value_["X"] >> X;
				Value_["Y"] >> Y;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << X;
				Stream_ << Y;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["X"] = X;
				Value_["Y"] = Y;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(float()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(float(), L"X") + L"," + 
					GetMemberName(float(), L"Y");
			}
		};
		struct SPoint3 : public SProto
		{
			float X{};
			float Y{};
			float Z{};
			SPoint3()
			{
			}
			SPoint3(const float& X_, const float& Y_, const float& Z_) : X(X_), Y(Y_), Z(Z_)
			{
			}
			SPoint3(float&& X_, float&& Y_, float&& Z_) : X(std::move(X_)), Y(std::move(Y_)), Z(std::move(Z_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> X;
				Stream_ >> Y;
				Stream_ >> Z;
			}
			void operator << (const Value& Value_) override
			{
				Value_["X"] >> X;
				Value_["Y"] >> Y;
				Value_["Z"] >> Z;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << X;
				Stream_ << Y;
				Stream_ << Z;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["X"] = X;
				Value_["Y"] = Y;
				Value_["Z"] = Z;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(float()) + L"," + 
					GetStdName(float()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(float(), L"X") + L"," + 
					GetMemberName(float(), L"Y") + L"," + 
					GetMemberName(float(), L"Z");
			}
		};
		struct STransform : public SProto
		{
			SPoint LocalPosition{};
			SPoint3 LocalRotation{};
			SPoint LocalScale{};
			STransform()
			{
			}
			STransform(const SPoint& LocalPosition_, const SPoint3& LocalRotation_, const SPoint& LocalScale_) : LocalPosition(LocalPosition_), LocalRotation(LocalRotation_), LocalScale(LocalScale_)
			{
			}
			STransform(SPoint&& LocalPosition_, SPoint3&& LocalRotation_, SPoint&& LocalScale_) : LocalPosition(std::move(LocalPosition_)), LocalRotation(std::move(LocalRotation_)), LocalScale(std::move(LocalScale_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> LocalPosition;
				Stream_ >> LocalRotation;
				Stream_ >> LocalScale;
			}
			void operator << (const Value& Value_) override
			{
				Value_["LocalPosition"] >> LocalPosition;
				Value_["LocalRotation"] >> LocalRotation;
				Value_["LocalScale"] >> LocalScale;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << LocalPosition;
				Stream_ << LocalRotation;
				Stream_ << LocalScale;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["LocalPosition"] = LocalPosition;
				Value_["LocalRotation"] = LocalRotation;
				Value_["LocalScale"] = LocalScale;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint3()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"LocalPosition") + L"," + 
					GetMemberName(SPoint3(), L"LocalRotation") + L"," + 
					GetMemberName(SPoint(), L"LocalScale");
			}
		};
		struct SVector : public SProto
		{
			SPoint Pos{};
			SPoint Dir{};
			SVector()
			{
			}
			SVector(const SPoint& Pos_, const SPoint& Dir_) : Pos(Pos_), Dir(Dir_)
			{
			}
			SVector(SPoint&& Pos_, SPoint&& Dir_) : Pos(std::move(Pos_)), Dir(std::move(Dir_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Pos;
				Stream_ >> Dir;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Pos"] >> Pos;
				Value_["Dir"] >> Dir;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Pos;
				Stream_ << Dir;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Pos"] = Pos;
				Value_["Dir"] = Dir;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"Pos") + L"," + 
					GetMemberName(SPoint(), L"Dir");
			}
		};
		struct SCollisionInfo : public SProto
		{
			float Time{};
			SPoint Point{};
			SPoint Normal{};
			SCollisionInfo()
			{
			}
			SCollisionInfo(const float& Time_, const SPoint& Point_, const SPoint& Normal_) : Time(Time_), Point(Point_), Normal(Normal_)
			{
			}
			SCollisionInfo(float&& Time_, SPoint&& Point_, SPoint&& Normal_) : Time(std::move(Time_)), Point(std::move(Point_)), Normal(std::move(Normal_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Time;
				Stream_ >> Point;
				Stream_ >> Normal;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Time"] >> Time;
				Value_["Point"] >> Point;
				Value_["Normal"] >> Normal;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Time;
				Stream_ << Point;
				Stream_ << Normal;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Time"] = Time;
				Value_["Point"] = Point;
				Value_["Normal"] = Normal;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(float()) + L"," + 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(float(), L"Time") + L"," + 
					GetMemberName(SPoint(), L"Point") + L"," + 
					GetMemberName(SPoint(), L"Normal");
			}
		};
		struct SVLine : public SProto
		{
			float X{};
			SVLine()
			{
			}
			SVLine(const float& X_) : X(X_)
			{
			}
			SVLine(float&& X_) : X(std::move(X_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> X;
			}
			void operator << (const Value& Value_) override
			{
				Value_["X"] >> X;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << X;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["X"] = X;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(float(), L"X");
			}
		};
		struct SHLine : public SProto
		{
			float Y{};
			SHLine()
			{
			}
			SHLine(const float& Y_) : Y(Y_)
			{
			}
			SHLine(float&& Y_) : Y(std::move(Y_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Y;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Y"] >> Y;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Y;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Y"] = Y;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(float(), L"Y");
			}
		};
		struct SLine : public SProto
		{
			SPoint Point0{};
			SPoint Point1{};
			SLine()
			{
			}
			SLine(const SPoint& Point0_, const SPoint& Point1_) : Point0(Point0_), Point1(Point1_)
			{
			}
			SLine(SPoint&& Point0_, SPoint&& Point1_) : Point0(std::move(Point0_)), Point1(std::move(Point1_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Point0;
				Stream_ >> Point1;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Point0"] >> Point0;
				Value_["Point1"] >> Point1;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Point0;
				Stream_ << Point1;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Point0"] = Point0;
				Value_["Point1"] = Point1;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"Point0") + L"," + 
					GetMemberName(SPoint(), L"Point1");
			}
		};
		struct SVSegment : public SVLine
		{
			float Bottom{};
			float Top{};
			SVSegment()
			{
			}
			SVSegment(const SVLine& Super_, const float& Bottom_, const float& Top_) : SVLine(Super_), Bottom(Bottom_), Top(Top_)
			{
			}
			SVSegment(SVLine&& Super_, float&& Bottom_, float&& Top_) : SVLine(std::move(Super_)), Bottom(std::move(Bottom_)), Top(std::move(Top_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SVLine::operator << (Stream_);
				Stream_ >> Bottom;
				Stream_ >> Top;
			}
			void operator << (const Value& Value_) override
			{
				SVLine::operator << (Value_);
				Value_["Bottom"] >> Bottom;
				Value_["Top"] >> Top;
			}
			void operator >> (CStream& Stream_) const override
			{
				SVLine::operator >> (Stream_);
				Stream_ << Bottom;
				Stream_ << Top;
			}
			void operator >> (Value& Value_) const override
			{
				SVLine::operator >> (Value_);
				Value_["Bottom"] = Bottom;
				Value_["Top"] = Top;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SVLine()) + L"," + 
					GetStdName(float()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SVLine(), L"") + L"," + 
					GetMemberName(float(), L"Bottom") + L"," + 
					GetMemberName(float(), L"Top");
			}
		};
		struct SHSegment : public SHLine
		{
			float Left{};
			float Right{};
			SHSegment()
			{
			}
			SHSegment(const SHLine& Super_, const float& Left_, const float& Right_) : SHLine(Super_), Left(Left_), Right(Right_)
			{
			}
			SHSegment(SHLine&& Super_, float&& Left_, float&& Right_) : SHLine(std::move(Super_)), Left(std::move(Left_)), Right(std::move(Right_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SHLine::operator << (Stream_);
				Stream_ >> Left;
				Stream_ >> Right;
			}
			void operator << (const Value& Value_) override
			{
				SHLine::operator << (Value_);
				Value_["Left"] >> Left;
				Value_["Right"] >> Right;
			}
			void operator >> (CStream& Stream_) const override
			{
				SHLine::operator >> (Stream_);
				Stream_ << Left;
				Stream_ << Right;
			}
			void operator >> (Value& Value_) const override
			{
				SHLine::operator >> (Value_);
				Value_["Left"] = Left;
				Value_["Right"] = Right;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SHLine()) + L"," + 
					GetStdName(float()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SHLine(), L"") + L"," + 
					GetMemberName(float(), L"Left") + L"," + 
					GetMemberName(float(), L"Right");
			}
		};
		struct SSegment : public SProto
		{
			SPoint Point0{};
			SPoint Point1{};
			SSegment()
			{
			}
			SSegment(const SPoint& Point0_, const SPoint& Point1_) : Point0(Point0_), Point1(Point1_)
			{
			}
			SSegment(SPoint&& Point0_, SPoint&& Point1_) : Point0(std::move(Point0_)), Point1(std::move(Point1_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Point0;
				Stream_ >> Point1;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Point0"] >> Point0;
				Value_["Point1"] >> Point1;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Point0;
				Stream_ << Point1;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Point0"] = Point0;
				Value_["Point1"] = Point1;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"Point0") + L"," + 
					GetMemberName(SPoint(), L"Point1");
			}
		};
		struct SCircle : public SPoint
		{
			float Radius{};
			SCircle()
			{
			}
			SCircle(const SPoint& Super_, const float& Radius_) : SPoint(Super_), Radius(Radius_)
			{
			}
			SCircle(SPoint&& Super_, float&& Radius_) : SPoint(std::move(Super_)), Radius(std::move(Radius_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SPoint::operator << (Stream_);
				Stream_ >> Radius;
			}
			void operator << (const Value& Value_) override
			{
				SPoint::operator << (Value_);
				Value_["Radius"] >> Radius;
			}
			void operator >> (CStream& Stream_) const override
			{
				SPoint::operator >> (Stream_);
				Stream_ << Radius;
			}
			void operator >> (Value& Value_) const override
			{
				SPoint::operator >> (Value_);
				Value_["Radius"] = Radius;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"") + L"," + 
					GetMemberName(float(), L"Radius");
			}
		};
		struct SRect : public SProto
		{
			float Left{};
			float Right{};
			float Bottom{};
			float Top{};
			SRect()
			{
			}
			SRect(const float& Left_, const float& Right_, const float& Bottom_, const float& Top_) : Left(Left_), Right(Right_), Bottom(Bottom_), Top(Top_)
			{
			}
			SRect(float&& Left_, float&& Right_, float&& Bottom_, float&& Top_) : Left(std::move(Left_)), Right(std::move(Right_)), Bottom(std::move(Bottom_)), Top(std::move(Top_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Left;
				Stream_ >> Right;
				Stream_ >> Bottom;
				Stream_ >> Top;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Left"] >> Left;
				Value_["Right"] >> Right;
				Value_["Bottom"] >> Bottom;
				Value_["Top"] >> Top;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Left;
				Stream_ << Right;
				Stream_ << Bottom;
				Stream_ << Top;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Left"] = Left;
				Value_["Right"] = Right;
				Value_["Bottom"] = Bottom;
				Value_["Top"] = Top;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(float()) + L"," + 
					GetStdName(float()) + L"," + 
					GetStdName(float()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(float(), L"Left") + L"," + 
					GetMemberName(float(), L"Right") + L"," + 
					GetMemberName(float(), L"Bottom") + L"," + 
					GetMemberName(float(), L"Top");
			}
		};
		struct SRectTheta : public SRect
		{
			float Theta{};
			SRectTheta()
			{
			}
			SRectTheta(const SRect& Super_, const float& Theta_) : SRect(Super_), Theta(Theta_)
			{
			}
			SRectTheta(SRect&& Super_, float&& Theta_) : SRect(std::move(Super_)), Theta(std::move(Theta_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SRect::operator << (Stream_);
				Stream_ >> Theta;
			}
			void operator << (const Value& Value_) override
			{
				SRect::operator << (Value_);
				Value_["Theta"] >> Theta;
			}
			void operator >> (CStream& Stream_) const override
			{
				SRect::operator >> (Stream_);
				Stream_ << Theta;
			}
			void operator >> (Value& Value_) const override
			{
				SRect::operator >> (Value_);
				Value_["Theta"] = Theta;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SRect()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SRect(), L"") + L"," + 
					GetMemberName(float(), L"Theta");
			}
		};
		struct SSector : public SCircle
		{
			float LeftTheta{};
			float RightTheta{};
			SSector()
			{
			}
			SSector(const SCircle& Super_, const float& LeftTheta_, const float& RightTheta_) : SCircle(Super_), LeftTheta(LeftTheta_), RightTheta(RightTheta_)
			{
			}
			SSector(SCircle&& Super_, float&& LeftTheta_, float&& RightTheta_) : SCircle(std::move(Super_)), LeftTheta(std::move(LeftTheta_)), RightTheta(std::move(RightTheta_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SCircle::operator << (Stream_);
				Stream_ >> LeftTheta;
				Stream_ >> RightTheta;
			}
			void operator << (const Value& Value_) override
			{
				SCircle::operator << (Value_);
				Value_["LeftTheta"] >> LeftTheta;
				Value_["RightTheta"] >> RightTheta;
			}
			void operator >> (CStream& Stream_) const override
			{
				SCircle::operator >> (Stream_);
				Stream_ << LeftTheta;
				Stream_ << RightTheta;
			}
			void operator >> (Value& Value_) const override
			{
				SCircle::operator >> (Value_);
				Value_["LeftTheta"] = LeftTheta;
				Value_["RightTheta"] = RightTheta;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SCircle()) + L"," + 
					GetStdName(float()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SCircle(), L"") + L"," + 
					GetMemberName(float(), L"LeftTheta") + L"," + 
					GetMemberName(float(), L"RightTheta");
			}
		};
		struct SPointRect : public SProto
		{
			SPoint TopRight{};
			SPoint TopLeft{};
			SPoint BottomLeft{};
			SPoint BottomRight{};
			SPointRect()
			{
			}
			SPointRect(const SPoint& TopRight_, const SPoint& TopLeft_, const SPoint& BottomLeft_, const SPoint& BottomRight_) : TopRight(TopRight_), TopLeft(TopLeft_), BottomLeft(BottomLeft_), BottomRight(BottomRight_)
			{
			}
			SPointRect(SPoint&& TopRight_, SPoint&& TopLeft_, SPoint&& BottomLeft_, SPoint&& BottomRight_) : TopRight(std::move(TopRight_)), TopLeft(std::move(TopLeft_)), BottomLeft(std::move(BottomLeft_)), BottomRight(std::move(BottomRight_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> TopRight;
				Stream_ >> TopLeft;
				Stream_ >> BottomLeft;
				Stream_ >> BottomRight;
			}
			void operator << (const Value& Value_) override
			{
				Value_["TopRight"] >> TopRight;
				Value_["TopLeft"] >> TopLeft;
				Value_["BottomLeft"] >> BottomLeft;
				Value_["BottomRight"] >> BottomRight;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << TopRight;
				Stream_ << TopLeft;
				Stream_ << BottomLeft;
				Stream_ << BottomRight;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["TopRight"] = TopRight;
				Value_["TopLeft"] = TopLeft;
				Value_["BottomLeft"] = BottomLeft;
				Value_["BottomRight"] = BottomRight;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"TopRight") + L"," + 
					GetMemberName(SPoint(), L"TopLeft") + L"," + 
					GetMemberName(SPoint(), L"BottomLeft") + L"," + 
					GetMemberName(SPoint(), L"BottomRight");
			}
		};
		struct SPosTheta : public SProto
		{
			SPoint Pos{};
			float Theta{};
			SPosTheta()
			{
			}
			SPosTheta(const SPoint& Pos_, const float& Theta_) : Pos(Pos_), Theta(Theta_)
			{
			}
			SPosTheta(SPoint&& Pos_, float&& Theta_) : Pos(std::move(Pos_)), Theta(std::move(Theta_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Pos;
				Stream_ >> Theta;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Pos"] >> Pos;
				Value_["Theta"] >> Theta;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Pos;
				Stream_ << Theta;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Pos"] = Pos;
				Value_["Theta"] = Theta;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"Pos") + L"," + 
					GetMemberName(float(), L"Theta");
			}
		};
		struct SObjectStraight : public SProto
		{
			float Time{};
			SPosTheta PosTheta{};
			float Speed{};
			float Dist{};
			SObjectStraight()
			{
			}
			SObjectStraight(const float& Time_, const SPosTheta& PosTheta_, const float& Speed_, const float& Dist_) : Time(Time_), PosTheta(PosTheta_), Speed(Speed_), Dist(Dist_)
			{
			}
			SObjectStraight(float&& Time_, SPosTheta&& PosTheta_, float&& Speed_, float&& Dist_) : Time(std::move(Time_)), PosTheta(std::move(PosTheta_)), Speed(std::move(Speed_)), Dist(std::move(Dist_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Time;
				Stream_ >> PosTheta;
				Stream_ >> Speed;
				Stream_ >> Dist;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Time"] >> Time;
				Value_["PosTheta"] >> PosTheta;
				Value_["Speed"] >> Speed;
				Value_["Dist"] >> Dist;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Time;
				Stream_ << PosTheta;
				Stream_ << Speed;
				Stream_ << Dist;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Time"] = Time;
				Value_["PosTheta"] = PosTheta;
				Value_["Speed"] = Speed;
				Value_["Dist"] = Dist;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(float()) + L"," + 
					GetStdName(SPosTheta()) + L"," + 
					GetStdName(float()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(float(), L"Time") + L"," + 
					GetMemberName(SPosTheta(), L"PosTheta") + L"," + 
					GetMemberName(float(), L"Speed") + L"," + 
					GetMemberName(float(), L"Dist");
			}
		};
		struct SRectCollider2D : public SProto
		{
			SPoint Size{};
			SPoint Offset{};
			SRectCollider2D()
			{
			}
			SRectCollider2D(const SPoint& Size_, const SPoint& Offset_) : Size(Size_), Offset(Offset_)
			{
			}
			SRectCollider2D(SPoint&& Size_, SPoint&& Offset_) : Size(std::move(Size_)), Offset(std::move(Offset_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Size;
				Stream_ >> Offset;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Size"] >> Size;
				Value_["Offset"] >> Offset;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Size;
				Stream_ << Offset;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Size"] = Size;
				Value_["Offset"] = Offset;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SPoint()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SPoint(), L"Size") + L"," + 
					GetMemberName(SPoint(), L"Offset");
			}
		};
		struct SStructMove : public SProto
		{
			bool IsMoving{};
			int32 Direction{};
			float StoppedDuration{};
			SStructMove()
			{
			}
			SStructMove(const bool& IsMoving_, const int32& Direction_, const float& StoppedDuration_) : IsMoving(IsMoving_), Direction(Direction_), StoppedDuration(StoppedDuration_)
			{
			}
			SStructMove(bool&& IsMoving_, int32&& Direction_, float&& StoppedDuration_) : IsMoving(std::move(IsMoving_)), Direction(std::move(Direction_)), StoppedDuration(std::move(StoppedDuration_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> IsMoving;
				Stream_ >> Direction;
				Stream_ >> StoppedDuration;
			}
			void operator << (const Value& Value_) override
			{
				Value_["IsMoving"] >> IsMoving;
				Value_["Direction"] >> Direction;
				Value_["StoppedDuration"] >> StoppedDuration;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << IsMoving;
				Stream_ << Direction;
				Stream_ << StoppedDuration;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["IsMoving"] = IsMoving;
				Value_["Direction"] = Direction;
				Value_["StoppedDuration"] = StoppedDuration;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(bool()) + L"," + 
					GetStdName(int32()) + L"," + 
					GetStdName(float());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(bool(), L"IsMoving") + L"," + 
					GetMemberName(int32(), L"Direction") + L"," + 
					GetMemberName(float(), L"StoppedDuration");
			}
		};
		struct SStructMovePosition : public SStructMove
		{
			SPoint LocalPosition{};
			SStructMovePosition()
			{
			}
			SStructMovePosition(const SStructMove& Super_, const SPoint& LocalPosition_) : SStructMove(Super_), LocalPosition(LocalPosition_)
			{
			}
			SStructMovePosition(SStructMove&& Super_, SPoint&& LocalPosition_) : SStructMove(std::move(Super_)), LocalPosition(std::move(LocalPosition_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				SStructMove::operator << (Stream_);
				Stream_ >> LocalPosition;
			}
			void operator << (const Value& Value_) override
			{
				SStructMove::operator << (Value_);
				Value_["LocalPosition"] >> LocalPosition;
			}
			void operator >> (CStream& Stream_) const override
			{
				SStructMove::operator >> (Stream_);
				Stream_ << LocalPosition;
			}
			void operator >> (Value& Value_) const override
			{
				SStructMove::operator >> (Value_);
				Value_["LocalPosition"] = LocalPosition;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(SStructMove()) + L"," + 
					GetStdName(SPoint());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(SStructMove(), L"") + L"," + 
					GetMemberName(SPoint(), L"LocalPosition");
			}
		};
		struct SBoxCollider2D : public STransform
		{
			SRectCollider2D RectCollider2D{};
			SBoxCollider2D()
			{
			}
			SBoxCollider2D(const STransform& Super_, const SRectCollider2D& RectCollider2D_) : STransform(Super_), RectCollider2D(RectCollider2D_)
			{
			}
			SBoxCollider2D(STransform&& Super_, SRectCollider2D&& RectCollider2D_) : STransform(std::move(Super_)), RectCollider2D(std::move(RectCollider2D_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				STransform::operator << (Stream_);
				Stream_ >> RectCollider2D;
			}
			void operator << (const Value& Value_) override
			{
				STransform::operator << (Value_);
				Value_["RectCollider2D"] >> RectCollider2D;
			}
			void operator >> (CStream& Stream_) const override
			{
				STransform::operator >> (Stream_);
				Stream_ << RectCollider2D;
			}
			void operator >> (Value& Value_) const override
			{
				STransform::operator >> (Value_);
				Value_["RectCollider2D"] = RectCollider2D;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(STransform()) + L"," + 
					GetStdName(SRectCollider2D());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(STransform(), L"") + L"," + 
					GetMemberName(SRectCollider2D(), L"RectCollider2D");
			}
		};
	}
}
