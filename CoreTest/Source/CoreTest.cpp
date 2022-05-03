// RsoMailTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Rso/Core/Alarm.h>
#include <Rso/Core/DateTime.h>
#include <Rso/Core/Stream.h>
//#include <Rso/Core/Bounded.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <typeinfo>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;
using namespace rso;
using namespace core;
using namespace chrono;

#define UTF_Test 1

template<typename TType>
void Push(int32 Length_, const TType* pData_)
{
	cout << sizeof(TType) << endl;
}

std::wstring u16_to_wstring(const std::u16string& str)
{
	std::wstring_convert< std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>,
		wchar_t> conv;

	std::wstring wstr = conv.from_bytes(
		reinterpret_cast<const char*> (&str[0]),
		reinterpret_cast<const char*> (&str[0] + str.size())
	);

	return(wstr);
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout.imbue(std::locale("korean"));
	wcout.imbue(std::locale("korean"));

	CStream stm;

	wstring wstr = L"ok";
	stm << wstr;

	cout << stm.size() << endl;

	stm >> wstr;

	cout << stm.size() << endl;

	return 0;


	vector<wstring> lw;
	lw.emplace_back(L"sample 하 韓 ★");
	lw.emplace_back(L"sample 하 韓 ★");
	lw.emplace_back(L"sample 하 韓 ★");
	lw.emplace_back(L"sample 하 韓 ★");
	stm << lw;

	vector<wstring> olw;
	stm >> olw;

	for (auto& i : olw)
		wcout << i << endl;

	return 0;

#if UTF_Test

	{
		auto Converted = u8string_to_u32string(u8"•°♻☺☆«"s);
		cout << Converted.find(U"•") << endl;
		cout << Converted.find(U"°") << endl;
		cout << Converted.find(U"♻") << endl;
		cout << Converted.find(U"☺") << endl;
		cout << Converted.find(U"☆") << endl;
		cout << Converted.find(U"«") << endl;

		auto ReConverted = u32string_to_u8string(Converted);
		cout << ReConverted.find(u8"•") << endl;
		cout << ReConverted.find(u8"°") << endl;
		cout << ReConverted.find(u8"♻") << endl;
		cout << ReConverted.find(u8"☺") << endl;
		cout << ReConverted.find(u8"☆") << endl;
		cout << ReConverted.find(u8"«") << endl;
	}

	{
		auto Converted = u8string_to_u16string(u8"•°♻☺☆«"s);
		cout << Converted.find(u"•") << endl;
		cout << Converted.find(u"°") << endl;
		cout << Converted.find(u"♻") << endl;
		cout << Converted.find(u"☺") << endl;
		cout << Converted.find(u"☆") << endl;
		cout << Converted.find(u"«") << endl;

		auto ReConverted = u16string_to_u8string(Converted);
		cout << ReConverted.find(u8"•") << endl;
		cout << ReConverted.find(u8"°") << endl;
		cout << ReConverted.find(u8"♻") << endl;
		cout << ReConverted.find(u8"☺") << endl;
		cout << ReConverted.find(u8"☆") << endl;
		cout << ReConverted.find(u8"«") << endl;
	}

#elif DateTime_Test
	for (int i = 0; i < 100; ++i)
	{
		auto Now = system_clock::now();

		milliseconds ms2 = duration_cast<milliseconds>(Now.time_since_epoch());
		seconds s2 = duration_cast<seconds>(ms2);
		auto t2 = s2.count();
		auto tm_t = system_clock::to_time_t(Now);

		std::size_t fractional_seconds2 = ms2.count() % 1000;



		CDateTime ts(system_clock::now());
		cout << ts.fraction << "	" << fractional_seconds2 << endl;

		this_thread::sleep_for(milliseconds(10));
	}
	return 0;



	CTime Now(nowtime, EDayOfWeek::Tue, 18, 0, 0);
	wcout << Now.ToString() << endl;

	return 0;

	// CBounded Test////////////////////////////
//	CBounded<int, 1, 10> b;
	//b -= 5.0;
	//b -= 5.0;
	//b -= 5.0;
	//cout << (b - b) << endl;
	//cout << b << endl;


	// Alarm Test
	//CAlarmPeriod<seconds> a(seconds(1));

	//while (true)
	//{
	//	auto RepMonth = a.Proc();
	//	if (RepMonth > 0)
	//		cout << RepMonth << endl;
	//}

	// Limiter Test
	//CLimiter<seconds, uint64> l(seconds(1), 10);
	//while (true)
	//{
	//	if (l.Proc(1))
	//		cout << "Proc" << endl;
	//}


	// Limiters Test
	//CLimiters a;

	//auto Now = system_clock::now();

	//a.AddDuration<seconds>(seconds(1), 1);
	//a.AddPeriod<seconds>(Now, seconds(1), 1);
	//a.AddPeriod<seconds>(Now, seconds(10), 3);

	//while (true)
	//{
	//	this_thread::sleep_for(milliseconds(100));
	//	if (a.Proc(1))
	//		cout << "Proc" << endl;
	//}

#elif 1

	cout.imbue(std::locale("korean"));
	wcout.imbue(std::locale("korean"));

	CStream stream;

	// container test ///////////////////////////////////////
	using TA = array<int, 2>;
	using TV = vector<int>;
	using TL = list<int>;
	using TM = map<int, int>;
	using TMM = multimap<string, int>;
	using TS = set<string>;
	using TMS = multiset<int>;

	TA a;
	TV v;
	TL l;
	TM m;
	TMM mm;
	TS s;
	TMS ms;

	a[0] = 1;
	a[1] = 2;

	v.emplace_back(3);
	v.emplace_back(4);

	l.emplace_back(5);
	l.emplace_back(6);

	m.emplace(7, 8);
	m.emplace(9, 10);

	mm.emplace("a", 11);
	mm.emplace("b", 11);

	s.emplace("c");
	s.emplace("d");

	ms.emplace(100);
	ms.emplace(130);

	stream << a;
	stream << v;
	stream << l;
	stream << m;
	stream << mm;
	stream << s;
	stream << ms;

	TA a2;
	TV v2;
	TL l2;
	TM m2;
	TMM mm2;
	TS s2;
	TMS ms2;

	stream >> a2;
	stream >> v2;
	stream >> l2;
	stream >> m2;
	stream >> mm2;
	stream >> s2;
	stream >> ms2;




	// basic test ///////////////////////////////////////
	string str("abc소☆恨");
	stream << str;

	string str2;
	stream >> str2;
	cout << str2 << endl;

	wstring wstr(L"abc소☆恨");
	stream << wstr;
	wstring wstr2;
	stream >> wstr2;
	wcout << wstr2 << endl;

	stream.SaveFile(L"ok.ini");

#endif

	return 0;
}
