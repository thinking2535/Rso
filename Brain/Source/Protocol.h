#pragma once

#include <string>
#include <array>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <Rso/Core/Stream.h>
#include <Rso/Json/Json.h>

namespace rso
{
	namespace brain
	{
		struct SNeuron : public SProto
		{
			double Input = 0.0;
			double Output = 0.0;
			double Bias = 0.0;
			list<double> NextSynapses;
			SNeuron()
			{
			}
			SNeuron(const double& Input_, const double& Output_, const double& Bias_, const list<double>& NextSynapses_) : Input(Input_), Output(Output_), Bias(Bias_), NextSynapses(NextSynapses_)
			{
			}
			SNeuron(double&& Input_, double&& Output_, double&& Bias_, list<double>&& NextSynapses_) : Input(std::move(Input_)), Output(std::move(Output_)), Bias(std::move(Bias_)), NextSynapses(std::move(NextSynapses_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Input;
				Stream_ >> Output;
				Stream_ >> Bias;
				Stream_ >> NextSynapses;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Input"] >> Input;
				Value_["Output"] >> Output;
				Value_["Bias"] >> Bias;
				Value_["NextSynapses"] >> NextSynapses;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Input;
				Stream_ << Output;
				Stream_ << Bias;
				Stream_ << NextSynapses;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Input"] = Input;
				Value_["Output"] = Output;
				Value_["Bias"] = Bias;
				Value_["NextSynapses"] = NextSynapses;
			}
			static wstring StdName(void)
			{
				return L"double,double,double,{double}";
			}
			static wstring MemberName(void)
			{
				return L"Input,Output,Bias,NextSynapses";
			}
		};
		struct SLayer : public SProto
		{
			list<SNeuron> Neurons;
			SLayer()
			{
			}
			SLayer(const list<SNeuron>& Neurons_) : Neurons(Neurons_)
			{
			}
			SLayer(list<SNeuron>&& Neurons_) : Neurons(std::move(Neurons_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Neurons;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Neurons"] >> Neurons;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Neurons;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Neurons"] = Neurons;
			}
			static wstring StdName(void)
			{
				return L"{double,double,double,{double}}";
			}
			static wstring MemberName(void)
			{
				return L"Neurons";
			}
		};
		struct SLayers : public SProto
		{
			list<SLayer> Layers;
			SLayers()
			{
			}
			SLayers(const list<SLayer>& Layers_) : Layers(Layers_)
			{
			}
			SLayers(list<SLayer>&& Layers_) : Layers(std::move(Layers_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Layers;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Layers"] >> Layers;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Layers;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Layers"] = Layers;
			}
			static wstring StdName(void)
			{
				return L"{{double,double,double,{double}}}";
			}
			static wstring MemberName(void)
			{
				return L"Layers";
			}
		};
	}
}
