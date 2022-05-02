#pragma once

#include "Neuron.h"

namespace rso::neuralnetwork
{
	class CFunction abstract : public SNeuron
	{
	protected:
		double _InputWeight = GetRandom(1, 1);
		double _StateInputWeight = GetRandom(1, 1);

	public:
		virtual double FeedForward(double Input_, double StateInput_) = 0;
		virtual void FeedBackward(double Input_, double StateInput_, double Error_) = 0;
	};
	class CFunctionSigmoid : public CFunction
	{
	public:
		inline double FeedForward(double Input_, double StateInput_) noexcept override
		{
			_Input = Input_ * _InputWeight + StateInput_ * _StateInputWeight + _Bias;
			return _Output = Sigmoid(_Input);
		}
		inline void FeedBackward(double Input_, double StateInput_, double Error_) noexcept override
		{
			const auto Gradient = DerivativeSigmoid(_Input) * Error_;
			_Gradient = _InputWeight * Gradient;
			_Bias += Gradient;
			_InputWeight += Input_ * Gradient;
			_StateInputWeight += StateInput_ * Gradient;
		}
	};
	class CFunctionTanh : public CFunction
	{
	public:
		inline double FeedForward(double Input_, double StateInput_) noexcept override
		{
			_Input = Input_ * _InputWeight + StateInput_ * _StateInputWeight + _Bias;
			return _Output = TanH(_Input);
		}
		inline void FeedBackward(double Input_, double StateInput_, double Error_) noexcept override
		{
			const auto Gradient = DerivativeTanH(_Input) * Error_;
			_Gradient = _InputWeight * Gradient;
			_Bias += Gradient;
			_InputWeight += Input_ * Gradient;
			_StateInputWeight += StateInput_ * Gradient;
		}
	};

	class CLongShortTermNeuron : public CNeuron
	{
		CFunctionSigmoid _FunctionF;
		CFunctionSigmoid _FunctionI;
		CFunctionTanh _FunctionG;
		CFunctionSigmoid _FunctionO;
		double _StateInput = 0.0;
		double _StateOutput = 0.0;
		double _StateOutputWeight = GetRandom(1, 1);
		double _MemoryInput = 0.0;
		double _MemoryOutput = 0.0;
		double _MemoryOutputActivate = 0.0; // FeedBackward »ç¿ë

	public:
		void FeedForward(double Input_) noexcept override;
		void FeedBackward(double Error_) noexcept override;
	};
}
