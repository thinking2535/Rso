#include "stdafx.h"
#include <iomanip>
#include <Rso/Util/RandomGeometry.h>
#include <Rso/Math/Average.h>

using namespace rso;
using namespace base;
using namespace util;

#define XOR_241 1

int _tmain(int argc, _TCHAR* argv[])
{
	const TEndLayer* Input;
	const TEndLayer* Output;

	try
	{
		cout << fixed;
		cout.precision(4);


#if XOR_11 // xor using hidden layer ///////////////////////////////////////////////////////

		TBrain b(0.1, 0.03, { 1, 1 }, Input, Output);
		for (int r = 0; r < 50; ++r)
		{
			auto rnd = rand() % 2;
			rnd = 1;
			(*Input)[0]->SetOutput(rnd);

			b.FeedForward();

			(*Output)[0]->SetTarget(1);

			b.FeedBackward();
		}
		for (int r = 0; r < 500; ++r)
		{
			auto rnd = rand() % 2;
			rnd = 0;
			(*Input)[0]->SetOutput(rnd);

			b.FeedForward();

			(*Output)[0]->SetTarget(0);

			b.FeedBackward();
		}

		for (size_t r = 0; r < 20; ++r)
		{
			(*Input)[0]->SetOutput(1);

			cout << "[Input] : ";
			cout << (*Input)[0]->GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			cout << (*Output)[0]->GetOutput() << " ";

			cout << endl;
		}


#elif FUNC_TEST_1

		//w 변경시 일정 값 이상 넘어가지 않도록?? 아니면 일정변화량 이상 넘어가지 않도록??
		//아니면 변화량이 커질수록 로그적으로 줄이도록?
		//	=> 값이 너무 작거나 커질수록 오차가 커지기 때문에
		//	=> 또한 이것 때문에 오차가 줄어들지 않음.

		const size_t c_BuffSize = 2;
		const size_t c_DataSize = 1;

		TBrain b(0.1, { c_BuffSize, 4, c_DataSize }, Input, Output);
		std::random_device rd;
		std::mt19937 gen(rd());

		auto input = [&](void)
		{
			for (auto& i : *Input)
			{
				auto r = rand() % 2;
				if (r == 0)
					i.SetOutput(double((rand() % 5) - 5));
				else
					i.SetOutput(double((rand() % 5) + 1));

				//				cout << setw(20) << i.GetInput();
			}

		};

		streamsize width = 8;
		double avg_e = 0.0;
		int print_period = 10000;
		// train
		for (int r = 0; r < 100000; ++r)
		{
			input();

			b.FeedForward();

			auto t = ((*Input)[0].GetInput() * (*Input)[1].GetInput() > 0.0 ? 1.0 : 0.0);
			(*Output)[0].SetTarget(t);

			//			cout << setw(20) << t << endl;

			avg_e += ((*Output)[0].GetError() * (*Output)[0].GetError());

			if (r % print_period == 0)
			{
				cout << setw(width) << r << setw(width) << avg_e / print_period;
				avg_e = 0.0;

				for (size_t l = 0; l < b.GetLayers().size() - 1; ++l)
				{
					for (auto& n : b.GetLayers()[l])
					{
						for (auto& s : n.GetNextSynapses())
							cout << setw(width) << s->GetWeight();
					}
				}
				cout << endl;
			}

			b.FeedBackward();
		}

		//Value v;
		//v = b.ToLayers();
		//cout << v.toStyledString() << endl;

		// test
		for (int r = 0; r < 40; ++r)
		{
			input();

			b.FeedForward();

			for (auto& i : *Input)
				cout << " " << i.GetInput();
			cout << " :";

			for (auto& o : *Output)
				cout << " " << o.GetOutput();
			cout << endl;
		}

#elif FUNC_TEST_2

		const size_t c_BuffSize = 2;
		const size_t c_DataSize = 1;

		TBrain b(0.1, { c_BuffSize, 4, c_DataSize }, Input, Output);

		std::random_device rd;
		std::mt19937 gen(rd());
		default_random_engine en;

		int in = 0;
		SPoint pnt;
		auto input = [&](void)
		{
			in = rand() % 2;
			if (in == 0)
				pnt = CRandomCircle<double>(2.5)(en);
			else
				pnt = CRandomDonut<double>(3.5, 2.0)(en);

			//				cout << setw(20) << i.GetInput();

			(*Input)[0].SetOutput(pnt.X * pnt.X);
			(*Input)[1].SetOutput(pnt.Y * pnt.Y);
		};

		streamsize width = 8;
		double avg_e = 0.0;
		int print_period = 10000;
		// train
		for (int r = 0; r < 1000000; ++r)
		{
			input();

			b.FeedForward();

			if (in == 0)
				(*Output)[0].SetTarget(0.0);
			else
				(*Output)[0].SetTarget(1.0);

			//			cout << setw(20) << t << endl;

			avg_e += ((*Output)[0].GetError() * (*Output)[0].GetError());

			if (r % print_period == 0)
			{
				cout << setw(width) << r << setw(width) << avg_e / print_period;
				avg_e = 0.0;

				for (size_t l = 0; l < b.GetLayers().size() - 1; ++l)
				{
					for (auto& n : b.GetLayers()[l])
					{
						for (auto& s : n.GetNextSynapses())
							cout << setw(width) << s->GetWeight();
					}
				}
				cout << endl;
			}

			b.FeedBackward();
		}

		//Value v;
		//v = b.ToLayers();
		//cout << v.toStyledString() << endl;

		// test
		for (int r = 0; r < 40; ++r)
		{
			input();

			b.FeedForward();

			cout << in << " :";
			for (auto& i : *Input)
				cout << " " << i.GetInput();
			cout << " :";

			for (auto& o : *Output)
				cout << " " << o.GetOutput();
			cout << endl;
		}

#elif BINARY_TEST

		const size_t c_BuffSize = 3;
		const size_t c_DataSize = pow(2, c_BuffSize);

		TBrain b(0.01, { c_BuffSize, c_DataSize }, Input, Output);

		auto input = [&](unsigned __int64 v)
		{
			for (auto& i : *Input)
			{
				i.SetOutput(v & 1);
				v >>= 1;
			}
		};

		// train
		for (int r = 0; r < 1000000; ++r)
		{
			unsigned __int64 v = rand() % c_DataSize;

			input(v);

			b.FeedForward();

			for (auto& o : *Output)
				o.SetTarget(0.0);

			(*Output)[v].SetTarget(1.0);

			b.FeedBackward();
		}

		Value v;
		v = b.ToLayers();
		cout << v.toStyledString() << endl;

		// test
		for (int r = 0; r < 40; ++r)
		{
			unsigned __int64 v = rand() % c_DataSize;

			input(v);

			b.FeedForward();

			cout << v;
			for (auto& o : *Output)
				cout << " " << o.GetOutput();

			cout << endl;
		}

#elif MORSE_TEST

		//모르스 부호 를 학습하기 위해서는
		//	1. 비지도학습으로 뉴런그룹을 생성하고, 
		//	2. 지도학습으로 뉴런그룹 강화?

		const size_t c_BuffSize = 4;
		const size_t c_DataSize = 6;

		array<double, c_BuffSize> Buff{};

		vector<vector<double>> data(c_DataSize);



		data[0] = { 0, 1 };
		data[1] = { 0, 0, 1 };
		data[2] = { 0, 1, 1 };
		data[3] = { 0, 0, 0, 1 };
		data[4] = { 0, 0, 1, 1 };
		data[5] = { 0, 1, 1, 1 };



		//auto d = [&](char c) -> vector<double>&
		//{
		//	return data[c - 'a'];
		//};

		//d('a') = { 0, 1 };
		//d('b') = { 1, 0, 0, 0 };
		//d('c') = { 1, 0, 1, 0 };
		//d('d') = { 1, 0, 0 };
		//d('e') = { 0 };
		//d('f') = { 0, 0, 1, 0 };
		//d('g') = { 1, 1, 0 };
		//d('h') = { 0, 0, 0, 0 };
		//d('i') = { 0, 0 };
		//d('j') = { 0, 1, 1, 1 };
		//d('k') = { 1, 0, 1 };
		//d('l') = { 0, 1, 0, 0 };
		//d('m') = { 1, 1 };
		//d('n') = { 1, 0 };
		//d('o') = { 1, 1, 1 };
		//d('p') = { 0, 1, 1, 0 };
		//d('q') = { 1, 1, 0, 1 };
		//d('r') = { 0, 1, 0 };
		//d('s') = { 0, 0, 0 };
		//d('t') = { 1 };
		//d('u') = { 0, 0, 1 };
		//d('v') = { 0, 0, 0, 1 };
		//d('w') = { 0, 1, 1 };
		//d('x') = { 1, 0, 0, 1 };
		//d('y') = { 1, 0, 1, 1 };
		//d('z') = { 1, 1, 0, 0 };

		auto push = [&](const vector<double>& v)
		{
			for (auto i = Buff.size() - v.size(); i > 0; --i)
			{
				auto f = i - 1;
				auto t = f + v.size();

				Buff[t] = Buff[f];
			}

			for (size_t i = 0; i < v.size(); ++i)
				Buff[i] = v[i];
		};

		auto print = [&]()
		{
			for (auto& i : Buff)
				cout << i << " ";
			cout << endl;
		};

		TBrain b(0.5, { Buff.size(), 4, data.size() }, Input, Output);

		// train
		for (int r = 0; r < 100000; ++r)
		{
			size_t index = rand() % data.size();
			push(data[index]);

			for (size_t i = 0; i < Buff.size(); ++i)
				(*Input)[i].SetOutput(Buff[i]);

			b.FeedForward();

			//cout << index;

			//for (size_t i = 0; i < Buff.size(); ++i)
			//	cout << " " << (*Input)[i].GetInput();
			//cout << "	";

			//for (size_t i = 0; i < data.size(); ++i)
			//	cout << " " << (*Output)[i].GetOutput();

			//cout << endl;


			for (auto& o : *Output)
				o.SetTarget(0.0);

			(*Output)[index].SetTarget(1.0);

			b.FeedBackward();
		}

		Value v;
		v = b.ToLayers();
		cout << v.toStyledString() << endl;

		// test
		for (size_t r = 0; r < 10; ++r)
		{
			size_t index = rand() % data.size();
			push(data[index]);

			for (size_t i = 0; i < Buff.size(); ++i)
				(*Input)[i].SetOutput(Buff[i]);

			b.FeedForward();

			cout << index;
			for (size_t i = 0; i < data.size(); ++i)
				cout << " " << (*Output)[i].GetOutput();

			cout << endl;
		}


#elif XOR_244 // xor classifying layer ///////////////////////////////////////////////////////

		TBrain b(1.0, 0.03, { 2, 8, 4 }, Input, Output);
		//		srand(time(0));

		for (int r = 0; r < 100000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i->SetOutput(rand() % 2);
			//(*Input)[0].SetOutput(0.0);
			//(*Input)[1].SetOutput(0.0);

			b.FeedForward();

			[&](const TLayer& Input_, TLayer& Output_)
			{
				for (auto& o : Output_)
					o->SetTarget(0.0);

				if (Input_[0]->GetInput() == 0.0 && Input_[1]->GetInput() == 0.0)
					Output_[0]->SetTarget(1.0);
				else if (Input_[0]->GetInput() == 0.0 && Input_[1]->GetInput() == 1.0)
					Output_[1]->SetTarget(1.0);
				else if (Input_[0]->GetInput() == 1.0 && Input_[1]->GetInput() == 0.0)
					Output_[2]->SetTarget(1.0);
				else
					Output_[3]->SetTarget(1.0);
			}
			(*Input, *Output);

			//b.PrintLayers(cout);
			//b.PrintWeightBias(cout);
			b.FeedBackward();
		}

		for (size_t r = 0; r < 10; ++r)
		{
			for (auto& i : *Input)
				i->SetOutput(rand() % 2);

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i->GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i->GetOutput() << " ";

			cout << endl;
		}

#elif XOR_241 // xor using hidden layer ///////////////////////////////////////////////////////

		const double c_LearningRate = 0.1;

		CBrain b(1.0,
			new CNeuronGroup({ 2, 100, 100, 100, 100, 1 })
		);

		//CBrain b(1.0,
		//	new CNeuronGroupUnionD(
		//		{
		//			new CNeuronGroupUnionW(
		//				{
		//					new CNeuronGroupUnionD({ new CNeuronGroup({1, 4, 10, 10}), new CNeuronGroup({50, 60}), new CNeuronGroup({1}) }),
		//					new CNeuronGroupUnionD({ new CNeuronGroup({1, 3}) })
		//				}
		//			),
		//			new CNeuronGroup({ 8, 8, 8, 1 })
		//		}
		//	)
		//);

		//CBrain b(1.0,
		//	new CNeuronGroupUnionD(
		//		{
		//			new CNeuronGroupUnionW(
		//				{
		//					new CNeuronGroupUnionD({ new CNeuronGroup({1}) }),
		//					new CNeuronGroupUnionD({ new CNeuronGroup({1}) })
		//				}
		//			),
		//			new CNeuronGroup({ 4, 1 })
		//		}
		//	)
		//);

		//CBrain b(1.0, 0.003, new CNeuronGroupUnionD({ new CNeuronGroup({2, 4}), new CNeuronGroup({5, 6}), new CNeuronGroup({1}) }));

		//CBrain b(1.0, 0.003, new CNeuronGroup({ 2, 4, 1 }));

		Input = &b.GetInputLayer();
		Output = &b.GetOutputLayer();

		CAverage Loss(100);

		for (int r = 0; r < 100; ++r)
		{
			// Set Input
			double in[2] = { rand() % 2 ? 1.0 : -1.0 , rand() % 2 ? 1.0 : -1.0 };

			for (size_t i = 0; i < Input->size(); ++i)
				(*Input)[i]->SetOutput(in[i]);

			//cout << "[Input] : ";
			//for (auto& i : *Input)
			//	cout << i.GetInput() << " ";

			b.FeedForward();

			//cout << "[Out] : ";
			//for (auto& i : *Output)
			//	cout << i.GetOutput() << " ";
			//cout << "------------------------------------------------------------------------" << endl;

			[&](const TEndLayer& Input_, const TEndLayer& Output_)
			{
				if (in[0] * in[1] < 0.0)
					Output_[0]->SetTarget(0.0, c_LearningRate);
				else
					Output_[0]->SetTarget(1.0, c_LearningRate);
			}
			(*Input, *Output);

			//cout << Value(b.ToLayers()).toStyledString();
			//cout << "------------------------------------------------------------------------" << endl;

			//auto Error = (*Output)[0]->GetError();
			//Loss += Error;
			//cout << Error << " " << (double)Loss << endl;
			b.FeedBackward();
		}

		//cout << Value(b.ToLayers()).toStyledString();


		for (size_t r = 0; r < 20; ++r)
		{
			double in[2] = { rand() % 2 ? 1.0 : -1.0 , rand() % 2 ? 1.0 : -1.0 };

			cout << "[Input] : ";
			for (size_t i = 0; i < Input->size(); ++i)
			{
				(*Input)[i]->SetOutput(in[i]);
				cout << in[i] << " ";
			}

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i->GetOutput() << " ";

			cout << endl;
		}

#elif XOR_221 // xor using hidden layer ///////////////////////////////////////////////////////

		TBrain b(1.0, 0.003, { 2, 2, 1 }, Input, Output);

		for (int r = 0; r < 10000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i->SetOutput(rand() % 2);

			b.FeedForward();

			[&](const TLayer& Input_, TLayer& Output_)
			{
				if (Input_[0]->GetInput() == Input_[1]->GetInput())
					Output_[0]->SetTarget(0.0);
				else
					Output_[0]->SetTarget(1.0);
			}
			(*Input, *Output);

			//b.PrintLayers(cout);
			//b.PrintWeightBias(cout);
			b.FeedBackward();
		}

		for (size_t r = 0; r < 20; ++r)
		{
			for (auto& i : *Input)
				i->SetOutput(rand() % 2);

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i->GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i->GetOutput() << " ";

			cout << endl;
		}

#elif XOR_242 // xor using hidden layer 2 ///////////////////////////////////////////////////////

		TBrain b(0.1, { 2, 4, 2 }, Input, Output);

		//b.SetBiasAndWeights(0, 0, -0.683, { -36.725, -37.964, 37.731, 36.064 });
		//b.SetBiasAndWeights(0, 1, -0.681, { -36.725, 37.730, -37.963, 36.065 });
		//b.SetBiasAndWeights(1, 0, 28.883, {});
		//b.SetBiasAndWeights(1, 1, -4.663, {});
		//b.SetBiasAndWeights(1, 2, -4.664, {});
		//b.SetBiasAndWeights(1, 3, -37.472, {});

		for (int r = 0; r < 100000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i.SetOutput(rand() % 2);

			b.FeedForward();

			[&](const TLayer& Input_, TLayer& Output_)
			{
				for (auto& o : Output_)
					o.SetTarget(0.0);

				if (Input_[0].GetInput() == Input_[1].GetInput())
					Output_[0].SetTarget(1.0);
				else
					Output_[1].SetTarget(1.0);
			}
			(*Input, *Output);

			b.FeedBackward();
		}

		cout << "After Train -------------------" << endl;
		cout << Value(b.ToLayers()).toStyledString();

		for (size_t r = 0; r < 20; ++r)
		{
			for (auto& i : *Input)
				i.SetOutput(rand() % 2);

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i.GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif XOR_24 // xor using classifying ///////////////////////////////////////////////////////

		TBrain b({ 2, 4 }, Input, Output);

		//		srand(time(0));

		for (int r = 0; r < 1000000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i.SetOutput(rand() % 2);
			//(*Input)[0].SetOutput(0.0);
			//(*Input)[1].SetOutput(0.0);

			b.FeedForward();

			[&](const TLayer& Input_, TLayer& Output_)
			{
				for (auto& o : Output_)
					o.SetTarget(0.0);

				if (Input_[0].GetInput() == 0.0 && Input_[1].GetInput() == 0.0)
					Output_[0].SetTarget(1.0);
				else if (Input_[0].GetInput() == 0.0 && Input_[1].GetInput() == 1.0)
					Output_[1].SetTarget(1.0);
				else if (Input_[0].GetInput() == 1.0 && Input_[1].GetInput() == 0.0)
					Output_[2].SetTarget(1.0);
				else
					Output_[3].SetTarget(1.0);
			}
			(*Input, *Output);

			b.FeedBackward();
		}

		b.PrintBiasAndWeights(cout);

		for (size_t r = 0; r < 10; ++r)
		{
			for (auto& i : *Input)
				i.SetOutput(rand() % 2);

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i.GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif XOR_41 // 4 to 1 for xor ///////////////////////////////////////////////////////

		TBrain b(0.01, { 4, 1 }, Input, Output);

		for (int r = 0; r < 100000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i.SetOutput(0.0);

			(*Input)[rand() % 4].SetOutput(1.0);

			b.FeedForward();

			[](const TLayer& Input_, TLayer& Output_)
			{
				if (Input_[0].GetInput() == 1.0 || Input_[3].GetInput() == 1.0)
					Output_[0].SetTarget(0.0);
				else
					Output_[0].SetTarget(1.0);
			}
			(*Input, *Output);

			//			b.PrintWeight(cout);
			b.FeedBackward();
		}

		for (size_t r = 0; r < 30; ++r)
		{
			for (auto& i : *Input)
				i.SetOutput(0.0);

			(*Input)[rand() % 4].SetOutput(1.0);

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i.GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif XOR_42 // 4 to 2 for xor ///////////////////////////////////////////////////////

		TBrain b({ 4, 2 }, Input, Output); // Fail

		for (int r = 0; r < 100000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i.SetOutput(0.0);

			auto rnd = rand() % 4;
			(*Input)[((rnd & 0x1) > 0 ? 1 : 0)].SetOutput(1.0);
			(*Input)[2 + ((rnd & 0x2) > 0 ? 1 : 0)].SetOutput(1.0);

			b.FeedForward();

			[&](const TLayer& Input_, TLayer& Output_)
			{
				if (rnd == 0 || rnd == 3)
				{
					Output_[0].SetTarget(1.0);
					Output_[1].SetTarget(0.0);
				}
				else
				{
					Output_[0].SetTarget(0.0);
					Output_[1].SetTarget(1.0);
				}
			}
			(*Input, *Output);

			b.FeedBackward();
		}

		for (size_t r = 0; r < 30; ++r)
		{
			for (auto& i : *Input)
				i.SetOutput(0.0);

			auto rnd = rand() % 4;
			(*Input)[((rnd & 0x1) > 0 ? 1 : 0)].SetOutput(1.0);
			(*Input)[2 + ((rnd & 0x2) > 0 ? 1 : 0)].SetOutput(1.0);

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i.GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif XOR_44 // 4 to 4 for xor ///////////////////////////////////////////////////////

		TBrain b(1.0, 0.003, { 4, 4 }, Input, Output);

		for (int r = 0; r < 100000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i.SetOutput(0.0);

			auto rnd = rand() % 4;
			(*Input)[((rnd & 0x1) > 0 ? 1 : 0)].SetOutput(1.0);
			(*Input)[2 + ((rnd & 0x2) > 0 ? 1 : 0)].SetOutput(1.0);

			b.FeedForward();

			[&](const TLayer& Input_, TLayer& Output_)
			{
				for (auto& o : Output_)
					o.SetTarget(0.0);

				Output_[rnd].SetTarget(1.0);
			}
			(*Input, *Output);

			b.FeedBackward();
		}

		for (size_t r = 0; r < 30; ++r)
		{
			for (auto& i : *Input)
				i.SetOutput(0.0);

			auto rnd = rand() % 4;
			(*Input)[((rnd & 0x1) > 0 ? 1 : 0)].SetOutput(1.0);
			(*Input)[2 + ((rnd & 0x2) > 0 ? 1 : 0)].SetOutput(1.0);

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i.GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif LOGIC_21 // or, and, xor ///////////////////////////////////////////////////////

		TBrain b(0.003, { 2, 1, 1, 1, 1 }, Input, Output);

		for (int r = 0; r < 1000000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i.SetOutput((rand() % 2));

			b.FeedForward();

			[](const TLayer& Input_, TLayer& Output_)
			{
#if 0 // or
				if (Input_[0].GetInput() > 0.5 || Input_[1].GetInput() > 0.5)
					Output_[0].SetTarget(1.0);
				else
					Output_[0].SetTarget(0.0);
#elif 1 // and
				if (Input_[0].GetInput() > 0.5 && Input_[1].GetInput() > 0.5)
					Output_[0].SetTarget(1.0);
				else
					Output_[0].SetTarget(0.0);
#elif 1 // xor  // can not learn
				if (Input_[0].GetInput() == Input_[1].GetInput())
					Output_[0].SetTarget(1.0);
				else
					Output_[0].SetTarget(0.0);
#endif
			}
			(*Input, *Output);

			b.FeedBackward();
		}

		b.PrintBiasAndWeights(cout);

		for (size_t r = 0; r < 10; ++r)
		{
			for (auto& i : *Input)
				i.SetOutput((rand() % 2));

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i.GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif FORWARD // forward, not ///////////////////////////////////////////////////////

		TBrain b({ 1, 1 }, Input, Output);

		for (int r = 0; r < 100000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i.SetOutput((rand() % 2));

			b.FeedForward();

			[](const TLayer& Input_, TLayer& Output_)
			{
#if 0 // forward
				Output_[0].SetTarget(Input_[0].GetInput());
#else // not
				Output_[0].SetTarget(1 - Input_[0].GetInput());
#endif
			}
			(*Input, *Output);

			//			b.PrintWeight(cout);
			b.FeedBackward();
		}

		for (size_t r = 0; r < 10; ++r)
		{
			for (auto& i : *Input)
				i.SetOutput((rand() % 2));

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i.GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif CLASSIFY_13 // classifying ///////////////////////////////////////////////////////
		TBrain b(1.0, 0.003, { 1, 4, 3 }, Input, Output);

		for (int r = 0; r < 10000; ++r)
		{
			// Set Input
			for (auto& i : *Input)
				i->SetOutput((rand() % Output->size()));

			b.FeedForward();

			[](const TLayer& Input_, TLayer& Output_)
			{
				for (auto& o : Output_)
					o->SetTarget(0.0);

				Output_[int(Input_[0]->GetInput())]->SetTarget(1.0);
			}
			(*Input, *Output);

			//			b.PrintWeight(cout);
			b.FeedBackward();
		}

		for (size_t r = 0; r < 10; ++r)
		{
			for (auto& i : *Input)
				i->SetOutput((rand() % Output->size()));

			cout << "[Input] : ";
			for (auto& i : *Input)
				cout << i->GetInput() << " ";

			b.FeedForward();

			cout << "[Out] : ";
			for (auto& i : *Output)
				cout << i->GetOutput() << " ";

			cout << endl;
		}

#endif

	}
	catch (const SException & e)
	{
		cout << e.what();
	}

	return 0;
}
