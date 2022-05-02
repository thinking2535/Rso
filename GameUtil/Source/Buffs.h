#pragma once


#include "Base.h"


namespace rso
{
	namespace gameutil
	{
		using namespace std;

		template<typename TStat, typename TCode>
		class CBuffs
		{
		public:
			struct SBuff
			{
				TTime ExpireTime;
				TStat Stat;

				SBuff(TTime ExpireTime_, const TStat& Stat_) :
					ExpireTime(ExpireTime_), Stat(Stat_)
				{}
			};
			using TBuffs = map<TCode, SBuff>;

		private:
			TBuffs _Buffs;
			typename TBuffs::iterator _Iterator;

		public:
			CBuffs() :
				_Iterator(_Buffs.begin())
			{
			}
			CBuffs(TBuffs&& Buffs_) :
				_Buffs(std::move(Buffs_)),
				_Iterator(_Buffs.begin())
			{
			}
			bool Add(TCode Code_, const SBuff& Buff_)
			{
				try
				{
					auto ib = _Buffs.emplace(Code_, Buff_);
					if (!ib.second && ib.first->second.ExpireTime < Buff_.ExpireTime)
						ib.first->second = Buff_;

					return ib.second;
				}
				catch (...)
				{
					return false;
				}
			}
			void Sub(TCode Code_)
			{
				_Buffs.erase(Code_);
			}
			TStat Get(void) const
			{
				TStat Stat;

				for (auto& Buff : _Buffs)
					Stat += Buff.second.Stat;

				return Stat;
			}
			TBuffs& Buffs(void)
			{
				return _Buffs;
			}
			void erase(typename TBuffs::iterator It_)
			{
				_Buffs.erase(It_);
				_Iterator = _Buffs.begin();
			}
			list<TCode> BuffCodes(void) const
			{
				list<TCode> Codes;

				try
				{
					for (auto& itBuff : _Buffs)
						Codes.emplace_back(itBuff.first);
				}
				catch (...)
				{
				}

				return Codes;
			}
			void Clear(void)
			{
				_Buffs.clear();
				_Iterator = _Buffs.begin();
			}
			bool NextExpired(TCode& ExpiredCode_)
			{
				while (_Iterator != _Buffs.end())
				{
					auto itCheck = _Iterator;
					++_Iterator;

					if (itCheck->second.ExpireTime < system_clock::now())
					{
						ExpiredCode_ = itCheck->first;
						_Buffs.erase(itCheck);
						return true;
					}
				}

				_Iterator = _Buffs.begin();
				return false;
			}
		};
	}
}