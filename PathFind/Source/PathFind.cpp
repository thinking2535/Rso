#include "PathFind.h"
#include <iostream>

using namespace std;

namespace rso
{
	namespace pathfind
	{
		bool CPathFind::_Trace(_TTracesIt It_, const SCoord& Coord_)
		{
			auto& Node = _GetNode(Coord_);

			if (Node.Obstacle || Node.Index == _Index)
				return false;

			Node.PrevCoord = It_->second.Coord;

			if (Coord_ == _From)
			{
				for (auto i = Node.PrevCoord; i != _To; i = _GetNode(i).PrevCoord)
					_Path.emplace_back(i);

				return true;
			}

			auto WeightSum = It_->second.WeightSum + 1.0;
			_Traces.emplace(WeightSum + Coord_.GetDistance(_From), _STrace(WeightSum, Coord_));
			Node.Index = _Index;

			return false;
		}
		CPathFind::CPathFind(__int32 Width_, __int32 Height_) :
			_Map(Width_, vector<_SNode>(Height_))
		{
		}
		void CPathFind::SetObstacle(__int32 X_, __int32 Y_, __int32 Width_, __int32 Height_, bool Obstacle_)
		{
			for (__int32 X = X_; X < X_ + Width_; ++X)
				for (__int32 Y = Y_; Y < Y_ + Height_; ++Y)
					_Map[X][Y].Obstacle = Obstacle_;
		}
		const TPath& CPathFind::Find(const SCoord& From_, const SCoord& To_)
		{
			if (From_ == To_)
				return _Path;

			_From = From_;
			_To = To_;

			if (_Index + 1 < _Index)
			{
				for (auto& Row : _Map)
					for (auto& Cel : Row)
						Cel.Index = 0;
			}

			++_Index;
			_Path.clear();
			_Traces.clear();
			_Traces.emplace(0.0, _STrace(From_.GetDistance(To_), To_)); // To_ -> From_ (reverse tracking)
			_GetNode(To_).Index = _Index;

			for (auto it = _Traces.begin(); it != _Traces.end(); it = _Traces.begin())
			{
				_GetNode(it->second.Coord).Index = _Index;

				if (it->second.Coord.X + 1 < _Map.size()) if (_Trace(it, it->second.Coord.GetRight())) return _Path;
				if (it->second.Coord.X - 1 >= 0) if (_Trace(it, it->second.Coord.GetLeft())) return _Path;
				if (it->second.Coord.Y + 1 < _Map[0].size()) if (_Trace(it, it->second.Coord.GetUp())) return _Path;
				if (it->second.Coord.Y - 1 >= 0) if (_Trace(it, it->second.Coord.GetDown())) return _Path;

				_Traces.erase(it);
			}

			return _Path;
		}
	}
}
