#pragma once


#include "Base.h"
#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>


namespace rso
{
	namespace crypto
	{
		using namespace std;

		class sha256
		{
			using stream = array<unsigned char, 32>;

		private:
			static const uint32 add_value[64];
			static const uint32 block_size = 64;

			inline uint32 _sha2_rotr(uint32 num_, uint32 value_) const
			{
				return ((num_ >> value_) | (num_ << ((sizeof(num_) << 3) - value_)));
			}
			inline uint32 _sha256_f1(uint32 num_) const
			{
				return (_sha2_rotr(num_, 2) ^ _sha2_rotr(num_, 13) ^ _sha2_rotr(num_, 22));
			}
			inline uint32 _sha256_f2(uint32 num_) const
			{
				return (_sha2_rotr(num_, 6) ^ _sha2_rotr(num_, 11) ^ _sha2_rotr(num_, 25));
			}
			inline uint32 _sha256_f3(uint32 num_) const
			{
				return (_sha2_rotr(num_, 7) ^ _sha2_rotr(num_, 18) ^ (num_ >> 3));
			}
			inline uint32 _sha256_f4(uint32 num_) const
			{
				return (_sha2_rotr(num_, 17) ^ _sha2_rotr(num_, 19) ^ (num_ >> 10));
			}
			inline uint32 _sha2_ch(uint32 num0_, uint32 num1_, uint32 num2_) const
			{
				return ((num0_ & num1_) ^ (~num0_ & num2_));
			}
			inline uint32 _sha2_maj(uint32 num0_, uint32 num1_, uint32 num2_) const
			{
				return ((num0_ & num1_) ^ (num0_ & num2_) ^ (num1_ & num2_));
			}
			inline void _sha2_pack32(const unsigned char* str_, uint32* w_) const
			{
				*(w_) = ((uint32) *((str_)+3)) | ((uint32) *((str_)+2) << 8) | ((uint32) *((str_)+1) << 16) | ((uint32) *((str_)+0) << 24);
			}
			inline void _sha2_unpack32(uint32 num_, unsigned char* str_) const
			{
				*((str_)+3) = (unsigned char)((num_));
				*((str_)+2) = (unsigned char)((num_) >> 8);
				*((str_)+1) = (unsigned char)((num_) >> 16);
				*((str_)+0) = (unsigned char)((num_) >> 24);
			}

			void _transform(uint32* init_digest_, const unsigned char* msg_, uint32 block_nb_) const;
			stream _to_stream(const unsigned char* msg_, uint32 size_) const;
			string _to_string(const stream& stream_) const;

		public:
			stream to_stream(const string& str_) const;
			stream to_stream(const wstring& str_) const;
			string to_string(const string& str_) const;
			string to_string(const wstring& str_) const;
		};
	}
}
