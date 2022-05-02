#pragma once

#include "Base.h"
#include "Protocol.h"

namespace rso::brain
{
	class CNeuron
	{
	protected:
		double _Input = 0.0;
		double _Bias = 0.0; // Input Neuron 도 Bias를 갖도록 하고 입력은 _Input으로 받는것이 결과가 안정적으로 나옴... 왜?
		double _Sum = 0.0;
		double _Output = 0.0; // 디버깅 끝나면 제거할것.
		double _Error = 0.0;
		double _LearningRate = 1.0;
		vector<unique_ptr<CSynapse>> _NextSynapses;
		vector<CSynapse*> _PrevSynapses;

	public:
		inline void AddInput(double Input_) noexcept
		{
			_Input += Input_;
		}
		void Link(size_t PrevLayerSize_, size_t NextLayerSize_, CNeuron* Neuron_);
		inline void AddError(double Error_) noexcept
		{
			_Error += Error_;
		}
		inline double GetOutput(void) const noexcept
		{
			return _Output;
		}
		inline void SetOutput(double Output_) noexcept
		{
			_Output = Output_;
		}
		inline void SetTarget(double Target_, double LearningRate_) noexcept
		{
			_Error = (Target_ - _Output) * LearningRate_;
		}
		inline double GetError(void) const noexcept
		{
			return _Error;
		}
		inline const vector<unique_ptr<CSynapse>>& GetNextSynapses(void) const noexcept
		{
			return _NextSynapses;
		}
		void SetBiasAndWeights(double Bias_, const initializer_list<double>& Weights_);
		SNeuron GetSNeuron(void) const;
		void PrintBiasAndWeights(ostream& o_) const;
		virtual void FeedForward(double Alpha_);
		virtual void FeedBackward(double Alpha_);
	};
}
