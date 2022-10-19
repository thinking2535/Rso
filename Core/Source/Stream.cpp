#include "Stream.h"
#include <filesystem>

namespace rso::core
{
	using namespace filesystem;

	bool CStream::operator==(const CStream& Stream_) const
	{
		if (size() != Stream_.size())
			return false;

		return (memcmp(buff(), Stream_.buff(), size()) == 0);
	}
	CStream& CStream::operator>>(system_clock::time_point& Value_)
	{
		int64 Ticks = 0;
		_Pop(Ticks);
		Value_ = system_clock::time_point(t_duration(Ticks));

		return *this;
	}
	CStream& CStream::operator>>(microseconds& Value_)
	{
		int64 Ticks = 0;
		_Pop(Ticks);
		Value_ = microseconds(Ticks / 10);

		return *this;
	}
	CStream& CStream::operator>>(milliseconds& Value_)
	{
		int64 Ticks = 0;
		_Pop(Ticks);
		Value_ = milliseconds(Ticks / 10000);

		return *this;
	}
	CStream& CStream::operator>>(seconds& Value_)
	{
		int64 Ticks = 0;
		_Pop(Ticks);
		Value_ = seconds(Ticks / 10000000);

		return *this;
	}
	CStream& CStream::operator>>(minutes& Value_)
	{
		int64 Ticks = 0;
		_Pop(Ticks);
		Value_ = minutes(Ticks / 600000000);

		return *this;
	}
	CStream& CStream::operator>>(hours& Value_)
	{
		int64 Ticks = 0;
		_Pop(Ticks);
		Value_ = hours(Ticks / 36000000000);

		return *this;
	}
	CStream& CStream::operator>>(SDateTime& Value_)
	{
		if (size() < SDateTime::c_StreamSize)
			THROWEX();

		Value_.year = *(decltype(Value_.year)*)(_Bytes.data() + _Head + 0);
		Value_.month = *(decltype(Value_.month)*)(_Bytes.data() + _Head + 2);
		Value_.day = *(decltype(Value_.day)*)(_Bytes.data() + _Head + 4);
		Value_.hour = *(decltype(Value_.hour)*)(_Bytes.data() + _Head + 6);
		Value_.minute = *(decltype(Value_.minute)*)(_Bytes.data() + _Head + 8);
		Value_.second = *(decltype(Value_.second)*)(_Bytes.data() + _Head + 10);
		Value_.fraction = *(decltype(Value_.fraction)*)(_Bytes.data() + _Head + 12);
		PopSize(SDateTime::c_StreamSize);

		return *this;
	}
	CStream& CStream::operator>>(CStream& Stream_)
	{
		int32 Size = 0;
		const char* pData = nullptr;

		Pop(Size, pData);
		Stream_.Push(pData, Size);

		return *this;
	}
	CStream& CStream::operator<<(void* Value_)
	{
		_Push(Value_);
		return *this;
	}
	CStream& CStream::operator<<(const SDateTime& Value_)
	{
		Reserve(_Tail + SDateTime::c_StreamSize);
		memcpy(_Bytes.data() + (_Tail + 0), &Value_.year, 2);
		memcpy(_Bytes.data() + (_Tail + 2), &Value_.month, 2);
		memcpy(_Bytes.data() + (_Tail + 4), &Value_.day, 2);
		memcpy(_Bytes.data() + (_Tail + 6), &Value_.hour, 2);
		memcpy(_Bytes.data() + (_Tail + 8), &Value_.minute, 2);
		memcpy(_Bytes.data() + (_Tail + 10), &Value_.second, 2);
		memcpy(_Bytes.data() + (_Tail + 12), &Value_.fraction, 4);
		_PushSize(SDateTime::c_StreamSize);
		return *this;
	}
	CStream& CStream::operator<<(const CStream& Stream_)
	{
		Push(int32(Stream_.size()), Stream_.buff());
		return *this;
	}

	// 속도가 더 느림
	//bool CStream::Push(const char* pData_, size_t Length_)
	//{
	// if (Length_ <= 0)
	// return true;

	// try
	// {
	// auto AvailableSize = _Bytes.size() - _Tail;
	// if (AvailableSize >= Length_)
	// {
	// memcpy(_Bytes.data() + _Tail, pData_, Length_);
	// }
	// else
	// {
	// if (_Bytes.capacity() < _Tail + Length_)
	// _Bytes.reserve((_Tail + Length_) * 2);

	// if (AvailableSize > 0)
	// memcpy(_Bytes.data() + _Tail, pData_, AvailableSize);

	// _Bytes.insert(_Bytes.end(), &pData_[AvailableSize], &pData_[Length_]);
	// }

	// _Tail += Length_;

	// return true;
	// }
	// catch (...)
	// {
	// return false;
	// }
	//}
	void CStream::Push(const char* pData_, size_t Length_)
	{
		if (Length_ <= 0)
			return;

		Reserve(_Tail + Length_);
		memcpy(_Bytes.data() + _Tail, pData_, Length_);
		_PushSize(Length_);
	}
	void CStream::PopSize(size_t Length_)
	{
		if (size() > Length_)
			_Head += Length_;
		else
			clear();
	}
	void CStream::Reserve(size_t Size_)
	{
		if (_Bytes.size() >= Size_)
			return;

		// 초기화를 하더라도 디버그 모드에서 reserve 를 쓰는것 보다 속도가 더 빠르므로 그냥 사용
		_Bytes.resize(Size_);
	}
	void CStream::LoadFile(const wstring& FileName_)
	{
		ifstream f(absolute(FileName_), ios::binary);
		if (!f.is_open())
			THROWEXA(L"File Open Fail [%s]", FileName_);

		std::streampos begin;
		std::streamoff Size;

		f.seekg(0, ios::beg);
		begin = f.tellg();
		f.seekg(0, ios::end);
		Size = (f.tellg() - begin);
		f.seekg(0, ios::beg);

		Reserve(size_t(Size));
		clear();
		f.read(_Bytes.data(), Size);
		_PushSize((size_t)Size);
	}
	void CStream::SaveFile(const wstring& FileName_)
	{
		auto FullPath = absolute(FileName_);
		create_directories(FullPath.parent_path());

		ofstream f(FullPath, ios::binary);
		if (!f.is_open())
			THROWEX();

		f.write(buff(), size());
	}
}
