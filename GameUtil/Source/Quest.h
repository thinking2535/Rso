#pragma once


#include "Base.h"


namespace rso
{
	namespace gameutil
	{
		class CQuest
		{
		public:
			using TIndex = int64;
			struct SQuest
			{
				TIndex QuestIndex = 0;
				TIndex RequirementIndex = 0;
				int32 RequireCnt = 0;

				SQuest(TIndex QuestIndex_, TIndex RequirementIndex_, int32 RequireCnt_) :
					QuestIndex(QuestIndex_), RequirementIndex(RequirementIndex_), RequireCnt(RequireCnt_)
				{
				}
			};
			using TQuests = list<SQuest>;
			using TRequirements = map<TIndex, int32>;

		private:
			using _TQuests = map<TIndex, TRequirements>;

			_TQuests _Quests;

		public:
			bool Add(const TQuests& Quests_);
			bool Add(TIndex QuestIndex_, TIndex RequirementIndex_, int32 RequirementCnt_);
			bool Add(TIndex QuestIndex_, const TRequirements& Requirements_);
			bool CompleteRequirement(TIndex QuestIndex_, TIndex RequirementIndex_, int32 RequirementCnt_); // return  true : Quest is Completed
			bool IsCompleted(TIndex Index_);
		};
	}
}