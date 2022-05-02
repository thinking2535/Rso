#pragma once

#include "Physics.h"
#include <Rso/Base/List.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace rso::physics
{
	using namespace std;
	using namespace placeholders;
	using namespace base;

	template<typename _TObject>
	class CSight
	{
	private:
		struct _SCellPos
		{
			int32 X = 0;
			int32 Y = 0;

			_SCellPos(int32 X_, int32 Y_) :
				X(X_), Y(Y_)
			{
			}
			bool operator == (const _SCellPos& rhs_) const
			{
				return (X == rhs_.X && Y == rhs_.Y);
			}
		};
		struct map_hash_func
		{
			size_t operator()(const _SCellPos& Value_) const
			{
				return (((size_t)Value_.X << 32) + Value_.Y);
			}
		};

		struct _SObject;
		using _TObjects = CList<_SObject>;
	public:
		using iterator = typename _TObjects::iterator;

		struct set_hash_func
		{
			size_t operator()(const iterator& Value_) const
			{
				return Value_.Index();
			}
		};

		using iterators = unordered_set<iterator, set_hash_func>;
	private:
		struct _SCell
		{
			uint64 Counter = 0;
			iterators Seers;
			iterators Blinds;
		};
		using _TCells = unordered_map<_SCellPos, _SCell, map_hash_func>;
		using _TCellsIt = typename _TCells::iterator;
		struct _SObject
		{
			bool CanSee = false;
			SPoint Pos;
			_SCellPos CellPos; // float 오차로 매번 계산할 경우 CellPos가 달라질 수 있다고 가정하고 저장
			float Radius = 0.0f;
			iterators InSightObjects;
			_TObject Object{};

			_SObject(bool CanSee_, const SPoint& Pos_, const _SCellPos& CellPos_, float Radius_, const _TObject& Object_) :
				CanSee(CanSee_), Pos(Pos_), CellPos(CellPos_), Radius(Radius_), Object(Object_)
			{}
		};
	public:
		using TSightProc = function<void(iterator it_, iterator Target_)>;

	private:
		float _MaxRadius = 0.0f;
		float _Error = 0.2f;
		TSightProc _InSightCallback;
		TSightProc _OutSightCallback;
		_TObjects _Objects;
		_TCells _Cells;
		uint64 _Counter = 0;

		int32 _CellCoord(float Value_)
		{
			if (Value_ >= 0.0)
				return int32(Value_ / _MaxRadius);
			else
				return int32(Value_ / _MaxRadius) - 1;
		}
		_SCellPos _CellPos(const SPoint& Pos_)
		{
			return _SCellPos(_CellCoord(Pos_.X), _CellCoord(Pos_.Y));
		}
		bool _CanSee(iterator it_, iterator itTarget_)
		{
			return (it_->Radius >= Distance(it_->Pos, itTarget_->Pos));
		}
		bool _CanNotSee(iterator it_, iterator itTarget_)
		{
			return (it_->Radius * (1.0 + _Error) < Distance(it_->Pos, itTarget_->Pos));
		}
		void _InSightProc(iterator it_, iterator itTarget_)
		{
			if (!_CanSee(it_, itTarget_))
				return;

			it_->InSightObjects.emplace(itTarget_);
			_InSightCallback(it_, itTarget_);
		}
		void _OutSightProc(iterator it_, iterator itTarget_)
		{
			auto itTarget = it_->InSightObjects.find(itTarget_);
			if (itTarget != it_->InSightObjects.end())
			{
				it_->InSightObjects.erase(itTarget);
				_OutSightCallback(it_, itTarget_);
			}
		}
		void _SightProc(iterator it_, iterator itTarget_)
		{
			if (_CanSee(it_, itTarget_))
			{
				if (it_->InSightObjects.emplace(itTarget_).second)
					_InSightCallback(it_, itTarget_);
			}
			else if (_CanNotSee(it_, itTarget_))
			{
				auto itTarget = it_->InSightObjects.find(itTarget_);
				if (itTarget != it_->InSightObjects.end())
				{
					it_->InSightObjects.erase(itTarget);
					_OutSightCallback(it_, itTarget_);
				}
			}
		}
		void _InSightProcCell(iterator it_, const _SCell& Cell_) // call when insert
		{
			if (it_->CanSee)
			{
				for (auto& it : Cell_.Seers)
				{
					if (it.Index() == it_.Index()) // it's me
						continue;

					_InSightProc(it_, it);
					_InSightProc(it, it_);
				}

				for (auto& it : Cell_.Blinds)
					_InSightProc(it_, it);
			}
			else
			{
				for (auto& it : Cell_.Seers)
					_InSightProc(it, it_);
			}
		}
		void _OutSightProcCell(iterator it_, const _SCell& Cell_, bool TotallyOut_) // call when erase
		{
			for (auto& it : Cell_.Seers)
			{
				if (!TotallyOut_)
					_OutSightProc(it_, it);

				_OutSightProc(it, it_);
			}

			if (!TotallyOut_ && it_->CanSee)
			{
				for (auto& it : Cell_.Blinds)
					_OutSightProc(it_, it);
			}
		}
		void _SightProcCell(iterator it_, const _SCell& Cell_)
		{
			if (it_->CanSee)
			{
				for (auto& it : Cell_.Seers)
				{
					if (it.Index() == it_.Index()) // it's me
						continue;

					_SightProc(it_, it);
					_SightProc(it, it_);
				}

				for (auto& it : Cell_.Blinds)
					_SightProc(it_, it);
			}
			else
			{
				for (auto& it : Cell_.Seers)
					_SightProc(it, it_);
			}
		}
		void _ChangeInProcCore(iterator it_, _TCellsIt itCell_)
		{
			if (itCell_->second.Counter == _Counter - 1)
			{
				itCell_->second.Counter = _Counter;
				_SightProcCell(it_, itCell_->second);
			}
			else
			{
				_InSightProcCell(it_, itCell_->second);
			}
		}
		void _ChangeInProc(iterator it_, const _SCellPos& CellPos_)
		{
			auto itCell = _Cells.find(CellPos_);
			if (itCell == _Cells.end())
				return;

			_ChangeInProcCore(it_, itCell);
		}
		void _ChangeOutProcCore(iterator it_, _TCellsIt itCell_)
		{
			if (itCell_->second.Counter == _Counter - 1)
				_OutSightProcCell(it_, itCell_->second, false);
		}
		void _ChangeOutProc(iterator it_, const _SCellPos& CellPos_)
		{
			auto itCell = _Cells.find(CellPos_);
			if (itCell == _Cells.end())
				return;

			if (itCell->second.Counter == _Counter - 1)
				_OutSightProcCell(it_, itCell->second, false);
		}
		_TCellsIt _InToCell(iterator it_, const _SCellPos& CellPos_)
		{
			try
			{
				auto ib = _Cells.emplace(CellPos_, _SCell());

				try
				{
					if (it_->CanSee)
						ib.first->second.Seers.emplace(it_); // _Objects 에 삽입 성공했다면 itObject.Index()가 고유할 것이므로 여기도 성공한다고 간주
					else
						ib.first->second.Blinds.emplace(it_); // _Objects 에 삽입 성공했다면 itObject.Index()가 고유할 것이므로 여기도 성공한다고 간주
				}
				catch (...)
				{
					if (ib.second) // 처음에 Cell이 없었으면 다시 제거
					{
						_Cells.erase(ib.first);
						return _Cells.end();
					}
				}

				return ib.first;
			}
			catch (...)
			{
				return _Cells.end();
			}
		}
		_TCellsIt _OutFromCell(iterator it_, const _SCellPos& CellPos_, bool TotallyOut_)
		{
			auto itCell = _Cells.find(CellPos_);
			if (itCell == _Cells.end())
				return _Cells.end();

			if (it_->CanSee)
				itCell->second.Seers.erase(it_);
			else
				itCell->second.Blinds.erase(it_);

			if (itCell->second.Seers.empty() &&
				itCell->second.Blinds.empty())
			{
				_Cells.erase(itCell);
				return _Cells.end();
			}

			return itCell;
		}

	public:
		CSight(float MaxRadius_, TSightProc InSightCallback_, TSightProc OutSightCallback_) :
			_MaxRadius(MaxRadius_),
			_InSightCallback(InSightCallback_),
			_OutSightCallback(OutSightCallback_)
		{
		}
		iterator insert(bool CanSee_, const SPoint& Pos_, float Radius_, const _TObject& Object_)
		{
			auto CellPos = _CellPos(Pos_);

			auto itObject = _Objects.emplace(CanSee_, Pos_, CellPos, Radius_, Object_);
			auto itCell = _InToCell(itObject, CellPos);
			if (itCell == _Cells.end())
			{
				_Objects.erase(itObject);
				THROWEX();
			}

			try
			{
				_InSightProcCell(itObject, itCell->second);
				++CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
				--CellPos.Y; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
				--CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
				--CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
				++CellPos.Y; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
				++CellPos.Y; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
				++CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
				++CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _InSightProcCell(itObject, itCell->second);
			}
			catch (...)
			{
			}

			return itObject;
		}
		void erase(iterator it_)
		{
			auto CellPos = it_->CellPos;
			auto itCell = _OutFromCell(it_, CellPos, true);

			if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			++CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			--CellPos.Y; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			--CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			--CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			++CellPos.Y; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			++CellPos.Y; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			++CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);
			++CellPos.X; itCell = _Cells.find(CellPos); if (itCell != _Cells.end()) _OutSightProcCell(it_, itCell->second, true);

			_Objects.erase(it_);
		}
		void change(iterator it_, const SPoint& Pos_, float Radius_)
		{
			if (Distance(it_->Pos, Pos_) + sqrt((it_->Radius - Radius_) * (it_->Radius - Radius_)) < (Radius_ * _Error))
				return;

			auto NewCellPos = _CellPos(Pos_);

			try
			{
				if (NewCellPos == it_->CellPos)
				{
					it_->Pos = Pos_;

					auto itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					++NewCellPos.X; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					--NewCellPos.Y; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					--NewCellPos.X; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					--NewCellPos.X; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					++NewCellPos.Y; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					++NewCellPos.Y; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					++NewCellPos.X; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
					++NewCellPos.X; itCell = _Cells.find(NewCellPos); if (itCell != _Cells.end()) _SightProcCell(it_, itCell->second);
				}
				else
				{
					auto itNewCell = _InToCell(it_, NewCellPos);
					if (itNewCell == _Cells.end())
						return;

					auto itOldCell = _OutFromCell(it_, it_->CellPos, false);

					auto OldCellPos = it_->CellPos;
					it_->Pos = Pos_;
					it_->CellPos = NewCellPos;

					// OldPos 9Cells 과 NewPos 9Cells 와 비교하여
					//	중복 Cells에는 SightProcCell 처리
					//	NewCells 에는 InSightProcCell 처리
					//	OldCells 에는 OutSightProcCell 처리
					// 처리 전에 _Counter 가 0 이었다면
					//	OldCells 에는 1
					//	중복 Cells에는 2 가 채워지게 됨.


					// Reset Counter (prevent overflow)
					if (_Counter + 2 < _Counter)
					{
						for (auto& Cell : _Cells)
							Cell.second.Counter = 0;

						_Counter = 0;
					}

					++_Counter;
					if (itOldCell != _Cells.end()) itOldCell->second.Counter = _Counter;
					++OldCellPos.X; auto itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;
					--OldCellPos.Y; itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;
					--OldCellPos.X; itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;
					--OldCellPos.X; itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;
					++OldCellPos.Y; itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;
					++OldCellPos.Y; itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;
					++OldCellPos.X; itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;
					++OldCellPos.X; itCell = _Cells.find(OldCellPos); if (itCell != _Cells.end()) itCell->second.Counter = _Counter;

					++_Counter;

					_ChangeInProcCore(it_, itNewCell);
					++NewCellPos.X; _ChangeInProc(it_, NewCellPos);
					--NewCellPos.Y; _ChangeInProc(it_, NewCellPos);
					--NewCellPos.X; _ChangeInProc(it_, NewCellPos);
					--NewCellPos.X; _ChangeInProc(it_, NewCellPos);
					++NewCellPos.Y; _ChangeInProc(it_, NewCellPos);
					++NewCellPos.Y; _ChangeInProc(it_, NewCellPos);
					++NewCellPos.X; _ChangeInProc(it_, NewCellPos);
					++NewCellPos.X; _ChangeInProc(it_, NewCellPos);

					if (itOldCell != _Cells.end()) _ChangeOutProcCore(it_, itOldCell);
					--OldCellPos.Y; _ChangeOutProc(it_, OldCellPos);
					--OldCellPos.Y; _ChangeOutProc(it_, OldCellPos);
					--OldCellPos.X; _ChangeOutProc(it_, OldCellPos);
					--OldCellPos.X; _ChangeOutProc(it_, OldCellPos);
					++OldCellPos.Y; _ChangeOutProc(it_, OldCellPos);
					++OldCellPos.Y; _ChangeOutProc(it_, OldCellPos);
					++OldCellPos.X; _ChangeOutProc(it_, OldCellPos);
					++OldCellPos.X; _ChangeOutProc(it_, OldCellPos);
				}
			}
			catch (...)
			{
			}
		}
	};
}