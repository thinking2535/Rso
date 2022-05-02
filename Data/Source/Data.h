#pragma once


#include "LoaderHttp.h"
#include "LoaderFile.h"


namespace rso
{
	namespace data
	{
		using namespace base;
		using namespace std;
		using namespace chrono;

		class CData
		{
		public:
			CData();
			virtual ~CData();

			static void Get(const wstring& FileName_, CStream& Stream_);
			static void Get(const string& Server_, const string& Obj_, const string& Param_, THttpPort Port_, CStream& Stream_);
			static void Get(const wstring& FileName_, SProto& Proto_);
			static void Get(const string& Server_, const string& Obj_, const string& Param_, THttpPort Port_, SProto& Proto_);
		};


		enum class EType
		{
			Int64,
			Double,
			WString,
			Max,
			Null
		};

		EType GetType(const wstring& Value_);
		void IsConvertable(EType CSVType_, const wstring& ProtoType_);
		bool GetBool(const wstring& Data_);
		__int8 GetInt8(const wstring& Data_);
		__int16 GetInt16(const wstring& Data_);
		__int32 GetInt32(const wstring& Data_);
		__int64 GetInt64(const wstring& Data_);
		float GetFloat(const wstring& Data_);
		double GetDouble(const wstring& Data_);
		string GetString(const wstring& Data_);

		using TRows = vector<vector<wstring>>;

		template<typename _TProto>
		void Extract(const TRows& Rows_, const wstring& Columns_, _TProto& Proto_)
		{
			// Rows_��  �÷��� ������ �� �� ���̷�
			// Columns_(������ �÷���) �� �����Ͽ� CStream ���� ��ȯ
			// Rows_ �� ����� �÷� ������ ��� ���� Columns_�� �÷� ������ �°� ����

			// ������ �÷� ���� Ȯ�� ���� RowInfo ���� ////////////////////////////
			struct _SColumnInfo
			{
				wstring Name;
				wstring Type;

				_SColumnInfo()
				{
				}
				_SColumnInfo(const wstring& Name_) :
					Name(Name_)
				{}
			};
			vector<_SColumnInfo> RowInfo; // ������ �÷� ���� Ȯ�� ���� ����( false : �������� ���� )
			// �ӵ� ���� Vector �� ( �� ������ ��� �����̳ʰ� �� �ʿ��� )



			// Get First Row /////////////////////////////////////
			auto itRow = Rows_.begin();
			if (itRow == Rows_.end())
				THROWEXA(L"Can not find Title");

			size_t ColumnCnt = itRow->size();
			struct _SCntIndex
			{
				size_t ColumnCnt{ 0 };
				size_t Index{ 0 };

				_SCntIndex(size_t Index_) :
					Index(Index_), ColumnCnt(1) {}
			};
			map<wstring, _SCntIndex> Index; // For Column Order

			size_t ColumnCounter{};

			for (auto& Cell : *itRow)
			{
				Trim(Cell);

				RowInfo.emplace_back(_SColumnInfo(Cell)); // ���� �̸��� �����ϰ� ���� Columns �� ���� �� ���� �ٽ� ��ĵ�Ͽ� ������ ��, ���θ� ����, ������ �����߿� ���� Ÿ�� ����, ���� Ÿ�� ����

				auto ib = Index.emplace(Cell, _SCntIndex(ColumnCounter));
				if (!ib.second)
					++(ib.first->second.ColumnCnt);

				++ColumnCounter;
			}

			// Set Column Order ////////////////////////////////////
			auto Columns = Columns_;

			set<wstring> ExtractColumns;
			vector<size_t> ColumnOrder;
			for (auto Column = SplitWithBlank(L',', Columns); Column.size() > 0; Column = SplitWithBlank(L',', Columns))
			{
				Trim(Column);

				auto itIndex = Index.find(Column);
				if (itIndex == Index.end())
					THROWEXA(L"Invalid Column [%s]", Column);

				if (itIndex->second.ColumnCnt > 1)
					THROWEXA(L"Ambiguous Column [%s] to Extract", Column);

				// RowInfo �� ����� ���� �����̳� ����
				ExtractColumns.emplace(Column);

				// ������ �÷� ���� ����
				ColumnOrder.emplace_back(itIndex->second.Index);
			}

			// ������ ���� ///////////////////////////////////////////////////////////
			auto* Row = new wstring[Index.size()];

			try
			{
				CStream Stream;

				Stream << __int32();
				__int32 Count = 0;

				wstring ExtractTypes{ Proto_.StdName() };
				if (ExtractTypes.size() < 2)
					THROWEXA(L"Proto Is Not Container Type");

				ExtractTypes = ExtractTypes.substr(1, ExtractTypes.size() - 2);

				size_t RowNum = 1;
				for (++itRow; itRow != Rows_.end(); ++itRow, ++RowNum)
				{
					if (itRow->size() != ColumnCnt)
						THROWEXA(L"Current Row[%d]'s ColumnCnt[%d] is different FirstColumnCnt[%d]", RowNum, itRow->size(), ColumnCnt);


					// �÷��� ���� �ѹ� ��ĵ�Ͽ� vector �� �ְ� ���� ��������� �°� CStream�� ����
					for (auto& ColumnNum : ColumnOrder)
					{
						auto Cell = (*itRow)[ColumnNum];
						Cell = Trim(Cell);

						if (RowNum == 1)
						{
							auto ColumnType = SplitWithBlank(L',', ExtractTypes);

							if (RowInfo[ColumnNum].Type == L"") // ���ʿ� �̾ L"" �� �ƴ� ��찡 ����
								RowInfo[ColumnNum].Type = ColumnType;
						}

						IsConvertable(GetType(Cell), RowInfo[ColumnNum].Type);

						if (RowInfo[ColumnNum].Type == L"") // ���� �� �̹Ƿ� Ÿ�� ����
						{
							auto ColumnType = SplitWithBlank(L',', ExtractTypes);

							auto CSVType = GetType(Cell);

							IsConvertable(CSVType, ColumnType);

							RowInfo[ColumnNum].Type = ColumnType;
						}
						else // �ι�° ���� ���� ���� ���� ���Ͽ� �ٸ��� �ͼ���
						{
							IsConvertable(GetType(Cell), RowInfo[ColumnNum].Type);
						}


						if (RowInfo[ColumnNum].Type == L"bool")
							Stream << GetBool(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"int8")
							Stream << GetInt8(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"int16")
							Stream << GetInt16(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"int32")
							Stream << GetInt32(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"int64")
							Stream << GetInt64(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"float")
							Stream << GetFloat(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"double")
							Stream << GetDouble(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"string")
							Stream << GetString(std::move(Cell));
						else if (RowInfo[ColumnNum].Type == L"wstring")
							Stream << std::move(Cell);
						else
							THROWEXA(L"Current Row[%d]'s ColumnCnt[%d] Can Not Convert ProtoType[%s] To CSVType", RowNum, itRow->size(), RowInfo[ColumnNum].Type);
					}

					++Count;
				}

				*((__int32*)Stream.at(0)) = Count;


				// Type Check
				Stream >> Proto_;
			}
			catch (...)
			{
				delete[] Row;
				throw;
			}
		}
	}
}
