#include "Application.h"

namespace rso
{
	namespace excel
	{
		void CApplication::_Finalize(void)
		{
			if (_Application)
			{
				_Application->Quit();
				_Application.Release();
			}
		}
		CApplication::CApplication()
		{
			if (FAILED(_Application.CreateInstance(L"Excel.Application")))
				THROWEX();

			if (!_Application)
				THROWEX();
		}
		CApplication::~CApplication()
		{
			_Finalize();
		}
		_WorkbookPtr CApplication::Open(const wstring& FileName_)
		{
			_variant_t Option(static_cast<long>(DISP_E_PARAMNOTFOUND), VT_ERROR);

			try
			{
				auto Ret = _Application->Workbooks->Open(
					absolute(FileName_).wstring().c_str(),
					Option,
					Option,
					Option,
					Option,
					Option,
					Option,
					Option,
					Option,
					Option,
					Option,
					Option,
					Option);
				if (!Ret)
					THROWEX();

				return Ret;
			}
			catch (...)
			{
				_Finalize();
				throw;
			}
		}
	}
}
