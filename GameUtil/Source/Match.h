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
		using FMatching = function<void(const TKey& Key_, size_t MatchingCount_)>;
		using FMatched = function<void(const TMatchedUsers& Users_, const TArgs&... Args_)>;

	private:
		_TUsers _Users;
		_TMatchedUsers _MatchedUsers;
		FMatching _fMatching;
		FMatched _fMatched;
		double _Factor = 0.0;
		tuple<TArgs...> _Args;

		bool _CanMatch(TTime MyStartTime_, double MyGrade_, double OppGrade_)
		{
			auto Elapsed = duration_cast<seconds>(system_clock::now() - MyStartTime_);
			auto MatchableGradeGap = _Factor * double(Elapsed.count() * Elapsed.count());
			return (OppGrade_ >= MyGrade_ - MatchableGradeGap && OppGrade_ <= MyGrade_ + MatchableGradeGap);
		}
		bool _CanMatch(TTime MyStartTime_, double MyGrade_, TTime OppStartTime_, double OppGrade_)
		{
			return (_CanMatch(MyStartTime_, MyGrade_, OppGrade_) && _CanMatch(OppStartTime_, OppGrade_, MyGrade_));
		}
		void _Match(_TTimeIterator itMe_, _TTimeIterator itOpp_)
		{
			double OppGrade = _Users.get(itOpp_->second)->second;

			if (!_CanMatch(itMe_->first, _Users.get(itMe_->second)->second, itOpp_->first, OppGrade))
				return;

			// ���ݲ� ���� �������� �� ������ ��Ī �����ϸ�
			for (auto& i : _MatchedUsers)
				if (!_CanMatch(std::get<1>(i->first)->first, i->second, itOpp_->first, OppGrade))
					return;

			_MatchedUsers.emplace_back(_Users.get(itOpp_->second));
		}
		template<size_t... Indices_>
		void _Matched(const TMatchedUsers& MatchedUsers_, index_sequence<Indices_...>)
		{
			_fMatched(MatchedUsers_, std::get<Indices_>(_Args)...);
		}

	public:
		CMatch(size_t MatchUserCount_, FMatching fMatching_, FMatched fMatched_, seconds TimeOutSeconds_, double GradeRange_, const TArgs&... Args_) :
			_fMatching(fMatching_), _fMatched(fMatched_), _Factor(double(GradeRange_) / double(TimeOutSeconds_.count() * TimeOutSeconds_.count())), _Args(Args_...)
		{
			_MatchedUsers.reserve(MatchUserCount_);
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
				for (auto itOpp = _Users.begin<1>(); itOpp != _Users.end<1>(); ++itOpp)
				{
					if (itOpp == itUser)
						continue;

					_Match(itUser, itOpp);

					if (_MatchedUsers.size() == _MatchedUsers.capacity() - 1)
						break;
				}

				if (_MatchedUsers.size() == _MatchedUsers.capacity() - 1)
				{
					// ++itUser �� ���� �ϸ� _MatchedUsers �� _Users ���� ���ﶧ ++itUser �� ������ ������ ������ �� �ֱ� ������ itUser�� ���� ���߿� ++ �ϰ� ����
					TMatchedUsers MatchedUsers;
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
					_fMatching(std::get<0>(_Users.get(itUser->second)->first)->first, _MatchedUsers.size());
					_MatchedUsers.clear();
					++itUser;
				}
			}
		}
	};
}