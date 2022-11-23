#include "stdafx.h"
#include <iostream>
#include <thread>
#include <string>
#include <Rso/GameUtil/Match.h>
#include <Rso/GameUtil/PeriodicUpdater.h>

using namespace std;
using namespace rso;
using namespace gameutil;

using Period = milliseconds;
using FatigueUpdater = PeriodicUpdater<Period>;

int32 main()
{
	if (true)
	{
		FatigueUpdater periodicUpdater(Period(500), system_clock::now());

		for (int32 i = 0; i < 10000; ++i)
		{
			if (periodicUpdater.update(system_clock::now()))
				cout << "updated" << endl;

			this_thread::sleep_for(milliseconds(1));
		}
	}

	return 0;
}




#if defined( Test_Match )

using TMatch = CMatch<int>;

void Matched(const TMatch::TMatchedUsers& Users_)
{
	cout << "matched ------------------------------- " << Users_.size() << endl;

	for (auto& i : Users_)
		cout << "Key : " << i << endl;

	cout << endl;
}

void main(void)
{
	TMatch m(
		7,
		[](t_duration e, double m, double o) {
			return true; },
		Matched);

	if (!m.insert(0, 10))
		THROWEX();

	if (!m.insert(1, 11))
		THROWEX();

	if (!m.insert(2, 12))
		THROWEX();

	if (!m.insert(3, 13))
		THROWEX();

	if (!m.insert(4, 14))
		THROWEX();

	if (!m.insert(5, 15))
		THROWEX();

	if (!m.insert(6, 20))
		THROWEX();

	while (true)
	{
		m.Proc();
		this_thread::sleep_for(seconds(1));
	}
}
#elif defined( Test_SkillTimer )

#include <iostream>
#include <thread>
#include <Rso/Core/DateTime.h>
#include <Rso/GameUtil/SkillTimer.h>
//
//using namespace std;
//using namespace rso;
//using namespace gameskill;
//
//using TSkillTimer = CSkillTimer<int, size_t>;
//using TEffect = TSkillTimer::SEffect;
//using TSkill = TSkillTimer::SSkill;
//
//void Callback(const TTime& Time_, size_t PlayerIndex_, const TEffect& Effect_, bool End_)
//{
//	cout << "Time[" << CDateTime(Time_).ToString() << "] PlayerIndex[" << PlayerIndex_ << "] Target[" << Effect_.Target << "] End[" << End_ << "]" << endl;
//}

int main(void)
{
	//TSkillTimer st(Callback);
	//st.AddPlayer();
	//st.AddPlayer();

	//auto Now = system_clock::now();
	//st.Cast(Now + seconds(0), 0, TSkill(TEffect(1, 100), Now + seconds(3), milliseconds(500)));
	//st.Buff(Now + seconds(0), 0, TSkill(TEffect(1, 1000), Now + seconds(3), milliseconds(500)));

	//while (true)
	//{
	//	st.Proc();
	//	this_thread::sleep_for(milliseconds(10));
	//}

	return 0;
}

#elif defined( Test_PunishLimiter )

#include <thread>
#include <chrono>>
#include <Rso/GameUtil/PunishLimiter.h>

using namespace chrono;
using namespace rso;
using namespace gameutil;

CPunishLimiter<seconds, int32> p;

void Set(int32 Count_)
{
	if (!p.Set(Count_))
		cout << "Limit!!" << endl;
	else
		cout << "Ok" << endl;
}

void main(void)
{
	p.Add(seconds(2), 2, seconds(4));
	p.Add(seconds(1), 1, seconds(2));

	Set(1);
	this_thread::sleep_for(seconds(1));
	Set(2);
	this_thread::sleep_for(seconds(1));
	Set(2);
	this_thread::sleep_for(seconds(1));
	Set(2);
	this_thread::sleep_for(seconds(1));
	Set(2);
	this_thread::sleep_for(seconds(1));
	Set(2);
	this_thread::sleep_for(seconds(1));
	Set(2);
	this_thread::sleep_for(seconds(1));
	Set(2);
}


#elif defined( Test_OptionJson )

#include <Rso/GameUtil/OptionJson.h>
using namespace rso;
using namespace gameutil;

struct SAccount : public SProto
{
	wstring ID;
	wstring PW;
	SAccount()
	{
	}
	SAccount(const wstring& ID_, const wstring& PW_) : ID(ID_), PW(PW_)
	{
	}
	SAccount(wstring&& ID_, wstring&& PW_) : ID(std::move(ID_)), PW(std::move(PW_))
	{
	}
	void operator << (CStream& Stream_) override
	{
		Stream_ >> ID;
		Stream_ >> PW;
	}
	void operator << (const Value& Value_) override
	{
		Value_["ID"] >> ID;
		Value_["PW"] >> PW;
	}
	void operator >> (CStream& Stream_) const override
	{
		Stream_ << ID;
		Stream_ << PW;
	}
	void operator >> (Value& Value_) const override
	{
		Value_["ID"] = ID;
		Value_["PW"] = PW;
	}
	static wstring StdName(void)
	{
		return L"wstring,wstring";
	}
	static wstring MemberName(void)
	{
		return L"ID,PW";
	}
};

#include "Protocol.h"

void main(void)
{
	COptionJson<STest<wstring>> o(L"Option.ini", true);
	cout << o->ty << endl;
	cout << o->base.Name << endl;
	cout << o->base.Age << endl;

}

#elif defined( Test_TimeBoost )

#include <Rso/GameUtil/Base.h>
#include <functional>
#include <thread>

using namespace rso;
using namespace gameutil;

void main(void)
{
	STimeBoost s;
	MergeBoost(STimeBoost(), TTime(), STimeBoost());
	MergeBoost(s, TTime(), STimeBoost());

	auto Now = system_clock::now();
	auto Duration = seconds(5);
	auto EndTime = Now + Duration;
	auto Speed = 1.0;

	STimeBoost Boost(Now, Now, Speed);

	for (auto t = system_clock::now(); t < EndTime; t = system_clock::now())
		cout << GetBoostedDuration(Boost).count() << endl;
}


#elif defined( Test_Timer )

#include <thread>
#include <Rso/GameUtil/Timer.h>

using namespace rso;
using namespace gameutil;

void Callback(const int64& Key_)
{
	cout << "[Timer Callback]  Key : " << Key_ << endl;
}

void main(void)
{
	CTimer<int64> t(Callback);
	t.emplace(0, system_clock::now() + seconds(2));
	t.emplace(1, system_clock::now() + seconds(4));
	t.emplace(2, system_clock::now() + seconds(5));
	t.emplace(3, system_clock::now() + seconds(5));
	t.emplace(4, system_clock::now() + seconds(5));
	t.emplace(5, system_clock::now() + seconds(5));

	//t.move(0, seconds(-2));
	t.move(0, system_clock::now());
	t.erase(0);

	while (true)
	{
		if (t.empty())
			break;

		t.Proc();
		this_thread::sleep_for(milliseconds(1));
	}
}

#elif defined( Test_Ranking )

#include <Rso/GameUtil/Ranking.h>

using namespace rso;
using namespace gameutil;

struct SData
{
	string Name;

	SData(const string& Name_) :
		Name(Name_)
	{}
};

void main(void)
{
	CRanking<double, SData> Ranking;
	Ranking.emplace(1.0, SData("1"));
	Ranking.emplace(1.0, SData("1_2"));
	Ranking.emplace(10.0, SData("10"));
	Ranking.emplace(10.0, SData("10_2"));
	Ranking.emplace(5.0, SData("5"));

	cout << "ranking" << endl;
	for (auto& i : Ranking.ranking())
		cout << i.first.Ranking << " " << i.second.Name << endl;

	cout << "dense_ranking" << endl;
	for (auto& i : Ranking.dense_ranking())
		cout << i.first.Ranking << " " << i.second.Name << endl;
}


#elif defined( Test_Rank )

#include <Rso/GameUtil/Rank.h>

using namespace rso;
using namespace gameutil;

void main(void)
{
	CRank<int, int> r;
	r.emplace(2, 0);
	r.emplace(4, 1);

	for (int i = -1; i < 6; ++i)
	{
		auto t = r.get(i);
		if (t == r.end())
			cout << i << " null" << endl;
		else
			cout << i << " " << t->first << " " << t->second << endl;
	}

	return;


	cout << "-------------------------------" << endl;
	CRank<int, int, greater<int>> r2;
	r2.emplace(0, 0);
	r2.emplace(1, 1);
	r2.emplace(3, 3);
	r2.emplace(5, 5);

	for (int i = -1; i < 7; ++i)
	{
		auto t = r2.get(i);
		if (t == r2.end())
			cout << i << " null" << endl;
		else
			cout << i << " " << t->first << " " << t->second << endl;
	}


	cout << "-------------------------------" << endl;
	CRank<double, int> r3;
	r3.emplace(0, 0);
	r3.emplace(1, 1);
	r3.emplace(3, 3);
	r3.emplace(5, 5);

	for (int i = -1; i < 7; ++i)
	{
		auto t = r3.get(i);
		if (t == r3.end())
			cout << i << " null" << endl;
		else
			cout << i << " " << t->first << " " << t->second << endl;
	}
}


#elif defined( Test_Randomizer )

#include <Rso/GameUtil/Randomizer.h>
#include <Rso/GameUtil/Randomizers.h>

using namespace rso;
using namespace gameutil;

struct SRand
{
	int i;

	SRand(int i_) :
		i(i_)
	{}
};

void main(void)
{
	/////////////////////////////////////////////
	CRandomizer<SRand> r;

	r.insert(1, SRand(1));
	r.insert(2, SRand(2));

	for (int i = 0; i < 10; ++i)
		cout << r.get().i << endl;


	/////////////////////////////////////////////
	cout << "==============================================" << endl;
	CRandomizers<SRand> rs;

	rs.add_randomizer();
	rs.insert(1, SRand(1));
	rs.insert(2, SRand(2));

	rs.add_randomizer();
	rs.insert(50, SRand(50));
	rs.insert(10, SRand(10));

	for (int i = 0; i < 10; ++i)
	{
		for (auto& a : rs.get())
			cout << a.i << endl;
	}
}

#elif defined( Test_RandomBox )

#include <Rso/GameUtil/RandomBox.h>

using namespace rso;
using namespace gameutil;

struct STest
{
	int i = 3;
};

void main(void)
{
	/////////////////////////////////////////////
	CRandomBox<double, STest> r;

	r.Insert(10.0, STest{ 1 });
	r.Insert(0.1, STest{ 2 });
	r.Insert(0.1, STest{ 9 });

	cout << r.Get().i << endl;

	for (auto& i : r)
	{
		cout << i.second.i << endl;
	}
}

#elif defined( Test_KeyCnt )

#include <string>
#include <Rso/GameUtil/KeyCnt.h>
#include <Rso/GameUtil/Inventory.h>
#include <Rso/GameUtil/Rank.h>

using namespace rso;
using namespace gameutil;

void main(void)
{
	// KeyCnt ////////////////////////////////////
	cout << "KeyCnt Test" << endl;
	using TKeyCnt = CKeyCnt<int32, uint32>;
	using value_type = TKeyCnt::value_type;

	TKeyCnt KeyCnt;
	KeyCnt += value_type{ 1, 2 };
	KeyCnt += value_type{ 1, 4 };
	KeyCnt += value_type{ 2, 3 };
	KeyCnt += value_type{ 3, 6 };
	KeyCnt *= 2;

	for (auto& i : KeyCnt)
		cout << i.first << " " << i.second << endl;

	// Inventory //////////////////////////////////////
	cout << "Inventory Test" << endl;
	using TInventory = CInventory<int32, int32, int32, string>;
	using TItem = TInventory::SItem;
	using TIndexCnts = TInventory::TIndexCnts;
	using TCodeCnts = TInventory::TCodeCnts;

	TInventory Inventory;
	Inventory.Set(TItem("item0", 1, 100, 3));

	TIndexCnts IndexCnts;
	auto GotItem = Inventory.Get(1);
	cout << "GotItems" << endl;
	if (GotItem)
		cout << GotItem->Index << " " << GotItem->Code << " " << GotItem->Cnt << " " << GotItem->Data << endl;


	TCodeCnts CodeCnts;
	CodeCnts.emplace(100, 1);
	IndexCnts.clear();
	if (Inventory.Get(CodeCnts, IndexCnts))
	{
		cout << "Got IndexCnts" << endl;
		for (auto& i : IndexCnts)
			cout << i.Index << " " << i.Cnt << endl;
	}

	IndexCnts.clear();
	IndexCnts.emplace_back(2, 4);
	Inventory.Set(IndexCnts);

}

#elif defined( Test_Boost )


using namespace rso;
using namespace gameutil;

void main(void)
{
	STimeBoost b;
}
#endif
