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

		// CStrema ��ü�� �� �Է�
		CStream stm;
		stm << 10 << string("Name");

		// �������� ����ü�� �� �Է�
		STestBase tb;
		tb.i16 = 16;
		tb.i32 = 32;
		tb.s = "STestBase";
		tb.l.emplace_back(3);
		tb.l.emplace_back(4);

		// CStream ��ü�� �������� ��ü �Է�
		stm << tb;


		// �Ǵٸ� �������� ����ü�� CStream ��ü �� ���� �����̳� �Է�
		STest ProtoForSave(
			STestBase(
				ETest::B, 0, 1, 2, 3, 4, 5, 6, 7, 8.8f, 9.9, "abc", L"DEF", stm, SDateTime(2019, 1, 2, 3, 4, 5), system_clock::now(),
				// �����̳� Ÿ��
				array<int32, 3>{1, 2, 3},
				vector<CStream>{stm},
				list<int8>{'a', 'b'},
				set<float>{1.0f, 2.0f},
				map<int32, SDateTime>{map<int32, SDateTime>::value_type(3, SDateTime())}),
			0, TMyMap{}
		);

		// ��������, �⺻Ÿ��, ��Ʈ�� �� 
		CStream StreamForSave;
		StreamForSave << ProtoForSave;

		// ���� ����
		StreamForSave.SaveFile(L"a.bin");

		// ���� �ε�
		CStream StreamForLoad(L"a.bin");

		// STest ��ü Pop
		auto ProtoForLoad = StreamForLoad.get_pop<STest>();

		// �о�� �������� ��ü�� Json �������� ���
		cout << "STest ��ü�� ���� ���" << endl;
		cout << CJson(ProtoForLoad).toStyledString() << endl;

		// ProtoForLoad �� ������ CStream �� ����� ���� Pop
		auto int_out = ProtoForLoad.stm.get_pop<int32>();
		auto string_out = ProtoForLoad.stm.get_pop<string>();

		// STestBase ��ü Pop
		auto tb_out = ProtoForLoad.stm.get_pop<STestBase>();

		// �о�� ��Ʈ�� ���� �������� ��ü�� Json �������� ���
		cout << "STestBase ��ü�� ���� ���" << endl;
		cout << CJson(tb_out).toStyledString() << endl << endl;
	}
#endif
	return 0;
}
