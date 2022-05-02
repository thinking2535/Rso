#pragma once

#include "Base.h"

namespace rso
{
	namespace gameutil
	{
		using namespace std;

		template<typename _TKey, typename _TCnt>
		class CKeyCnt : public map<_TKey, _TCnt>
		{
		public:
			using TKeyCnts = map<_TKey, _TCnt>;
			using key_type = typename TKeyCnts::key_type;
			using mapped_type = typename TKeyCnts::mapped_type;
			using value_type = typename TKeyCnts::value_type;

		public:
			CKeyCnt operator += (const value_type& KeyCnt_)
			{
				if (KeyCnt_.second > 0)
				{
					auto ib = emplace(KeyCnt_);
					if (!ib.second)
					{
						if (ib.first->second + KeyCnt_.second == 0)
							erase(ib.first);
						else if (ib.first->second + KeyCnt_.second > ib.first->second)
							ib.first->second += KeyCnt_.second;
						else
							ib.first->second = MaxValue<_TCnt>();
					}
				}
				else if (KeyCnt_.second < 0)
				{
					auto ib = emplace(KeyCnt_);
					if (!ib.second)
					{
						if (ib.first->second + KeyCnt_.second == 0)
							erase(ib.first);
						else if (ib.first->second + KeyCnt_.second < ib.first->second)
							ib.first->second += KeyCnt_.second;
						else
							ib.first->second = MinValue<_TCnt>();
					}
				}

				return *this;
			}
			CKeyCnt operator -= (const value_type& KeyCnt_)
			{
				return operator += (value_type(KeyCnt_.first, -KeyCnt_.second));
			}
			CKeyCnt operator += (const TKeyCnts& KeyCnts_)
			{
				for (auto& i : KeyCnts_)
					operator += (i);

				return *this;
			}
			CKeyCnt operator -= (const TKeyCnts& KeyCnts_)
			{
				for (auto& i : KeyCnts_)
					operator -= (i);

				return *this;
			}
			CKeyCnt operator *= (_TCnt Cnt_)
			{
				if (Cnt_ != 0)
				{
					for (auto& i : *this)
						i.second *= Cnt_;
				}
				else
				{
					clear();
				}

				return *this;
			}
			CKeyCnt operator /= (_TCnt Cnt_)
			{
				for (auto it = begin(); it != end();)
				{
					auto itCheck = it;
					++it;

					i.second /= Cnt_;

					if (i.second == 0)
						erase(itCheck);
				}

				return *this;
			}
			bool operator < (const TKeyCnts& KeyCnts_) const
			{
				for (auto& i : *this)
				{
					auto it = KeyCnts_.find(i.first);
					if (it == KeyCnts_.end())
						return false;

					if (i.second >= it->second)
						return false;
				}

				return true;
			}
			bool operator <= (const TKeyCnts& KeyCnts_) const
			{
				for (auto& i : *this)
				{
					auto it = KeyCnts_.find(i.first);
					if (it == KeyCnts_.end())
						return false;

					if (i.second > it->second)
						return false;
				}

				return true;
			}
			bool operator > (const TKeyCnts& KeyCnts_) const
			{
				for (auto& i : KeyCnts_)
				{
					auto it = find(i.first);
					if (it == end())
						return false;

					if (it->second <= i.second)
						return false;
				}

				return true;
			}
			bool operator >= (const TKeyCnts& KeyCnts_) const
			{
				for (auto& i : KeyCnts_)
				{
					auto it = find(i.first);
					if (it == end())
						return false;

					if (it->second < i.second)
						return false;
				}

				return true;
			}
			_TCnt total_cnt(void) const
			{
				_TCnt Cnt{};

				for (auto& i : *this)
					Cnt += i.second;

				return Cnt;
			}
		};
	}
}