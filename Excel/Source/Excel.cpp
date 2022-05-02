#include "Excel.h"

namespace rso
{
	namespace excel
	{
		CExcel::CExcel(const wstring& FileName_, const wstring& Sheets_, wchar_t Delimiter_)
		{
			_Workbook.reset(new CWorkbook(FileName_));

			set<wstring> SheetsToExtract;
			wstring Sheets = Sheets_;
			for (auto Sheet = SplitWithBlank(Delimiter_, Sheets); Sheet.size() > 0; Sheet = SplitWithBlank(Delimiter_, Sheets))
				SheetsToExtract.emplace(Sheet);

			for (int SheetNum = 0; SheetNum < _Workbook->GetSheets()->Count; ++SheetNum)
			{
				Excel::_WorksheetPtr Sheet = _Workbook->GetSheets()->Item[SheetNum + 1];

				wstring SheetName(Sheet->Name);
				if (SheetsToExtract.find(SheetName) == SheetsToExtract.end())
					continue;

				auto ib = _Sheets.emplace(SheetName, TRows());
				if (!ib.second)
					THROWEXA(L"Already inserted sheet Name[%s]", ib.first->first);

				Excel::RangePtr Range = Sheet->GetRange(_bstr_t(L"A1"))->CurrentRegion;

				ib.first->second.reserve(Range->Rows->Count);

				// Get Columns' DataType
				for (int RowNum = 0; RowNum < Range->Rows->Count; ++RowNum)
				{
					ib.first->second.emplace_back(vector<wstring>());
					ib.first->second.back().reserve(Range->Columns->Count);

					for (int ColNum = 0; ColNum < Range->Columns->Count; ++ColNum)
						ib.first->second.back().emplace_back(_GetStr(Range->Item[RowNum + 1][ColNum + 1]));
				}
			}

			_Workbook.reset();
		}
	}
}
