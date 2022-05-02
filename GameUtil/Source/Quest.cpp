#include "Quest.h"


namespace rso
{
	namespace gameutil
	{
		bool CQuest::Add(const TQuests& Quests_)
		{
			try
			{
				for (auto& Quest : Quests_)
					_Quests.emplace(Quest.QuestIndex, TRequirements()).first->second.emplace(Quest.RequirementIndex, Quest.RequireCnt);

				return true;
			}
			catch (...)
			{
				return false;
			}
		}
		bool CQuest::Add(TIndex QuestIndex_, TIndex RequirementIndex_, int32 RequirementCnt_)
		{
			try
			{
				auto ib = _Quests.emplace(QuestIndex_, TRequirements()).first->second.emplace(RequirementIndex_, RequirementCnt_);
				if (!ib.second)
					ib.first->second += RequirementCnt_;

				return true;
			}
			catch (...)
			{
				return false;
			}
		}
		bool CQuest::Add(TIndex QuestIndex_, const TRequirements& Requirements_)
		{
			try
			{
				return _Quests.emplace(QuestIndex_, Requirements_).second;
			}
			catch (...)
			{
				return false;
			}
		}
		bool CQuest::CompleteRequirement(TIndex QuestIndex_, TIndex RequirementIndex_, int32 RequirementCnt_)
		{
			auto itQuest = _Quests.find(QuestIndex_);
			if (itQuest == _Quests.end())
				return false;

			auto itRequirement = itQuest->second.find(RequirementIndex_);
			if (itRequirement == itQuest->second.end())
				return false;

			if (itRequirement->second > RequirementCnt_)
			{
				itRequirement->second -= RequirementCnt_;
				return false;
			}
			else
			{
				itQuest->second.erase(itRequirement);
				return itQuest->second.empty();
			}
		}
		bool CQuest::IsCompleted(TIndex Index_)
		{
			auto itQuest = _Quests.find(Index_);
			if (itQuest == _Quests.end())
				return false;

			return (itQuest->second.empty());
		}
	}
}