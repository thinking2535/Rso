// Copyright 2007-2010 Baptiste Lepilleur
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef CPPTL_JSON_H_INCLUDED
#define CPPTL_JSON_H_INCLUDED

#include "Base.h"

#if !defined(JSON_IS_AMALGAMATION)
#include "forwards.h"
#endif // if !defined(JSON_IS_AMALGAMATION)
#include <string>
#include <vector>

# ifndef JSON_USE_CPPTL_SMALLMAP
#  include <array>
#  include <vector>
#  include <set>
#  include <map>
#  include <list>
# else
#  include <cpptl/smallmap.h>
# endif

// Disable warning C4251: <data member>: <type> needs to have dll-interface to be used by...
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
# pragma warning(push)
# pragma warning(disable:4251)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)


namespace rso
{

	/** \brief JSON (JavaScript Object Notation).
	 */
	namespace json {

		/** \brief Type of the value held by a Value object.
		 */
		enum ValueType
		{
			nullValue = 0, ///< 'null' value
			intValue,      ///< signed integer value
			uintValue,     ///< unsigned integer value
			realValue,     ///< double value
			stringValue,   ///< UTF-8 string value
			booleanValue,  ///< bool value
			arrayValue,    ///< array value (ordered list)
			objectValue    ///< object value (collection of name/value pairs).
		};

		enum CommentPlacement
		{
			commentBefore = 0,        ///< a comment placed on the line before a value
			commentAfterOnSameLine,   ///< a comment just after a value on the same line
			commentAfter,             ///< a comment on the line after a value (only make sense for root value)
			numberOfCommentPlacement
		};

		class StaticString
		{
		public:
			explicit StaticString(const char *czstring)
				: str_(czstring)
			{
			}

			operator const char *() const
			{
				return str_;
			}

			const char *c_str() const
			{
				return str_;
			}

		private:
			const char *str_;
		};

		/** \brief Represents a <a HREF="http://www.json.org">JSON</a> value.
		 *
		 * This class is a discriminated union wrapper that can represents a:
		 * - signed integer [range: Value::minInt - Value::maxInt]
		 * - unsigned integer (range: 0 - Value::maxUInt)
		 * - double
		 * - UTF-8 string
		 * - boolean
		 * - 'null'
		 * - an ordered list of Value
		 * - collection of name/value pairs (javascript object)
		 *
		 * The type of the held value is represented by a #ValueType and
		 * can be obtained using type().
		 *
		 * values of an #objectValue or #arrayValue can be accessed using operator[]() methods.
		 * Non const methods will automatically create the a #nullValue element
		 * if it does not exist.
		 * The sequence of an #arrayValue will be automatically resize and initialized
		 * with #nullValue. resize() can be used to enlarge or truncate an #arrayValue.
		 *
		 * The get() methods can be used to obtanis default value in the case the required element
		 * does not exist.
		 *
		 * It is possible to iterate over the list of a #objectValue values using
		 * the getMemberNames() method.
		 */
		class Value
		{
			template <typename TType, bool IsEnum_>
			struct _SOperator
			{
				Value operator()(const TType& Value_)
				{
					return Value((const SProto&)Value_);
				}
			};

			template <typename TType>
			struct _SOperator<TType, true>
			{
				Value operator()(const TType& Value_)
				{
					return Value((typename std::underlying_type<TType>::type)Value_);
				}
			};

			friend class ValueIteratorBase;
		public:
			typedef std::vector<string> Members;
			typedef ValueIterator iterator;
			typedef ValueConstIterator const_iterator;
			typedef json::UInt UInt;
			typedef json::Int Int;
# if defined(JSON_HAS_INT64)
			typedef json::UInt64 UInt64;
			typedef json::Int64 Int64;
#endif // defined(JSON_HAS_INT64)
			typedef json::LargestInt LargestInt;
			typedef json::LargestUInt LargestUInt;
			typedef json::ArrayIndex ArrayIndex;

			static const Value null;
			/// Minimum signed integer value that can be stored in a json::Value.
			static const LargestInt minLargestInt;
			/// Maximum signed integer value that can be stored in a json::Value.
			static const LargestInt maxLargestInt;
			/// Maximum unsigned integer value that can be stored in a json::Value.
			static const LargestUInt maxLargestUInt;

			/// Minimum signed int value that can be stored in a json::Value.
			static const Int minInt;
			/// Maximum signed int value that can be stored in a json::Value.
			static const Int maxInt;
			/// Maximum unsigned int value that can be stored in a json::Value.
			static const UInt maxUInt;

# if defined(JSON_HAS_INT64)
			/// Minimum signed 64 bits int value that can be stored in a json::Value.
			static const Int64 minInt64;
			/// Maximum signed 64 bits int value that can be stored in a json::Value.
			static const Int64 maxInt64;
			/// Maximum unsigned 64 bits int value that can be stored in a json::Value.
			static const UInt64 maxUInt64;
#endif // defined(JSON_HAS_INT64)

		private:
#ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
			class CZString
			{
			public:
				enum DuplicationPolicy
				{
					noDuplication = 0,
					duplicate,
					duplicateOnCopy
				};
				CZString(ArrayIndex index);
				CZString(const char *cstr, DuplicationPolicy allocate);
				CZString(const CZString &other);
				~CZString();
				CZString &operator =(const CZString &other);
				bool operator<(const CZString &other) const;
				bool operator==(const CZString &other) const;
				ArrayIndex index() const;
				const char *c_str() const;
				bool isStaticString() const;
			private:
				void swap(CZString &other);
				const char *cstr_;
				ArrayIndex index_;
			};
		public:
#  ifndef JSON_USE_CPPTL_SMALLMAP
			struct _SKeyValuePtr
			{
				CZString key;
				Value* value_ptr;
			};
			typedef std::list<_SKeyValuePtr> ObjectListValues;
			template<typename _TType>
			struct _ValueIt : public _TType
			{
				typename ObjectListValues::iterator itList;

				_ValueIt(const _TType& Super_) :
					_TType(Super_)
				{
				}
			};
			typedef std::map<CZString, _ValueIt<Value>> ObjectValues;
#  else
			typedef CppTL::SmallMap<CZString, Value> ObjectValues;
#  endif // ifndef JSON_USE_CPPTL_SMALLMAP
#endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

		public:
			/** \brief Create a default Value of the given type.

			  This is a very useful constructor.
			  To create an empty array, pass arrayValue.
			  To create an empty object, pass objectValue.
			  Another Value can then be set to this one by assignment.
		  This is useful since clear() and resize() will not alter types.

			  Examples:
		  \code
		  json::Value null_value; // null
		  json::Value arr_value(json::arrayValue); // []
		  json::Value obj_value(json::objectValue); // {}
		  \endcode
			*/
			Value(ValueType type = nullValue);
			Value(int8 value);
			Value(uint8 value);
			Value(int16 value);
			Value(uint16 value);
			Value(Int value);
			Value(UInt value);
#if defined(JSON_HAS_INT64)
			Value(Int64 value);
			Value(UInt64 value);
#endif // if defined(JSON_HAS_INT64)
			Value(float value);
			Value(double value);
			Value(const char *value);
			Value(const char *beginValue, const char *endValue);
			/** \brief Constructs a value from a static string.

			 * Like other value string constructor but do not duplicate the string for
			 * internal storage. The given string must remain alive after the call to this
			 * constructor.
			 * Example of usage:
			 * \code
			 * json::Value aValue( StaticString("some text") );
			 * \endcode
			 */
			Value(const StaticString &value);
			Value(const string &value);
			Value(const u16string& value);
			Value(const u32string& value);
			Value(const wstring &value);
			Value(bool value);
			Value(const TTime& value);
			Value(const SDateTime& value);
			Value(const CStream& value);
			Value(const SProto& value);
			template <typename TValue, size_t Size>
			Value(const array<TValue, Size>& Value_) :
				Value(arrayValue)
			{
				ArrayIndex i = 0;
				for (auto& it : Value_)
					(*this)[i++] = it;
			}
			template <typename TValue>
			Value(const list<TValue>& Value_) :
				Value(arrayValue)
			{
				ArrayIndex i = 0;
				for (auto& it : Value_)
					(*this)[i++] = it;
			}
			template <typename TValue>
			Value(const vector<TValue>& Value_) :
				Value(arrayValue)
			{
				ArrayIndex i = 0;
				for (auto& it : Value_)
					(*this)[i++] = it;
			}
			template <typename TKey, typename TValue>
			Value(const map<TKey, TValue>& Value_) :
				Value(arrayValue)
			{
				ArrayIndex i = 0;
				for (auto& it : Value_)
					(*this)[i++] = it.first;
			}
			template <typename TKey, typename TValue>
			Value(const multimap<TKey, TValue>& Value_) :
				Value(arrayValue)
			{
				ArrayIndex i = 0;
				for (auto& it : Value_)
					(*this)[i++] = it.first;
			}
			template <typename TKey>
			Value(const set<TKey>& Value_) :
				Value(arrayValue)
			{
				ArrayIndex i = 0;
				for (auto& it : Value_)
					(*this)[i++] = it;
			}
			template <typename TKey>
			Value(const multiset<TKey>& Value_) :
				Value(arrayValue)
			{
				ArrayIndex i = 0;
				for (auto& it : Value_)
					(*this)[i++] = it;
			}
			template<typename T>
			Value(const T& value) :
				Value(_SOperator<T, is_enum<T>::value>()(value))
			{
			}
			/// Swap values.
			/// \note Currently, comments are intentionally not swapped, for
			/// both logic and efficiency.
			ValueType type() const;

			bool operator <(const Value &other) const;
			bool operator <=(const Value &other) const;
			bool operator >=(const Value &other) const;
			bool operator >(const Value &other) const;

			bool operator ==(const Value &other) const;
			bool operator !=(const Value &other) const;

			int compare(const Value &other) const;

			const char *asCString() const;
			string asString() const;
			Int asInt() const;
			UInt asUInt() const;
#if defined(JSON_HAS_INT64)
			Int64 asInt64() const;
			UInt64 asUInt64() const;
#endif // if defined(JSON_HAS_INT64)
			LargestInt asLargestInt() const;
			LargestUInt asLargestUInt() const;
			float asFloat() const;
			double asDouble() const;
			bool asBool() const;

			template<typename _TType>
			_TType getValue() const
			{
				_TType value;
				operator >> (value);
				return value;
			}

			void operator >> (bool& Value_) const { Value_ = asBool(); }
			void operator >> (int8& Value_) const { Value_ = int8(asInt()); }
			void operator >> (uint8& Value_) const { Value_ = uint8(asUInt()); }
			void operator >> (int16& Value_) const { Value_ = int16(asInt()); }
			void operator >> (uint16& Value_) const { Value_ = uint16(asUInt()); }
			void operator >> (int32& Value_) const { Value_ = asInt(); }
			void operator >> (uint32& Value_) const { Value_ = asUInt(); }
			void operator >> (int64& Value_) const { Value_ = asInt64(); }
			void operator >> (uint64& Value_) const { Value_ = asUInt64(); }
			void operator >> (long& Value_) const { Value_ = long(asInt()); }
			void operator >> (unsigned long& Value_) const { Value_ = unsigned long(asUInt()); }
			void operator >> (float& Value_) const { Value_ = asFloat(); }
			void operator >> (double& Value_) const { Value_ = asDouble(); }
			void operator >> (long double& Value_) const { Value_ = long double(asDouble()); }
			void operator >> (string& Value_) const { Value_ = asString(); }
			void operator >> (u16string& Value_) const { Value_ = to_utf16(asString()); }
			void operator >> (u32string& Value_) const { Value_ = to_utf32(asString()); }
			void operator >> (wstring& Value_) const { Value_ = MBSToWCS(asString()); }
			void operator >> (system_clock::time_point& Value_) const { Value_ = system_clock::time_point(t_duration(asInt64())); }
			void operator >> (SDateTime& Value_) const { Value_ = CDateTime(system_clock::time_point(t_duration(asInt64()))); }
			void operator >> (CStream& Stream_) const { Stream_.clear(); Stream_.Push(asString().c_str(), asString().size()); }
			void operator >> (SProto& Proto_) const { Proto_ << *this; }
			template <typename TValue, size_t Size>
			void operator >> (array<TValue, Size>& Value_) const
			{
				size_t index = 0;
				for (auto& i : *this)
					Value_[index++] = i.getValue<TValue>();
			}
			template <typename TValue>
			void operator >> (list<TValue>& Value_) const
			{
				Value_.clear();
				for (auto& i : *this)
					Value_.emplace_back(i.getValue<TValue>());
			}
			template <typename TValue>
			void operator >> (vector<TValue>& Value_) const
			{
				Value_.clear();
				Value_.reserve(size());
				for (auto& i : *this)
					Value_.emplace_back(i.getValue<TValue>());
			}
			template <typename TKey, typename TValue>
			void operator >> (map<TKey, TValue>& Value_) const
			{
				Value_.clear();
				for (auto& i : *this)
					Value_.emplace(i.getValue<TKey>(), TValue{});
			}
			template <typename TKey, typename TValue>
			void operator >> (multimap<TKey, TValue>& Value_) const
			{
				Value_.clear();
				for (auto& i : *this)
					Value_.emplace(i.getValue<TKey>(), TValue{});
			}
			template <typename TKey>
			void operator >> (set<TKey>& Value_) const
			{
				Value_.clear();
				for (auto& i : *this)
					Value_.emplace(i.getValue<TKey>());
			}
			template <typename TKey>
			void operator >> (multiset<TKey>& Value_) const
			{
				Value_.clear();
				for (auto& i : *this)
					Value_.emplace(i.getValue<TKey>());
			}
			template <typename TFirst, typename TSecond>
			void operator >> (pair<TFirst, TSecond>& Pair_) const
			{
			}
			template <typename TProto>
			void operator >> (TProto& Value_) const { operator >> ((SProto&)Value_); }

			bool isNull() const;
			bool isBool() const;
			bool isInt() const;
			bool isInt64() const;
			bool isUInt() const;
			bool isUInt64() const;
			bool isIntegral() const;
			bool isDouble() const;
			bool isNumeric() const;
			bool isString() const;
			bool isArray() const;
			bool isObject() const;

			bool isConvertibleTo(ValueType other) const;

			/// Number of values in array or object
			ArrayIndex size() const;

			/// \brief Return true if empty array, empty object, or null;
			/// otherwise, false.
			bool empty() const;

			/// Return isNull()
			bool operator!() const;

			/// Remove all object members and array elements.
			/// \pre type() is arrayValue, objectValue, or nullValue
			/// \post type() is unchanged
			void clear();

			/// Resize the array to size elements. 
			/// New elements are initialized to null.
			/// May only be called on nullValue or arrayValue.
			/// \pre type() is arrayValue or nullValue
			/// \post type() is arrayValue
			void resize(ArrayIndex size);

			/// Access an array element (zero based index ).
			/// If the array contains less than index element, then null value are inserted
			/// in the array so that its size is index+1.
			/// (You may need to say 'value[0u]' to get your compiler to distinguish
			///  this from the operator[] which takes a string.)
			Value &operator[](ArrayIndex index);

			/// Access an array element (zero based index ).
			/// If the array contains less than index element, then null value are inserted
			/// in the array so that its size is index+1.
			/// (You may need to say 'value[0u]' to get your compiler to distinguish
			///  this from the operator[] which takes a string.)
			Value &operator[](int index);

			/// Access an array element (zero based index )
			/// (You may need to say 'value[0u]' to get your compiler to distinguish
			///  this from the operator[] which takes a string.)
			const Value &operator[](ArrayIndex index) const;

			/// Access an array element (zero based index )
			/// (You may need to say 'value[0u]' to get your compiler to distinguish
			///  this from the operator[] which takes a string.)
			const Value &operator[](int index) const;

			/// If the array contains at least index+1 elements, returns the element value, 
			/// otherwise returns defaultValue.
			Value get(ArrayIndex index,
				const Value &defaultValue) const;
			/// Return true if index < size().
			bool isValidIndex(ArrayIndex index) const;
			/// \brief Append value to array at the end.
			///
			/// Equivalent to jsonvalue[jsonvalue.size()] = value;
			/// Access an object value by name, create a null member if it does not exist.
			Value &operator[](const char *key);
			/// Access an object value by name, returns null if there is no member with that name.
			const Value &operator[](const char *key) const;
			/// Access an object value by name, create a null member if it does not exist.
			Value &operator[](const string &key);
			Value &operator[](const wstring &key);
			/// Access an object value by name, returns null if there is no member with that name.
			const Value &operator[](const string &key) const;
			const Value &operator[](const wstring &key) const;
			/** \brief Access an object value by name, create a null member if it does not exist.

			 * If the object as no entry for that name, then the member name used to store
			 * the new entry is not duplicated.
			 * Example of use:
			 * \code
			 * json::Value object;
			 * static const StaticString code("code");
			 * object[code] = 1234;
			 * \endcode
			 */
			Value &operator[](const StaticString &key);
			/// Return the member named key if it exist, defaultValue otherwise.
			Value get(const char *key,
				const Value &defaultValue) const;
			/// Return the member named key if it exist, defaultValue otherwise.
			Value get(const string &key,
				const Value &defaultValue) const;
			/// \brief Remove and return the named member.  
			///
			/// Do nothing if it did not exist.
			/// \return the removed Value, or null.
			/// \pre type() is objectValue or nullValue
			/// \post type() is unchanged
			Value removeMember(const char* key);
			/// Same as removeMember(const char*)
			Value removeMember(const string &key);

			/// Return true if the object has a member named key.
			bool isMember(const char *key) const;
			/// Return true if the object has a member named key.
			bool isMember(const string &key) const;

			/// \brief Return a list of the member names.
			///
			/// If null, return an empty list.
			/// \pre type() is objectValue or nullValue
			/// \post if type() was nullValue, it remains nullValue
			Members getMemberNames() const;
			string toStyledString() const;
			wstring toStyledWString() const;

			const_iterator begin() const;
			const_iterator end() const;

			iterator begin();
			iterator end();

		private:
			Value &resolveReference(const char *key, bool isStatic);

			ValueType type_ = nullValue;
			LargestInt int_ = 0;
			LargestUInt uint_ = 0;
			double real_ = 0.0;
			bool bool_ = false;
			string string_;
			ObjectValues map_;
			ObjectListValues list_;
		};


		/** \brief Experimental and untested: represents an element of the "path" to access a node.
		 */
		class PathArgument
		{
		public:
			friend class Path;

			PathArgument();
			PathArgument(ArrayIndex index);
			PathArgument(const char *key);
			PathArgument(const string &key);

		private:
			enum Kind
			{
				kindNone = 0,
				kindIndex,
				kindKey
			};
			string key_;
			ArrayIndex index_;
			Kind kind_;
		};

		/** \brief Experimental and untested: represents a "path" to access a node.
		 *
		 * Syntax:
		 * - "." => root node
		 * - ".[n]" => elements at index 'n' of root node (an array value)
		 * - ".name" => member named 'name' of root node (an object value)
		 * - ".name1.name2.name3"
		 * - ".[0][1][2].name1[3]"
		 * - ".%" => member name is provided as parameter
		 * - ".[%]" => index is provied as parameter
		 */
		class Path
		{
		public:
			Path(const string &path,
				const PathArgument &a1 = PathArgument(),
				const PathArgument &a2 = PathArgument(),
				const PathArgument &a3 = PathArgument(),
				const PathArgument &a4 = PathArgument(),
				const PathArgument &a5 = PathArgument());

			const Value &resolve(const Value &root) const;
			Value resolve(const Value &root,
				const Value &defaultValue) const;
			/// Creates the "path" to access the specified node and returns a reference on the node.
			Value &make(Value &root) const;

		private:
			typedef std::vector<const PathArgument *> InArgs;
			typedef std::vector<PathArgument> Args;

			void makePath(const string &path,
				const InArgs &in);
			void addPathInArg(const string &path,
				const InArgs &in,
				InArgs::const_iterator &itInArg,
				PathArgument::Kind kind);
			void invalidPath(const string &path,
				int location);

			Args args_;
		};


		/** \brief base class for Value iterators.
		 *
		 */
		class ValueIteratorBase
		{
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef unsigned int size_t;
			typedef int difference_type;
			typedef ValueIteratorBase SelfType;

			ValueIteratorBase();
			explicit ValueIteratorBase(const Value::ObjectListValues::iterator& current);

			bool operator ==(const SelfType &other) const
			{
				return isEqual(other);
			}

			bool operator !=(const SelfType &other) const
			{
				return !isEqual(other);
			}

			difference_type operator -(const SelfType &other) const
			{
				return computeDistance(other);
			}

			/// Return either the index or the member name of the referenced value as a Value.
			Value key() const;

			/// Return the index of the referenced Value. -1 if it is not an arrayValue.
			UInt index() const;

			/// Return the member name of the referenced Value. "" if it is not an objectValue.
			const char *memberName() const;

		protected:
			Value &deref() const;

			void increment();

			void decrement();

			difference_type computeDistance(const SelfType &other) const;

			bool isEqual(const SelfType &other) const;

			void copy(const SelfType &other);

		private:
			Value::ObjectListValues::iterator current_;
			// Indicates that iterator is for a null value.
			bool isNull_;
		};

		/** \brief const iterator for object and array value.
		 *
		 */
		class ValueConstIterator : public ValueIteratorBase
		{
			friend class Value;
		public:
			typedef const Value value_type;
			typedef unsigned int size_t;
			typedef int difference_type;
			typedef const Value &reference;
			typedef const Value *pointer;
			typedef ValueConstIterator SelfType;

			ValueConstIterator();
		private:
			/*! \internal Use by Value to create an iterator.
			 */
			explicit ValueConstIterator(const Value::ObjectListValues::iterator& current);
		public:
			SelfType &operator =(const ValueIteratorBase &other);

			SelfType operator++(int)
			{
				SelfType temp(*this);
				++*this;
				return temp;
			}

			SelfType operator--(int)
			{
				SelfType temp(*this);
				--*this;
				return temp;
			}

			SelfType &operator--()
			{
				decrement();
				return *this;
			}

			SelfType &operator++()
			{
				increment();
				return *this;
			}

			reference operator *() const
			{
				return deref();
			}
		};


		/** \brief Iterator for object and array value.
		 */
		class ValueIterator : public ValueIteratorBase
		{
			friend class Value;
		public:
			typedef Value value_type;
			typedef unsigned int size_t;
			typedef int difference_type;
			typedef Value &reference;
			typedef Value *pointer;
			typedef ValueIterator SelfType;

			ValueIterator();
			ValueIterator(const ValueConstIterator &other);
			ValueIterator(const ValueIterator &other);
		private:
			/*! \internal Use by Value to create an iterator.
			 */
			explicit ValueIterator(const Value::ObjectListValues::iterator &current);
		public:

			SelfType &operator =(const SelfType &other);

			SelfType operator++(int)
			{
				SelfType temp(*this);
				++*this;
				return temp;
			}

			SelfType operator--(int)
			{
				SelfType temp(*this);
				--*this;
				return temp;
			}

			SelfType &operator--()
			{
				decrement();
				return *this;
			}

			SelfType &operator++()
			{
				increment();
				return *this;
			}

			reference operator *() const
			{
				return deref();
			}
		};


	} // namespace json
}

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
# pragma warning(pop)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)


#endif // CPPTL_JSON_H_INCLUDED
