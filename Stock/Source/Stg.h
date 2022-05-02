//#pragma once
//
//
//#include "Asset.h"
//#include "SMADynamic.h"
//#include <functional>
//
//
//namespace rso
//{
//	namespace stock
//	{
//		using namespace std::placeholders;
//
//		template<size_t InDataCnt_, size_t OutDataCnt_>
//		class CStg : public CAsset
//		{
//			using _TNeuron = CNeuronDummy<STick, InDataCnt_, OutDataCnt_>;
//
//		public:
//			static const size_t InDataCnt = InDataCnt_;
//			using TOrderCallback = function<bool(const SOrder& Order_)>;
//			using TChangeCallback = function<bool(__int64 OrderNo_, const SOrder& Order_)>;
//			using TCancelCallback = function<bool(__int64 OrderNo_, TValue Volume_)>;
//			using TIn = typename _TNeuron::TIn;
//			using TOut = typename _TNeuron::TOut;
//			struct SDataInfo
//			{
//				bool IsBegin = false;
//				bool IsEnd = false;
//
//				SDataInfo()
//				{}
//				SDataInfo(bool IsBegin_, bool IsEnd_) :
//					IsBegin(IsBegin_), IsEnd(IsEnd_)
//				{}
//			};
//
//		private:
//			bool _SimulateServer = false;
//			CDateTime _TradeStartTimeStamp;
//			TOrderCallback _OrderCallback;
//			TChangeCallback _ChangeCallback;
//			TCancelCallback _CancelCallback;
//			SDataInfo _DataInfo;
//			TSqlUSmallInt _OldDay = 0;
//			CDateTime _LastCanTradeTime;
//			_TNeuron _Neuron;
//			time_t _NeuronTime = 0;
//			bool _RemainReady = false;
//			bool _CanTrade = false;
//			bool _Busy = false;
//			STick _CurTick;
//			TTime _CurTime;
//			/// Order Info //////////////////
//			// 나중에 Order 클래스에 넣을것.
//			//int _ChangedCount = 0;	// 진입 이후 정정된 회수
//			TTime _OrderBeginTime;
//			EOrderType _OrderType = EOrderType::Null;
//			/////////////////////////////////
//			TTime _TradedTime;
//			const TRmns* _Offers = nullptr;
//			const TRmns* _Bids = nullptr;
//
//			virtual bool _InputCallback(const STick& Data_, const SDataInfo& DataInfo_, TIn& In_) = 0;
//			virtual void _RemindCallback(const STick& Data_, const TRmns& Offers_, const TRmns& Bids_, const SDataInfo& DataInfo_, const TIn& In_, const TOut& OutMin_, const TOut& OutMax_) = 0;
//			virtual bool _MemoryCallback(const STick& Data_, TOut& Out_) = 0;
//			virtual void _Proc(void) = 0;
//
//			void _NeuronCallbackRaw(const TIn& In_, const TOut& OutMin_, const TOut& OutMax_)
//			{
//				if (_Busy)
//					return;
//
//				_RemindCallback(_Neuron.BackData(), *_Offers, *_Bids, _DataInfo, In_, OutMin_, OutMax_);
//			}
//
//		protected:
//		public: // 임시
//			SDataInfo DataInfo(void) const
//			{
//				return _DataInfo;
//			}
//			const STick& CurTick(void) const
//			{
//				return _CurTick;
//			}
//			TTime CurTime(void) const
//			{
//				return _CurTime;
//			}
//			TTime OrderBeginTime(void) const
//			{
//				return _OrderBeginTime;
//			}
//			EOrderType OrderType(void) const
//			{
//				return _OrderType;
//			}
//			//int ChangedCount(void) const
//			//{
//			//	return _ChangedCount;
//			//}
//			TTime TradedTime(void) const
//			{
//				return _TradedTime;
//			}
//			bool OrderBegin(const SOrder& Order_)
//			{
//				if (_Busy)
//					return false;
//
//				if (OrderVolume() != 0)
//					return false;
//
//				if (!_OrderCallback(Order_))
//					return false;
//
//				_OrderBeginTime = _CurTime;
//				_OrderType = Order_.Type;
//				//_ChangedCount = 0; // 이건 이후에 주문 건별로 처리 되어야 하므로 이곳에서는 제거하고 주문 건의 Volume 이 0 이 되면(즉, Trade되거나 Cancel 되어 Volume이 0 이 되면
//									// 그때 Order map에서 제거시키면 됨.
//				_Busy = true;
//
//				return true;
//			}
//			bool ChangeBegin(__int64 OrderNo_, const SOrder& Order_)
//			{
//				if (_Busy)
//					return false;
//
//				if (OrderVolume() == 0)
//					return false;
//
//				if (!_ChangeCallback(OrderNo_, Order_))
//					return false;
//
//				_OrderType = Order_.Type;
//				//++_ChangedCount; // Order map 에서 Count 증가시킬것.
//				_Busy = true;
//
//				return true;
//			}
//			bool CancelBegin(__int64 OrderNo_, TValue Volume_)
//			{
//				if (_Busy)
//					return false;
//
//				if (OrderVolume() == 0)
//					return false;
//
//				if (!_CancelCallback(OrderNo_, Volume_))
//					return false;
//
//				_Busy = true;
//
//				return true;
//			}
//
//		public:
//			CStg(
//				bool SimulateServer_,
//				size_t SampleCnt_,
//				time_t TimeGap_,
//				const CDateTime& TradeStartTimeStamp_,
//				TOrderCallback OrderCallback_,
//				TChangeCallback ChangeCallback_,
//				TCancelCallback CancelCallback_
//				) :
//				_SimulateServer(SimulateServer_),
//				_Neuron(std::bind(&CStg::_NeuronCallbackRaw, this, _1, _2, _3), SampleCnt_, TimeGap_),
//				_TradeStartTimeStamp(TradeStartTimeStamp_),
//				_OrderCallback(OrderCallback_),
//				_ChangeCallback(ChangeCallback_),
//				_CancelCallback(CancelCallback_)
//			{
//			}
//			bool IsSimulateServer(void) const
//			{
//				return _SimulateServer;
//			}
//			void SetRemainReady(bool RemainReady_)
//			{
//				_RemainReady = RemainReady_;
//			}
//			void Order(__int64 OrderNo_, TValue Price_, TValue Volume_)
//			{
//				CAsset::Order(OrderNo_, Price_, Volume_);
//				_Busy = false;
//			}
//			void Change(__int64 OrgOrderNo_, __int64 OrderNo_, TValue Price_, TValue Volume_)
//			{
//				CAsset::Change(OrgOrderNo_, OrderNo_, Price_, Volume_);
//				_Busy = false;
//			}
//			void Cancel(__int64 OrderNo_, TValue Volume_)
//			{
//				CAsset::Cancel(OrderNo_, Volume_);
//				_Busy = false;
//			}
//			void Trade(__int64 OrderNo_, TValue Price_, TValue Volume_)
//			{
//				CAsset::Trade(OrderNo_, Price_, Volume_);
//				_TradedTime = _CurTime;
//				_Busy = false;
//			}
//			void Input(const STick& Tick_, const TRmns& Offers_, const TRmns& Bids_) // 최초 호출
//			{
//				_CurTick = Tick_;
//				_CurTime = Tick_.InsertedTime.ToTimePoint();
//				++_NeuronTime;
//
//				bool IsBegin = false;
//				if (_OldDay != Tick_.InsertedTime.day)
//				{
//					_OldDay = Tick_.InsertedTime.day;
//					_LastCanTradeTime = LastCanTradeTime(Tick_.InsertedTime);
//					IsBegin = true;
//				}
//
//				bool IsEnd = false;
//				//if (Tick_.InsertedTime == _LastCanTradeTime) STick 의 경우 DB에 삽입되는 시간으로 처리되므로 정확한 Exchange의 시각도 아닐 뿐더러, Minute데이터가 아닌 Tick이므로 마지막 시간이 애매함.
//				//	IsEnd = true;
//
//				if (Tick_.InsertedTime < _TradeStartTimeStamp)
//					_CanTrade = false;
//				else
//					_CanTrade = true;
//
//				new(&_DataInfo)SDataInfo(IsBegin, IsEnd);
//
//				TIn In{};
//				if (!_InputCallback(Tick_, _DataInfo, In))
//					return;
//
//				if (!_RemainReady)
//					return;
//
//				if (_CanTrade)
//				{
//					_Offers = &Offers_;
//					_Bids = &Bids_;
//					_Neuron.Remind(_NeuronTime, Tick_, In);
//				}
//
//				TOut Out{};
//				if (!_MemoryCallback(Tick_, Out))
//					return;
//
//				_Neuron.Memory(_NeuronTime, Out);
//			}
//			const STick& FrontData(void) const
//			{
//				return _Neuron.FrontData();
//			}
//			const STick& BackData(void) const
//			{
//				return _Neuron.BackData();
//			}
//			const TIn& FrontIn(void) const
//			{
//				return _Neuron.FrontIn();
//			}
//			const TIn& BackIn(void) const
//			{
//				return _Neuron.BackIn();
//			}
//			const TOut& OutMin(void) const
//			{
//				return _Neuron.OutMin();
//			}
//			const TOut& OutMax(void) const
//			{
//				return _Neuron.OutMax();
//			}
//			void Proc(void)
//			{
//				if (_Busy)
//					return;
//
//				_Proc();
//			}
//		};
//	}
//}
