#pragma once

#include "Base.h"

namespace rso::neuralnetwork
{
	class CNeuron
	{
		friend CSynapse;
		friend CNeuronGroup;
		friend CNeuronGroupUnionD;
		friend CNeuronGroupUnionW;

		double _LearningRate = 1.0; // 뉴런의 학습률은 직전에 연결된 시냅스의 개수 + 1 에 비례하므로 각 뉴런에 종속 (전역 학습률은 별도)

	protected:
		double _Bias = 0.0; // 입력단은 학습하지 않음
		double _Input = 0.0; // 학습시 미분함수에 쓰임
		double _Output = 0.0;
		double _Error = 0.0;
		vector<unique_ptr<CSynapse>> _PrevSynapses;

		void Link(const TLayer& PrevLayer_);
		void FeedForward(void);
		void FeedBackward(void);
		inline void AddError(double Error_) noexcept
		{
			_Error += Error_;
		}
		void CopyTo(CNeuron* Neuron_);

	public:
		inline void SetOutput(double Output_) noexcept
		{
			_Output = Output_;
		}
		inline double GetOutput(void) const noexcept
		{
			return _Output;
		}
		inline void SetTarget(double Target_) noexcept
		{
			_Error = (Target_ - _Output) * c_GlobalLearningRate;
		}
	};
}
