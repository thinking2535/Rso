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

		double _LearningRate = 1.0; // ������ �н����� ������ ����� �ó����� ���� + 1 �� ����ϹǷ� �� ������ ���� (���� �н����� ����)

	protected:
		double _Bias = 0.0; // �Է´��� �н����� ����
		double _Input = 0.0; // �н��� �̺��Լ��� ����
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
