#include "stdafx.h"
#include <iomanip>

using namespace std;

HANDLE hConsole;

double loss = 0.0;
void setLoss(double diff)
{
	auto lossSquare = diff * diff;
	if (loss < lossSquare)
		loss = lossSquare;
	else
	{
		loss *= 0.99;
		loss += lossSquare * 0.01;
	}
}

template<class TNeuralNetwork>
void XorTest(TNeuralNetwork b)
{
	using TLayer = TNeuralNetwork::TLayer;

	auto& Input = b.GetInputLayer();
	auto& Output = b.GetOutputLayer();
	auto Old = 0;
	auto New = 0;

	for (int r = 0; true; ++r)
	{
		New = rand() % 2;

		for (auto& i : Input)
			i.SetOutput(0.0);

		Input[New].SetOutput(1.0);

		b.FeedForward();

		if (r % 100000 == 0)
		{
			cout << r << " Loss : " << loss << " [Input] : ";
			for (auto& i : Input)
				cout << i.GetOutput() << " ";

			cout << "[Out] : ";
			for (auto& i : Output)
				cout << i.GetOutput() << " ";

			cout << endl;
		}

		[&](const TLayer& Input_, const TLayer& Output_)
		{
			if (Old == 0 && New == 0)
			{
				b.SetTarget(0, 1.0);
				b.SetTarget(1, 0.0);
				b.SetTarget(2, 0.0);
				b.SetTarget(3, 0.0);

				setLoss(1.0 - Output[0].GetOutput());
			}
			else if (Old == 0 && New == 1)
			{
				b.SetTarget(0, 0.0);
				b.SetTarget(1, 1.0);
				b.SetTarget(2, 0.0);
				b.SetTarget(3, 0.0);

				setLoss(1.0 - Output[1].GetOutput());
			}
			else if (Old == 1 && New == 0)
			{
				b.SetTarget(0, 0.0);
				b.SetTarget(1, 0.0);
				b.SetTarget(2, 1.0);
				b.SetTarget(3, 0.0);

				setLoss(1.0 - Output[2].GetOutput());
			}
			else
			{
				b.SetTarget(0, 0.0);
				b.SetTarget(1, 0.0);
				b.SetTarget(2, 0.0);
				b.SetTarget(3, 1.0);

				setLoss(1.0 - Output[3].GetOutput());
			}

			Old = New;
		}
		(Input, Output);

		b.FeedBackward();
	}
}

template<class TNeuralNetwork>
void WordTest(TNeuralNetwork b, vector<string> Words)
{
	auto& Input = b.GetInputLayer();
	auto& Output = b.GetOutputLayer();

	int r = 0;
	auto fInputWord = [&](const size_t WordIndex_)
	{
		auto& Word = Words[WordIndex_];

		for (int c = 0; c < Word.size(); ++c)
		{
			auto Character = Word[c];

			for (auto& i : Input)
				i.SetOutput(0.0);
			Input[Character - 'a'].SetOutput(1.0);

			b.FeedForward();

			if (r > 10000 && r % 10000 == 0)
			{
				cout << "[Loss:" << loss << "] ";

				if (c < Word.size() - 1)
				{
					SetConsoleTextAttribute(hConsole, 0x0f);
					cout << Character << " :";
					for (size_t o = 0; o < Words.size(); ++o)
						cout << ' ' << o << '[' << Output[o].GetOutput() << ']';

					SetConsoleTextAttribute(hConsole, 0x20);
					cout << ' ' << Words.size() << '[' << Output[Words.size()].GetOutput() << ']';

					cout << endl;
				}
				else
				{
					cout << Character << " :";
					for (size_t o = 0; o < Output.size(); ++o)
					{
						if (o == WordIndex_)
							SetConsoleTextAttribute(hConsole, 0x20);
						else
							SetConsoleTextAttribute(hConsole, 0x40);

						cout << ' ' << o << '[' << Output[o].GetOutput() << ']';
					}
					cout << endl;
				}
			}

			if (c == Word.size() - 1)
			{
				for (int w = 0; w < Words.size(); ++w)
				{
					if (w == WordIndex_)
					{
						setLoss(1.0 - Output[w].GetOutput());
						b.SetTarget(w, 1.0);
					}
					else
					{
						setLoss(-Output[w].GetOutput());
						b.SetTarget(w, 0.0);
					}
				}

				b.SetTarget(Words.size(), 0.0);
			}
			else
			{
				for (int w = 0; w < Words.size(); ++w)
					b.SetTarget(w, 0.0);

				b.SetTarget(Words.size(), 1.0);
			}

			b.FeedBackward();
		}
	};

	for (; true; ++r)
		fInputWord(rand() % Words.size());
}

int _tmain(int argc, _TCHAR* argv[])
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	cout << fixed;
	cout.precision(2);
	cout << std::setfill('0') << std::setw(5);

	{
		//TNeuralNetworkRNN b(0.03, { 2, 4, 4, 4, 4 });
		//XorTest<TNeuralNetworkRNN>(b);

		//TNeuralNetworkLSTM l(0.01, { 2, 2, 4, 4});
		//XorTest<TNeuralNetworkLSTM>(l);
	}

	{
		//vector<string> Words = {
		//	"hello",
		//	"american",
		//	"accouchement",
		//	"acoustic",
		//	"acupuncture",
		//	"advertisement",
		//	"advocatory",
		//	"oleomargarin",
		//	"heptadecanoic",
		//	"dontopedalogy",
		//	"combustibility",
		//	"cantabrigian",
		//	"stabilizer",
		//	"noordbrabant",
		//	"normothermia",
		//	"planktotrophic",
		//	"repurchase",
		//	"sanguinopurulent",
		//	"skulduggery",
		//	"stethoscopic",
		//	"technicalize",
		//	"vacuum",
		//	"venomously",
		//	"wandering",
		//	"wellhead",
		//	"yellowbellied",
		//	"ytterbium",
		//	"zygopteran",
		//};

		//TNeuralNetworkRNN b(0.01, { 26, 26, 26, Words.size() });
		//WordTest(b, Words); 
	}

	{
		const int AlphabetCount = 26;
		vector<string> Words = {
			"baaaa", // aba baa
			"caaaa", // bba bab
			//"americanawjssdfsdfsdfsdfasdfaweaasjklklsk",
			//"accouchementsdasfasdfaasdfasdfdssdasdfsd",
			//"acousticsdfsfdasfsdsaahajajajajaooieueirioomeme",
			//"commewijgdoicvyweioernfsnsldkfjsdfgh",
			//"jjvnvnvzmxcdxmcvsdfnvlkjspdosnpduvnaadsssdfsdfsdf",
			//"jjjweajjajajajaaiiwiwiijfajsjdjsdjfaajwjjas",
			//"asdfkksskkk",
			//"heptadecanoic",
			//"donsdfsdffofsdfsdfsdfsfsdfsdfdfspedalogy",
			//"comfwbusdssdfsdtibility",
			//"canfsetabrigian",
			//"stadfsdfbilizer",
			//"noordbrablkjxcvnnnxvczxjcpvwijnijnrfuhvnrhant",
			//"norsdmosdfsdfagasdfawegthermia",
			//"planktotjkkhkjhksjhdfrophic",
			//"repusddssdfsdfsdsdfsdfawerchase",
			//"sdfasdfawefaaafaagadfgag",
			//"skulduggyrtery",
			//"stertrtthrtyrtoscopic",
			//"technahgjghjyuiuioioicassdfasdfasdfasdfasdfalize",
			//"vacsdfasdfasdfafiuwenoiuhiuhiuhiouzhiouhiuum",
			//"venoopdiofpdsopfgopudfgmously",
			//"wanopfigpoewirpoiwpeoridering",
			//"wellhdfgdfgdfgoidoifguodifead",
			//"yellowbellied",
			//"yttesdfsdfwpoewurrbium",
			//"zygfsdfwefsadfasdfoiewuproiupoiuidk",
		};
		lstm 랜덤값 조정필요 ?
		TNeuralNetworkRNN b(0.03, { 3, 5, 4, Words.size() + 1 });
		WordTest(b, Words);
	}

	return 0;
}