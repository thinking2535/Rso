#pragma once


#include <Rso/Base/Base.h>
#pragma comment( lib, "Rso/Excel_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )

#include "Workbook.h"
#include <Rso/Data/Data.h>


namespace rso
{
	namespace excel
	{
		using namespace std;
		using namespace data;

		class CExcel
		{
			using _TSheets = map<wstring, TRows>;

			unique_ptr<CWorkbook> _Workbook;
			_TSheets _Sheets;

			wstring	_GetStr(const _variant_t& Item_) const { return wstring(_bstr_t(Item_)); }

		public:
			CExcel(const wstring& FileName_, const wstring& Sheets_, wchar_t Delimiter_);

			template<typename _TProto>
			void Extract(const wstring& Sheet_, const wstring& Columns_, _TProto& Proto_)
			{
				auto itSheet = _Sheets.find(Sheet_);
				if (itSheet == _Sheets.end())
					THROWEXA(L"Invalid Sheet Name[%s]", Sheet_);

				data::Extract(itSheet->second, Columns_, Proto_);
			}
			template<typename _TProto>
			TCoreCheckSum SaveFile(const wstring& Sheet_, const wstring& Columns_, const wstring& FileName_)
			{
				_TProto Proto;
				Extract(Sheet_, Columns_, Proto);

				CStream Stream;
				Stream << Proto;
				Stream.SaveFile(FileName_);
				return Stream.checksum();
			}
		};
	}
}
