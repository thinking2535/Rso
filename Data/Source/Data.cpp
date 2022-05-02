#include "Data.h"


namespace rso
{
	namespace data
	{
		CData::CData()
		{
		}

		CData::~CData()
		{
		}

		void CData::Get(const wstring& FileName_, CStream& Stream_)
		{
			CLoaderFile(FileName_).Get(Stream_);
		}

		void CData::Get(const string& Server_, const string& Obj_, const string& Param_, THttpPort Port_, CStream& Stream_)
		{
			CLoaderHttp(Server_, Obj_, Param_, Port_).Get(Stream_);
		}

		void CData::Get(const wstring& FileName_, SProto& Proto_)
		{
			CLoaderFile(FileName_).Get(Proto_);
		}

		void CData::Get(const string& Server_, const string& Obj_, const string& Param_, THttpPort Port_, SProto& Proto_)
		{
			CLoaderHttp(Server_, Obj_, Param_, Port_).Get(Proto_);
		}

		EType GetType(const wstring& Value_)
		{
			if (Value_.size() == 0)
				return EType::WString;

			int DotCnt = 0;
			size_t Counter = 0;
			for (auto& c : Value_)
			{
				if (
					(Counter == 0 && c == L'-') ||
					(c >= L'0' && c <= L'9') ||
					c == L'.')
				{
					if (c == L'.')
					{
						++DotCnt;

						if (DotCnt > 1)
							return EType::WString;
					}
				}
				else
				{
					return EType::WString;
				}

				++Counter;
			}

			if (DotCnt == 0)
				return EType::Int64;
			else
				return EType::Double;
		}

		void IsConvertable(EType CSVType_, const wstring& ProtoType_)
		{
			if (CSVType_ == EType::Int64)
			{
				// Int64 는 모든 형으로 변환 가능
				//if (ProtoType_ != L"bool" &&
				// ProtoType_ != L"int8" &&
				// ProtoType_ != L"int16" &&
				// ProtoType_ != L"int32" &&
				// ProtoType_ != L"int64")
				// THROWEXA(L"CanNot Convert Int64(CSV) To %s(Proto)", ProtoType_);
			}
			else if (CSVType_ == EType::Double)
			{
				if (ProtoType_ != L"float" &&
					ProtoType_ != L"double" &&
					ProtoType_ != L"int32" &&
					ProtoType_ != L"int64" &&
					ProtoType_ != L"wstring" &&
					ProtoType_ != L"string")
					THROWEXA(L"CanNot Convert Double(CSV) To %s(Proto)", ProtoType_);
			}
			else if (CSVType_ == EType::WString)
			{
				if (ProtoType_ != L"wstring" &&
					ProtoType_ != L"string")
					THROWEXA(L"CanNot Convert WString(CSV) To %s(Proto)", ProtoType_);
			}
			else
			{
				THROWEXA(L"Invalid CSVType[%d]", __int32(CSVType_));
			}
		}

		bool GetBool(const wstring& Data_)
		{
			if (Data_ == L"0" ||
				Data_ == L"")
				return false;

			return true;
		}
		__int8 GetInt8(const wstring& Data_)
		{
			auto Ret = stoll(Data_);
			if (Ret > MaxValue<__int8>() ||
				Ret < MinValue<__int8>())
				THROWEXA(L"Can Not Convert CSV Type To Int8[%s] Out Of Range", Data_);

			return __int8(Ret);
		}
		__int16 GetInt16(const wstring& Data_)
		{
			auto Ret = stoll(Data_);
			if (Ret > MaxValue<__int16>() ||
				Ret < MinValue<__int16>())
				THROWEXA(L"Can Not Convert CSV Type To Int16[%s] Out Of Range", Data_);

			return __int16(Ret);
		}
		__int32 GetInt32(const wstring& Data_)
		{
			auto Ret = stoi(Data_);
			if (Ret > MaxValue<__int32>() ||
				Ret < MinValue<__int32>())
				THROWEXA(L"Can Not Convert CSV Type To Int16[%s] Out Of Range", Data_);

			return __int32(Ret);
		}
		__int64 GetInt64(const wstring& Data_)
		{
			return stoll(Data_);
		}
		float GetFloat(const wstring& Data_)
		{
			auto Ret = stof(Data_);
			if (std::isinf(Ret))
				THROWEXA(L"Can Not Convert CSV Type To Float[%s] Out Of Range", Data_);

			return Ret;
		}
		double GetDouble(const wstring& Data_)
		{
			auto Ret = stod(Data_);
			if (std::isinf(Ret))
				THROWEXA(L"Can Not Convert CSV Type To Double[%s] Out Of Range", Data_);

			return Ret;
		}
		string GetString(const wstring& Data_)
		{
			return WCSToMBS(Data_);
		}
	}
}
