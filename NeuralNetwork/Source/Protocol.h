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
			double Bias{};
			list<double> PrevSynapses{};
			SNeuron()
			{
			}
			SNeuron(const double& Bias_, const list<double>& PrevSynapses_) : Bias(Bias_), PrevSynapses(PrevSynapses_)
			{
			}
			SNeuron(double&& Bias_, list<double>&& PrevSynapses_) : Bias(std::move(Bias_)), PrevSynapses(std::move(PrevSynapses_))
			{
			}
			void operator << (CStream& Stream_) override
			{
				Stream_ >> Bias;
				Stream_ >> PrevSynapses;
			}
			void operator << (const Value& Value_) override
			{
				Value_["Bias"] >> Bias;
				Value_["PrevSynapses"] >> PrevSynapses;
			}
			void operator >> (CStream& Stream_) const override
			{
				Stream_ << Bias;
				Stream_ << PrevSynapses;
			}
			void operator >> (Value& Value_) const override
			{
				Value_["Bias"] = Bias;
				Value_["PrevSynapses"] = PrevSynapses;
			}
			static wstring StdName(void)
			{
				return 
					GetStdName(double()) + L"," + 
					GetStdName(list<double>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(double(), L"Bias") + L"," + 
					GetMemberName(list<double>(), L"PrevSynapses");
			}
		};
		struct SLayer : public SProto
		{
			list<SNeuron> Neurons{};
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
				return 
					GetStdName(list<SNeuron>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SNeuron>(), L"Neurons");
			}
		};
		struct SNeuronGroup : public SProto
		{
			list<SLayer> Layers{};
			SNeuronGroup()
			{
			}
			SNeuronGroup(const list<SLayer>& Layers_) : Layers(Layers_)
			{
			}
			SNeuronGroup(list<SLayer>&& Layers_) : Layers(std::move(Layers_))
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
				return 
					GetStdName(list<SLayer>());
			}
			static wstring MemberName(void)
			{
				return 
					GetMemberName(list<SLayer>(), L"Layers");
			}
		};
	}
}
