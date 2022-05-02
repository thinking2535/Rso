#include "stdafx.h"

#include <iostream>
#include <Rso/Core/Stream.h>
#include <Rso/Proto/Proto.h>

#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace core;
using namespace proto;

using namespace ns;
using namespace inc;

int _tmain(int argc, _TCHAR* argv[])
{
#if true

#else
	// Test 0 ////////////////////////////////////////////////////////
	cout << "Test 0 ---------------------------------------------" << endl;
	{
		CStream StreamForSave;
		StreamForSave << 100;
		StreamForSave.SaveFile(L"a.bin");

		CStream StreamForLoad;
		StreamForLoad.LoadFile(L"a.bin");

		int32 int_out = 0;
		StreamForLoad >> int_out;
		cout << int_out << endl << endl;
	}


	// Test 1 ////////////////////////////////////////////////////////
	{
		cout << "Test 1 ---------------------------------------------" << endl;
		(CStream() << 100).SaveFile(L"a.bin");
		cout << CStream(L"a.bin").get_pop<int32>() << endl << endl;
	}


	// Test 2 ////////////////////////////////////////////////////////
	{
		cout << "Test 2 ---------------------------------------------" << endl;

		// CStrema 객체에 값 입력
		CStream stm;
		stm << 10 << string("Name");

		// 프로토콜 구조체에 값 입력
		STestBase tb;
		tb.i16 = 16;
		tb.i32 = 32;
		tb.s = "STestBase";
		tb.l.emplace_back(3);
		tb.l.emplace_back(4);

		// CStream 객체에 프로토콜 객체 입력
		stm << tb;


		// 또다른 프로토콜 구조체에 CStream 객체 및 각종 컨테이너 입력
		STest ProtoForSave(
			STestBase(
				ETest::B, 0, 1, 2, 3, 4, 5, 6, 7, 8.8f, 9.9, "abc", L"DEF", stm, SDateTime(2019, 1, 2, 3, 4, 5), system_clock::now(),
				// 컨테이너 타입
				array<int32, 3>{1, 2, 3},
				vector<CStream>{stm},
				list<int8>{'a', 'b'},
				set<float>{1.0f, 2.0f},
				map<int32, SDateTime>{map<int32, SDateTime>::value_type(3, SDateTime())}),
			0, TMyMap{}
		);

		// 프로토콜, 기본타입, 스트림 및 
		CStream StreamForSave;
		StreamForSave << ProtoForSave;

		// 파일 저장
		StreamForSave.SaveFile(L"a.bin");

		// 파일 로드
		CStream StreamForLoad(L"a.bin");

		// STest 객체 Pop
		auto ProtoForLoad = StreamForLoad.get_pop<STest>();

		// 읽어온 프로토콜 객체를 Json 형식으로 출력
		cout << "STest 객체의 내용 출력" << endl;
		cout << CJson(ProtoForLoad).toStyledString() << endl;

		// ProtoForLoad 의 내용중 CStream 에 저장된 내용 Pop
		auto int_out = ProtoForLoad.stm.get_pop<int32>();
		auto string_out = ProtoForLoad.stm.get_pop<string>();

		// STestBase 객체 Pop
		auto tb_out = ProtoForLoad.stm.get_pop<STestBase>();

		// 읽어온 스트림 안의 프로토콜 객체를 Json 형식으로 출력
		cout << "STestBase 객체의 내용 출력" << endl;
		cout << CJson(tb_out).toStyledString() << endl << endl;
	}
#endif
	return 0;
}
