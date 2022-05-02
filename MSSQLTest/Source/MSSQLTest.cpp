// RsoMailTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#define Test_StoredProcedure 1

#if Test_StoredProcedure

#include <Rso/MSSQL/StoredProcedure.h>
#include <iostream>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace rso::mssql;

using TKey = int;
using TGameDB = CStoredProcedure<int>;
using TCallback = TGameDB::TCallback;
using TCallbacks = TGameDB::TCallbacks;
using TOutObject = TGameDB::SOutObject;

TCallbacks g_DBCallbacks;

void DBCallback(TOutObject& OutObject_)
{
	if (OutObject_.Ret == -1)
	{
		cout << "Error" << endl;
		return;
	}
	else
	{
		cout << "OK" << endl;
	}

	SAuthLoginInDb In;
	SAuthLoginOutDb Out;
	OutObject_.InStream >> In;
	OutObject_.OutStream >> Out;
}

TGameDB* _pDB = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		_pDB = new TGameDB(SDBOption(5000, 5000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"GameAuth", 1433, 5000)), DBCallback);
//		_pDB->AddCmd<SUTFTestDBIn, SUTFDummyDBOut>(0, L"dbo.sys_sp_Login", true);
		_pDB->AddCmd<SAuthLoginInDb, SAuthLoginOutDb>(0, L"dbo.sys_sp_Login", true);


//		_pDB->AddCmd<STestDBIn, STestDBOut>(0, L"spTest", true);

		//_pDB->AddCmd<SAuthCreateInDb, SAuthLoginOutDb>(0, L"sys_sp_Create", true);
		//_pDB->AddCmd<SAuthLoginInDb, SAuthLoginOutDb>(1, L"sys_sp_Login", true);

		//_pDB->AddCmd<STest2DBIn, STestDBOut>(0, L"spTest2", true);
		//_pDB->AddCmd<STest3DBIn, STestDBOut>(0, L"spTest3", true);
		_pDB->Start(1000);
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
		return 0;
	}

	while (!_pDB->Connected())
	{
		_pDB->Proc();
		this_thread::sleep_for(milliseconds(1));
	}

	//_pDB->Push(0, 0, SUTFTestDBIn("a한韓•°♻☺☆«Б бД д", u"a한韓•°♻☺☆«Б бД д"s));
	_pDB->Push(0, 0, SAuthLoginInDb(u"IDID"s));

	//	_pDB->Push(0, 0, SAuthCreateInDb(SAuthLoginInDb(L"testid"), L"testnick"));
	//_pDB->Push(1, 0, SAuthLoginInDb(L"testid"));

	while (true)
	{
		this_thread::sleep_for(milliseconds(100));
		_pDB->Proc();
	}

	return 0;
}

#elif Test_MSSQL

#include <Rso/MSSQL/StoredProcedure.h>
#include <iostream>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace rso::mssql;

using TKey = int;
using TGameDB = CStoredProcedure<int>;
using TCallback = TGameDB::TCallback;
using TCallbacks = TGameDB::TCallbacks;
using TOutObject = TGameDB::SOutObject;

TCallbacks g_DBCallbacks;

void DBCallback(TOutObject& OutObject_)
{
	g_DBCallbacks[OutObject_.SPNum](OutObject_);
}
void TestOut0(TOutObject& OutObject_)
{
	try
	{
		SDBTestIn In;
		SDBTestOut Out;
		OutObject_.InStream >> In;
		OutObject_.OutStream >> Out;

		cout << "<SQL Results>--------------------------" << endl;
		cout << "Ret : " << OutObject_.Ret << " SPRet : " << OutObject_.SPRet << endl;

		cout << "<Inputs>--------------------------" << endl;
		cout << "i8 : " << In.i8 << " i32 : " << In.i32 << " s.size() : " << In.s.size() << endl;

		cout << "<Results>--------------------------" << endl;
		cout << "<int8s>" << endl;
		for (auto& i : Out.int8s)
			cout << int32(i) << endl;

		cout << "<int32s>" << endl;
		for (auto& i : Out.int32s)
			cout << i << endl;

		cout << "<streams>" << endl;
		for (auto& i : Out.streams)
			cout << i.size() << endl;

		cout << "<tps>" << endl;
		for (auto& i : Out.tps)
			cout << i.time_since_epoch().count() << endl;

		cout << "<dts>" << endl;
		for (auto& i : Out.dts)
			wcout << i.ToString() << endl;

		cout << "<strs>" << endl;
		for (auto& i : Out.strs)
			cout << i.size() << " " << i << endl;

		cout << "<wstrs>" << endl;
		for (auto& i : Out.wstrs)
			wcout << i.size() << " " << i << endl;

		cout << "<Outputs>--------------------------" << endl;
		cout << Out.out_i32 << endl;
		wcout << Out.dt_io.ToString() << endl;
	}
	catch (...)
	{
		cout << "Recv Error" << endl;
	}
}
void TestOut1(TOutObject& OutObject_)
{
	try
	{
		SDBTest1In In;
		SDBTest1Out Out;

		OutObject_.InStream >> In;
		OutObject_.OutStream >> Out;

		cout << "<SQL Results>--------------------------" << endl;
		cout << "Ret : " << OutObject_.Ret << " SPRet : " << OutObject_.SPRet << endl;

		cout << "<Results>--------------------------" << endl;
		//cout << "<strs>" << endl;
		//for (auto& i : Out.strs)
		//	cout << i.size() << " " << i << endl;

		cout << "<streams>" << endl;
		for (auto& i : Out.streams)
			cout << i.size() << endl;
	}
	catch (...)
	{
		cout << "Recv Error" << endl;
	}
}
void TestOut2(TOutObject& OutObject_)
{
	try
	{
		SDBTest2In In;
		SDBTest2Out Out;

		OutObject_.InStream >> In;
		OutObject_.OutStream >> Out;

		for (auto& i : Out.Times)
		{
			if (i == system_clock::time_point())
				cout << "Same" << endl;
		}
	}
	catch (...)
	{
		cout << "Recv Error" << endl;
	}
}

template<typename _TType> struct SDB { };
//template<> struct SDB<SDBTestIn> { static const size_t SPNum = 0; };
//template<> struct SDB<SDBTest1In> { static const size_t SPNum = 1; };
template<> struct SDB<SDBTest2In> { static const size_t SPNum = 0; };

TGameDB* _pDB = 0;

template<typename _TIn, typename _TOut>
void AddCmd(const wstring& SPName_, const wstring& Param_, bool Return_, TCallback Callback_)
{
	_pDB->AddCmd<_TIn, _TOut>(SDB<_TIn>::SPNum, SPName_, Param_, Return_);
	g_DBCallbacks.emplace_at(SDB<_TIn>::SPNum, Callback_);
}
template<typename _TIn>
void	Push(size_t ConnNum_, const TKey& Key_, const _TIn& In_)
{
	_pDB->Push(ConnNum_, SDB<_TIn>::SPNum, Key_, In_);
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		_pDB = new TGameDB(SDBOption(5000, 5000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"Test", 1433, 5000)), DBCallback);

		//AddCmd<SDBTestIn, SDBTestOut>(L"spTest0", L"01001000100", true, TestOut0);
		//AddCmd<SDBTest1In, SDBTest1Out>(L"spTest1", L"0", true, TestOut1);
		AddCmd<SDBTest2In, SDBTest2Out>(L"spTest2", L"0", true, TestOut2);
		_pDB->Start();
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
		return 0;
	}

	_pDB->Proc();
	this_thread::sleep_for(milliseconds(3000));
	_pDB->Proc();

	//CStream s;

	//list<int32> i32s;
	//i32s.emplace_back(1);
	//i32s.emplace_back(2);
	//i32s.emplace_back(3);

	//list<system_clock::time_point> tps;
	//tps.emplace_back(system_clock::now());

	//_pDB->Push(0, TKey(c_PeerCnt_Null, 0), SDBTestIn(8, 3333, s, system_clock::now(), system_clock::now(), i32s, tps, CDateTime(system_clock::now()), CDateTime(system_clock::now()), "a", L""));

	//if (!_pDB->Push(1, TKey(c_PeerCnt_Null, 0), SDBTest1In(CStream())))
	//{
	//	cout << "Push Error" << endl;
	//	return 0;
	//}

	_pDB->Push(0, TKey(), SDBTest2In(system_clock::time_point()));

	while (true)
	{
		this_thread::sleep_for(milliseconds(10));
		_pDB->Proc();
	}

	return 0;
}
#elif Test_Tvp

#include <Rso/MSSQL/StoredProcedure.h>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace mssql;

using TSP = CStoredProcedure<SKey>;

void Callback(TSP::SOutObject& OutObj_)
{
	
}

void main(void)
{
	TSP sp(SDBOption(1000, 1000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"Test", 1433, 1000)), Callback);
	sp.AddCmd<STestDBIn, STestDBOut>(0, L"spTest", true);

	sp.Start();

	while (!sp.Connected())
		this_thread::sleep_for(milliseconds(100));

	list<string> a;
	list<wstring> b;
	list<SStringWStringInt> c;
	for (size_t i = 0; i < 5; ++i)
	{
		a.emplace_back(to_string(i));
		b.emplace_back(to_wstring(i));
		c.emplace_back(SStringWStringInt(a.back(), b.back(), i));
	}

	sp.Push(0, SKey(), STestDBIn(std::move(a), std::move(b), std::move(c)));

	for (int i = 0; i < 100; ++i)
	{
		sp.Proc();
		this_thread::sleep_for(milliseconds(1));
	}
}

#elif Test_BulkSelect

#include <Rso/MSSQL/BulkSelect.h>
#include <iostream>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace mssql;

bool RowCallback(SQLLEN RowNum_, CStream& Row_)
{
	return false;
}
void ParamCallback(SQLRETURN Ret_, int32 SpRet_, CStream& OutParams_)
{
	cout << "Ret:" << Ret_ << " SPRet:" << SpRet_ << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CBulkSelect<SDBTestIn, SDBTestOut> bs(
		SDBOption(1000, 1000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"Test", 1433, 1000)),
		RowCallback,
		ParamCallback,
		L"spBulkSelectTest");

	if (!bs.Push(SDBTestIn()))
		cout << "Push Fail" << endl;

	cout << "Proc" << endl;
	bs.Proc();
	cout << "Proc" << endl;
	bs.Proc();
	cout << "Proc" << endl;
	bs.Proc();
	cout << "Proc" << endl;
	bs.Proc();
	cout << "Proc" << endl;
	bs.Proc();
	cout << "Proc" << endl;
	bs.Proc();

	return 0;
}

#elif Test_BulkCopy

#include <Rso/MSSQL/BulkCopy.h>
#include <iostream>
#include <chrono>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace mssql;
using namespace bulkcopy;

int _tmain(int argc, _TCHAR* argv[])
{
	CBulkCopy<SBulkCopyDBIn> bc(SDBOption(1000, 1000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"Test", 1433, 1000)), L"tTest", L"0");

	for (int i = 0; i < 1000; ++i)
		bc.Push(SBulkCopyDBIn(i));

	throw 0;
}

#endif