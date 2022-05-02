#include "stdafx.h"
#include <iostream>
#include <Rso/GameBill/Bill.h>

using namespace rso;
using namespace gamebill;

void Callback(CBill::TOutObject& OutObject_)
{
	if (OutObject_.Ret == 0)
	{
		SPurchaseIn In;
		OutObject_.InStream >> In;

		int32 r;
		OutObject_.InStream >> r;
		cout << r << endl;
	}
	else
	{
		cout << "callback : " << OutObject_.Ret << " " << OutObject_.SPRet << endl;
	}
}

void main(void)
{
	SDBOption DB(1000, 1000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"Test", 1433, 1000));

	CBill b(DB, Callback);
	b.Start();
	b.Proc();

//	여기서 오류나는 이유는? (소멸자에서 오류?)
	CStream stm;
	stm << 999;
	auto Ret = b.Push(SPurchaseIn(0, "OrderID", EOS::Android, "ZZ", 0, 0, "ProductID", stm));
}
