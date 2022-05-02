#if 0
#include "stdafx.h"
#include <Rso/Physics/Physics.h>
#include <Rso/Physics/Vehicle.h>
#include <iostream>
#include <string>
#include <iostream>

using namespace rso;
using namespace physics;
using namespace std;


void PrintPoint(const SPoint& Point_)
{
	cout << "X : " << Point_.X << " Y : " << Point_.Y << endl;
}


void main( void )
{
	if (IsOverlappedCircleSector(SCircle(SPoint(0.0, -1.0), 0.1), SSector(SCircle(SPoint(0.0, 0.0), 1.0), c_PI_2 + c_PI_2 * 0.5, -c_PI_2 - c_PI_2 * 0.5)))
		cout << "col" << endl;
	else
		cout << "not" << endl;
	return;



	//////////////////////////////////////////////////////////////////////////////////////////////////
	if (IsOverlappedPointSector(SPoint(0.5, 0.0001), SSector(SCircle(SPoint(0.0, 0.0), 1.0), c_PI_2, 0.0)))
		cout << "col" << endl;
	else
		cout << "not" << endl;
	return;



	
	for (double d = 0.0; d < c_2_PI; d += 0.05)
	{
		cout << d << " " << atan2(sin(d), cos(d)) << endl;
	}

	cout << ThetaOfTwoVectors(SPoint(1.0, 0.000001), SPoint(1.0, -0.00001)) << endl;
	return;


	// test vehicle move
	CVehicle v(SVehicle(
		SPoint(10.0, 10.0),
		0.78,	// Theta
		-0.1,	// vTheta
		0.0,	// EndvTheta
		0.0,	// aTheta
		1.0,	// Velocity
		0.0,	// EndVelocity
		0.0		// Accel
	), 0.0);	// time

	SPoint Pos;

	for (double d = 0.0; d < 100.0; d += 1.0)
	{
		Pos = v.GetPos(d); cout << Pos.X << "	" << Pos.Y << endl;
	}

	return;
	////////////////////////////////////////



	SRect r(0.0, 1.0, 0.0, 1.0);
	auto aa = (SRect(r) *= 2.0);

	SRect r1(0.0, 1.0, 0.0, 1.0);
	SRectTheta rr(r1, 1.0);
	auto bb = rr *= 2.0;

	SCollisionInfo Info;
	if (IsCollidedCircleCircle2(SCircle(SPoint(1.0, -2.0), 1.0), SCircle(SPoint(0.0, 0.0), 1.0), SPoint(0.0, 1.2), SPoint(1.0, 1.0), Info))
		cout << "[" << Info.Point.X << "," << Info.Point.Y << "] " << Info.Time << endl;
	else
		cout << "no" << endl;

	return;

	auto PointRect = RectThetaToPointRect(SRectTheta(SRect(0.0, 2.0, 0.0, 1.0), 1.57));


	if (IsOverlappedCircleRectTheta(SCircle(SPoint(-1.95, 0.95), 1.0), SRectTheta(SRect(-2.0, 2.0, -1.0, 1.0), 1.0)))
		cout << "coll" << endl;
	else
		cout << "not coll" << endl;


	if (IsOverlappedPointRectTheta(SPoint(-1.95, 0.95), SRectTheta(SRect(-2.0, 2.0, -1.0, 1.0), c_2_PI + 1.1)))
		cout << "coll" << endl;
	else
		cout << "not coll" << endl;



	SSegment Segment(SPoint(1.0 + sqrt(2.0) / 2.0, 1.0 + sqrt(2.0) / 2.0), SPoint(2.0 + sqrt(2.0), 0.0));
	//SSegment Segment(SPoint(1.0 + sqrt(2.0), 1.0), SPoint(2.0 + sqrt(2.0), 0.0));
	SCircle Circle(SPoint(1.0, 1.0), 1.0);

	if (IsOverlappedSegmentCircle(Segment, Circle))
		cout << "Over" << endl;
	else
		cout << "Not Over" << endl;


	SRect r0(0.0f, 1.0f, 0.0f, 1.0f);
	SPoint p(0.5f, 2.0f);
	SPoint p0(1.0f, 1.0f);
	SVLine l0(0.0f);
	SHLine l1(0.0f);
	SVSegment s0(l0, -1.0f, 1.0f);
	SHSegment s1(l1, -1.0f, 1.0f);
	SCollisionInfo i;

	// Speed Test
	const size_t c_i = 100000000;

	auto s = system_clock::time_point();
	auto e = system_clock::time_point();

	SPoint a;
	SPoint b;

	s = system_clock::now();
	for (size_t i = 0; i < c_i; ++i)
	{
		b = b * 1.2;
	}
	e = system_clock::now();
	cout << duration_cast<milliseconds>(e - s).count() << " " << b.X << endl;

	s = system_clock::now();
	for (size_t i = 0; i < c_i; ++i)
	{
		a = a / 1.2;
	}
	e = system_clock::now();
	cout << duration_cast<milliseconds>(e - s).count() << " " << a.X << endl;


	cout << cout.fixed << cos(c_PI_2);
}
#elif 1

#include "stdafx.h"
#include <iostream>
#include <Rso/Physics/Sight.h>



using namespace std;
using namespace rso;
using namespace physics;

using TSight = CSight<int>;

void InSightCallback(TSight::iterator Seer_, const TSight::iterator& InSightTarget_)
{
	cout << "InSight " << Seer_.Index() << " " << InSightTarget_.Index() << endl;
}
void OutSightCallback(TSight::iterator Seer_, const TSight::iterator& OutSightTarget_)
{
	cout << "OutSight " << Seer_.Index() << " " << OutSightTarget_.Index() << endl;
}

int main()
{
	TSight s(10.0, InSightCallback, OutSightCallback);

	auto it0 = s.insert(true, SPoint(0, 0), 10.0, 0);
	auto it1 = s.insert(true, SPoint(100, 0), 10.0, 0);

	s.change(it0, SPoint(80.0, 0), 10.0);
	s.change(it0, SPoint(89.0, 0), 10.0);
	s.change(it0, SPoint(91.0, 0), 10.0);
	s.change(it0, SPoint(94.0, 0), 10.0);
	s.change(it0, SPoint(80.0, 0), 10.0);

	auto it2 = s.insert(false, SPoint(100, 9.0), 5.0, 0);
	s.erase(it1);

	return 0;
}


#elif 1

#include "stdafx.h"
#include <iostream>
#include <Rso/Physics/Physics.h>

using namespace std;
using namespace rso;
using namespace physics;

void main(void)
{
	SCollisionInfo Info;
	auto r = IsCollidedPointCircle(SPoint(2, 0), SCircle(SPoint(0, 0), 1), SPoint(1, 0), Info);

	r = IsCollidedCircleCircle2(SCircle(SPoint(0, 0), 1), SCircle(SPoint(-4, 0), 1), SPoint(1, 0), SPoint(-1, 0), Info);
	r = IsCollidedCircleRect(SCircle(SPoint(0, 0), 1), SRect(-4, -3, -1, 1), SPoint(1, 0), Info);
}


#endif