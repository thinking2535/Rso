#pragma once


#include "Base.h"
#include "value.h"
#include "reader.h"
#include "writer.h"
#include "features.h"


namespace rso
{
	namespace json
	{
		class CJson : public Value
		{
		public:
			CJson() {}
			CJson(const SProto& Proto_);
			CJson(const string& String_);
			CJson(const wstring& String_);
			CJson(const CStream& Stream_);
		};
	}
}