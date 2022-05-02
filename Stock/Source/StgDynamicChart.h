#pragma once


#include "Stg.h"


namespace rso
{
	namespace stock
	{
		//class CStgDynamicChart : public CStg<0, 0>
		//{
		//	static const size_t c_BaseOutIndexCnt = 0; // 0 보다 클 경우 _GetPriceLevel 파라미터에 OutChart 의 시작, 종료 인덱스 넣을것.
		//	static const size_t c_ChartOutIndexCnt = 17;
		//	static const size_t c_OutIndexCnt = (c_BaseOutIndexCnt + c_ChartOutIndexCnt);
		//	static const size_t c_ChartInIndexUnitCnt = 2;
		//	static const size_t c_ChartInIndexCnt = 17;
		//	static const size_t c_BaseInIndexCnt = 0;

		//	using _TNeuron = CNeuron<1, c_OutIndexCnt>;

		//	template<size_t InDataCnt_>
		//	class CNeuronTuple : public CNeuronTuple<InDataCnt_ - 1>
		//	{
		//	public:
		//		static const size_t c_InIndexCnt = c_BaseInIndexCnt + c_ChartInIndexUnitCnt * InDataCnt_;
		//		using TType = CNeuron<c_InIndexCnt, c_OutIndexCnt>;
		//		TType	Neuron;

		//		CNeuronTuple(_TNeuron::TRemindCallback Callback_, const double MaxDistance_, size_t SampleCnt_) :
		//			CNeuronTuple<InDataCnt_ - 1>(Callback_, MaxDistance_, SampleCnt_),
		//			Neuron(Callback_, sqrt(MaxDistance_ * double(c_InIndexCnt)), SampleCnt_)
		//		{
		//		}
		//	};
		//	template<>
		//	class CNeuronTuple<1>
		//	{
		//	public:
		//		static const size_t c_InIndexCnt = c_BaseInIndexCnt + c_ChartInIndexUnitCnt;
		//		using TType = CNeuron<c_InIndexCnt, c_OutIndexCnt>;
		//		TType	Neuron;

		//		CNeuronTuple(_TNeuron::TRemindCallback Callback_, const double MaxDistance_, size_t SampleCnt_) :
		//			Neuron(Callback_, sqrt(MaxDistance_ * double(c_InIndexCnt)), SampleCnt_)
		//		{
		//		}
		//	};

		//	template<size_t Num_, typename TNeurons>
		//	auto&	Get(TNeurons& Neurons_)
		//	{
		//		return ((CNeuronTuple<Num_ + 1>&)Neurons_).Neuron;
		//	}
		//	template<size_t Num_>
		//	class Neuron_Element
		//	{
		//	public:
		//		using TType = CNeuron<c_BaseInIndexCnt + c_ChartInIndexUnitCnt * (Num_ + 1), c_OutIndexCnt>;
		//	};

		//	struct _SOutInfo
		//	{
		//		double High = 0.0;
		//		double Low = 0.0;
		//		double PriceLevel = 0.0;
		//		double End = 0.0;
		//		double EndSD = 0.0;

		//		_SOutInfo()
		//		{}
		//		_SOutInfo(double High_, double Low_, double PriceLevel_, double End_, double EndSD_) :
		//			High(High_), Low(Low_), PriceLevel(PriceLevel_), End(End_), EndSD(EndSD_)
		//		{}
		//		_SOutInfo operator += (const _SOutInfo& Data_)
		//		{
		//			High += Data_.High;
		//			Low += Data_.Low;
		//			PriceLevel += Data_.PriceLevel;
		//			End += Data_.End;
		//			EndSD += Data_.EndSD;

		//			return *this;
		//		}
		//		_SOutInfo operator + (const _SOutInfo& Data_) const
		//		{
		//			_SOutInfo Info(*this);
		//			return (Info += Data_);
		//		}
		//		_SOutInfo operator -= (const _SOutInfo& Data_)
		//		{
		//			High -= Data_.High;
		//			Low -= Data_.Low;
		//			PriceLevel -= Data_.PriceLevel;
		//			End -= Data_.End;
		//			EndSD -= Data_.EndSD;

		//			return *this;
		//		}
		//		_SOutInfo operator - (const _SOutInfo& Data_) const
		//		{
		//			_SOutInfo Info(*this);
		//			return (Info -= Data_);
		//		}
		//		_SOutInfo operator /= (size_t Value_)
		//		{
		//			High /= Value_;
		//			Low /= Value_;
		//			PriceLevel /= Value_;
		//			End /= Value_;
		//			EndSD /= Value_;

		//			return *this;
		//		}
		//		_SOutInfo operator / (size_t Value_) const
		//		{
		//			_SOutInfo Info(*this);
		//			return (Info /= Value_);
		//		}
		//	};
		//public:
		//	using TOut = _TNeuron::TOut;
		//	using TSMADynamic = CSMADynamic<SPriceVolume>;
		//	using THighLow = CRange<TValue>;
		//private:
		//	using _TCallback = function<void(time_t TimeT_, TValue Cnt_)>;

		//	static const double c_EnterHighLowGap;
		//	static const double c_EnterPriceLevel;
		//	static const double c_LeaveHighLowGap;
		//	static const double c_LeavePriceLevel;

		//	CSMADynamic<SDataDb> _SMADynamicOut{
		//		1, 2, 4, 9, 16,
		//		30, 30,
		//		30, 30,
		//		30, 30,
		//		30, 30,
		//		30, 30,
		//		30, 30 };
		//	TSMADynamic _SMADynamicIn{
		//		30, 30,
		//		30, 30,
		//		30, 30,
		//		30, 30,
		//		30, 30,
		//		30, 30,
		//		16, 9, 4, 2, 1 };
		//	TValue		_PrevDayClose = 0;
		//	TValue		_CurDayOpen = 0;
		//	CNeuronTuple<c_ChartInIndexCnt>	_Neurons;
		//	SDataDb		_InData;
		//	CSMADynamic<_SOutInfo> _SMAOutInfo{ 3 };
		//	_TCallback	_Callback;

		//	bool		_PushIn(const SPriceVolume& Data_);
		//	_SOutInfo	_GetOutInfo(size_t DataCnt_, const TOut& OutMean_, const TOut& OutSD_) const;
		//	void		_NeuronCallback(size_t DataCnt_, const TOut& OutMean_, const TOut& OutSD_);
		//	void		_ChangeDate(void);
		//	template<typename _TIn>
		//	void	_GetInCore(_TIn& In_)
		//	{
		//		size_t InIndex = 0;
		//		//In_[InIndex++] = _DayData.High() - _DayData.Low();
		//		//In_[InIndex++] = GetLevel(_DayData.High(), _DayData.Low(), _DayData.CurData().Price());

		//		for (size_t i = 0; i < _SMADynamicIn.size(); ++i)
		//		{
		//			//In_[InIndex++] = _SMADynamicIn.reverse_at(i).Price - _PrevDayClose;
		//			In_[InIndex++] = _SMADynamicIn.reverse_at(i).Price - _DayData.CurData().Price();
		//			In_[InIndex++] = _SMADynamicIn.reverse_at(i).Volume;
		//		}
		//	}
		//	template<size_t Num_>
		//	void	_Remind(void)
		//	{
		//		Neuron_Element<Num_ - 1>::TType::TIn In{};
		//		_GetInCore(In);
		//		Get<Num_ - 1>(_Neurons).Remind(std::move(In));
		//	}
		//	template<size_t Num_>
		//	void	_Input(TOut&& Out_)
		//	{
		//		Neuron_Element<Num_ - 1>::TType::TIn In{};
		//		_GetInCore(In);
		//		Get<Num_ - 1>(_Neurons).Input(std::move(In), std::move(Out_));
		//	}

		//public:
		//	CStgDynamicChart(TValue TradeCntUnit_, TValue DayTradeCntMax_, _TCallback Callback_, const double MaxDistance_, size_t SampleCnt_);
		//	void Remind(const SDataDb& Data_) override;
		//	void Input(const vector<SDataDb>& Datas_) override;
		//	void InputAbnormal(const vector<SDataDb>& Datas_) override;
		//	void ClearRemind(void) override;
		//	void clear(void) override;
		//};
	}
}
