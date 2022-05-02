#include "Workbook.h"

namespace rso
{
	namespace excel
	{
		CWorkbook::CWorkbook(const wstring& FileName_) :
			_Workbook(Open(FileName_))
		{
		}
		CWorkbook::~CWorkbook()
		{
			if (_Workbook)
			{
				_Workbook->Close(VARIANT_FALSE);
				_Workbook.Release(); // 디버깅 걸고 천천히 진행하면 EXCEL 프로세스가 정상적으로 삭제되나, 브레이크 걸지 않으면 여기 오더라도 프로세스가 남아있음.
			}
		}
		SheetsPtr CWorkbook::GetSheets(void)
		{
			return _Workbook->Sheets;
		}
	}
}
