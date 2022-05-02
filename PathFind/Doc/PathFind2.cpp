#include "PathFind.h"
#include <iostream>
using namespace std;

namespace rso
{
	namespace pathfind
	{
		bool CPathFind::_CanProceedNorth(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const
		{
			if (Coord_.Y + 1 >= _Map[Coord_.X].size())
				return false;

			Dir_ = 0;
			ToCoord_.X = Coord_.X;
			ToCoord_.Y = Coord_.Y + 1;

			return true;
		}
		bool CPathFind::_CanProceedEast(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const
		{
			if (Coord_.X + 1 >= _Map.size())
				return false;

			Dir_ = 1;
			ToCoord_.X = Coord_.X + 1;
			ToCoord_.Y = Coord_.Y;

			return true;
		}
		bool CPathFind::_CanProceedSouth(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const
		{
			if (Coord_.Y <= 0)
				return false;

			Dir_ = 2;
			ToCoord_.X = Coord_.X;
			ToCoord_.Y = Coord_.Y - 1;

			return true;
		}
		bool CPathFind::_CanProceedWest(const SCoord& Coord_, __int8& Dir_, SCoord& ToCoord_) const
		{
			if (Coord_.X <= 0)
				return false;

			Dir_ = 3;
			ToCoord_.X = Coord_.X - 1;
			ToCoord_.Y = Coord_.Y;

			return true;
		}
		bool CPathFind::_Trace(const _STrace& Trace_)
		{
			if (!_CanProceeds[Trace_.Dir](Trace_.Coord, _CurDir, _CurCoord))
				return false;

			if (_CurCoord == _From)
			{
				for (auto i = Trace_.Coord; i != _To; i = _GetNode(i).PrevCoord)
					_Path.emplace_back(i);

				return true;
			}

			auto& Node = _GetNode(_CurCoord);

			if (Node.Obstacle || Node.Index == _Index)
				return false;

			Node.Index = _Index;
			Node.PrevCoord = Trace_.Coord;

			아래 _CurPoint 가 아니고 다음 Coord 이도록? 아니면 _CurTraceIt 이 맞나?
			_Traces.emplace(WeightSum + _CurPoint.GetDistance(_From), _STrace(_CurTraceIt->second.WeightSum, _CurPoint)); _CurTraceIt 는 null 일 수 있음.
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
		CPathFind::TPath&& CPathFind::Find(const SPoint& From_, const SPoint& To_)
		{
			_From = SCoord(From_);
			_To = SCoord(To_);

			if (_From == _To)
				return std::move(_Path);

			if (_Index + 1 < _Index)
			{
				for (auto& Row : _Map)
					for (auto& Cel : Row)
						Cel.Index = 0;
			}

			++_Index;
			_Traces.clear();

			if (_Trace(_STrace(0.0, _To, 0))) return std::move(_Path);
			if (_Trace(_STrace(0.0, _To, 1))) return std::move(_Path);
			if (_Trace(_STrace(0.0, _To, 2))) return std::move(_Path);
			if (_Trace(_STrace(0.0, _To, 3))) return std::move(_Path);

			for (auto it = _Traces.begin(); it != _Traces.end(); it = _Traces.begin())
			{
				Print();

				if (_Trace(it->second)) return _Path;
				if (_Trace(it->second.GetTurnDirLeft())) return _Path;
				if (_Trace(it->second.GetTurnDirRight())) return _Path;

				직	, Prev.Prev  를 설정
				우 , 재우 위치에 Obs 이면 Prev 는 현재 아니면 Prev.Prev  굴절노드마다 Point 및 Prev 설정할것.
				좌 , 재좌 위치에 Obs 이면 Prev 는 현재 아니면 Prev.Prev
					만 시도할것.

				_Traces.erase(it);
			}

			return _Path;
		}




		void CPathFind::Print(void) const
		{
			return;
			const size_t g_Width = 10;
			const size_t g_Height = 10;

			char m[g_Height][g_Width + 1];

			for (size_t h = 0; h < g_Height; ++h)
			{
				for (size_t w = 0; w < g_Width; ++w)
					m[h][w] = '0';

				m[h][g_Width] = '\0';
			}

			for (auto& i : _Traces)
				m[i.second.Point.Y][i.second.Point.X] = '*';

			for (size_t h = g_Height; h > 0; --h)
				cout << m[h - 1] << endl;
		}
	}
}
