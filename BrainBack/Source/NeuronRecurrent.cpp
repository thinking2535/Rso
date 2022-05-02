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
			//	s->FeedBackward(Gradiant, LearningRate_); Gradiant 에 이미 LearningRate_ 가 포함됨.


			//	가격은 점점 주기가 커지는 필터로 점점 데이터를 줄이고 해당 데이터만 연결하였을 때 가격그래프가 비슷하게 (해당 영역에서의 High, Low 는(둘중 하나만 표시) 정확하게 표현되도록)

			//_OldH = _NewH; _OldH 는 _NewH 로대체되어야 하는가? 아니면 _OldH *= Gradiant *(변경전 _WeightO) 가 되어야 하는가?
			_Error = 0.0;
		}
	}
}