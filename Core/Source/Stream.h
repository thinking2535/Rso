#pragma once

#include "DateTime.h"
#include <fstream>
#include <array>
#include <list>
#include <map>
#include <set>
#include <type_traits>

namespace rso::core
{
	using namespace std;

	using TStreamSize = int32;

	class CStream
	{
		template <typename TType, bool IsEnum_>
		struct _SOperator
		{
			static CStream& Push(CStream& Stream_, const SProto& Data_)
			{
				return (Stream_ << Data_);
			}
			static CStream& Pop(CStream& Stream_, SProto& Data_)
			{
				return (Stream_ >> Data_);
			}
		};

		template <typename TType>
		struct _SOperator<TType, true>
		{
			static CStream& Push(CStream& Stream_, const TType& Data_)
			{
				return (Stream_ << (typename std::underlying_type<TType>::type)Data_);
			}
			static CStream& Pop(CStream& Stream_, TType& Data_)
			{
				return (Stream_ >> (typename std::underlying_type<TType>::type&)Data_);
			}
		};

		size_t _Head = 0;
		size_t _Tail = 0;
		size_t _SavedHead = 0;
		size_t _SavedTail = 0;
		vector<char> _Bytes{ 0 };

	public:
		CStream()
		{
		}
		CStream(const SProto& Proto_)
		{
			operator<<(Proto_);
		}
		CStream(const CStream& Stream_)
		{
			_Head = Stream_._Head;
			_Tail = Stream_._Tail;
			_Bytes = Stream_._Bytes;
		}
		CStream(CStream&& Stream_)
		{
			_Head = Stream_._Head;
			_Tail = Stream_._Tail;
			_Bytes = std::move(Stream_._Bytes);

			Stream_.clear();
		}
		CStream(const wstring& FileName_)
		{
			LoadFile(FileName_);
		}
		CStream(const char* pData_, size_t Length_)
		{
			Push(pData_, Length_);
		}
		bool operator==(const CStream& Stream_) const;
		bool operator!=(const CStream& Stream_) const { return (!(*this == Stream_)); }
		CStream& operator=(const CStream& Stream_)
		{
			new (this) CStream(Stream_);
			return *this;
		}
		CStream& operator=(CStream&& Stream_)
		{
			new (this) CStream(std::move(Stream_));
			return *this;
		}

	private:
		template <typename _TType>
		void _Pop(_TType& Value_)
		{
			if (size() < sizeof(_TType))
				THROWEX();

			Value_ = *(_TType*)(_Bytes.data() + _Head);
			PopSize(sizeof(_TType));
		}
		template <typename _TType>
		inline void _Push(const _TType& Value_)
		{
			Push((const char*)&Value_, sizeof(_TType));
		}

	public:
		template <typename _TType>
		_TType get_pop(void)
		{
			_TType value;
			operator>>(value);
			return value;
		}

		CStream& operator>>(bool& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(int8& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(uint8& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(int16& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(uint16& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(int32& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(uint32& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(int64& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(uint64& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(long& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(unsigned long& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(float& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(double& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		CStream& operator>>(long double& Value_)
		{
			_Pop(Value_);
			return *this;
		}
		template<typename TCharType>
		CStream& operator>>(basic_string<TCharType>& Value_)
		{
			int32 Size = 0;
			const TCharType* pData = nullptr;

			Pop(Size, pData);
			Value_.assign(pData, Size);
			return *this;
		}
		CStream& operator>>(system_clock::time_point& Value_);
		CStream& operator>>(microseconds& Value_);
		CStream& operator>>(milliseconds& Value_);
		CStream& operator>>(seconds& Value_);
		CStream& operator>>(minutes& Value_);
		CStream& operator>>(hours& Value_);
		CStream& operator>>(SDateTime& Value_);
		CStream& operator>>(CStream& Stream_);
		CStream& operator>>(SProto& Proto_o)
		{
			Proto_o << *this;
			return *this;
		}
		template <typename TValue, size_t Size>
		CStream& operator>>(array<TValue, Size>& Value_)
		{
			for (auto& i : Value_)
				i = get_pop<TValue>();

			return *this;
		}
		template <typename TValue>
		CStream& operator>>(list<TValue>& Value_)
		{
			Value_.clear();
			auto cnt = get_pop<int32>();
			for (int32 i = 0; i < cnt; ++i)
				Value_.emplace_back(get_pop<TValue>());

			return *this;
		}
		template <typename TValue>
		CStream& operator>>(vector<TValue>& Value_)
		{
			Value_.clear();
			auto cnt = get_pop<int32>();
			Value_.reserve(cnt);
			for (int32 i = 0; i < cnt; ++i)
				Value_.emplace_back(get_pop<TValue>());

			return *this;
		}
		template <typename TKey, typename TValue>
		CStream& operator>>(map<TKey, TValue>& Value_)
		{
			Value_.clear();
			auto cnt = get_pop<int32>();
			for (int32 i = 0; i < cnt; ++i)
				Value_.emplace(get_pop<map<TKey, TValue>::value_type>());

			return *this;
		}
		template <typename TKey, typename TValue>
		CStream& operator>>(multimap<TKey, TValue>& Value_)
		{
			Value_.clear();
			auto cnt = get_pop<int32>();
			for (int32 i = 0; i < cnt; ++i)
				Value_.emplace(get_pop<multimap<TKey, TValue>::value_type>());

			return *this;
		}
		template <typename TKey>
		CStream& operator>>(set<TKey>& Value_)
		{
			Value_.clear();
			auto cnt = get_pop<int32>();
			for (int32 i = 0; i < cnt; ++i)
				Value_.emplace(get_pop<TKey>());

			return *this;
		}
		template <typename TKey>
		CStream& operator>>(multiset<TKey>& Value_)
		{
			Value_.clear();
			auto cnt = get_pop<int32>();
			for (int32 i = 0; i < cnt; ++i)
				Value_.emplace(get_pop<TKey>());

			return *this;
		}
		template <typename TFirst, typename TSecond>
		CStream& operator>>(pair<TFirst, TSecond>& Pair_)
		{
			operator>>((typename remove_const<TFirst>::type&)Pair_.first);
			operator>>(Pair_.second);

			return *this;
		}
		CStream& operator<<(bool Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(int8 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(uint8 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(int16 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(uint16 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(int32 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(uint32 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(int64 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(uint64 Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(long Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(unsigned long Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(float Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(double Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(long double Value_)
		{
			_Push(Value_);
			return *this;
		}
		CStream& operator<<(void* Value_);
		template<typename TCharType>
		CStream& operator<<(const basic_string<TCharType>& Value_)
		{
			Push(int32(Value_.size()), Value_.data());
			return *this;
		}
		CStream& operator<<(const system_clock::time_point& Value_)
		{
			operator<<(Value_.time_since_epoch().count());
			return *this;
		}
		CStream& operator<<(const microseconds& Value_)
		{
			operator<<(Value_.count() * 10);
			return *this;
		}
		CStream& operator<<(const milliseconds& Value_)
		{
			operator<<(Value_.count() * 10000);
			return *this;
		}
		CStream& operator<<(const seconds& Value_)
		{
			operator<<(Value_.count() * 10000000);
			return *this;
		}
		CStream& operator<<(const minutes& Value_)
		{
			operator<<(Value_.count() * 600000000);
			return *this;
		}
		CStream& operator<<(const hours& Value_)
		{
			operator<<(Value_.count() * 36000000000);
			return *this;
		}
		CStream& operator<<(const SDateTime& Value_);
		CStream& operator<<(const char* Value_)
		{
			Push(Value_, std::strlen(Value_) + 1);
			return *this;
		}
		CStream& operator<<(const wchar_t* Value_)
		{
			Push((const char*)Value_, (std::wcslen(Value_) + 1) * 2);
			return *this;
		}
		CStream& operator<<(const CStream& Stream_);
		CStream& operator<<(const SProto& Proto_)
		{
			Proto_ >> *this;
			return *this;
		}
		template <typename TValue, size_t Size>
		CStream& operator<<(const array<TValue, Size>& Value_)
		{
			for (auto& i : Value_)
				operator<<(i);

			return *this;
		}
		template <typename TValue>
		CStream& operator<<(const list<TValue>& Value_)
		{
			_Push(int32(Value_.size()));
			for (auto& i : Value_)
				operator<<(i);

			return *this;
		}
		template <typename TValue>
		CStream& operator<<(const vector<TValue>& Value_)
		{
			_Push(int32(Value_.size()));
			for (auto& i : Value_)
				operator<<(i);

			return *this;
		}
		template <typename TKey, typename TValue>
		CStream& operator<<(const map<TKey, TValue>& Value_)
		{
			_Push(int32(Value_.size()));
			for (auto& i : Value_)
				operator<<(i);

			return *this;
		}
		template <typename TKey, typename TValue>
		CStream& operator<<(const multimap<TKey, TValue>& Value_)
		{
			_Push(int32(Value_.size()));
			for (auto& i : Value_)
				operator<<(i);

			return *this;
		}
		template <typename TKey>
		CStream& operator<<(const set<TKey>& Value_)
		{
			_Push(int32(Value_.size()));
			for (auto& i : Value_)
				operator<<(i);

			return *this;
		}
		template <typename TKey>
		CStream& operator<<(const multiset<TKey>& Value_)
		{
			_Push(int32(Value_.size()));
			for (auto& i : Value_)
				operator<<(i);

			return *this;
		}
		template <typename TFirst, typename TSecond>
		CStream& operator<<(const pair<TFirst, TSecond>& Pair_)
		{
			operator<<(Pair_.first);
			operator<<(Pair_.second);

			return *this;
		}
		template <typename TType>
		CStream& operator<<(const TType& Data_)
		{
			return _SOperator<TType, is_enum<TType>::value>::Push(*this, Data_);
		}
		template <typename TType>
		CStream& operator>>(TType& Data_)
		{
			return _SOperator<TType, is_enum<TType>::value>::Pop(*this, Data_);
		}

		template <typename TType>
		CStream& Send(TType* Arg_) = delete;
		template <typename TType>
		inline CStream& Send(const TType& Arg_) { return (operator<<(Arg_)); }
		template <typename TType, typename... TTypes>
		CStream& Send(const TType& Arg_, const TTypes &...Args_)
		{
			Send(Arg_);
			return Send(Args_...);
		}

	private:
		inline void _PushSize(size_t Length_) { _Tail += Length_; }

	public:
		void PopSize(size_t Length_);
		void Push(const char* pData_, size_t Length_);	// pData_ 만 Push
		template<typename TType>
		void Push(int32 Length_, const TType* pData_) // Length_ Push, pData_ Push
		{
			Reserve(_Tail + sizeof(Length_) + (Length_ * sizeof(TType)));
			memcpy(_Bytes.data() + _Tail, &Length_, sizeof(Length_));
			memcpy(_Bytes.data() + _Tail + sizeof(Length_), pData_, (Length_ * sizeof(TType)));
			_PushSize(sizeof(Length_) + (Length_ * sizeof(TType)));
		}
		template<typename TType>
		void Pop(int32& Length_, const TType*& pData_)
		{
			if (size() < sizeof(Length_))
				THROWEX();

			Length_ = *(int32*)(_Bytes.data() + _Head);

			if (Length_ > 0)
			{
				if (size() < sizeof(Length_) + Length_)
					THROWEX();

				pData_ = (const TType*)(_Bytes.data() + _Head + sizeof(Length_));
			}
			else
			{
				pData_ = (const TType*)_Bytes.data(); // nullptr 을 반환할 경우 MSSQL Type의 CTypeBinary 에서 문자열이 공백인 경우 null을 반환하면 null 이 입력되어 문제가 되므로 유효한 주소를 반환하여 공백으로 처리되도록
			}

			PopSize(sizeof(Length_) + (Length_ * sizeof(TType)));
		}
		inline void SetHead(size_t Head_) { _Head = Head_; }
		inline void SetTail(size_t Tail_) { _Tail = Tail_; }
		inline bool empty(void) const { return (size() == 0); }
		inline size_t size(void) const { return (_Tail - _Head); }
		inline char* at(size_t Index_) { return _Bytes.data() + Index_; }
		void LoadState(void)
		{
			_Head = _SavedHead;
			_Tail = _SavedTail;
		}
		void SaveState(void)
		{
			_SavedHead = _Head;
			_SavedTail = _Tail;
		}
		inline void clear(void) { _Head = _Tail = 0; }
		inline size_t Capacity(void) const { return _Bytes.size(); }
		void Reserve(size_t Size_);
		inline char* data(void) { return _Bytes.data(); }
		inline const char* data(void) const { return _Bytes.data(); }
		inline char* buff(void) { return _Bytes.data() + _Head; }
		inline const char* buff(void) const { return _Bytes.data() + _Head; }
		inline string to_string(void) const { return string(buff(), size()); }
		inline TCoreCheckSum checksum(void) const { return GetCheckSum(buff(), size()); }
		void LoadFile(const wstring& FileName_);
		void SaveFile(const wstring& FileName_);
	};

	template <typename TType, bool IsEnum_>
	struct SEnumChecker;

	// for GetStdName /////////////////////////////////////////////////////////////////////////
	template<typename TType>
	inline wstring GetStdName(const TType&)
	{
		return SEnumChecker<TType, is_enum<TType>::value>::StdName();
	}
	template<> inline wstring GetStdName(const bool&) { return L"bool"; }
	template<> inline wstring GetStdName(const int8&) { return L"int8"; }
	template<> inline wstring GetStdName(const uint8&) { return L"uint8"; }
	template<> inline wstring GetStdName(const int16&) { return L"int16"; }
	template<> inline wstring GetStdName(const uint16&) { return L"uint16"; }
	template<> inline wstring GetStdName(const int32&) { return L"int32"; }
	template<> inline wstring GetStdName(const uint32&) { return L"uint32"; }
	template<> inline wstring GetStdName(const int64&) { return L"int64"; }
	template<> inline wstring GetStdName(const uint64&) { return L"uint64"; }
	template<> inline wstring GetStdName(const float&) { return L"float"; }
	template<> inline wstring GetStdName(const double&) { return L"double"; }
	template<> inline wstring GetStdName(const string&) { return L"string"; }
	template<> inline wstring GetStdName(const wstring&) { return L"wstring"; }
	template<> inline wstring GetStdName(const u16string&) { return L"u16string"; }
	template<> inline wstring GetStdName(const u32string&) { return L"u32string"; }
	template<> inline wstring GetStdName(const system_clock::time_point&) { return L"time_point"; }
	template<> inline wstring GetStdName(const microseconds&) { return L"microseconds"; }
	template<> inline wstring GetStdName(const milliseconds&) { return L"milliseconds"; }
	template<> inline wstring GetStdName(const seconds&) { return L"seconds"; }
	template<> inline wstring GetStdName(const minutes&) { return L"minutes"; }
	template<> inline wstring GetStdName(const hours&) { return L"hours"; }
	template<> inline wstring GetStdName(const SDateTime&) { return L"datetime"; }
	template<> inline wstring GetStdName(const CStream&) { return L"stream"; }
	template<typename TValue, size_t Size> inline wstring GetStdName(const array<TValue, Size>&)
	{
		wstring Name;
		Name += GetStdName(TValue());
		for (size_t i = 1; i < Size; ++i)
			Name += (L"," + GetStdName(TValue()));
		return Name;
	}
	template<typename TValue> inline wstring GetStdName(const list<TValue>&) { return L'{' + GetStdName(TValue()) + L'}'; }
	template<typename TValue> inline wstring GetStdName(const vector<TValue>&) { return L'{' + GetStdName(TValue()) + L'}'; }
	template<typename TKey> inline wstring GetStdName(const set<TKey>&) { return L'{' + GetStdName(TKey()) + L'}'; }
	template<typename TKey, typename TValue> inline wstring GetStdName(const map<TKey, TValue>&) { return L'{' + GetStdName(TKey()) + L',' + GetStdName(TValue()) + L'}'; }
	template<typename TKey> inline wstring GetStdName(const multiset<TKey>&) { return L'{' + GetStdName(TKey()) + L'}'; }
	template<typename TKey, typename TValue> inline wstring GetStdName(const multimap<TKey, TValue>&) { return L'{' + GetStdName(TKey()) + L',' + GetStdName(TValue()) + L'}'; }

	// for GetMemberName /////////////////////////////////////////////////////////////////////////
	template<typename TType>
	inline wstring GetMemberName(const TType&, const wstring& MemberName_)
	{
		return SEnumChecker<TType, is_enum<TType>::value>::MemberName(MemberName_);
	}
	template<> inline wstring GetMemberName(const bool&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const int8&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const uint8&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const int16&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const uint16&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const int32&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const uint32&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const int64&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const uint64&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const float&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const double&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const string&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const wstring&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const u16string&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const u32string&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const system_clock::time_point&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const microseconds&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const milliseconds&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const seconds&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const minutes&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const hours&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const SDateTime&, const wstring& MemberName_) { return MemberName_; }
	template<> inline wstring GetMemberName(const CStream&, const wstring& MemberName_) { return MemberName_; }
	template<typename TValue, size_t Size> inline wstring GetMemberName(const array<TValue, Size>&, const wstring& MemberName_) { return MemberName_; }
	template<typename TValue> inline wstring GetMemberName(const list<TValue>&, const wstring& MemberName_) { return MemberName_; }
	template<typename TValue> inline wstring GetMemberName(const vector<TValue>&, const wstring& MemberName_) { return MemberName_; }
	template<typename TKey> inline wstring GetMemberName(const set<TKey>&, const wstring& MemberName_) { return MemberName_; }
	template<typename TKey, typename TValue> inline wstring GetMemberName(const map<TKey, TValue>&, const wstring& MemberName_) { return MemberName_; }
	template<typename TKey> inline wstring GetMemberName(const multiset<TKey>&, const wstring& MemberName_) { return MemberName_; }
	template<typename TKey, typename TValue> inline wstring GetMemberName(const multimap<TKey, TValue>&, const wstring& MemberName_) { return MemberName_; }
	////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename TType, bool IsEnum_>
	struct SEnumChecker
	{
		static inline wstring StdName(void)
		{
			return TType::StdName();
		}
		static inline wstring MemberName(const wstring&)
		{
			return TType::MemberName();
		}
	};
	template <typename TType>
	struct SEnumChecker<TType, true>
	{
		static inline wstring StdName(void)
		{
			return GetStdName(typename underlying_type<TType>::type());
		}
		static inline wstring MemberName(const wstring& MemberName_)
		{
			return GetMemberName(typename underlying_type<TType>::type(), MemberName_);
		}
	};
}
