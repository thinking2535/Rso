#pragma once

#include "Base.h"

namespace rso
{
	namespace gameutil
	{
		template<typename TData>
		class CEquip
		{
		public:
			using TIndex = int64;
			using TSlot = int32;
			using TCapacity = int32;
			using TSlotInfos = map<TSlot, TCapacity>;
			struct SEquipInfo
			{
				TCapacity Occupy = 0;
				const TData& Data;

				SEquipInfo(TCapacity Occupy_, const TData& Data_) :
					Occupy(Occupy_), Data(Data_)
				{}
			};
			using TEquipInfos = map<TIndex, SEquipInfo>;
			using TUnEquippedInfos = vector<typename TEquipInfos::value_type>;

		private:
			class _CSlotInfo
			{
			private:
				TCapacity _Capacity = 0;
				TCapacity _Occupied = 0;
				TEquipInfos _EquipInfos;

				bool _CanEquipCore(TCapacity Occupy_, TUnEquippedInfos& UnEquippedInfos_) const
				{
					if (_Capacity < Occupy_)
						return false;

					if (_Capacity < _Occupied + Occupy_)
					{
						try
						{
							auto Occupied = _Occupied;

							for (auto& Info : _EquipInfos)
							{
								if (_Capacity - Occupied >= Occupy_)
									break;

								UnEquippedInfos_.emplace_back(Info);
								Occupied -= Info.second.Occupy;
							}
						}
						catch (...)
						{
							return false;
						}
					}

					return true;
				}

			public:
				_CSlotInfo(TCapacity Capacity_) :
					_Capacity(Capacity_)
				{}
				bool CanEquip(TIndex Index_, TCapacity Occupy_, TUnEquippedInfos& UnEquippedInfos_) const
				{
					if (_EquipInfos.find(Index_) != _EquipInfos.end())
						return false;

					return _CanEquipCore(Occupy_, UnEquippedInfos_);
				}
				bool Equip(TIndex Index_, TCapacity Occupy_, const TData& Data_, TUnEquippedInfos& UnEquippedInfos_)
				{
					if (!_CanEquipCore(Occupy_, UnEquippedInfos_))
						return false;

					try
					{
						if (!_EquipInfos.emplace(Index_, SEquipInfo(Occupy_, Data_)).second)
							return false;

						_Occupied += Occupy_;

						for (auto& Info : UnEquippedInfos_)
						{
							_EquipInfos.erase(Info.first);
							_Occupied -= Info.second.Occupy;
						}
					}
					catch (...)
					{
						return false;
					}

					return true;
				}
				bool CanUnEquip(TIndex Index_)
				{
					return (_EquipInfos.find(Index_) != _EquipInfos.end());
				}
				bool UnEquip(TIndex Index_)
				{
					auto itInfo = _EquipInfos.find(Index_);
					if (itInfo == _EquipInfos.end())
						return false;

					_Occupied -= itInfo->second.Occupy;
					_EquipInfos.erase(itInfo);

					return true;
				}
			};
			using _TSlotInfos = map<TSlot, _CSlotInfo>;

			_TSlotInfos _SlotInfos;
			TUnEquippedInfos _UnEquippedInfos;

		public:
			CEquip(const TSlotInfos& SlotInfos_)
			{
				for (auto& Info : SlotInfos_)
					_SlotInfos.emplace(Info.first, _CSlotInfo(Info.second));
			}
			// TODO : 아래 함수 추가하려면 슬롯 용량 줄이거나 제거시 UnEquip 될 항목 처리할것.
			//bool CanAddSlot(TSlot Slot_)
			//{
			//	return (_SlotInfos.find(Slot_) == _SlotInfos.end());
			//}
			//bool AddSlot(TSlot Slot_, TCapacity Capacity_) // throw
			//{
			//	return _SlotInfos.emplace(Slot_, _CSlotInfo(Capacity_)).second;
			//}
			//void SubSlot(TSlot Slot_)
			//{
			//	_SlotInfos.erase(Slot_);
			//}
			//bool CanChangeSlot(TSlot Slot_, TCapacity Capacity_)
			//{
			//	return (_SlotInfos.find(Slot_) != _SlotInfos.end());
			//}
			//bool ChangeSlot(TSlot Slot_, TCapacity Capacity_) // throw
			//{
			//	auto it = _SlotInfos.find(Slot_);
			//	if (it == _SlotInfos.end())
			//		return false;

			//	it->second.
			//}
			const TUnEquippedInfos& UnEquippedInfos(void) const
			{
				return _UnEquippedInfos;
			}
			bool CanEquip(TIndex Index_, TSlot Slot_, TCapacity Capacity_)
			{
				auto it = _SlotInfos.find(Slot_);
				if (it == _SlotInfos.end())
					return false;

				_UnEquippedInfos.clear();

				return it->second.CanEquip(Index_, Capacity_, _UnEquippedInfos);
			}
			bool Equip(TIndex Index_, TSlot Slot_, TCapacity Capacity_, const TData& Data_)
			{
				auto it = _SlotInfos.find(Slot_);
				if (it == _SlotInfos.end())
					return false;

				_UnEquippedInfos.clear();

				return it->second.Equip(Index_, Capacity_, Data_, _UnEquippedInfos);
			}
			bool CanUnEquip(TSlot Slot_, TIndex Index_)
			{
				auto it = _SlotInfos.find(Slot_);
				if (it == _SlotInfos.end())
					return false;

				return it->second.CanUnEquip(Index_);
			}
			bool UnEquip(TSlot Slot_, TIndex Index_)
			{
				auto it = _SlotInfos.find(Slot_);
				if (it == _SlotInfos.end())
					return false;

				return it->second.UnEquip(Index_);
			}
		};
	}
}