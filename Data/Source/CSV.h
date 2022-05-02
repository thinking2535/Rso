#pragma once


#include "Data.h"
#include <map>
#include <vector>


namespace rso
{
	namespace data
	{
		using namespace std;

		template<typename _TProto>
		void Extract(const wstring& DataString_, wchar_t DataDelimiter_, const wstring& Columns_, wchar_t ColumnDelimiter_, _TProto& Proto_)
		{
			// Delimiter_ 로 구분된 문자열 DataString_을  wstring 으로 추출하여 Rows 에 삽입

			auto DataString(DataString_);
			TRows Rows;

			size_t ColumnCnt = 0;

			// Insert Columns
			for (auto ColumnsRow = Split(L"\n\r", DataString);; ColumnsRow = Split(L"\n\r", DataString))
			{
				if (ColumnsRow.empty())
					THROWEX();

				if (ColumnsRow.front() == L'#')
					continue;

				Rows.emplace_back(vector<wstring>());

				for (auto Value = SplitWithBlank(ColumnDelimiter_, ColumnsRow); Value.size() > 0; Value = SplitWithBlank(ColumnDelimiter_, ColumnsRow))
					Rows.back().emplace_back(Value);

				if (ColumnsRow.size() > 0)
					THROWEXA(L"Column Name Must Be Filled");

				break;
			}

			ColumnCnt = Rows.back().size();

			// Insert Datas
			for (auto DataRow = Split(L"\n\r", DataString); DataRow.size() > 0; DataRow = Split(L"\n\r", DataString))
			{
				if (DataRow.front() == L'#')
					continue;

				Rows.emplace_back(vector<wstring>());

				for (size_t i = 0; i < ColumnCnt; ++i)
					Rows.back().emplace_back(SplitWithBlank(DataDelimiter_, DataRow));
			}

			if (Rows.size() == 0)
				THROWEXA(L"Can not find CSV Title");

			Extract(Rows, Columns_, Proto_);
		}

		template<typename _TProto>
		void ExtractFromFile(const wstring& FileName_, wchar_t DataDelimiter_, const wstring& Columns_, wchar_t ColumnDelimiter_, _TProto& Proto_, const char* Locale_)
		{
			CStream Stream;
			CData::Get(FileName_, Stream);

			wstring WString;

			string s(Stream.buff(), Stream.size());
			WString = MBSToWCS(s, Locale_);

			Extract(WString, DataDelimiter_, Columns_, ColumnDelimiter_, Proto_);
		}

		template<typename _TProto>
		void ExtractFromFile(const wstring& Server_, const wstring& Obj_, const wstring& Param_, THttpPort Port_, wchar_t DataDelimiter_, const wstring& Columns_, wchar_t ColumnDelimiter_, _TProto& Proto_, const char* Locale_)
		{
			CStream Stream;
			CData::Get(Server_, Obj_, Param_, Port_, Stream);

			wstring WString;

			string s(Stream.buff(), Stream.size());
			WString = MBSToWCS(s, Locale_);

			Extract(WString, DataDelimiter_, Columns_, ColumnDelimiter_, Proto_);
		}
	}
}