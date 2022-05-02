#include "stdafx.h"
#include <fstream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <codecvt>

#define MORSE 1
#define MORSE_EXT 0
#define XOR_24_RNN 0
#define XOR_24 0
#define VOWEL 0
#define ALPHABET 0

int _tmain(int argc, _TCHAR* argv[])
{
	cout << fixed;
	cout.precision(2);

#if false

	unordered_map<string, int> Tags;
	{
		ifstream FS("tags.csv");
		if (!FS.is_open())
			throw 0;

		unordered_set<string> checker;

		string Line;
		getline(FS, Line);
		while (getline(FS, Line))
		{
			istringstream ss(Line);
			string TagID;
			getline(ss, TagID, ',');

			if (!checker.emplace(TagID).second)
				throw;

			Tags.emplace(TagID, Tags.size());
		}
	}

	unordered_map<string, int> CompanySizeIndex{
		{"", 0},
		{"1-10", 1},
		{"11-50", 2},
		{"51-100", 3},
		{"101-200", 4},
		{"201-500", 5},
		{"501-1000", 6},
		{"1000 이상", 7},
	};


	unordered_map<string, int> JobCompanies;
	{
		ifstream FS("job_companies.csv");
		if (!FS.is_open())
			throw 0;

		unordered_set<string> checker;

		string Line;
		getline(FS, Line);
		while (getline(FS, Line))
		{
			istringstream ss(Line);
			string CompanyID;
			string JobID;
			string CompanySize;
			getline(ss, CompanyID, ',');
			getline(ss, JobID, ',');
			getline(ss, CompanySize, ',');

			if (!checker.emplace(JobID).second)
				throw;

			JobCompanies.emplace(JobID, CompanySizeIndex[CompanySize]);
		}
	}

	unordered_map<string, vector<int>> JobTags;
	{
		ifstream FS("job_tags.csv");
		if (!FS.is_open())
			throw 0;

		string Line;
		getline(FS, Line);
		while (getline(FS, Line))
		{
			istringstream ss(Line);
			string JobID;
			string TagID;
			getline(ss, JobID, ',');
			getline(ss, TagID, ',');

			auto ib = JobTags.emplace(JobID, vector<int>(Tags.size() + CompanySizeIndex.size()));
			ib.first->second[Tags[TagID]] = 1;
			ib.first->second[Tags.size() + JobCompanies[JobID]] = 1;
		}
	}


	unordered_map<string, vector<int>> UserTags;
	{
		ifstream FS("user_tags.csv");
		if (!FS.is_open())
			throw 0;

		string Line;
		getline(FS, Line);
		while (getline(FS, Line))
		{
			istringstream ss(Line);
			string UserID;
			string TagID;
			getline(ss, UserID, ',');
			getline(ss, TagID, ',');

			auto ib = UserTags.emplace(UserID, vector<int>(Tags.size()));
			ib.first->second[Tags[TagID]] = 1;
		}
	}

	struct STest
	{
		const vector<int>* pUser;
		const vector<int>* pJob;

		STest(const vector<int>* pUser_, const vector<int>* pJob_) :
			pUser(pUser_),
			pJob(pJob_)
		{
		}
	};
	struct STrain : public STest
	{
		int Applied;

		STrain(const STest& Test_, int Applied_) :
			STest(Test_),
			Applied(Applied_)
		{
		}
	};

	vector<STrain> Train;
	{
		ifstream FS("train.csv");
		if (!FS.is_open())
			throw 0;

		string Line;
		getline(FS, Line);
		while (getline(FS, Line))
		{
			istringstream ss(Line);
			string UserID;
			string JobID;
			string Applied;
			getline(ss, UserID, ',');
			getline(ss, JobID, ',');
			getline(ss, Applied, ',');

			auto itUser = UserTags.find(UserID);
			if (itUser == UserTags.end())
				throw;

			auto itJob = JobTags.find(JobID);
			if (itJob == JobTags.end())
				throw;

			Train.emplace_back(STest(&itUser->second, &itJob->second), stoi(Applied));
		}
	}

	vector<STest> TestJob;
	{
		ifstream FS("test_job.csv");
		if (!FS.is_open())
			throw 0;

		string Line;
		getline(FS, Line);
		while (getline(FS, Line))
		{
			istringstream ss(Line);
			string UserID;
			string JobID;
			getline(ss, UserID, ',');
			getline(ss, JobID, ',');

			auto itUser = UserTags.find(UserID);
			if (itUser == UserTags.end())
				throw;

			auto itJob = JobTags.find(JobID);
			if (itJob == JobTags.end())
				throw;

			TestJob.emplace_back(&itUser->second, &itJob->second);
		}
	}

	size_t InputSize = TestJob.front().pUser->size() + TestJob.front().pJob->size();
	size_t OutputSize = 2;

	TNeuralNetworkRNN b(0.5, { InputSize, (InputSize + OutputSize) / 2, OutputSize });
	using TLayer = TNeuralNetworkRNN::TLayer;

	auto& In = b.GetInputLayer();
	auto& Out = b.GetOutputLayer();

	auto fInput = [&](const STest& Test_)
	{
		int Index = 0;
		for (auto& i : (*Test_.pUser))
		{
			In[Index].SetOutput(i);
			++Index;
		}

		for (auto& i : (*Test_.pJob))
		{
			In[Index].SetOutput(i);
			++Index;
		}
	};

	// Train
	auto timeStart = clock();
	for (size_t i = 0; i < 1000; ++i)
	{
		cout << "---------------------------------" << endl;
		int TrainIndex = 0;
		for (auto& t : Train)
		{
			fInput(t);

			b.FeedForward(); // Predict

			if (++TrainIndex % 100 == 0)
				cout << TrainIndex << " " << Out[0].GetOutput() << " " << Out[1].GetOutput() << endl;

			b.SetTarget(t.Applied, 1);
			b.SetTarget(t.Applied ^ 1, 0);

			b.FeedBackward(); // Train
		}

		//		if (i % 1000 == 0)
		cout << "Count:" << i << " AvgTime: " << (clock() - timeStart) / (i + 1) << endl;
	}

	// Test
	cout << "Test ==================================" << endl;
	int TrainIndex = 0;

	ofstream out("test.csv");
	ofstream out_value("test_value.csv");
	out << "applied" << endl;
	out_value << "applied" << endl;

	for (auto& t : TestJob)
	{
		fInput(t);

		b.FeedForward(); // Predict

		if (Out[0].GetOutput() > Out[1].GetOutput())
			out << "0" << endl;
		else
			out << "1" << endl;

		out_value << Out[0].GetOutput() << " " << Out[1].GetOutput() << endl;
	}

	out.close();
	out_value.close();

	return 0;

#endif

	try
	{
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
			cout << (*Output)[0].GetOutput() << " ";

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

		auto input = [&](uint64 v)
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
			uint64 v = rand() % c_DataSize;

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
			uint64 v = rand() % c_DataSize;

			input(v);

			b.FeedForward();

			cout << v;
			for (auto& o : *Output)
				cout << " " << o.GetOutput();

			cout << endl;
		}

#elif MORSE

		TNeuralNetworkDNN b(1.0, { 3, 26, 26, 26 });

		auto& Input = b.GetInputLayer();
		auto& Output = b.GetOutputLayer();

		vector<vector<int>> Alphabet;

		Alphabet.push_back({ 0, 1 });		//('a')
		Alphabet.push_back({ 1, 0, 0, 0 }); //('b')
		Alphabet.push_back({ 1, 0, 1, 0 }); //('c')
		Alphabet.push_back({ 1, 0, 0 });	//('d')
		Alphabet.push_back({ 0 });			//('e')
		Alphabet.push_back({ 0, 0, 1, 0 }); //('f')
		Alphabet.push_back({ 1, 1, 0 });	//('g')
		Alphabet.push_back({ 0, 0, 0, 0 }); //('h')
		Alphabet.push_back({ 0, 0 });		//('i')
		Alphabet.push_back({ 0, 1, 1, 1 }); //('j')
		Alphabet.push_back({ 1, 0, 1 });	//('k')
		Alphabet.push_back({ 0, 1, 0, 0 }); //('l')
		Alphabet.push_back({ 1, 1 });		//('m')
		Alphabet.push_back({ 1, 0 });		//('n')
		Alphabet.push_back({ 1, 1, 1 });	//('o')
		Alphabet.push_back({ 0, 1, 1, 0 }); //('p')
		Alphabet.push_back({ 1, 1, 0, 1 }); //('q')
		Alphabet.push_back({ 0, 1, 0 });	//('r')
		Alphabet.push_back({ 0, 0, 0 });	//('s')
		Alphabet.push_back({ 1 });			//('t')
		Alphabet.push_back({ 0, 0, 1 });	//('u')
		Alphabet.push_back({ 0, 0, 0, 1 }); //('v')
		Alphabet.push_back({ 0, 1, 1 });	//('w')
		Alphabet.push_back({ 1, 0, 0, 1 }); //('x')
		Alphabet.push_back({ 1, 0, 1, 1 }); //('y')
		Alphabet.push_back({ 1, 1, 0, 0 }); //('z')

		int r = 0;
		int cout_cnt = 100;
		double Avg = 0.0;
		auto fInput = [&](const size_t Index_)
		{
			for (auto a : Alphabet[Index_])
			{
				for (auto& i : Input)
					i.SetOutput(0.0);

				Input[a].SetOutput(1.0);
				b.FeedForward();

				for (size_t o = 0; o < Output.size(); ++o)
					b.SetTarget(o, 0.0);

				b.FeedBackward();
			}

			for (auto& i : Input)
				i.SetOutput(0.0);

			Input[2].SetOutput(1.0);
			b.FeedForward();

			Avg *= 0.99;
			Avg += (Output[Index_].GetOutput() * 0.01);

			if (r > cout_cnt)
				//				cout << Avg << endl;
			{
				cout << Avg << ' ' << char(Index_ + 'a') << " :";
				for (size_t o = 0; o < Output.size(); ++o)
					cout << ' ' << char(o + 'a') << '[' << Output[o].GetOutput() << ']';
				cout << endl;
			}

			for (size_t o = 0; o < Output.size(); ++o)
				if (o == Index_)
					b.SetTarget(o, 1.0);
				else
					b.SetTarget(o, 0.0);

			b.FeedBackward();
		};

		for (; true; ++r)
			fInput(rand() % 26);


#elif MORSE_EXT

		TNeuralNetworkRNN b(0.06, { 10, 26, 26, 26, 26 });

		auto& Input = b.GetInputLayer();
		auto& Output = b.GetOutputLayer();

		vector<vector<int>> Alphabet(26);

		for (int i = 0; i < Alphabet.size(); ++i)
		{
			for (int j = 0; j < 48; ++j)
				Alphabet[i].push_back(((sin(c_PI * j / 24  +  (c_PI * i / 24)) + 1.0) * 5.0) - 1.0);
		}

		//char c = 'a';
		//for (auto& a : Alphabet)
		//{
		//	cout << c << "Alphabet.push_back({";

		//	for (int& i : a)
		//		cout << ' ' << i << ',';

		//	cout << " });\n";

		//	++c;
		//}

		//return 0;

		//Alphabet.push_back({ 1, 3, 5, 6, 9, 9, 9, 9, 8, 7, 8, 9, 1, 3, 5, 6, 9, 9, 9, 9, 8, 7, 8, 9, 3, 3, 5, 6, 9, 9, 9, 9, 8, 7, 8, 9, 1, 3, 5, 5, 9, 9, 9, 9, 8, 7, 8, 9 });		//('a')
		//Alphabet.push_back({ 1, 3, 5, 6, 9, 9, 9, 9, 8, 7, 8, 9, 1, 3, 5, 6, 9, 9, 9, 9, 8, 7, 8, 9, 1, 3, 5, 6, 9, 9, 9, 9, 8, 7, 8, 9, 1, 3, 5, 6, 9, 9, 9, 9, 8, 7, 8, 9 }); //('b')
		//Alphabet.push_back({ 1, 0, 1, 2, 3, 7, 8, 6, 3, 3, 4, 1 }); //('c')
		//Alphabet.push_back({ 4, 5, 5, 6, 6, 7, 8, 8, 9 });	//('d')
		//Alphabet.push_back({ 9, 8, 7 });			//('e')
		//Alphabet.push_back({ 5, 6, 7, 1, 1, 3, 0, 2, 3, 4, 6, 7 }); //('f')
		//Alphabet.push_back({ 7, 8, 9, 8, 7, 4, 1, 2, 4 });	//('g')
		//Alphabet.push_back({ 8, 7, 4, 5, 6, 4, 4, 4, 4, 1, 1, 9 }); //('h')
		//Alphabet.push_back({ 4, 5, 2, 1, 2, 1 });		//('i')
		//Alphabet.push_back({ 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 }); //('j')
		//Alphabet.push_back({ 2, 3, 3, 3, 6, 6, 8, 8, 7 });	//('k')
		//Alphabet.push_back({ 6, 6, 9, 9, 8, 7, 4, 4, 2, 1, 0, 0 }); //('l')
		//Alphabet.push_back({ 1, 1, 4, 4, 1, 1 });		//('m')
		//Alphabet.push_back({ 1, 5, 1, 4, 1, 0 });		//('n')
		//Alphabet.push_back({ 1, 1, 1, 1, 1, 1, 1, 1, 1 });	//('o')
		//Alphabet.push_back({ 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0 }); //('p')
		//Alphabet.push_back({ 1, 2, 1, 2, 2, 2, 0, 1, 1, 1, 0, 1 }); //('q')
		//Alphabet.push_back({ 0, 1, 0, 3, 3, 5, 1, 1, 0 });	//('r')
		//Alphabet.push_back({ 0, 4, 6, 7, 0, 4, 1, 0, 0 });	//('s')
		//Alphabet.push_back({ 7, 1, 1 });			//('t')
		//Alphabet.push_back({ 5, 5, 2, 0, 2, 1, 0, 0, 1 });	//('u')
		//Alphabet.push_back({ 0, 0, 7, 1, 9, 0, 9, 9, 8, 8, 0, 1 }); //('v')
		//Alphabet.push_back({ 4, 5, 4, 5, 4, 5, 4, 5, 1 });	//('w')
		//Alphabet.push_back({ 1, 5, 6, 4, 5, 6, 4, 6, 1, 0, 0, 1 }); //('x')
		//Alphabet.push_back({ 1, 0, 1, 2, 3, 4, 5, 6, 4, 7, 8, 9 }); //('y')
		//Alphabet.push_back({ 1, 1, 8, 9, 7, 9, 7, 9, 7, 9, 9, 0 }); //('z')

		int r = 0;
		int cout_cnt = 100000;
		double Avg = 0.0;
		auto fInput = [&](const size_t Index_)
		{
			for (auto a : Alphabet[Index_])
			{
				for (auto& i : Input)
					i.SetOutput(0.0);

				Input[a].SetOutput(1.0);
				b.FeedForward();

				for (size_t o = 0; o < Output.size(); ++o)
					b.SetTarget(o, 0.0);

				b.FeedBackward();
			}

			for (auto& i : Input)
				i.SetOutput(0.0);

			b.FeedForward();

			Avg *= 0.99;
			Avg += (Output[Index_].GetOutput() * 0.01);

			if (r > cout_cnt)
				//				cout << Avg << endl;
			{
				cout << Avg << ' ' << char(Index_ + 'a') << " :";
				for (size_t o = 0; o < Output.size(); ++o)
					cout << ' ' << char(o + 'a') << '[' << Output[o].GetOutput() << ']';
				cout << endl;
			}

			for (size_t o = 0; o < Output.size(); ++o)
				if (o == Index_)
					b.SetTarget(o, 1.0);
				else
					b.SetTarget(o, 0.0);

			b.FeedBackward();
		};

		for (; true; ++r)
			fInput(rand() % Alphabet.size());

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
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#elif VOWEL

		size_t CharBitCount = 6;
		auto InputAlphabetNum = [&](size_t AlphabetNum_, const TLayer& InputLayer_) -> void
		{
			int32 Mask = 1;
			for (int32 i = 0; i < CharBitCount; ++i)
			{
				InputLayer_[i]->SetOutput((AlphabetNum_ & Mask) > 0 ? 1 : 0); // 문자 번호의 각 비트를 입력노드에 매칭
				Mask <<= 1;
			}
		};

		// DNN 을 이용한 알파벳 자음 모음 분류
		CNeuronGroupUnionD b({ new CNeuronGroup{6, 27}, new CNeuronGroup{2} });

		auto& In = b.GetInputLayer();
		auto& Out = b.GetOutputLayer();

		auto IsConsonant = [&](char Alphabet_)
		{
			// 자음 모듬 구분
			switch (Alphabet_)
			{
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u':
				return 0;
			default:
				return 1;
			}
		};

		const size_t AlphabetCount = 'z' - 'a' + 2;

		for (size_t i = 0; true; ++i)
		{
			size_t AlphabetNum = (i % (AlphabetCount - 1) + 1);

			InputAlphabetNum(AlphabetNum, In);

			b.FeedForward(); // Predict

			char Alphabet = (AlphabetNum - 1) + 'a';

			if (i % 1000 == 0)
				cout << "Count:" << i << "   Alphabet:" << Alphabet << " Vowel:" << Out[0].GetOutput() << " Consonant:" << Out[1].GetOutput() << endl;

			auto Output = IsConsonant(Alphabet);
			Out[Output]->SetTarget(1);
			Out[Output ^ 1]->SetTarget(0);

			b.FeedBackward(); // Train
		}

#elif ALPHABET

		size_t CharBitCount = 6;
		auto InputAlphabetNum = [&](size_t AlphabetNum_, const TLayer& InputLayer_)
		{
			int32 Mask = 1;
			for (int32 i = 0; i < CharBitCount; ++i)
			{
				InputLayer_[i]->SetOutput(((AlphabetNum_)&Mask) > 0 ? 1 : 0); // 문자 번호의 각 비트를 입력노드에 매칭
				Mask <<= 1;
			}
		};

		// DNN 을 이용한 알파벳 자음 모음 분류
		const size_t AlphabetCount = 'z' - 'a' + 2;
		//CNeuronGroupUnionD b({ new CNeuronGroup{6}, new CNeuronGroup{AlphabetCount} }); // 입력, 히든, 출력 노드 수
		CNeuronGroup b({ CharBitCount, AlphabetCount, AlphabetCount });

		auto& In = b.GetInputLayer();
		auto& Out = b.GetOutputLayer();

		for (size_t i = 0; true; ++i)
		{
			size_t AlphabetNum = (i % AlphabetCount);

			InputAlphabetNum(AlphabetNum, In);

			b.FeedForward(); // Predict

			if (i % 1000 == 0)
				cout << "Count:" << i << "   Alphabet:" << AlphabetNum << " " << Out[0].GetOutput() << " " << Out[1].GetOutput() << " " << Out[AlphabetNum].GetOutput() << endl;

			for (size_t a = 0; a < AlphabetCount; ++a)
				Out[a]->SetTarget(a == AlphabetNum ? 1 : 0);

			b.FeedBackward(); // Train
		}

#elif XOR_241 // xor using hidden layer ///////////////////////////////////////////////////////

		CBrain Brain(1, new CNeuronGroup({ 2, 4, 1 }));

		auto& In = Brain.GetInputLayer();
		auto& Out = Brain.GetOutputLayer();

		// Train
		for (int r = 0; r < 1000; ++r)
		{
			auto In0 = rand() % 2;
			auto In1 = rand() % 2;
			In[0]->SetOutput(In0);
			In[1]->SetOutput(In1);

			Brain.FeedForward();

			auto Out0 = In0 ^ In1;
			Brain.SetTarget(0, Out0);
			Brain.FeedBackward();
		}


		// Test
		for (int r = 0; r < 100; ++r)
		{
			auto In0 = rand() % 2;
			auto In1 = rand() % 2;
			In[0]->SetOutput(In0);
			In[1]->SetOutput(In1);

			Brain.FeedForward();

			auto Out0 = Out[0].GetOutput();
			cout << "In0:" << In0 << " In1:" << In1 << " Out0:" << Out0 << endl;
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
				cout << i.GetOutput() << " ";

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

#elif XOR_24_RNN // xor using classifying ///////////////////////////////////////////////////////

		TNeuralNetworkRNN b(0.03, { 1, 4, 4, 4 });
		using TLayer = TNeuralNetworkRNN::TLayer;

		auto& Input = b.GetInputLayer();
		auto& Output = b.GetOutputLayer();
		double Old = 0.0;

		vector<double> Inputs;

		Inputs.push_back(0);
		Inputs.push_back(1);
		Inputs.push_back(1);
		Inputs.push_back(0);

		for (int r = 0; true; ++r)
		{
			// Set Input
			//auto Index = r % 4;
			//Input[0].SetOutput(Inputs[Index]);
			Input[0].SetOutput(rand() % 2);

			b.FeedForward();

			if (r > 50000)
			{
				cout << r << " [Input] : ";
				for (auto& i : Input)
					cout << i.GetOutput() << " ";

				cout << "[Out] : ";
				for (auto& i : Output)
					cout << i.GetOutput() << " ";

				cout << endl;
			}

			[&](const TLayer& Input_, const TLayer& Output_)
			{
				//b.SetTarget(0, 0.0);
				//b.SetTarget(1, 1.0);
				//b.SetTarget(2, 0.0);
				//b.SetTarget(3, 0.0);

				//return; // 임시 테스트

				if (Old == 0 && Input[0].GetOutput() == 0.0)
				{
					b.SetTarget(0, 1.0);
					b.SetTarget(1, 0.0);
					b.SetTarget(2, 0.0);
					b.SetTarget(3, 0.0);
				}
				else if (Old == 0 && Input[0].GetOutput() == 1.0)
				{
					b.SetTarget(0, 0.0);
					b.SetTarget(1, 1.0);
					b.SetTarget(2, 0.0);
					b.SetTarget(3, 0.0);
				}
				else if (Old == 1 && Input[0].GetOutput() == 1.0)
				{
					b.SetTarget(0, 0.0);
					b.SetTarget(1, 0.0);
					b.SetTarget(2, 1.0);
					b.SetTarget(3, 0.0);
				}
				else
				{
					b.SetTarget(0, 0.0);
					b.SetTarget(1, 0.0);
					b.SetTarget(2, 0.0);
					b.SetTarget(3, 1.0);
				}

				Old = Input[0].GetOutput();
			}
			(Input, Output);

			b.FeedBackward();
		}


#elif XOR_24 // xor using classifying ///////////////////////////////////////////////////////

		TNeuralNetwork b(0.03, { 2, 4, 4 });
		using TLayer = TNeuralNetwork::TLayer;

		auto& Input = b.GetInputLayer();
		auto& Output = b.GetOutputLayer();

		double Avg = 0.0;

		for (int r = 0; true; ++r)
		{
			// Set Input
			for (auto& i : Input)
				i.SetOutput(rand() % 2);

			b.FeedForward();

			if (r % 10000 == 0)
			{
				cout << r << " " << Avg << " [Input] : ";
				for (auto& i : Input)
					cout << i.GetOutput() << " ";

				cout << "[Out] : ";
				for (auto& i : Output)
					cout << i.GetOutput() << " ";

				cout << endl;
			}

			Avg *= 0.99;
			Avg += [&](TLayer& Input_, TLayer& Output_)
			{
				double Loss = 0.0;

				if (Input[0].GetOutput() == 0.0 && Input[1].GetOutput() == 0.0)
				{
					Loss += abs(1.0 - Output[0].GetOutput());

					b.SetTarget(0, 1.0);
					b.SetTarget(1, 0.0);
					b.SetTarget(2, 0.0);
					b.SetTarget(3, 0.0);
				}
				else if (Input[0].GetOutput() == 0.0 && Input[1].GetOutput() == 1.0)
				{
					Loss += abs(1.0 - Output[1].GetOutput());

					b.SetTarget(0, 0.0);
					b.SetTarget(1, 1.0);
					b.SetTarget(2, 0.0);
					b.SetTarget(3, 0.0);
				}
				else if (Input[0].GetOutput() == 1.0 && Input[1].GetOutput() == 0.0)
				{
					Loss += abs(1.0 - Output[2].GetOutput());

					b.SetTarget(0, 0.0);
					b.SetTarget(1, 0.0);
					b.SetTarget(2, 1.0);
					b.SetTarget(3, 0.0);
				}
				else
				{
					Loss += abs(1.0 - Output[3].GetOutput());

					b.SetTarget(0, 0.0);
					b.SetTarget(1, 0.0);
					b.SetTarget(2, 0.0);
					b.SetTarget(3, 1.0);
				}

				return Loss * 0.25;
			}
			(Input, Output) * 0.01;

			b.FeedBackward();
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
				cout << i.GetOutput() << " ";

			cout << endl;
		}

#endif

	}
	catch (const SException& e)
	{
		cout << e.what();
	}

	return 0;
}
