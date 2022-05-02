#pragma once

#include "Base.h"

namespace rso
{
	namespace base
	{
		template<typename TData>
		class COption
		{
		protected:
			wstring _FileName;
			TData _Data{};

		public:
			COption(const wstring& FileName_) :
				_FileName(FileName_)
			{
			}
			COption(const wstring& FileName_, const TData& Data_) :
				_FileName(FileName_), _Data(Data_)
			{
			}
			virtual ~COption() {}
			TData& operator*(void)
			{
				return _Data;
			}
			const TData& operator*(void) const
			{
				return _Data;
			}
			TData* operator->(void)
			{
				return &_Data;
			}
			void operator = (const TData& Data_)
			{
				_Data = Data_;
				Save();
			}
			void Reset(void)
			{
				new (&_Data) TData{};
			}
			void Clear(void)
			{
				Reset();
				Save();
			}
			virtual void Save(void) = 0;
		};
	}
}
