#include "stdafx.h"
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Protocol.h"

using namespace rso;
using namespace core;
using namespace json;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CJson jjj(L"98");
	int fff;
	jjj >> fff;



	vector<int> vv;
	CJson jj(L"[1,3,2]");
	jj >> vv;
	for (auto& i : vv)
		cout << i << endl;

	return 0;


	CJson j(CStream(L"a.txt"));

	STest v;
	v.b = true;
	v.i8 = 8;
	v.i32 = 32;
	v.i64 = -64;
	v.ui64 = 10064;
	v.str = "str";
	v.wstr = L"wstr";
	v.tp = system_clock::now();
	v.dt = CDateTime(v.tp);
	v.e = ETest::a;
	v.s.Push("stream data", 11);

	STest su;
	su.wstr = L"susu";

	STest2 v2(v, "str2", su);

	j["proto"] = Value(v2);

	STest2 vv2;
	vv2 << j["proto"];

	cout << vv2.s.to_string() << endl;
	cout << j.toStyledString() << endl;


	// 중첩 컨테이터
	list<int> ll(3);
	list<list<int>> ll2;
	ll2.emplace_back(ll);
	ll2.emplace_back(ll);
	list<list<list<int>>> ll3;
	ll3.emplace_back(ll2);
	ll3.emplace_back(ll2);
	ll3.emplace_back(ll2);
	Value vvv;
	vvv["s"] = ll3;
	cout << vvv.toStyledString() << endl;


	return 0;
}
