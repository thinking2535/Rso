#include "NeuronRecurrent.h"
#include "Synapse.h"

namespace rso
{
	namespace brain
	{
		void CNeuronRecurrent::FeedForward(double /*Alpha_*/)
		{
			_Sum = _Input + _Bias + _WeightH * _OldH;
			_NewH = tanh(_Sum);
			_Output = _WeightO * _NewH + _BiasO;

			for (auto& s : _NextSynapses)
				s->FeedForward(_Output);
		}
		void CNeuronRecurrent::FeedBackward(double /*Alpha_*/, double LearningRate_)
		{
			// Y : New H /////////////////////////////////////////////////////////////////////////////
			// O = WeightO * tanh(WeightX * X + WeightH * H + Bias) + BiasO
			// 
			// S = Weight * X + WeightH * H + Bias
			// Y = tanh(S)
			// O = WeightO * Y + BiasO
			// E = 1/2(O - T)^2
			// 
			// dE/dO = O - T
			// dO/dBiasO = 1
			// dO/dWeightO = Y
			// dO/dWeightH = H
			// dO/dY = WeightO
			// dY/dS = 1-tanh^2(S)
			// dS/dBias = 1
			// dS/dWeight = X
			// dS/dX = Weight
			// 
			// dE/dX = dE/dO * dO/dY * dY/dS * dS/dX				= (O - T) * WeightO * (1 - tanh^2(S)) * Weight
			// dE/dWeight = dE/dO * dO/dY * dY/dS * dS/dWeight		= (O - T) * WeightO * (1 - tanh^2(S)) * X
			// dE/dBias = dE/dO * dO/dY * dY/dS * dS/dBias			= (O - T) * WeightO * (1 - tanh^2(S)) * 1
			// dE/dWeightH = dE/dO * dO/dY * dY/dS * dS/dWeightH	= (O - T) * WeightO * (1 - tanh^2(S)) * H
			// dE/dWeightO = dE/dO * dO/dWeightO					= (O - T) * Y
			// dE/dBiasO = dE/dO * dO/dBiasO						= (O - T) * 1
			////////////////////////////////////////////////////////////////////////////////////////////

			auto Gradiant = _Error * LearningRate_;
			_BiasO += Gradiant;
			Gradiant *= (1 - [](double Value_) {return Value_ * Value_; }(tanh(_Sum))) * _WeightO;
			_Bias += Gradiant;
			_WeightH += (Gradiant * _OldH);
			_WeightO += (_Error * _NewH);

			//for (auto& s : _PrevSynapses)
			//	s->FeedBackward(Gradiant, LearningRate_); Gradiant �� �̹� LearningRate_ �� ���Ե�.


			//	������ ���� �ֱⰡ Ŀ���� ���ͷ� ���� �����͸� ���̰� �ش� �����͸� �����Ͽ��� �� ���ݱ׷����� ����ϰ� (�ش� ���������� High, Low ��(���� �ϳ��� ǥ��) ��Ȯ�ϰ� ǥ���ǵ���)

			//_OldH = _NewH; _OldH �� _NewH �δ�ü�Ǿ�� �ϴ°�? �ƴϸ� _OldH *= Gradiant *(������ _WeightO) �� �Ǿ�� �ϴ°�?
			_Error = 0.0;
		}
	}
}