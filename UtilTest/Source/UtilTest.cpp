#include "stdafx.h"
#include <iostream>
#include <Rso/Util/Random.h>

using namespace std;
using namespace rso::util;

#define	Test_

#if defined( Test_ )

void main(void)
{
	std::mt19937_64 en;
	cout << SUniformDistribution<std::mt19937_64, float>()(en, 0.0, 1.0) << endl;

	CRandom<std::mt19937_64, float> r(0.0, 1.0);
	cout << r.Get() << endl;
}

#endif

