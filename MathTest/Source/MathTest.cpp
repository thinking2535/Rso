#include "stdafx.h"
#include <iostream>
using namespace std;

#if 0 // fresnel
#include <Rso/Math/Deviation.h>
#include <Rso/Math/fresnel/specialfunctions.h>

using namespace rso;
using namespace rso::math;
using namespace alglib;

int _tmain(int argc, _TCHAR* argv[])
{
	//CDeviation<double> d;

	//d << -1.0;
	//d << 1.0;

	//cout << d.Get() << endl;




	for (double d = 0.0; d < 10.0; d += 0.1)
	{
		cout << errorfunction(d) << "   " << erf(d) << endl;
	}




	//double c;
	//double s;
	//
	//for (double x = 0.0; x < 10.0; x += 0.1)
	//{
	//	fresnelintegral(x, c, s);
	//	cout << x << "	" << c << "	" << s << endl;
	//}

	return 0;
}
#elif 0 // CTracer
#include <Rso/Math/FixedAvg.h>
#include <Rso/Math/Tracer.h>

using namespace std;
using namespace rso;
using namespace rso::math;

int _tmain(int argc, _TCHAR* argv[])
{
	CTracer<double> t(10, 0, 0);

	++t; cout << double(t) << endl;
	++t; cout << double(t) << endl;
	++t; cout << double(t) << endl;
	--t; cout << double(t) << endl;
	--t; cout << double(t) << endl;

	

	return 0;
}

#elif 1
#include <chrono>
#include <Rso/Math/Base.h>
using namespace chrono;
using namespace rso;
using namespace math;

int gcd(int m, int n)
{
	while (m != n)
	{
		if (m>n)
			m = m - n;
		else
			n = n - m;
	}
	return m;
}

int lcm(int m, int n)
{
	return m*n / gcd(m, n);
}

int gcd2(int x, int y)
{
	int r = 0, a, b;
	a = (x > y) ? x : y; // a is greater number
	b = (x < y) ? x : y; // b is smaller number

	r = b;
	while (a % b != 0)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return r;
}
int lcm2(int x, int y)
{
	int a;
	a = (x > y) ? x : y; // a is greater number
	while (true)
	{
		if (a % x == 0 && a % y == 0)
			return a;
		++a;
	}
}

void main(void)
{
	// 0x17 gcd, lcm »ç¿ë

	//auto s = system_clock::time_point();
	//auto e = system_clock::time_point();
	//size_t c = 1000000;

	//int sm = 0;

	//s = system_clock::now();
	//for (size_t i = 0; i < c; ++i)
	//{
	//	auto r0 = rand() + 1;
	//	auto r1 = rand() + 1;
	//	sm += GetGCD(r0, r1);
	//	//sm += gcd(r0, r1);
	//	//sm += gcd2(r0, r1);

	//	sm += GetLCM(r0, r1);
	//	//sm += lcm(r0, r1);
	//	//sm += lcm2(r0, r1);
	//}
	//e = system_clock::now();
	//cout << duration_cast<milliseconds>(e - s).count() << " " << sm << endl;

}

#endif