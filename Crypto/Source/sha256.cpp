#include "Sha256.h"


namespace rso
{
	namespace crypto
	{
		const uint32 sha256::add_value[] =
		{
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
			0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
			0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
			0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
			0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
			0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
			0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
			0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
			0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
		};

		void sha256::_transform(uint32* init_digest_, const unsigned char* msg_, uint32 block_nb_) const
		{
			uint32 w[64];
			uint32 wv[8];
			uint32 t1, t2;
			const unsigned char* sub_block;

			for (int32 i = 0; i < (int32)block_nb_; i++)
			{
				sub_block = msg_ + (i << 6);

				for (int32 j = 0; j < 16; j++)
					_sha2_pack32(&sub_block[j << 2], &w[j]);

				for (int32 j = 16; j < 64; j++)
					w[j] = _sha256_f4(w[j - 2]) + w[j - 7] + _sha256_f3(w[j - 15]) + w[j - 16];

				for (int32 j = 0; j < 8; j++)
					wv[j] = init_digest_[j];

				for (int32 j = 0; j < 64; j++)
				{
					t1 = wv[7] + _sha256_f2(wv[4]) + _sha2_ch(wv[4], wv[5], wv[6]) + add_value[j] + w[j];
					t2 = _sha256_f1(wv[0]) + _sha2_maj(wv[0], wv[1], wv[2]);
					wv[7] = wv[6];
					wv[6] = wv[5];
					wv[5] = wv[4];
					wv[4] = wv[3] + t1;
					wv[3] = wv[2];
					wv[2] = wv[1];
					wv[1] = wv[0];
					wv[0] = t1 + t2;
				}

				for (int32 j = 0; j < 8; j++)
					init_digest_[j] += wv[j];
			}
		}

		sha256::stream sha256::_to_stream(const unsigned char* msg_, uint32 size_) const
		{
			uint32 init_digest[8];
			init_digest[0] = 0x6a09e667;
			init_digest[1] = 0xbb67ae85;
			init_digest[2] = 0x3c6ef372;
			init_digest[3] = 0xa54ff53a;
			init_digest[4] = 0x510e527f;
			init_digest[5] = 0x9b05688c;
			init_digest[6] = 0x1f83d9ab;
			init_digest[7] = 0x5be0cd19;

			uint32 len = 0;
			uint32 total_len = 0;
			unsigned char block[2 * block_size];
			uint32 block_nb;
			uint32 new_len;
			const unsigned char* shifted_message;

			auto tmp_len = block_size - len;
			auto rem_len = size_ < tmp_len ? size_ : tmp_len;
			memcpy(&block[len], msg_, rem_len);
			if (len + size_ < block_size)
			{
				len += size_;
			}
			else
			{
				new_len = size_ - rem_len;
				block_nb = new_len / block_size;
				shifted_message = msg_ + rem_len;
				_transform(init_digest, block, 1);
				_transform(init_digest, shifted_message, block_nb);
				rem_len = new_len % block_size;
				memcpy(block, &shifted_message[block_nb << 6], rem_len);
				len = rem_len;
				total_len += (block_nb + 1) << 6;
			}

			block_nb = (1 + ((block_size - 9) < (len % block_size)));
			auto len_b = (total_len + len) << 3;
			auto pm_len = block_nb << 6;
			memset(block + len, 0, pm_len - len);
			block[len] = 0x80;
			_sha2_unpack32(len_b, block + pm_len - 4);
			_transform(init_digest, block, block_nb);

			stream digest;
			for (int32 i = 0; i < 8; i++)
				_sha2_unpack32(init_digest[i], &digest[i << 2]);

			return digest;
		}
		string sha256::_to_string(const stream& stream_) const
		{
			stringstream ss;

			for (auto& i : stream_)
				ss << std::setfill('0') << std::setw(2) << std::hex << (int)i;

			return ss.str();
		}
		sha256::stream sha256::to_stream(const string& str_) const
		{
			return _to_stream((const unsigned char*)str_.data(), (uint32)str_.size());
		}
		sha256::stream sha256::to_stream(const wstring& str_) const
		{
			return _to_stream((const unsigned char*)str_.data(), (uint32)str_.size() * 2);
		}
		string sha256::to_string(const string& str_) const
		{
			return _to_string(to_stream(str_));
		}
		string sha256::to_string(const wstring& str_) const
		{
			return _to_string(to_stream(str_));
		}
	}
}
