// RsoMailTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#define ZIGZAG_TEST 1

#if ZIGZAG_TEST
#include <iostream>
#include <Rso/Stock/ZigZag.h>

using namespace std;
using namespace rso;
using namespace stock;

CZigZag z(0.5);

void print_zigzag(const CZigZag& z_)
{
    cout << "=============================" << endl;
    if (z.size() > 0)
    {
        for (size_t i = 0; i < z.size(); ++i)
        {
            auto& d = z.at(z.size() - i - 1);
            cout << d.Index << " " << d.Price << endl;
        }
    }
}
int main()
{
    z.insert(0,     SCandle(0, 0, 0, 0 )  ); print_zigzag(z);
    z.insert(1,     SCandle(0, 0, 0, 0 )  ); print_zigzag(z);
    z.insert(2,     SCandle(0, 3, 0, 1 )  ); print_zigzag(z);
    z.insert(3,     SCandle(0, 6, 0, 1 )  ); print_zigzag(z);
    z.insert(4,     SCandle(0, 2, 0, 0 )  ); print_zigzag(z);
    z.insert(5,     SCandle(0, 15, 0, 10)  ); print_zigzag(z);
    z.insert(6,     SCandle(0, 7, 0, 5 )  ); print_zigzag(z);
    z.insert(7,     SCandle(0, 11, 0, 9 )  ); print_zigzag(z);
    z.insert(8,     SCandle(0, 12, 0, 10)  ); print_zigzag(z);

    return 0;
}

#elif CHART_TEST

#include <iostream>
#include <tuple>
#include <Rso/Brain/Brain.h>
#include <Rso/Stock/Chart.h>
#include <Rso/MSSQL/BulkSelect.h>
#include "Protocol.h"

using namespace std;
using namespace rso;
using namespace brain;
using namespace stock;
using namespace mssql;

size_t c_InUnitSize = 21; // 입력 하나당 Classification 개수
size_t c_OutUnitSize = 9; // 출력 하나당 Classification 개수
const size_t g_OutIndex = 6; // c_SizesFuture 의 7번째 값을 말하는 것으로 대략 4분후의 값을 1차로 검사하기 위함
vector<size_t> c_SizesFuture = { 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1 };
vector<size_t> c_SizesPast = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };
// 입력 노드 정보
// 가격차트
// 2 : 시작시각, 종료시각 뉴런
CBrain g_Brain(1.0, 0.003,
    new CNeuronGroupUnionD({
        new CNeuronGroupUnionW({
            new CNeuronGroup({ // 가격 차트
                c_InUnitSize * c_SizesPast.size(), 100
            }),
            new CNeuronGroup({1}), // 시작 시각
            new CNeuronGroup({1}) // 종료 시각
        }),
        new CNeuronGroup({100, c_OutUnitSize})
    })
);

const TEndLayer* g_Input;
const TEndLayer* g_Output;

// 예측과 훈련의 진행상황으로 이 변수간의 차이를 두어 예측전의 데이터가 훈련되는 일이 없도록 하기 위함
struct SMarketInfo
{
    CChart ChartFuture{ c_SizesFuture };
    CChart ChartPast{ c_SizesPast };
    int32 Completed = 0;
    int32 Days = 0; // 날짜가 지났는지 체크
    TTime StartTime; // 장 시작 시각
    TTime EndTime; // 장 종료 시각

    void Tick(const SDataDB& Data_)
    {
        auto Chart = ChartFuture.Insert(SChartTime(SChart(Data_.Close, Data_.Volume), Data_.TimePoint));
        if (Chart)
        {
            ChartPast.Insert(*Chart);
            ++Completed;

            CDateTime DateTime(Chart->TimePoint);
            if (DateTime.day != Days) // Day Changed
            {
                Days = DateTime.day;
                StartTime = Chart->TimePoint;
                EndTime = GetFutureEndTime(Chart->TimePoint);
            }
        }
    }
    const SChartTime& FeedForward(size_t& OutIndex_)
    {
        auto& CurData = ChartPast.CurData();

        size_t InputIndex = 0;

        for (size_t u = 0; u < ChartPast.ChartSize(); ++u)
        {
            double InPriceDiff = CurData.Price - ChartPast.GetPrice(u);

            // 요거 함수로? 또는 그냥 Convolution(CNeuronGroupUnionW) 으로?
            size_t ChartIndex = 0;
            if (InPriceDiff < -2560)
                ChartIndex = 0;
            else if (InPriceDiff < -1280)
                ChartIndex = 1;
            else if (InPriceDiff < -640)
                ChartIndex = 2;
            else if (InPriceDiff < -320)
                ChartIndex = 3;
            else if (InPriceDiff < -160)
                ChartIndex = 4;
            else if (InPriceDiff < -80)
                ChartIndex = 5;
            else if (InPriceDiff < -40)
                ChartIndex = 6;
            else if (InPriceDiff < -20)
                ChartIndex = 7;
            else if (InPriceDiff < -10)
                ChartIndex = 8;
            else if (InPriceDiff < -5)
                ChartIndex = 9;
            else if (InPriceDiff <= 5)
                ChartIndex = 10;
            else if (InPriceDiff <= 10)
                ChartIndex = 11;
            else if (InPriceDiff <= 20)
                ChartIndex = 12;
            else if (InPriceDiff <= 40)
                ChartIndex = 13;
            else if (InPriceDiff <= 80)
                ChartIndex = 14;
            else if (InPriceDiff <= 160)
                ChartIndex = 15;
            else if (InPriceDiff <= 320)
                ChartIndex = 16;
            else if (InPriceDiff <= 640)
                ChartIndex = 17;
            else if (InPriceDiff <= 1280)
                ChartIndex = 18;
            else if (InPriceDiff <= 2560)
                ChartIndex = 19;
            else
                ChartIndex = 20;

            for (size_t i = 0; i < c_InUnitSize; ++i)
            {
                if (i == ChartIndex)
                    (*g_Input)[c_InUnitSize * u + i]->SetOutput(1.0);
                else
                    (*g_Input)[c_InUnitSize * u + i]->SetOutput(0.0);

                InputIndex = c_InUnitSize * u + i;
                ++InputIndex;
            }

            double ElapsedSecondsFromStart = static_cast<double>(duration_cast<seconds>(CurData.TimePoint - StartTime).count());
            double LeftSecondsToEnd = static_cast<double>(duration_cast<seconds>(EndTime - CurData.TimePoint).count());
            (*g_Input)[InputIndex++]->SetOutput(1.0 / (1.0 + ElapsedSecondsFromStart * ElapsedSecondsFromStart / 90000.0)); // 시작시각으로부터 지난시간 노드 5분 지나면 0.5
            (*g_Input)[InputIndex++]->SetOutput(1.0 / (1.0 + LeftSecondsToEnd * LeftSecondsToEnd / 90000.0)); // 종료시각으로까지 남은시간 노드 5분 남으면 0.5
        }

        g_Brain.FeedForward();

        double OutPriceDiff = ChartFuture.GetPrice(g_OutIndex) - CurData.Price;

        if (OutPriceDiff < -140)
            OutIndex_ = 0;
        else if (OutPriceDiff < -100)
            OutIndex_ = 1;
        else if (OutPriceDiff < -80)
            OutIndex_ = 2;
        else if (OutPriceDiff < -40)
            OutIndex_ = 3;
        else if (OutPriceDiff <= 40)
            OutIndex_ = 4;
        else if (OutPriceDiff <= 80)
            OutIndex_ = 5;
        else if (OutPriceDiff <= 100)
            OutIndex_ = 6;
        else if (OutPriceDiff <= 140)
            OutIndex_ = 7;
        else
            OutIndex_ = 8;

        return CurData;
    }
};

SMarketInfo g_MarketInfoPredict;
SMarketInfo g_MarketInfoTrain;

bool g_TrainStarted = false;
ofstream g_File("F:/out.txt");

bool RowCallbackPredict(SQLLEN RowNum_, CStream& Row_)
{
    SDataDB Data;
    Row_ >> Data;

    g_MarketInfoPredict.Tick(Data);

    if (g_MarketInfoPredict.ChartPast.Full())
    {
        size_t OutIndex = 0;
        auto& CurData = g_MarketInfoPredict.FeedForward(OutIndex);

        if (g_TrainStarted &&
            (
                (OutIndex == 0 || OutIndex == 1 || OutIndex == 2 || OutIndex == 3 || OutIndex == 5 || OutIndex == 6 || OutIndex == 7 || OutIndex == 8) ||
                (*g_Output)[0]->GetOutput() > 0.8 ||
                (*g_Output)[1]->GetOutput() > 0.8 ||
                (*g_Output)[2]->GetOutput() > 0.8 ||
                (*g_Output)[6]->GetOutput() > 0.8 ||
                (*g_Output)[7]->GetOutput() > 0.8 ||
                (*g_Output)[8]->GetOutput() > 0.8))
        {
            g_File << "Time[" << CurData.TimePoint.time_since_epoch().count() << "] RealOutIndex[" << OutIndex << "] Predict :";

            g_File.precision(3);
            for (size_t i = 0; i < g_Output->size(); ++i)
            {
                g_File << fixed << " [" << i << " ";
                g_File.width(6);
                g_File << (*g_Output)[i]->GetOutput() << "]";
            }

            g_File << endl;
        }

        // Log ////////////////////////////////////////////////////////////////////////////////////////////////////
        //if ((*g_Output)[0]->GetOutput() > 0.7) f << CurData.TimePoint << "  Error\Data" << (*g_Output)[0]->GetError() << "\tCurPrice:" << g_Chart.GetPrice(0) << " BasePrice:" << BasePrice << "  -140  " << (*g_Output)[0]->GetOutput() << endl;
        //if ((*g_Output)[1]->GetOutput() > 0.7) f << CurData.TimePoint << "  Error\Data" << (*g_Output)[1]->GetError() << "\tCurPrice:" << g_Chart.GetPrice(0) << " BasePrice:" << BasePrice << "  -100  " << (*g_Output)[1]->GetOutput() << endl;
        //if ((*g_Output)[2]->GetOutput() > 0.7) f << CurData.TimePoint << "  Error\Data" << (*g_Output)[2]->GetError() << "\tCurPrice:" << g_Chart.GetPrice(0) << " BasePrice:" << BasePrice << "  -60   " << (*g_Output)[2]->GetOutput() << endl;
        //if ((*g_Output)[6]->GetOutput() > 0.7) f << CurData.TimePoint << "  Error\Data" << (*g_Output)[6]->GetError() << "\tCurPrice:" << g_Chart.GetPrice(0) << " BasePrice:" << BasePrice << "   60   " << (*g_Output)[6]->GetOutput() << endl;
        //if ((*g_Output)[7]->GetOutput() > 0.7) f << CurData.TimePoint << "  Error\Data" << (*g_Output)[7]->GetError() << "\tCurPrice:" << g_Chart.GetPrice(0) << " BasePrice:" << BasePrice << "   100  " << (*g_Output)[7]->GetOutput() << endl;
        //if ((*g_Output)[8]->GetOutput() > 0.7) f << CurData.TimePoint << "  Error\Data" << (*g_Output)[8]->GetError() << "\tCurPrice:" << g_Chart.GetPrice(0) << " BasePrice:" << BasePrice << "   140  " << (*g_Output)[8]->GetOutput() << endl;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    return false;
}
void ParamCallbackPredict(SQLRETURN Ret_, int32 SpRet_, CStream& OutParams_)
{
    cout << "PredictDone : " << "Ret:" << Ret_ << " SPRet:" << SpRet_ << endl;
}
bool RowCallbackTrain(SQLLEN RowNum_, CStream& Row_)
{
    SDataDB Data;
    Row_ >> Data;

    g_MarketInfoTrain.Tick(Data);

    if (g_MarketInfoTrain.ChartPast.Full())
    {
        size_t OutIndex = 0;
        auto& CurData = g_MarketInfoTrain.FeedForward(OutIndex);

        for (size_t i = 0; i < g_Output->size(); ++i)
        {
            if (i == OutIndex)
                (*g_Output)[i]->SetTarget(1.0);
            else
                (*g_Output)[i]->SetTarget(0.0);
        }

        g_Brain.FeedBackward();
        g_TrainStarted = true;
    }

    return false;
}
void ParamCallbackTrain(SQLRETURN Ret_, int32 SpRet_, CStream& OutParams_)
{
    cout << "TrainDone : " << "Ret:" << Ret_ << " SPRet:" << SpRet_ << endl;
}
void main()
{
    g_Input = &g_Brain.GetInputLayer();
    g_Output = &g_Brain.GetOutputLayer();

    CBulkSelect<SBulkSelectTickDBIn, SBulkSelectTickDBOut> BulkSelectPredict(
        SDBOption(1000, 1000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"Stock", 1433, 1000)),
        RowCallbackPredict,
        ParamCallbackPredict,
        L"dbo.spBulkSelectTick");

    CBulkSelect<SBulkSelectTickDBIn, SBulkSelectTickDBOut> BulkSelectTrain(
        SDBOption(1000, 1000, SConnInfo(L"127.0.0.1", L"sa", L"QW34!@er", L"Stock", 1433, 1000)),
        RowCallbackTrain,
        ParamCallbackTrain,
        L"dbo.spBulkSelectTick");

    if (!BulkSelectPredict.Push(SBulkSelectTickDBIn()))
        cout << "Push Fail" << endl;

    if (!BulkSelectTrain.Push(SBulkSelectTickDBIn()))
        cout << "Push Fail" << endl;

    while (true)
    {
        if (g_MarketInfoPredict.Completed <= g_MarketInfoTrain.Completed + 24000) // 아직 예측하지 않은 데이터가 학습되지 않도록 대략 1일 지난 후부터 학습
            BulkSelectPredict.Proc();
        else
            BulkSelectTrain.Proc();
    }
}


#elif 0
#include <iostream>
#include <Rso/Stock/GroupData.h>

using namespace std;
using namespace rso;
using namespace stock;

void main(void)
{
    vector<TValue> v;

    v.begin();
    v.emplace_back(5);
    v.emplace_back(2);
    v.emplace_back(1);
    CGroupData g(v);

    g.push_back(1);
    g.push_back(2);
    g.push_back(3);
    g.push_back(4);
    g.push_back(5);
    g.push_back(6);
    g.push_back(7);
    g.push_back(8);
    g.push_back(9);
    g.push_back(10);
    g.push_back(11);

    for (size_t i = 0; i < g.size(); ++i)
        cout << g.avg(i) << endl;

    for (auto& i : g)
        cout << i << endl;

    cout << g.all_size() << endl;
}

#elif 1
#include <iostream>
//#include <Rso/Stock/DelayedData.h>

using namespace std;
//using namespace rso;
//using namespace stock;

void main(void)
{
    //CDelayedData<TValue> g(2);

    //cout << g.push_back(1) << endl;
    //cout << g.push_back(2) << endl;
    //cout << g.push_back(3) << endl;
    //cout << g.push_back(4) << endl;
    //cout << g.push_back(5) << endl;
    //cout << g.push_back(6) << endl;
}

#endif

//#include <Rso/Stock/RangeSum.h>
//#include <Rso/Stock/SMA.h>
//#include <Rso/Stock/TMA.h>
//#include <Rso/Stock/TrendLine.h>
//#include <Rso/Base/Queue.h>
//#include <iostream>
//#include <Rso/Stock/SMADynamic.h>
//#include <Rso/Stock/StgSMA.h>
//#include <Rso/Stock/SMARange.h>
//#include <Rso/Stock/StockExchange.h>
//#include <Rso/Stock/RangeSum.h>
//#include <Rso/Stock/ChartEx.h>
//#include <Rso/Stock/Trend.h>
//
//
//using namespace std;
//using namespace rso;
//using namespace base;
//using namespace stock;
//
//
//
//#if 1
//
//using TType = double;
//CSMADynamic<TType> cd{ 2, 1 };
//
//void Input(TType In_)
//{
//	cout.width(10);
//	cd.push_back(In_);
//	cout << "Input:" << In_ << "\tSize:" << cd.size() << "\tElements:";
//	for (size_t i = 0; i < cd.size(); ++i)
//		cout << cd.reverse_at(i) << " ";
//
//	cout << endl;
//}
//
//void Callback(time_t TimeT_, TValue Cnt_)
//{
//	cout << TimeT_ << " " << Cnt_ << endl;
//}
//
//
//struct STest
//{
//	CRangeSum<milliseconds> _Data;
//
//	STest(const milliseconds& d_) :
//		_Data(d_)
//	{}
//};
//void OrderCallback(__int64 OrderNo_, TValue Price_, TValue Volume_)
//{
//}
//void ChangeCallback(__int64 OrgOrderNo_, __int64 OrderNo_, TValue Price_, TValue Volume_)
//{
//}
//void CancelCallback(__int64 OrderNo_, TValue Volume_)
//{
//}
//void TradeCallback(__int64 OrderNo_, TValue Price_, TValue Volume_)
//{
//}
//int _tmain(int argc, _TCHAR* argv[])
//{
//	CTrend<int> Trend(3);
//
//	Trend.push(4);
//	Trend.push(0);
//	Trend.push(3);
//	Trend.push(3);
//
//	cout << Trend.Trend() << endl;
//
//	return 0;
//
//
//	CChartEx ChartEx(3, 5);
//	using TData = CChartEx::SData;
//	ChartEx.emplace(TData(0, 1));
//	ChartEx.emplace(TData(1, 7));
//
//	cout << ChartEx.Trend(0) << endl;
//	return 0;
//
//
//
//	STest t(milliseconds(3));
//
//	t._Data.get();
//
//	return 0;
//
//	CStockExchange e(
//		OrderCallback,
//		ChangeCallback,
//		CancelCallback,
//		TradeCallback,
//		milliseconds(300));
//	stock::SRemain Remain(
//		100, 90, 1, 1, 1, 1,
//		110, 80, 1, 1, 1, 1,
//		120, 70, 1, 1, 1, 1,
//		130, 60, 1, 1, 1, 1,
//		140, 50, 1, 1, 1, 1,
//		1, 1, 1, 1, SDateTime());
//	e.SetRemain(Remain);
////	auto Vol = e.SetPrice(110, 2, 110, 90);
//	e.Set(STick(90, 1, 100, 90, -1, SDateTime()));
//	e.OrderBegin(SOrder(EOrderType::시장가, 90, -1));
//
//	return 0;
//
//
//
//
//	CSMARange d(3, 5);
//	d.insert(0);
//	d.insert(1);
//	d.insert(2);
//	d.insert(3);
//	d.insert(3);
//	d.insert(3);
//	d.insert(3);
//
//	for (size_t i = 0; i < d.size(); ++i)
//	{
//		cout << d[i] << endl;
//	}
//
//	return 0;
//
//
//	CSMAs sss{ 5 };
//
//	sss[0].avg();
//
//	return 0;
//
//
//	auto prt = [&]()
//	{
//		//for (size_t i = 0; i < sss.size(); ++i)
//		//	cout << sss[i].avg() << " " << sss[i].diff() << " " << sss[i].diff2() << endl;
//
//		cout << endl;
//	};
//
//	sss.insert(10);	// 10, 0, 0
//	prt();
//
//	sss.insert(20);	// 
//	prt();
//
//	sss.insert(30);
//	prt();
//
//	sss.insert(30);
//	prt();
//
//	sss.insert(30);
//	prt();
//
//	sss.insert(30);
//	prt();
//
//	return 0;
//
//
//	CSMADynamic<double> g_Chart{ 3,2,1 };
//	//vector<double> d;
//	//d.push_back(1.0);
//	//d.push_back(2.0);
//	//d.push_back(3.0);
//	//d.push_back(2.0);
//	//d.push_back(1.0);
//	//d.push_back(0.0);
//	//g_Chart.insert(d);
//	//g_Chart.clear();
//
//	//g_Chart.insert(d);
//	//g_Chart.clear();
//
//	//g_Chart.insert(d);
//	//g_Chart.clear();
//
//	//g_Chart.insert(d);
//	//g_Chart.clear();
//
//	//g_Chart.insert(d);
//
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//cout << endl;
//
//	//double popped = 0.0;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//if(g_Chart.pop_front(popped)) cout << popped << endl; else cout << "no" << endl;
//	//for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.at(i) << " "; cout << endl;
//	//return 0;
//
//	g_Chart.push_back(1.0);
//	cout << endl << "=============" << endl; cout << g_Chart.size() << ":"; for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.reverse_at(i) << " ";
//	g_Chart.push_back(2.0);
//	cout << endl << "=============" << endl; cout << g_Chart.size() << ":"; for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.reverse_at(i) << " ";
//	g_Chart.push_back(3.0);
//	cout << endl << "=============" << endl; cout << g_Chart.size() << ":"; for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.reverse_at(i) << " ";
//	g_Chart.push_back(3.0);
//	cout << endl << "=============" << endl; cout << g_Chart.size() << ":"; for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.reverse_at(i) << " ";
//	g_Chart.push_back(3.0);
//	cout << endl << "=============" << endl; cout << g_Chart.size() << ":"; for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.reverse_at(i) << " ";
//	g_Chart.push_back(3.0);
//	cout << endl << "=============" << endl; cout << g_Chart.size() << ":"; for (size_t i = 0; i < g_Chart.size(); ++i) cout << g_Chart.reverse_at(i) << " ";
//
//	return 0;
//}
//
//#endif