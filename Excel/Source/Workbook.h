#pragma once

#include "Application.h"

namespace rso
{
	namespace excel
	{
		class CWorkbook : public CApplication
		{
			_WorkbookPtr _Workbook{};

		public:
			CWorkbook(const wstring& FileName_);
			virtual ~CWorkbook();
			SheetsPtr GetSheets(void);
		};
	}
}
