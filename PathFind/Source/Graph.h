#pragma once

#include "Base.h"

namespace rso
{
	namespace pathfind
	{
		class CGraph
		{
		public:
			using TPath = vector<SPoint>;

		private:
			struct _SNode
			{
				TIndex Index = 0;
				SPoint Point;
				map<_SNode*, double> Links; // second : weight
				_SNode* Prev = nullptr; // for returning path

				_SNode(const SPoint& Point_) :
					Point(Point_)
				{
				}
			};
			struct _STrace
			{
				double WeightSum = 0.0;
				_SNode* Node = nullptr;

				_STrace(double WeightSum_, _SNode* Node_) :
					WeightSum(WeightSum_), Node(Node_)
				{
				}
			};

			using _TNodes = list<_SNode>;
			using _TNodesIt = typename _TNodes::iterator;

		private:
			TIndex _Index = 0;
			_TNodes _Nodes;
			multimap<double, _STrace> _Traces; // first : total weight + last shortest weight
			TPath _Path;

		public:
			_TNodesIt AddNode(const SPoint& Point_);
			void DelNode(_TNodesIt NodeIt_);
			void AddLink(_SNode* Node0_, _SNode* Node1_);
			void DelLink(_SNode* Node0_, _SNode* Node1_);
			const TPath& Find(_SNode* From_, _SNode* To_);
		};
	}
}
