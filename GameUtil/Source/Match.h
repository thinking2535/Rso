#pragma once

#include "Base.h"
#include <Rso/Base/MultiIndexMap.h>
#include <functional>
#include <map>

namespace rso::gameutil
{
	using namespace std;
	using namespace base;

	template<typename TKey, typename... TArgs>
	class CMatch
	{
	private:
		using _TUsers = CMultiIndexMap<double, unique_index<TKey>, non_unique_index<TTime>>;
		using _TTimeIterator = typename tuple_element<1, typename _TUsers::const_iterators>::type;
	public:
		using iterator = typename _TUsers::iterator;
	private:
		using _TMatchedUsers = vector<iterator>;
	public:
		using TMatchedUsers = vector<TKey>;
		using FCanMatch = function<bool(t_duration ElapsedDuration_, double MyGrade_, double OppGrade_)>;
		using FMatched = function<void(const TMatchedUsers& Users_, const TArgs&... Args_)>;

	private:
		_TUsers _Users;
		_TMatchedUsers _MatchedUsers;
		FCanMatch _fCanMatch;
		FMatched _fMatched;
		tuple<TArgs...> _Args;

		bool _CanMatch(t_duration MyElapsedDuration_, double MyGrade_, t_duration OppElapsedDuration_, double OppGrade_)
		{
			return (_fCanMatch(MyElapsedDuration_, MyGrade_, OppGrade_) && _fCanMatch(OppElapsedDuration_, OppGrade_, MyGrade_));
		}
		bool _Match(_TTimeIterator itMe_, _TTimeIterator itOpp_)
		{
			auto Now = system_clock::now();
			double OppGrade = _Users.get(itOpp_->second)->second;

			if (!_CanMatch(Now - itMe_->first, _Users.get(itMe_->second)->second, Now - itOpp_->first, OppGrade))
				return false;

			// ���ݲ� ���� �������� �� ������ ��Ī �����ϸ�
			for (auto& i : _MatchedUsers)
				if (!_CanMatch(Now - std::get<1>(i->first)->first, i->second, Now - itOpp_->first, OppGrade))
					return false;

			_MatchedUsers.emplace_back(_Users.get(itOpp_->second));

			return _MatchedUsers.size() == _MatchedUsers.capacity();
		}
		template<size_t... Indices_>
		void _Matched(const TMatchedUsers& MatchedUsers_, index_sequence<Indices_...>)
		{
			_fMatched(MatchedUsers_, std::get<Indices_>(_Args)...);
		}

	public:
		CMatch(size_t MatchUserCount_, FCanMatch fCanMatch_, FMatched fMatched_, const TArgs&... Args_) :
			_fCanMatch(fCanMatch_), _fMatched(fMatched_), _Args(Args_...)
		{
			_MatchedUsers.reserve(MatchUserCount_ - 1);
		}
		inline size_t size(void) const
		{
			return _Users.size();
		}
		inline iterator insert(double Grade_, const TKey& Key_, const TTime& StartTime_)
		{
			return _Users.emplace(Grade_, Key_, StartTime_);
		}
		inline iterator insert(double Grade_, const TKey& Key_)
		{
			return insert(Grade_, Key_, system_clock::now());
		}
		inline void erase(iterator it_)
		{
			_Users.erase(it_);
		}
		inline void erase_at(size_t Index_)
		{
			_Users.erase(Index_);
		}
		void erase(const TKey& Key_)
		{
			auto it = _Users.find<0>(Key_);
			if (it == _Users.end<0>())
				return;

			erase_at(it->second);
		}
		void Proc(void)
		{
			for (auto itUser = _Users.begin<1>(); itUser != _Users.end<1>();)
			{
				auto itOpp = itUser;
				bool IsMatched = false;

				for (++itOpp; itOpp != _Users.end<1>(); ++itOpp)
				{
					if (_Match(itUser, itOpp))
					{
						IsMatched = true;
						break;
					}
				}

				if (IsMatched)
				{
					// ++itUser �� ���� �ϸ� _MatchedUsers �� _Users ���� ���ﶧ ++itUser �� ������ ������ ������ �� �ֱ� ������ itUser�� ���� ���߿� ++ �ϰ� ����
					TMatchedUsers MatchedUsers;
					MatchedUsers.reserve(_MatchedUsers.capacity());
					MatchedUsers.emplace_back(std::get<0>(_Users.get(itUser->second)->first)->first);

					for (auto& i : _MatchedUsers)
						MatchedUsers.emplace_back(std::get<0>(i->first)->first);

					// _fMatched() ���� �� ��Ī �õ� �� �� �ֱ� ������ ���⼭ �����ϵ� �켱 itUser�� �����ϰ� ����
					for (auto& i : _MatchedUsers)
						_Users.erase(i);

					_MatchedUsers.clear(); // �� �ڵ带 if, else �� �Ʒ��� �� ��� _fMatched ���� �� ��Ī �� ��� ������ �ϹǷ� _fMatched ���� ������ Clear�ϵ���
											// else ���� ������ �߰�
					auto itCheck = itUser;
					++itUser;

					_Users.erase(itCheck->second);
					_Matched(MatchedUsers, index_sequence_for<TArgs...>());
				}
				else
				{
					_MatchedUsers.clear();
					++itUser;
				}
			}
		}
	};
}