#include "stdafx.h"

#define MULTI_INDEX_TEST 1

#if THREAD_TEST

#include <iostream>
#include <Rso/Base/Thread.h>

using namespace std;
using namespace rso;
using namespace base;

void callback(const volatile bool* Exit_)
{
	while (*Exit_ == false)
	{
		this_thread::sleep_for(milliseconds(1));
	}
}

struct STest
{
	int i = 0;
	unique_ptr<CThread> t{ new CThread(callback) };

	STest(int i_) :
		i(i_)
	{
	}
	virtual ~STest()
	{
	}
};

STest t0{ 1 };
STest t1{ 2 };
STest t2{ 3 };
STest t3{ 4 };

void main(void)
{

	for (int i = 0; i < 1000; ++i)
	{
		this_thread::sleep_for(milliseconds(1));
	}

	cout << "ok";
}



#elif LIST_TEST

#include <iostream>
#include <Rso/Base/List.h>

using namespace std;
using namespace rso;
using namespace base;

struct STest
{
	int jj;

	STest(int i)
	{}
	virtual ~STest()
	{}
};

void main(void)
{
	CList<STest> l;
	l.emplace_at(1, STest(3));
	l.emplace(STest(3));
	l.emplace(STest(3));

	for (auto it = l.begin(); it; ++it)
		cout << it.Index() << endl;
}

#elif LIST_VOID_TEST

#include <iostream>
#include <Rso/Base/ListVoid.h>

using namespace std;
using namespace rso;
using namespace base;

struct STest
{
	STest(int i)
	{}
	virtual ~STest()
	{}
};

void main(void)
{
	CListVoid i;
	auto ii = i.emplace();
	if (ii)
		cout << ii.Index() << endl;

	ii = i.emplace();
	if (ii)
		cout << ii.Index() << endl;

	i.clear();

	ii = i.emplace();
	if (ii)
		cout << ii.Index() << endl;

}

#elif OPTION_TEST

#include <Rso/Base/OptionStream.h>
#include <string>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace base;

void main(void)
{


	COptionStream<int> Option(wstring(L"d:/Temp/Option.ini"));
	Option = 'o';
	Option.Clear();
}


#elif BIDIRECTION_TEST

#include <iostream>
#include <map>
#include <Rso/Base/Bidirection.h>

using namespace std;
using namespace rso;
using namespace base;

void main(void)
{
	CBidirection<map<int, int>> b;
	b.emplace(1, 1);
	b.emplace(4, 2);
	b.emplace(7, 3);
	b.emplace(9, 4);

	for (int i = 0; i < 10; ++i)
	{
		cout << "nearest from " << i << " is :";

		auto it = b.nearest(i);
		for (; it != b.end(); ++it)
			cout << " " << it->first;

		cout << endl;
	}

	return;
}

#elif EVENT_TEST

#include <iostream>
#include <thread>
#include <Rso/Base/Event.h>

using namespace std;
using namespace rso;
using namespace base;

void main(void)
{
	CEvent Event(0);
	int i = 0;

	while (true)
	{
		if (Event.CheckAndNextFixed())
			cout << ++i << endl;

		this_thread::sleep_for(milliseconds(1000));
	}
}

#elif TIMER_TEST

#include <iostream>
#include <thread>
#include <Rso/Base/Timer.h>

using namespace std;
using namespace rso;
using namespace base;

void Callback(void)
{
    cout << "Called" << endl;
}

void main(void)
{
    CTimer Timer(Callback, system_clock::now() + TSpan(0), TSpan());

    this_thread::sleep_for(milliseconds(2000));
}


#elif CYCLE_TEST

#include <iostream>
#include <Rso/Base/CycleSet.h>
#include <Rso/Base/CycleMap.h>
#include <Rso/Base/CycleCList.h>

using namespace std;
using namespace rso;
using namespace base;

void main(void)
{
    CCycleSet<int> cs;
	CCycleMap<int, int> cm;
	CCycleCList<int> cl;

//    srand(time(0));

    for (size_t i = 0; i < 1000; ++i)
    {
//        if (rand() % 2 == 0)
        {
            cs.emplace(i % 100);
			cm.emplace(i % 100, i % 100);
			cl.emplace(i % 100);
        }
        //else
        //{
        //    cm.erase(i % 100);
        //}

        auto sit = cs.cycle();
        if (sit != cs.end())
            cout << *sit << endl;


        auto mit = cm.cycle();
        if (mit != cm.end())
            cout << mit->first << " " << mit->second << endl;

	
		auto lit = cl.cycle();
		if (lit != cl.end())
			cout << *lit << endl;
	}
}

#elif RANGE_KEY_TEST

#include <iostream>
#include <thread>
#include <Rso/Base/Base.h>
#include <set>

using namespace std;
using namespace rso;
using namespace base;

#elif MULTI_INDEX_TEST

#include <iostream>
#include <chrono>
#include <Rso/Base/MultiIndexSet.h>
#include <Rso/Base/MultiIndexMap.h>

using namespace std;
using namespace chrono;
using namespace rso;
using namespace base;

void main(void)
{
	CMultiIndexMap<int, non_unique_index<int>, unique_index<string>> m;

	{
		auto it0 = m.emplace(100, 1, "aa");
		if (it0)
			cout << "inserted : " << it0->second << endl;
		else
			cout << "not inserted" << endl;

		auto it1 = m.emplace(200, 1, "bb");
		if (it1)
			cout << "inserted : " << it1->second << endl;
		else
			cout << "not inserted" << endl;

		auto it2 = m.emplace(200, 2, "bb");
		if (it2)
			cout << "inserted : " << it2->second << endl;
		else
			cout << "not inserted" << endl;


		auto iii = m.find<1>("bb");
		if (iii == m.end<1>())
			return;

		cout << iii->first << endl;
		cout << iii->second << endl;

		auto n = std::move(m);

		cout << n[0].second << endl;
		cout << n[1].second << endl;
		cout << n.front().second << endl;
		cout << n.back().second << endl;
		cout << n.begin()->second << endl;
		cout << n.last()->second << endl;


		n.erase(it1);

		if (n.begin() == n.end())
			cout << "empty" << endl;

		cout << n.empty() << endl;

		cout << "erase begin()" << endl;
		n.erase(n.begin());

		cout << "empty : " << n.empty() << endl;

		auto it = n.emplace(200, 1, "bb");
		if (it)
			cout << "inserted : " << it->second << endl;
		else
			cout << "not inserted" << endl;

		auto& [a, b] = n.get(n.find<0>(1)->second)->first;
		cout << a->first << " " << b->first << endl;


		cout << "clear()" << endl;
		n.clear();
		cout << "empty : " << n.empty() << endl;
	}

	{
		CMultiIndexSet<non_unique_index<int>, unique_index<string>> s;
		auto it0 = s.emplace(1, "aa");
		if (it0)
			cout << "inserted : " << endl;
		else
			cout << "not inserted" << endl;

		auto it1 = s.emplace(1, "bb");
		if (it1)
			cout << "inserted : " << endl;
		else
			cout << "not inserted" << endl;

		auto it2 = s.emplace(2, "bb");
		if (it2)
			cout << "inserted : " << endl;
		else
			cout << "not inserted" << endl;


		auto iii = s.find<1>("bb");
		if (iii == s.end<1>())
			return;

		cout << iii->first << endl;
		cout << iii->second << endl;

		auto n = std::move(s);


		n.erase(it1);

		if (n.begin() == n.end())
			cout << "empty" << endl;

		cout << n.empty() << endl;

		cout << "erase begin()" << endl;
		n.erase(n.begin());

		cout << "empty : " << n.empty() << endl;

		auto it = n.emplace(1, "bb");
		if (it)
			cout << "inserted : " << endl;
		else
			cout << "not inserted" << endl;

		auto& [a, b] = (*n.get(n.find<0>(1)->second));
		cout << a->first << " " << b->first << endl;

		cout << "clear()" << endl;
		n.clear();
		cout << "empty : " << n.empty() << endl;

		s.clear();
	}
}

#elif LFQUEUE_TEST

#include <iostream>
#include <thread>
#include <Rso/Base/LFQueue.h>

using namespace std;
using namespace rso;
using namespace base;

const int64 c_i = 1000000;

int64 g_sum0 = 0;
int64 g_sum1 = 0;

CLFQueue<int64> g_lfqueue;

void Thread(void)
{
	for (int64 i = 0; i < c_i;)
	{
		auto pData = g_lfqueue.get();
		if (!pData)
		{
			cout << ".";
			continue;
		}

		g_sum1 += *pData;
		g_lfqueue.pop();
		++i;
	}
}

void main(void)
{
	system_clock::time_point s, e;

	thread tLFQueue(Thread);
	s = system_clock::now();
	for (int64 i = 0; i < c_i; ++i)
	{
		auto r = rand();
		g_sum0 += r;
		g_lfqueue.emplace(r);
	}

	tLFQueue.join();
	e = system_clock::now();

	cout << duration_cast<milliseconds>(e - s).count() << endl;

	if (g_sum0 == g_sum1)
		cout << "ok" << endl;
}



//#include "stdafx.h"
//
//#include <iostream>
//#include <atltime.h>
//#include <oledb.h>
//#include <Rso/Base/Index.h>
//#include <Rso/Base/Base.h>
//#include <chrono>
//#include <new>
//#include <map>
//#include <thread>
//#include <atomic>
//#include <Rso/Base/Period.h>
//#include <Rso/Base/Timer.h>
//#include <Rso/Base/Enum.h>
//#include <Rso/Base/Crypt.h>
//#include <Rso/Base/Thread.h>
//#include <Rso/Base/Thread2.h>
//#include <Rso/Base/LFQueue.h>
//#include <Rso/Base/Queue.h>
//#include <Rso/Base/MapCount.h>
//#include <Rso/Base/Event.h>
//
//using namespace std;
//using namespace chrono;
//using namespace std;
//using namespace rso;
//using namespace base;
//

//
//#if 0	// Event
//void main(void)
//{
//
//	CRsoEvent	E;
//
//	E.Initialize();
//
//	CRsoEvent::SCond	Cond;
//	Cond.Cond = CRsoEvent::Cond_Second;
//	Cond.Time1.Year = 2012;
//	Cond.Time1.Month = 6;
//	Cond.Time1.Day = 19;
//	Cond.Time1.Hour = 10;
//	Cond.Time1.Minute = 19;
//	Cond.Time1.Second = 25;
//	//	Cond.Time1.Value = 19;
//	CRsoEvent::STime	Time2;
//	Time2.Year = 2012;
//	Time2.Month = 6;
//	Time2.Day = 19;
//	Time2.Hour = 14;
//	Time2.Minute = 14;
//	Time2.Second = 59;
//	//	Time2.Value = 19;
//	Cond.SetTime2(&Time2);
//	E.SetEvent(0, &Cond);
//
//	int i = 0;
//	for (;;)
//	{
//		if (E.GetEvent() != CRsoEvent::c_EventCnt_Null)
//		{
//			printf("%d\n", ++i);
//		}
//		Sleep(1);
//	}
//
//	E.Finalize();
//
//
//
//#elif 0	// Index
//
//	const int c_Rep = 1000000;
//	int Index = 0;
//	DWORD	dwS = 0;
//	DWORD	dwE = 0;
//	int DelIDX = 0;
//
//	CIndex<int>	RsoIndex;
//
//
//	dwS = GetTick();
//	for (int i = 0; i<c_Rep; ++i)
//	{
//		Index = RsoIndex.New();
//	}
//	for (int i = 0; i<c_Rep - 1; ++i)
//	{
//		RsoIndex.Del(i);
//	}
//	RsoIndex.Del(c_Rep - 1);
//	dwE = GetTick();
//
//	printf("%d %d\n", dwE - dwS, Index);
//
//
//	dwS = GetTick();
//	for (int i = 0; i<c_Rep; ++i)
//	{
//		Index = RsoIndex.New();
//	}
//	for (int i = 0; i<c_Rep - 1; ++i)
//	{
//		RsoIndex.Del(i);
//	}
//	RsoIndex.Del(c_Rep - 1);
//	dwE = GetTick();
//
//	printf("%d %d\n", dwE - dwS, Index);
//
//
//	dwS = GetTick();
//	for (int i = 0; i<c_Rep; ++i)
//	{
//		int* p = new int;
//	}
//	dwE = GetTick();
//
//	printf("%d %d\n", dwE - dwS, Index);
//
//
//
//#elif 0	// Crypto
//
//	char e[] = "{\"orderId\":\"12999763169054705758.1315160756665859\",\"packageName\":\"com.actoz.skyko\",\"productId\":\"gg_diamond_10\",\"purchaseTime\":1378894906000,\"purchaseState\":0,\"purchaseToken\":\"nyvikghcqkxhzkgyrohpnxjb.AO-J1OzyV1G_BCA9vQmJCnh4evjLem1_jVdjvkVzeLwsBP9RSNqZ57qsM1KqCwQ2JDSplCHdzTDQ9HurTf5xQU5ud9rX2o9RysMYLD1bT1fHtAbcV7ZRERc\"}";
//
//	CCrypt c(ECryptType::BlowFish);
//	c.SetPasswd( "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgoEbfIU5XEDkNQwptasLTmp1pZH58ndCY3kXJyRQIV4oDGukk2xhNuEy1RBaiKEPGLejYxclM+jB/6kWH6k7Sbm6iAK/ky19thJb3DpzfgjGWQGVuPvXbIpx5h7HxSCjIhBhFI54J8xTvcGNpO/zpkQmuaNboGUpcne9QdjxhPbVPUJu/YlrVOCvwvUKt40hy9cz1t6QwHuHnR0knzLxrX6qoDMHsUnUFWHHnfipNiWgAiRrcmBHE77FAWQghLjuqwora1Vqp1/Tax4VbJGLaqrPv/AC9nQ7yKndXSBonrspEFPbw+jF7MxLW8FGUMP2qZsWbD0ngx8inlqju8ccgQIDAQAB", 4 );
//
//	CCrypt d(ECryptType::BlowFish);
//	d.SetPasswd( "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAgoEbfIU5XEDkNQwptasLTmp1pZH58ndCY3kXJyRQIV4oDGukk2xhNuEy1RBaiKEPGLejYxclM+jB/6kWH6k7Sbm6iAK/ky19thJb3DpzfgjGWQGVuPvXbIpx5h7HxSCjIhBhFI54J8xTvcGNpO/zpkQmuaNboGUpcne9QdjxhPbVPUJu/YlrVOCvwvUKt40hy9cz1t6QwHuHnR0knzLxrX6qoDMHsUnUFWHHnfipNiWgAiRrcmBHE77FAWQghLjuqwora1Vqp1/Tax4VbJGLaqrPv/AC9nQ7yKndXSBonrspEFPbw+jF7MxLW8FGUMP2qZsWbD0ngx8inlqju8ccgQIDAQAB", 4 );
//
//
//	const int c_i = 10000;
//	auto start = system_clock::now();
//	for( int i=0; i<c_i; ++i )
//	{
//		c.Encrypt( e, sizeof(e) );
//		d.Decrypt( e, sizeof(e) );
//	}
//	auto end = system_clock::now();
//	cout << duration_cast<milliseconds>(end-start).count() << endl;
//
//
//#elif 0		// Timer
//
//
//	TTimer	t;
//	int i = 0;
//	int j = 0;
//
//	t.SetCallback([&i](std::size_t Key_, int* Data_, ETimer Timer) -> void
//	{
//		++i;
//		wstring Event;
//		switch (Timer)
//		{
//		case ETimer::Start:
//			Event = L"Start";
//			break;
//
//		case ETimer::End:
//			Event = L"End";
//			break;
//
//		case ETimer::Event:
//			Event = L"Event";
//			break;
//
//		case ETimer::Null:
//			Event = L"Null";
//			break;
//		}
//
//		Printf(_T("Key[%I64d]	Data[%d]	Event[%s]\n"), Key_, *Data_, Event.c_str());
//	});
//
//	//GetEvent 에서 Event_Spot 은  spot Period 에서 구간에 있다면 필요할텐데...어떻게 알수있나 ? (현재 spot 1 형태에서 초기 eventTimeIt 이 들어갈때 Cue_End인가 ? 아니면 EndTimeIt 이 NULL인가 ? )
//	//	그리고 아래 나머지 테스트 할것.
//
//	TTime Now = system_clock::now();
//
//	// spot	1
////		t.New(CKey(), 3, TTimer::STimeInfo(Now, TTimer::milliseconds(), TTime(), TTimer::milliseconds()));
//
//	// spot period
//	int a = 4;
//	t.New(&a, Now, Now+milliseconds(1000), milliseconds(100));
//
//	// range 1
////	 t.New(CKey(), 3, TTimer::STimeInfo( Now, TTimer::milliseconds(1), Now+TTimer::milliseconds(10), TTimer::milliseconds(3)) );
//
//	// range period
//	//	t.New(CKey(), 3, TTimer::STimeInfo( Now, TTimer::milliseconds(1), Now+TTimer::milliseconds(6), TTimer::milliseconds(3)) );
//
//	//t.Move( 0, milliseconds(2000) );
//	//t.Move( 0, milliseconds(-3000) );
//
//	//t.Del( 0 );
//	//t.Del( 1 );
//	//t.Del( 2 );
//	//t.Del( 3 );
//
//	// spot	1
//	//	t.New( 0, 3, STimeInfo( Now-COleDateTimeSpan(0,0,0,5), COleDateTimeSpan(0,0,0,0), base::CTime(), COleDateTimeSpan(0,0,0,0)) );
//	//t.Del( 0 );
//	//t.New( 0, 3, STimeInfo( Now-COleDateTimeSpan(0,0,0,5), COleDateTimeSpan(0,0,0,0), base::CTime(), COleDateTimeSpan(0,0,0,0)) );
//
//	//// spot	period
//	//t.New( 1, 3, STimeInfo( Now+COleDateTimeSpan(0,0,0,1), COleDateTimeSpan(0,0,0,0), Now+COleDateTimeSpan(0,0,0,7), COleDateTimeSpan(0,0,0,2)) );
//
//	//// rgn	1
//	//t.New( 2, 3, STimeInfo( Now, COleDateTimeSpan(0,0,0,1), base::CTime(), COleDateTimeSpan(0,0,0,0)) );
//
//	//// rgn	period
//	//t.New( 3, 3, STimeInfo( Now, COleDateTimeSpan(0,0,0,1), Now+COleDateTimeSpan(0,0,0,5), COleDateTimeSpan(0,0,0,3)) );
//
//	for (;;)
//	{
//		t.Proc();
//
//		//printf( "				EventID[%d]	EventType[%d]\n", 0, t.GetEvent(0) );
//		//printf( "				EventID[%d]	EventType[%d]\n", 1, t.GetEvent(1) );
//		//printf( "				EventID[%d]	EventType[%d]\n", 2, t.GetEvent(2) );
//		//printf( "				EventID[%d]	EventType[%d]\n", 3, t.GetEvent(3) );
//
//		Sleep(100);
//
//		if (i == 1 && j++ == 0)
//		{
//			//			t.Backward( 0, 1000 );
//		}
//	}
//
//#elif 0
//using TType = uint32;
//const TType c_Max = 16;
//CRand64<TType>	r64;
//for (TType i = 0; i < c_Max; ++i)
//{
//	auto r = r64.RandSq(c_Max);
//	cout << r << endl;
//}
//
//#elif 0
//struct STest
//{
//	int aa{ 1 };
//	int bb{ 2 };
//	STest()
//	{
//
//	}
//	STest(int aa_) :
//		aa(aa_)
//	{
//	}
//	STest(int aa_, int bb_) :
//		aa(aa_), bb(bb_)
//	{
//	}
//};

//CMem<STest> m{ false };
//
//auto i = m.emplace();
//auto i2 = m.emplace(1);
//auto i3 = m.emplace(1, 2);
//
//#elif 0
//CRing<int> r(3, 1000);
//
//r.emplace_back(1);
//cout << r[0] << " " << r[1] << " " << r[2] << endl;
////
////r.emplace_back(2);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////
////r.emplace_front(3);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
////r.emplace_front(4);
////cout << r[0] << " " << r[1] << " " << r[2] << endl;
//
//#elif 0
//static int g_i = 0;
//int g_check = 0;
//
//struct STest
//{
//	int64 a{ 0 };
//	STest(int64 a_) : a(a_)
//	{
//		++g_i;
//	}
//	~STest()
//	{
//		--g_i;
//	}
//};
//CQueue<STest> q;
//
//
//int64 g_a = 0;
//queue<int64> qq;
//
//for (srand(0);; srand(0))
//{
//	for (int i = 0; i < 1000; ++i)
//	{
//		cout << i;
//		if (rand() % 2 == 0)
//		{
//			++g_a;
//
//			cout << "	push";
//			q.emplace(g_a);
//			qq.emplace(g_a);
//			++g_check;
//		}
//		else
//		{
//			cout << "	pop";
//			if (qq.size() > 0)
//				qq.pop();
//
//			q.pop();
//			if (g_check > 0)
//				--g_check;
//		}
//
//		cout << "	Size " << g_i << " " << g_check << " " << q.size();
//
//		if (q.size() != g_i ||
//			g_i != g_check)
//		{
//			throw(0);
//		}
//
//		if (g_check > 0)
//		{
//			cout << " front[" << q.front().a << " " << qq.front() << "]" << " back[" << q.back().a << " " << qq.back() << "]";
//
//			if (q.front().a != qq.front())
//				throw(0);
//
//			if (q.back().a != qq.back())
//				throw(0);
//
//			auto front = qq.front();
//
//			for (int i = 1; i < g_check; ++i)
//			{
//				if (q[i].a != (front + i))
//					throw(0);
//			}
//		}
//
//		cout << endl;
//	}
//
//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//}
//
//#elif 1
//
//
//#include <Rso/Base/GeoMap.h>
//#include <iostream>
//#include <string>
//#include <tuple>
//#include <chrono>
//#include <array>
//#include <map>
//#include <set>
//#include <limits>
//#include <chrono>
//
//
//using namespace std;
//using namespace chrono;
//using namespace rso;
//using namespace base;
//
//struct SPoint
//{
//	double x;
//	double y;
//};
//struct SVector
//{
//	double x;
//	double y;
//};
//struct SUser
//{
//	SPoint Point;
//	SVector Vector;
//	double Radius;
//};
//
//using TOut = tuple<double>;
//
//double Dist(const SPoint& Point0_, const SPoint& Point1_)
//{
//	return sqrt((Point0_.x - Point1_.x)*(Point0_.x - Point1_.x) + (Point0_.y - Point1_.y)*(Point0_.y - Point1_.y));
//}
//#include <random>
//template<typename TType>
//class CRandom
//{
//public:
//	template<typename _TType>
//	struct _SDistribution
//	{
//		uniform_int_distribution<_TType> Distribution;
//
//		_SDistribution(_TType Min_, _TType Max_) :
//			Distribution(Min_, Max_)
//		{}
//		_TType	operator()(default_random_engine& RandomEngine_)
//		{
//			return Distribution(RandomEngine_);
//		}
//		static float get(default_random_engine& RandomEngine_, TType Min_, TType Max_)
//		{
//			return uniform_int_distribution<_TType>(Min_, Max_)(RandomEngine_);
//		}
//	};
//	template<>
//	struct _SDistribution<float>
//	{
//		uniform_real_distribution<float> Distribution;
//
//		_SDistribution(float Min_, float Max_) :
//			Distribution(Min_, Max_)
//		{}
//		float	operator()(default_random_engine& RandomEngine_)
//		{
//			return Distribution(RandomEngine_);
//		}
//		static float get(default_random_engine& RandomEngine_, float Min_, float Max_)
//		{
//			return uniform_real_distribution<float>(Min_, Max_)(RandomEngine_);
//		}
//	};
//	template<>
//	struct _SDistribution<double>
//	{
//		uniform_real_distribution<double> Distribution;
//
//		_SDistribution(double Min_, double Max_) :
//			Distribution(Min_, Max_)
//		{}
//		double	operator()(default_random_engine& RandomEngine_)
//		{
//			return Distribution(RandomEngine_);
//		}
//		static double get(default_random_engine& RandomEngine_, double Min_, double Max_)
//		{
//			return uniform_real_distribution<double>(Min_, Max_)(RandomEngine_);
//		}
//	};
//
//	_SDistribution<TType>	_Distribution;
//
//public:
//	CRandom(TType Min_, TType Max_) :
//		_Distribution(Min_, Max_)
//	{
//	}
//	TType	operator()(default_random_engine& RandomEngine_)
//	{
//		return _Distribution(RandomEngine_);
//	}
//	static TType get(default_random_engine& RandomEngine_, TType Min_, TType Max_)
//	{
//		return _SDistribution<TType>::get(RandomEngine_, Min_, Max_);
//	}
//};
//
//void main(void)
//{
//	CQueue<int> q;
//
//	auto i = q.begin();
//	++i;
//
//	return;
//
//	CEvent eee(EDayOfWeek::Tue, 18, 15, 5);
//
//	while (true)
//	{
//		if (eee.CheckAndNextFixed())
//			cout << "Ok" << endl;
//
//		this_thread::sleep_for(milliseconds(100));
//	}
//
//
//
//	using TMap = CMapCount<int32, int32>;
//	using Value = TMap::value_type;
//	TMap mm;
//	mm += Value(1, 2);
//	mm += Value(1, 2);
//	mm += Value(2, 2);
//	mm += Value(2, -2);
//	mm -= Value(1, 4);
//
//	return;
//
//	const size_t c_ArraySz = 5;
//
//	using TV = CGeoMap<double, int, c_ArraySz, true>;
//	using TK = TV::TKeys;
//
//	TV gg;
//	gg.insert(TK{ 0, 0 }, 0);
//	gg.insert(TK{ 2, 2 }, 0);
//
//	for (auto it = gg.lower_bound(TK{ 2, 2 }); it != gg.end(); ++it)
//	{
//		cout << it->first << " " << it->second->first[0] << " " << it->second->first[1] << endl;
//	}
//	return;
//	
//
//
//	const int c_i = 100;
//	auto s = system_clock::time_point();
//	auto e = system_clock::time_point();
//
//	using TM = CGeoMap<double, size_t, true>;
//	using TIn = TM::TKeys;
//
//
//	TM g;
//	TIn In;
//	for (size_t i = 0; i < 1000; ++i)
//	{
//		for (auto& in : In)
//			in = rand();
//
//		g.insert(std::move(In), std::move(i));
//	}
//
//	size_t sum = 0;
//	int vv = 0;
//	for (size_t i = 0; i < 10; ++i)
//	{
//		TIn in;
//		for (auto& it : in)
//			it = double(rand()) / 1000.0;
//		auto r = g.lower_bound(in);
//		for(auto& it : in)
//			cout << it << " ";
//		cout << "=== ";
//		cout << r->first << ":";
//		for (auto& it : r->second->first)
//			cout << it << " ";
//		cout << endl;
//	}
//
//	cout << sum << endl;
//
//
//	return;
//
//#if 0
//	const size_t c_ArraySize = 1;
//
//	using TNOld = CGeoMap<double, SUser*, c_ArraySize>;
//	TNOld nOld;
//
//	vector<TNOld::TKeys> v;
//	TNOld::TKeys in{ 0.0 };
//
//	v.emplace_back(TNOld::TKeys{ 5.0 });
//	v.emplace_back(TNOld::TKeys{ 10.0 });
//	v.emplace_back(TNOld::TKeys{ 0.0 });
//	v.emplace_back(TNOld::TKeys{ 9.0 });
//#elif 0
//	const size_t c_ArraySize = 2;
//
//	using TNOld = CGeoMap<double, SUser*, c_ArraySize>;
//	TNOld nOld;
//
//	vector<TNOld::TKeys> v;
//	TNOld::TKeys in{ 0.0, 0.0 };
//
//	v.emplace_back(TNOld::TKeys{ 5.0, 5.0 });
//	v.emplace_back(TNOld::TKeys{ 8.0, 8.0 });
//	v.emplace_back(TNOld::TKeys{ 9.0, 9.0 });
//	v.emplace_back(TNOld::TKeys{ 10.0, 0.0 });
//	v.emplace_back(TNOld::TKeys{ 0.0, 10.0 });
//#else
//
//
//	const size_t c_ArraySize = 4;
//
//	using TNOld = CGeoMap<double, SUser*, c_ArraySize>;
//	TNOld nOld;
//
//	vector<TNOld::TKeys> v;
////	TNOld::TKeys in{ 0.0, 0.0, 0.0, 0.0 };
//	//v.emplace_back(TNOld::TKeys{ 3.0, 2.0, 1.0, 0.0 });
//	//v.emplace_back(TNOld::TKeys{ 0.0, 0.0, 0.0, 3.0 });
//	//v.emplace_back(TNOld::TKeys{ 3.0, 0.0, 0.0, 0.0 });
//	//v.emplace_back(TNOld::TKeys{ 1.0, 1.0, 1.0, 1.0 });
//	//v.emplace_back(TNOld::TKeys{ 0.0, 0.0, 0.0, -1.0 });
//	//v.emplace_back(TNOld::TKeys{ 0.0, 0.0, 0.0, 1.0 });
//	//v.emplace_back(TNOld::TKeys{ 0.0, 0.0, 0.0, 2.0 });
//	//v.emplace_back(TNOld::TKeys{ 0.0, 0.0, 0.0, -2.0 });
//	//v.emplace_back(TNOld::TKeys{ 0.0, 0.0, 0.5, 0.0 });
//	//v.emplace_back(TNOld::TKeys{ 0.0, 9.0, 0.5, 0.0 });
//
//	CRandom<double> rr(0.0, 100.0);
//	default_random_engine RandomEngine;
//
//	for (int i = 0; i < 1000000; ++i)
//	{
//		v.emplace_back(TNOld::TKeys{ rr.operator()(RandomEngine) , rr.operator()(RandomEngine) , rr.operator()(RandomEngine) , rr.operator()(RandomEngine) });
//	}
//
//#endif
//
//	for (auto& i : v)
//	{
//		nOld.insert(std::move(i), nullptr);
//	}
//	
//	int cnt = 0;
//
//	for (auto it = nOld.lower_bound(TNOld::TKeys{ rr.operator()(RandomEngine) , rr.operator()(RandomEngine) , rr.operator()(RandomEngine) , rr.operator()(RandomEngine) }); it != nOld.end(); ++it)
//	{
//		++cnt;
//		//cout << it->first << " ";
//
//		//for (auto& i : it->second->first)
//		//	cout << i << " ";
//
//		//cout << endl;
//
////		cout << "[" << (*it).first[0] << ", " << (*it).first[1] << ", " << (*it).first[2] << ", " << (*it).first[3] << "]" << endl;
//	}
//
//	cout << endl << cnt << endl;
//	//s = system_clock::now();
//
//	//for (int ii = 0; ii < 10; ++ii)
//	//{
//	//	n.clear();
//
//	//	for (auto& User : Users)
//	//		n.insert(TN::TKeys{ User.Point.x, User.Point.y }, &User);
//
//	//	//모든 유저가 같은 거리에 있다면
//	//	//	서로간의 정보를 유저를 스캔하여 통보 필요 여부를 결정할것.
//
//
//	//	for (auto& User : Users)
//	//	{
//	//		auto it = n.lower_bound(TN::TKeys{ User.Point.x, User.Point.y });
//	//		if ((*it).second == &User)
//	//			continue;
//
//	//		// rrrr first 에 거리, 좌표 모두 포함할것.
//	//		for (; it != n.end(); ++it)
//	//		{
//	//			if (Dist(User.Point, (*it).second->Point) > User.Radius)
//	//				break;
//
//	//			if (User.Radius > (*it).second->Radius)
//	//			{
//	//				User.Vector.x += 1.0;
//	//				User.Vector.y += 1.0;
//	//			}
//	//			else
//	//			{
//	//				(*it).second->Vector.x += 1.0;
//	//				(*it).second->Vector.y += 1.0;
//	//			}
//	//		}
//	//	}
//	//}
//
//	//e = system_clock::now();
//	//cout << duration_cast<milliseconds>(e - s).count() << endl;
//
//
//
//
//
//
//
//
//
//
//	//cout << "lower_bound iterating time" << endl;
//	//double Sum = 0.0;
//	//s = system_clock::now();
//	//for (int i = 0; i < c_i; ++i)
//	//{
//	//	auto it = n.lower_bound(TN::TKeys({ double(rand()), double(rand()), double(rand()) }));
//	//	for (auto& i : (*it).first)
//	//		Sum += i;
//	//}
//	//e = system_clock::now();
//	//cout << duration_cast<milliseconds>(e - s).count() << " " << Sum << endl;
//
//
//	//e = system_clock::now();
//	//cout << duration_cast<milliseconds>(e - s).count() << endl;
//}
//
//#elif 0
//
//#include <Rso/Base/Timers.h>
//#include <iostream>
//#include <future>
//
//using namespace rso;
//using namespace base;
//using namespace std;
//
//using TTT = int;
//
//void Callback(TTT& Data_)
//{
//	cout << Data_ << endl;
//}
//
//void main(void)
//{
//	CTimers<TTT> t(Callback);
//	t.emplace(milliseconds(2000), 99);
//
//	while (true)
//	{
//		t.Proc();
//		this_thread::sleep_for(milliseconds(1));
//	}
//}
//
//
//#elif 0
//
//#include <Rso/Base/Vector.h>
//#include <iostream>
//
//using namespace rso;
//using namespace base;
//using namespace std;
//
//void main(void)
//{
//	CVector<int> v;
//	v.emplace_back(1);
//	v.emplace_back(3);
//	v.emplace_back(2);
//	v.emplace_back(4);
//
//	v.clear();
//	v.emplace_back(2);
//	v.emplace_back(1);
//	v.emplace_back(3);
//	v.emplace_back(4);
//
//	v.pop_back();
//
//	for (size_t i = 0; i < v.size(); ++i)
//		cout << v[i] << endl;
//}
//
//#elif 0
//
//#include <Rso/Base/Queue.h>
//#include <Rso/Base/Ring.h>
//#include <iostream>
//
//using namespace rso;
//using namespace base;
//using namespace std;
//
//struct STest
//{
//	int a[1024];
//};
//
//using TType = int;
//
//void main(void)
//{
//	CQueue<TType> q;
//	CRing<TType> r(10);
//
//
//	const int c_r = 10000000;
//	auto s = system_clock::time_point();
//	auto e = system_clock::time_point();
//
//
//
//	//for (size_t i = 0; i < 10; ++i)
//	//	r.emplace((int)i);
//
//	//r.emplace((int)10);
//	//for (size_t i = 0; i < r.size(); ++i)
//	//	cout << r[i] << endl;
//	//cout << endl;
//
//	//r.emplace((int)11);
//	//r.pop();
//	//for (size_t i = 0; i < r.size(); ++i)
//	//	cout << r[i] << endl;
//	//cout << endl;
//
//	//r.emplace((int)12);
//	//for (size_t i = 0; i < r.size(); ++i)
//	//	cout << r[i] << endl;
//	//cout << endl;
//
//
//
//
//}
//
//
//
//
//
#endif

