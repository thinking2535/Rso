#pragma once

#include "Option.h"
#include <Rso/Core/Stream.h>

namespace rso
{
	namespace base
	{
		using namespace core;

		template<typename TData>
		class COptionStream : public COption<TData>
		{
		public:
			using COption<TData>::operator*;
			using COption<TData>::operator->;
			using COption<TData>::operator=;
			using COption<TData>::_FileName;
			using COption<TData>::_Data;

			COptionStream(const wstring& FileName_, bool NoException_) : // true : ������ ���������� ������ ��ȯ �����ϸ� ���������� ������ ����
				COption<TData>(FileName_)
			{
				CStream Stream;

				try
				{
					Stream.LoadFile(_FileName);
				}
				catch (...)
				{
					if (!NoException_)
						throw;

					Save();
					return;
				}

				try
				{
					Stream >> _Data;
				}
				catch (...)
				{
					if (!NoException_)
						throw;

					Reset();
					Save();
				}
			}
			COptionStream(const wstring& FileName_, const TData& Data_) :
				COption<TData>(FileName_, Data_)
			{
				Save();
			}
			void Save(void) override
			{
				(CStream() << _Data).SaveFile(_FileName);
			}
		};
	}
}
