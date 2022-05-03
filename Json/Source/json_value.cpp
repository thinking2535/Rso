// Copyright 2011 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#if !defined(JSON_IS_AMALGAMATION)
#include "assertions.h"
#include "value.h"
#include "writer.h"
# ifndef JSON_USE_SIMPLE_INTERNAL_ALLOCATOR
#  include "json_batchallocator.h"
# endif // #ifndef JSON_USE_SIMPLE_INTERNAL_ALLOCATOR
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <math.h>
#include <sstream>
#include <utility>
#include <stdexcept>
#include <cstring>
#include <cassert>
#include <cstddef>    // size_t

#define JSON_ASSERT_UNREACHABLE assert( false )

namespace rso
{

	namespace json {

		const Value Value::null;
		const Int Value::minInt = Int(~(UInt(-1) / 2));
		const Int Value::maxInt = Int(UInt(-1) / 2);
		const UInt Value::maxUInt = UInt(-1);
# if defined(JSON_HAS_INT64)
		const Int64 Value::minInt64 = Int64(~(UInt64(-1) / 2));
		const Int64 Value::maxInt64 = Int64(UInt64(-1) / 2);
		const UInt64 Value::maxUInt64 = UInt64(-1);
		// The constant is hard-coded because some compiler have trouble
		// converting Value::maxUInt64 to a double correctly (AIX/xlC).
		// Assumes that UInt64 is a 64 bits integer.
		static const double maxUInt64AsDouble = 18446744073709551615.0;
#endif // defined(JSON_HAS_INT64)
		const LargestInt Value::minLargestInt = LargestInt(~(LargestUInt(-1) / 2));
		const LargestInt Value::maxLargestInt = LargestInt(LargestUInt(-1) / 2);
		const LargestUInt Value::maxLargestUInt = LargestUInt(-1);


		/// Unknown size marker
		static const unsigned int unknown = (unsigned)-1;

#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
		template <typename T, typename U>
		static inline bool InRange(double d, T min, U max) {
			return d >= min && d <= max;
		}
#else // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
		static inline double integerToDouble(json::UInt64 value)
		{
			return static_cast<double>(Int64(value / 2)) * 2.0 + Int64(value & 1);
		}

		template<typename T>
		static inline double integerToDouble(T value)
		{
			return static_cast<double>(value);
		}

		template <typename T, typename U>
		static inline bool InRange(double d, T min, U max) {
			return d >= integerToDouble(min) && d <= integerToDouble(max);
		}
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)


		/** Duplicates the specified string value.
		 * @param value Pointer to the string to duplicate. Must be zero-terminated if
		 *              length is "unknown".
		 * @param length Length of the value. if equals to unknown, then it will be
		 *               computed using strlen(value).
		 * @return Pointer on the duplicate instance of string.
		 */
		static inline char *
			duplicateStringValue(const char *value,
				unsigned int length = unknown)
		{
			if (length == unknown)
				length = (unsigned int)strlen(value);

			// Avoid an integer overflow in the call to malloc below by limiting length
			// to a sane value.
			if (length >= (unsigned)Value::maxInt)
				length = Value::maxInt - 1;

			char *newString = static_cast<char *>(malloc(length + 1));
			JSON_ASSERT_MESSAGE(newString != 0, "Failed to allocate string value buffer");
			memcpy(newString, value, length);
			newString[length] = 0;
			return newString;
		}


		/** Free the string duplicated by duplicateStringValue().
		 */
		static inline void
			releaseStringValue(char *value)
		{
			if (value)
				free(value);
		}

	} // namespace json


	// //////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////
	// ValueInternals...
	// //////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////

#if !defined(JSON_IS_AMALGAMATION)
#include "json_valueiterator.inl"
#endif // if !defined(JSON_IS_AMALGAMATION)

	namespace json {

		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////
		// class Value::CZString
		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////

// Notes: index_ indicates if the string was allocated when
// a string is stored.

		Value::CZString::CZString(ArrayIndex index)
			: cstr_(0)
			, index_(index)
		{
		}

		Value::CZString::CZString(const char *cstr, DuplicationPolicy allocate)
			: cstr_(allocate == duplicate ? duplicateStringValue(cstr)
				: cstr)
			, index_(allocate)
		{
		}

		Value::CZString::CZString(const CZString &other)
			: cstr_(other.index_ != noDuplication &&  other.cstr_ != 0
				? duplicateStringValue(other.cstr_)
				: other.cstr_)
			, index_(other.cstr_ ? (other.index_ == noDuplication ? noDuplication : duplicate)
				: other.index_)
		{
		}

		Value::CZString::~CZString()
		{
			if (cstr_  &&  index_ == duplicate)
				releaseStringValue(const_cast<char *>(cstr_));
		}

		void
			Value::CZString::swap(CZString &other)
		{
			std::swap(cstr_, other.cstr_);
			std::swap(index_, other.index_);
		}

		Value::CZString &
			Value::CZString::operator =(const CZString &other)
		{
			CZString temp(other);
			swap(temp);
			return *this;
		}

		bool
			Value::CZString::operator<(const CZString &other) const
		{
			if (cstr_)
				return strcmp(cstr_, other.cstr_) < 0;
			return index_ < other.index_;
		}

		bool
			Value::CZString::operator==(const CZString &other) const
		{
			if (cstr_)
				return strcmp(cstr_, other.cstr_) == 0;
			return index_ == other.index_;
		}


		ArrayIndex
			Value::CZString::index() const
		{
			return index_;
		}


		const char *
			Value::CZString::c_str() const
		{
			return cstr_;
		}

		bool
			Value::CZString::isStaticString() const
		{
			return index_ == noDuplication;
		}


		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////
		// class Value::Value
		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////
		// //////////////////////////////////////////////////////////////////

		/*! \internal Default constructor initialization must be equivalent to:
		 * memset( this, 0, sizeof(Value) )
		 * This optimization is used in ValueInternalMap fast allocator.
		 */
		Value::Value(ValueType type)
			: type_(type)
		{
		}
		Value::Value(UInt value) :
			type_(uintValue),
			uint_(value)
		{
		}
		Value::Value(int8 value) :
			Value((int32)value)
		{
		}
		Value::Value(uint8 value) :
			Value((uint32)value)
		{
		}
		Value::Value(int16 value) :
			Value((int32)value)
		{
		}
		Value::Value(uint16 value) :
			Value((uint32)value)
		{
		}
		Value::Value(Int value) :
			type_(intValue),
			int_(value)
		{
		}
# if defined(JSON_HAS_INT64)
		Value::Value(Int64 value) :
			type_(intValue),
			int_(value)
		{
		}
		Value::Value(UInt64 value) :
			type_(uintValue),
			uint_(value)
		{
		}
#endif // defined(JSON_HAS_INT64)
		Value::Value(float value) :
			type_(realValue),
			real_(value)
		{
		}
		Value::Value(double value) :
			type_(realValue),
			real_(value)
		{
		}
		Value::Value(const char *value) :
			type_(stringValue),
			string_(value)
		{
		}
		Value::Value(const char *beginValue, const char *endValue) :
			type_(stringValue),
			string_(beginValue, endValue)
		{
		}
		Value::Value(const string &value) :
			type_(stringValue),
			string_(value)
		{
		}
		Value::Value(const u16string& value) :
			type_(stringValue),
			string_(u16string_to_u8string(value))
		{
		}
		Value::Value(const u32string& value) :
			type_(stringValue),
			string_(u32string_to_u8string(value))
		{
		}
		Value::Value(const wstring& value) :
			Value(WCSToMBS(value))
		{
		}
		Value::Value(const StaticString &value) :
			type_(stringValue),
			string_(value.c_str())
		{
		}
		Value::Value(bool value) :
			type_(booleanValue),
			bool_(value)
		{
		}
		Value::Value(const TTime& value) :
			Value(value.time_since_epoch().count())
		{
		}
		Value::Value(const SDateTime& value) :
			Value(CDateTime(value).ToTimePoint().time_since_epoch().count())
		{
		}
		Value::Value(const CStream& /*value*/) :
			Value(string())
		{
		}
		Value::Value(const SProto& value) :
			Value(objectValue)
		{
			value >> *this;
		}

		ValueType
			Value::type() const
		{
			return type_;
		}


		int
			Value::compare(const Value &other) const
		{
			if (*this < other)
				return -1;
			if (*this > other)
				return 1;
			return 0;
		}


		bool Value::operator <(const Value &other) const
		{
			int typeDelta = type_ - other.type_;
			if (typeDelta)
				return typeDelta < 0 ? true : false;
			switch (type_)
			{
			case nullValue:
				return false;
			case intValue:
				return int_ < other.int_;
			case uintValue:
				return uint_ < other.uint_;
			case realValue:
				return real_ < other.real_;
			case booleanValue:
				return bool_ < other.bool_;
			case stringValue:
				return string_ < other.string_;
			case arrayValue:
			case objectValue:
			{
				int delta = int(map_.size() - other.map_.size());
				if (delta)
					return delta < 0;
				return (map_) < (other.map_);
			}
			default:
				JSON_ASSERT_UNREACHABLE;
			}
			return false;  // unreachable
		}

		bool
			Value::operator <=(const Value &other) const
		{
			return !(other < *this);
		}

		bool
			Value::operator >=(const Value &other) const
		{
			return !(*this < other);
		}

		bool
			Value::operator >(const Value &other) const
		{
			return other < *this;
		}

		bool
			Value::operator ==(const Value &other) const
		{
			//if ( type_ != other.type_ )
			// GCC 2.95.3 says:
			// attempt to take address of bit-field structure member `json::Value::type_'
			// Beats me, but a temp solves the problem.
			int temp = other.type_;
			if (type_ != temp)
				return false;
			switch (type_)
			{
			case nullValue:
				return true;
			case intValue:
				return int_ == other.int_;
			case uintValue:
				return uint_ == other.uint_;
			case realValue:
				return real_ == other.real_;
			case booleanValue:
				return bool_ == other.bool_;
			case stringValue:
				return string_ == other.string_;
			case arrayValue:
			case objectValue:
				return map_.size() == other.map_.size()
					&& (map_) == (other.map_);
			default:
				JSON_ASSERT_UNREACHABLE;
			}
			return false;  // unreachable
		}

		bool
			Value::operator !=(const Value &other) const
		{
			return !(*this == other);
		}

		const char *
			Value::asCString() const
		{
			JSON_ASSERT(type_ == stringValue);
			return string_.c_str();
		}


		string
			Value::asString() const
		{
			switch (type_)
			{
			case nullValue:
				return "";
			case stringValue:
				return string_;
			case booleanValue:
				return bool_ ? "true" : "false";
			case intValue:
				return valueToString(int_);
			case uintValue:
				return valueToString(uint_);
			case realValue:
				return valueToString(real_);
			default:
				JSON_FAIL_MESSAGE("Type is not convertible to string");
			}
		}
		Value::Int
			Value::asInt() const
		{
			switch (type_)
			{
			case intValue:
				JSON_ASSERT_MESSAGE(isInt(), "LargestInt out of Int range");
				return Int(int_);
			case uintValue:
				JSON_ASSERT_MESSAGE(isInt(), "LargestUInt out of Int range");
				return Int(uint_);
			case realValue:
				JSON_ASSERT_MESSAGE(InRange(real_, minInt, maxInt), "double out of Int range");
				return Int(real_);
			case nullValue:
				return 0;
			case booleanValue:
				return bool_ ? 1 : 0;
			default:
				break;
			}
			JSON_FAIL_MESSAGE("Value is not convertible to Int.");
		}


		Value::UInt
			Value::asUInt() const
		{
			switch (type_)
			{
			case intValue:
				JSON_ASSERT_MESSAGE(isUInt(), "LargestInt out of UInt range");
				return UInt(int_);
			case uintValue:
				JSON_ASSERT_MESSAGE(isUInt(), "LargestUInt out of UInt range");
				return UInt(uint_);
			case realValue:
				JSON_ASSERT_MESSAGE(InRange(real_, 0, maxUInt), "double out of UInt range");
				return UInt(real_);
			case nullValue:
				return 0;
			case booleanValue:
				return bool_ ? 1 : 0;
			default:
				break;
			}
			JSON_FAIL_MESSAGE("Value is not convertible to UInt.");
		}


# if defined(JSON_HAS_INT64)

		Value::Int64
			Value::asInt64() const
		{
			switch (type_)
			{
			case intValue:
				return Int64(int_);
			case uintValue:
				JSON_ASSERT_MESSAGE(isInt64(), "LargestUInt out of Int64 range");
				return Int64(uint_);
			case realValue:
				JSON_ASSERT_MESSAGE(InRange(real_, minInt64, maxInt64), "double out of Int64 range");
				return Int64(real_);
			case nullValue:
				return 0;
			case booleanValue:
				return bool_ ? 1 : 0;
			default:
				break;
			}
			JSON_FAIL_MESSAGE("Value is not convertible to Int64.");
		}


		Value::UInt64
			Value::asUInt64() const
		{
			switch (type_)
			{
			case intValue:
				JSON_ASSERT_MESSAGE(isUInt64(), "LargestInt out of UInt64 range");
				return UInt64(int_);
			case uintValue:
				return UInt64(uint_);
			case realValue:
				JSON_ASSERT_MESSAGE(InRange(real_, 0, maxUInt64), "double out of UInt64 range");
				return UInt64(real_);
			case nullValue:
				return 0;
			case booleanValue:
				return bool_ ? 1 : 0;
			default:
				break;
			}
			JSON_FAIL_MESSAGE("Value is not convertible to UInt64.");
		}
# endif // if defined(JSON_HAS_INT64)


		LargestInt
			Value::asLargestInt() const
		{
#if defined(JSON_NO_INT64)
			return asInt();
#else
			return asInt64();
#endif
		}


		LargestUInt
			Value::asLargestUInt() const
		{
#if defined(JSON_NO_INT64)
			return asUInt();
#else
			return asUInt64();
#endif
		}


		double
			Value::asDouble() const
		{
			switch (type_)
			{
			case intValue:
				return static_cast<double>(int_);
			case uintValue:
#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
				return static_cast<double>(uint_);
#else // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
				return integerToDouble(uint_);
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
			case realValue:
				return real_;
			case nullValue:
				return 0.0;
			case booleanValue:
				return bool_ ? 1.0 : 0.0;
			default:
				break;
			}
			JSON_FAIL_MESSAGE("Value is not convertible to double.");
		}

		float
			Value::asFloat() const
		{
			switch (type_)
			{
			case intValue:
				return static_cast<float>(int_);
			case uintValue:
#if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
				return static_cast<float>(uint_);
#else // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
				return integerToDouble(uint_);
#endif // if !defined(JSON_USE_INT64_DOUBLE_CONVERSION)
			case realValue:
				return static_cast<float>(real_);
			case nullValue:
				return 0.0;
			case booleanValue:
				return bool_ ? 1.0f : 0.0f;
			default:
				break;
			}
			JSON_FAIL_MESSAGE("Value is not convertible to float.");
		}

		bool
			Value::asBool() const
		{
			switch (type_)
			{
			case booleanValue:
				return bool_;
			case nullValue:
				return false;
			case intValue:
				return int_ ? true : false;
			case uintValue:
				return uint_ ? true : false;
			case realValue:
				return real_ ? true : false;
			default:
				break;
			}
			JSON_FAIL_MESSAGE("Value is not convertible to bool.");
		}

		bool
			Value::isConvertibleTo(ValueType other) const
		{
			switch (other)
			{
			case nullValue:
				return (isNumeric() && asDouble() == 0.0)
					|| (type_ == booleanValue && bool_ == false)
					|| (type_ == stringValue && asString() == "")
					|| (type_ == arrayValue && map_.size() == 0)
					|| (type_ == objectValue && map_.size() == 0)
					|| type_ == nullValue;
			case intValue:
				return isInt()
					|| (type_ == realValue && InRange(real_, minInt, maxInt))
					|| type_ == booleanValue
					|| type_ == nullValue;
			case uintValue:
				return isUInt()
					|| (type_ == realValue && InRange(real_, 0, maxUInt))
					|| type_ == booleanValue
					|| type_ == nullValue;
			case realValue:
				return isNumeric()
					|| type_ == booleanValue
					|| type_ == nullValue;
			case booleanValue:
				return isNumeric()
					|| type_ == booleanValue
					|| type_ == nullValue;
			case stringValue:
				return isNumeric()
					|| type_ == booleanValue
					|| type_ == stringValue
					|| type_ == nullValue;
			case arrayValue:
				return type_ == arrayValue
					|| type_ == nullValue;
			case objectValue:
				return type_ == objectValue
					|| type_ == nullValue;
			}
			JSON_ASSERT_UNREACHABLE;
			return false;
		}


		/// Number of values in array or object
		ArrayIndex
			Value::size() const
		{
			switch (type_)
			{
			case nullValue:
			case intValue:
			case uintValue:
			case realValue:
			case booleanValue:
			case stringValue:
				return 0;
			case arrayValue:  // size of the array is highest index + 1
				if (!map_.empty())
				{
					ObjectValues::const_iterator itLast = map_.end();
					--itLast;
					return (*itLast).first.index() + 1;
				}
				return 0;
			case objectValue:
				return ArrayIndex(map_.size());
			}
			JSON_ASSERT_UNREACHABLE;
			return 0; // unreachable;
		}


		bool
			Value::empty() const
		{
			if (isNull() || isArray() || isObject())
				return size() == 0u;
			else
				return false;
		}


		bool
			Value::operator!() const
		{
			return isNull();
		}


		void Value::clear()
		{
			JSON_ASSERT(type_ == nullValue || type_ == arrayValue || type_ == objectValue);

			switch (type_)
			{
			case arrayValue:
			case objectValue:
				list_.clear();
				map_.clear();
				break;
			default:
				break;
			}
		}

		void
			Value::resize(ArrayIndex newSize)
		{
			JSON_ASSERT(type_ == nullValue || type_ == arrayValue);
			if (type_ == nullValue)
				*this = Value(arrayValue);
			ArrayIndex oldSize = size();
			if (newSize == 0)
				clear();
			else if (newSize > oldSize)
				(*this)[newSize - 1];
			else
			{
				for (ArrayIndex index = newSize; index < oldSize; ++index)
				{
					map_.erase(index);
				}
				assert(size() == newSize);
			}
		}

		Value &
			Value::operator[](ArrayIndex index)
		{
			JSON_ASSERT(type_ == nullValue || type_ == arrayValue);
			if (type_ == nullValue)
				*this = Value(arrayValue);

			CZString key(index);
			ObjectValues::iterator it = map_.lower_bound(key);
			if (it != map_.end() && (*it).first == key)
				return (*it).second;

			ObjectValues::value_type defaultValue(key, null);
			it = map_.insert(it, defaultValue);
			try
			{
				list_.emplace_back(_SKeyValuePtr{ it->first, &it->second });
			}
			catch (...)
			{
				map_.erase(it);
				throw;
			}
			auto itListEnd = list_.end();
			--itListEnd;
			it->second.itList = itListEnd;

			return (*it).second;
		}


		Value &
			Value::operator[](int index)
		{
			JSON_ASSERT(index >= 0);
			return (*this)[ArrayIndex(index)];
		}


		const Value &
			Value::operator[](ArrayIndex index) const
		{
			JSON_ASSERT(type_ == arrayValue);

			CZString key(index);
			ObjectValues::const_iterator it = map_.find(key);
			if (it == map_.end())
				THROWEXA(L"Invalid Json Index[%d]", index);

			return (*it).second;
		}


		const Value &
			Value::operator[](int index) const
		{
			JSON_ASSERT(index >= 0);
			return (*this)[ArrayIndex(index)];
		}


		Value &
			Value::operator[](const char *key)
		{
			return resolveReference(key, false);
		}


		Value &
			Value::resolveReference(const char *key,
				bool isStatic)
		{
			JSON_ASSERT(type_ == nullValue || type_ == objectValue);
			if (type_ == nullValue)
				*this = Value(objectValue);

			CZString actualKey(key, isStatic ? CZString::noDuplication
				: CZString::duplicateOnCopy);
			ObjectValues::iterator it = map_.lower_bound(actualKey);
			if (it != map_.end() && (*it).first == actualKey)
				return (*it).second;

			ObjectValues::value_type defaultValue(actualKey, null);
			it = map_.insert(it, defaultValue);
			try
			{
				list_.emplace_back(_SKeyValuePtr{ it->first, &it->second });
			}
			catch (...)
			{
				map_.erase(it);
				throw;
			}
			auto itListEnd = list_.end();
			--itListEnd;
			it->second.itList = itListEnd;

			return (*it).second;
		}


		Value
			Value::get(ArrayIndex index,
				const Value &defaultValue) const
		{
			const Value *value = &((*this)[index]);
			return value == &null ? defaultValue : *value;
		}


		bool
			Value::isValidIndex(ArrayIndex index) const
		{
			return index < size();
		}



		const Value &
			Value::operator[](const char *key) const
		{
			JSON_ASSERT(type_ == objectValue);

			CZString actualKey(key, CZString::noDuplication);
			ObjectValues::const_iterator it = map_.find(actualKey);
			if (it == map_.end())
				THROWEXA(L"Invalid Json Key[%s]", MBSToWCS(string(key)));

			return (*it).second;
		}


		Value &
			Value::operator[](const string &key)
		{
			return (*this)[key.c_str()];
		}
		Value &
			Value::operator[](const wstring &key)
		{
			auto Key = WCSToMBS(key);
			return (*this)[Key.c_str()];
		}


		const Value &
			Value::operator[](const string &key) const
		{
			return (*this)[key.c_str()];
		}
		const Value &
			Value::operator[](const wstring &key) const
		{
			auto Key = WCSToMBS(key);
			return (*this)[Key.c_str()];
		}

		Value &
			Value::operator[](const StaticString &key)
		{
			return resolveReference(key, true);
		}

		Value
			Value::get(const char *key,
				const Value &defaultValue) const
		{
			const Value *value = &((*this)[key]);
			return value == &null ? defaultValue : *value;
		}


		Value
			Value::get(const string &key,
				const Value &defaultValue) const
		{
			return get(key.c_str(), defaultValue);
		}

		Value
			Value::removeMember(const char* key)
		{
			JSON_ASSERT(type_ == objectValue);

			CZString actualKey(key, CZString::noDuplication);
			ObjectValues::iterator it = map_.find(actualKey);
			if (it == map_.end())
				THROWEXA(L"Invalid Json Key[%s]", MBSToWCS(string(key)));

			Value old(it->second);
			list_.erase(it->second.itList);
			map_.erase(it);
			return old;
		}

		Value
			Value::removeMember(const string &key)
		{
			return removeMember(key.c_str());
		}
		bool
			Value::isMember(const char *key) const
		{
			const Value *value = &((*this)[key]);
			return value != &null;
		}


		bool
			Value::isMember(const string &key) const
		{
			return isMember(key.c_str());
		}

		Value::Members
			Value::getMemberNames() const
		{
			JSON_ASSERT(type_ == nullValue || type_ == objectValue);
			if (type_ == nullValue)
				return Value::Members();
			Members members;
			members.reserve(map_.size());

			ObjectListValues::const_iterator it = list_.begin();
			ObjectListValues::const_iterator itEnd = list_.end();
			for (; it != itEnd; ++it)
				members.push_back(string(it->key.c_str()));

			return members;
		}
		static bool IsIntegral(double d) {
			double integral_part;
			return modf(d, &integral_part) == 0.0;
		}


		bool
			Value::isNull() const
		{
			return type_ == nullValue;
		}


		bool
			Value::isBool() const
		{
			return type_ == booleanValue;
		}


		bool
			Value::isInt() const
		{
			switch (type_)
			{
			case intValue:
				return int_ >= minInt  &&  int_ <= maxInt;
			case uintValue:
				return uint_ <= UInt(maxInt);
			case realValue:
				return real_ >= minInt &&
					real_ <= maxInt &&
					IsIntegral(real_);
			default:
				break;
			}
			return false;
		}


		bool
			Value::isUInt() const
		{
			switch (type_)
			{
			case intValue:
				return int_ >= 0 && LargestUInt(int_) <= LargestUInt(maxUInt);
			case uintValue:
				return uint_ <= maxUInt;
			case realValue:
				return real_ >= 0 &&
					real_ <= maxUInt &&
					IsIntegral(real_);
			default:
				break;
			}
			return false;
		}

		bool
			Value::isInt64() const
		{
# if defined(JSON_HAS_INT64)
			switch (type_)
			{
			case intValue:
				return true;
			case uintValue:
				return uint_ <= UInt64(maxInt64);
			case realValue:
				// Note that maxInt64 (= 2^63 - 1) is not exactly representable as a
				// double, so double(maxInt64) will be rounded up to 2^63. Therefore we
				// require the value to be strictly less than the limit.
				return real_ >= double(minInt64) &&
					real_ < double(maxInt64) &&
					IsIntegral(real_);
			default:
				break;
			}
# endif  // JSON_HAS_INT64
			return false;
		}

		bool
			Value::isUInt64() const
		{
# if defined(JSON_HAS_INT64)
			switch (type_)
			{
			case intValue:
				return int_ >= 0;
			case uintValue:
				return true;
			case realValue:
				// Note that maxUInt64 (= 2^64 - 1) is not exactly representable as a
				// double, so double(maxUInt64) will be rounded up to 2^64. Therefore we
				// require the value to be strictly less than the limit.
				return real_ >= 0 &&
					real_ < maxUInt64AsDouble &&
					IsIntegral(real_);
			default:
				break;
			}
# endif  // JSON_HAS_INT64
			return false;
		}


		bool
			Value::isIntegral() const
		{
#if defined(JSON_HAS_INT64)
			return isInt64() || isUInt64();
#else
			return isInt() || isUInt();
#endif
		}


		bool
			Value::isDouble() const
		{
			return type_ == realValue || isIntegral();
		}


		bool
			Value::isNumeric() const
		{
			return isIntegral() || isDouble();
		}


		bool
			Value::isString() const
		{
			return type_ == stringValue;
		}


		bool
			Value::isArray() const
		{
			return type_ == arrayValue;
		}


		bool
			Value::isObject() const
		{
			return type_ == objectValue;
		}

		string
			Value::toStyledString() const
		{
			StyledWriter writer;
			return writer.write(*this);
		}
		wstring
			Value::toStyledWString() const
		{
			return MBSToWCS(toStyledString());
		}

		Value::const_iterator
			Value::begin() const
		{
			switch (type_)
			{
			case arrayValue:
			case objectValue:
				return const_iterator(((ObjectListValues&)list_).begin());
			default:
				break;
			}
			return const_iterator();
		}

		Value::const_iterator
			Value::end() const
		{
			switch (type_)
			{
			case arrayValue:
			case objectValue:
				return const_iterator(((ObjectListValues&)list_).end());
			default:
				break;
			}
			return const_iterator();
		}


		Value::iterator
			Value::begin()
		{
			switch (type_)
			{
			case arrayValue:
			case objectValue:
				return iterator(list_.begin());
			default:
				break;
			}
			return iterator();
		}

		Value::iterator
			Value::end()
		{
			switch (type_)
			{
			case arrayValue:
			case objectValue:
				return iterator(list_.end());
			default:
				break;
			}
			return iterator();
		}


		// class PathArgument
		// //////////////////////////////////////////////////////////////////

		PathArgument::PathArgument()
			: key_()
			, index_()
			, kind_(kindNone)
		{
		}


		PathArgument::PathArgument(ArrayIndex index)
			: key_()
			, index_(index)
			, kind_(kindIndex)
		{
		}


		PathArgument::PathArgument(const char *key)
			: key_(key)
			, index_()
			, kind_(kindKey)
		{
		}


		PathArgument::PathArgument(const string &key)
			: key_(key.c_str())
			, index_()
			, kind_(kindKey)
		{
		}

		// class Path
		// //////////////////////////////////////////////////////////////////

		Path::Path(const string &path,
			const PathArgument &a1,
			const PathArgument &a2,
			const PathArgument &a3,
			const PathArgument &a4,
			const PathArgument &a5)
		{
			InArgs in;
			in.push_back(&a1);
			in.push_back(&a2);
			in.push_back(&a3);
			in.push_back(&a4);
			in.push_back(&a5);
			makePath(path, in);
		}


		void
			Path::makePath(const string &path,
				const InArgs &in)
		{
			const char *current = path.c_str();
			const char *end = current + path.length();
			InArgs::const_iterator itInArg = in.begin();
			while (current != end)
			{
				if (*current == '[')
				{
					++current;
					if (*current == '%')
						addPathInArg(path, in, itInArg, PathArgument::kindIndex);
					else
					{
						ArrayIndex index = 0;
						for (; current != end && *current >= '0' && *current <= '9'; ++current)
							index = index * 10 + ArrayIndex(*current - '0');
						args_.push_back(index);
					}
					if (current == end || *current++ != ']')
						invalidPath(path, int(current - path.c_str()));
				}
				else if (*current == '%')
				{
					addPathInArg(path, in, itInArg, PathArgument::kindKey);
					++current;
				}
				else if (*current == '.')
				{
					++current;
				}
				else
				{
					const char *beginName = current;
					while (current != end && !strchr("[.", *current))
						++current;
					args_.push_back(string(beginName, current));
				}
			}
		}


		void
			Path::addPathInArg(const string &/*path*/,
				const InArgs &in,
				InArgs::const_iterator &itInArg,
				PathArgument::Kind kind)
		{
			if (itInArg == in.end())
			{
				// Error: missing argument %d
			}
			else if ((*itInArg)->kind_ != kind)
			{
				// Error: bad argument type
			}
			else
			{
				args_.push_back(**itInArg);
			}
		}


		void
			Path::invalidPath(const string &/*path*/,
				int /*location*/)
		{
			// Error: invalid path.
		}


		const Value &
			Path::resolve(const Value &root) const
		{
			const Value *node = &root;
			for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it)
			{
				const PathArgument &arg = *it;
				if (arg.kind_ == PathArgument::kindIndex)
				{
					if (!node->isArray() || !node->isValidIndex(arg.index_))
					{
						// Error: unable to resolve path (array value expected at position...
					}
					node = &((*node)[arg.index_]);
				}
				else if (arg.kind_ == PathArgument::kindKey)
				{
					if (!node->isObject())
					{
						// Error: unable to resolve path (object value expected at position...)
					}
					node = &((*node)[arg.key_]);
					if (node == &Value::null)
					{
						// Error: unable to resolve path (object has no member named '' at position...)
					}
				}
			}
			return *node;
		}


		Value
			Path::resolve(const Value &root,
				const Value &defaultValue) const
		{
			const Value *node = &root;
			for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it)
			{
				const PathArgument &arg = *it;
				if (arg.kind_ == PathArgument::kindIndex)
				{
					if (!node->isArray() || !node->isValidIndex(arg.index_))
						return defaultValue;
					node = &((*node)[arg.index_]);
				}
				else if (arg.kind_ == PathArgument::kindKey)
				{
					if (!node->isObject())
						return defaultValue;
					node = &((*node)[arg.key_]);
					if (node == &Value::null)
						return defaultValue;
				}
			}
			return *node;
		}


		Value &
			Path::make(Value &root) const
		{
			Value *node = &root;
			for (Args::const_iterator it = args_.begin(); it != args_.end(); ++it)
			{
				const PathArgument &arg = *it;
				if (arg.kind_ == PathArgument::kindIndex)
				{
					if (!node->isArray())
					{
						// Error: node is not an array at position ...
					}
					node = &((*node)[arg.index_]);
				}
				else if (arg.kind_ == PathArgument::kindKey)
				{
					if (!node->isObject())
					{
						// Error: node is not an object at position...
					}
					node = &((*node)[arg.key_]);
				}
			}
			return *node;
		}

	} // namespace json

}