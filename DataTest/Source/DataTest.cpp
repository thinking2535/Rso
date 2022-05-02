#include "stdafx.h"

#if 0
#include <Rso/Data/Data.h>
#include <Rso/Data/CSV.h>
#include <Rso/Data/DynamicData.h>
#include <iostream>
#include <string>
#include "Protocol.h"


using namespace rso;
using namespace base;
using namespace data;
using namespace std;

struct STest : public SProto
{
	string s;
	STest()
	{
	}
	bool operator << (CStream& Stream_) override
	{
		if (!(Stream_ >> s)) return false;
		return true;
	}
	bool operator >> (CStream& Stream_o) const override
	{
		if (!(Stream_o << s)) return false;
		return true;
	}
	static wstring	GetBasicTypeString(void)
	{
		return L"string";
	}
};

struct SData : public IData
{
	STest s;

	void Load(void) override
	{
		if (!CLoaderHttp(L"127.0.0.1", L"/IronLady/Dev/ServerData/Conf.txt", L"", 8000).Get(s))
			throw("Data Load Fail");
	}
};

void main( void )
{
	try
	{
		STest2 s;
		ExtractFromFile(L"a.csv", L"a,b", s, "korean");

		for (auto& d : s.Datas)
			wcout << d.Name << L" " << d.Index << endl;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
}

#elif 1

#include <Rso/Data/DoubleBuffer.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace rso;
using namespace data;
using namespace std;
using namespace chrono;

CDoubleBuffer<int> db;

void th(void)
{
    this_thread::sleep_for(seconds(2));

    for (int i = 0; i < 10000000; ++i)
    {
        db.Set(std::move(i));
//        this_thread::sleep_for(milliseconds(1));
    }
}

void main(void)
{
    thread t(th);

    int sum = 0;

    auto s = system_clock::now();
    for (int i = 0; i < 10000000; ++i)
        sum += *db.Get();
    auto e = system_clock::now();
    cout << duration_cast<milliseconds>(e - s).count() << endl;

    t.join();
}

#endif