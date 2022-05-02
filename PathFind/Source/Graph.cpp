#include "Graph.h"

namespace rso
{
	namespace pathfind
	{
		CGraph::_TNodesIt CGraph::AddNode(const SPoint& Point_)
		{
			_Nodes.push_front(_SNode(Point_));
			return _Nodes.begin();
		}
		void CGraph::DelNode(_TNodesIt NodeIt_)
		{
			for (auto& i : NodeIt_->Links)
				i.first->Links.erase(&(*NodeIt_));

			_Nodes.erase(NodeIt_);
		}
		void CGraph::AddLink(_SNode* Node0_, _SNode* Node1_)
		{
			Node0_->Links.emplace(Node1_, Distance(Node0_->Point, Node1_->Point));
			Node1_->Links.emplace(Node0_, Distance(Node1_->Point, Node0_->Point));
		}
		void CGraph::DelLink(_SNode* Node0_, _SNode* Node1_)
		{
			Node0_->Links.erase(Node1_);
			Node1_->Links.erase(Node0_);
		}
		const CGraph::TPath& CGraph::Find(_SNode* From_, _SNode* To_)
		{
			if (From_ == To_)
				return _Path;

			if (_Index + 1 < _Index)
			{
				for (auto& i : _Nodes)
					i.Index = 0;
			}

			++_Index;
			_Path.clear();
			_Traces.clear();
			_Traces.emplace(0.0, _STrace(Distance(From_->Point, To_->Point), To_)); // To_ -> From_ (reverse tracking)
			To_->Index = _Index;

			for (auto it = _Traces.begin(); it != _Traces.end(); it = _Traces.begin())
			{
				for (auto& n : it->second.Node->Links)
				{
					if (n.first->Index == _Index)
						continue;

					n.first->Prev = it->second.Node;

					if (n.first == From_)
					{
						for (auto i = From_->Prev; i != To_; i = i->Prev)
							_Path.emplace_back(i->Point);

						return _Path;
					}

					auto WeightSum = it->second.WeightSum + n.second;
					_Traces.emplace(WeightSum + Distance(n.first->Point, From_->Point), _STrace(WeightSum, n.first));
					n.first->Index = _Index;
				}

				_Traces.erase(it);
			}

			return _Path;
		}
	}
}
