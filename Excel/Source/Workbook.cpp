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
				_Workbook.Release(); // ����� �ɰ� õõ�� �����ϸ� EXCEL ���μ����� ���������� �����ǳ�, �극��ũ ���� ������ ���� ������ ���μ����� ��������.
			}
		}
		SheetsPtr CWorkbook::GetSheets(void)
		{
			return _Workbook->Sheets;
		}
	}
}
