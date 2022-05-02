#pragma once

#include <filesystem>
#include <Rso/Win/CoInitializer.h>
#include <Rso/Win/Win.h>

#import "Rso/MSO.DLL" \
	rename("RGB", "MSORGB") \
	rename("DocumentProperties", "MSDocumentProperties")

#import "Rso/VBE6EXT.OLB"

#import "Rso/EXCEL.EXE" \
	rename("DialogBox", "ExcelDialogBox") \
	rename("RGB", "ExcelRGB") \
	rename("CopyFile", "ExcelCopyFile") \
	rename("ReplaceText", "ExcelReplaceText")

namespace rso
{
	namespace excel
	{
		using namespace std;
		using namespace filesystem;
		using namespace win;
		using namespace Office;
		using namespace VBIDE;
		using namespace Excel;

		class CApplication : public CCoInitializer
		{
			_ApplicationPtr _Application{};

			void _Finalize(void);

		public:
			CApplication();
			CApplication(const CApplication& Var_) = delete;
			CApplication(CApplication&& Var_) = delete;
			virtual ~CApplication();
			CApplication& operator = (const CApplication& Var_) = delete;
			CApplication& operator = (CApplication&& Var_) = delete;
			_WorkbookPtr Open(const wstring& FileName_);
		};
	}
}
