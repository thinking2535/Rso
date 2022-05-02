#pragma once

#include "Base.h"

namespace rso::neuralnetwork
{
	class SNeuron
	{
	protected:
		double _Input = 0.0; // 학습시 미분함수에 쓰임
		double _Bias = 0.0; // 입력단은 학습하지 않음
		double _Output = 0.0;
		double _Gradient = 0.0;

	public:
		inline void SetOutput(double Output_) noexcept
		{
			_Output = Output_;
		}
		inline double GetOutput(void) const noexcept
		{
			return _Output;
		}
		inline double GetGradient(void) const noexcept
		{
			return _Gradient;
		}
	};
	class CNeuron : public SNeuron
	{
	public:
		virtual void FeedForward(double Input_) noexcept = 0;
		virtual void FeedBackward(double Error_) noexcept = 0;
	};
}
