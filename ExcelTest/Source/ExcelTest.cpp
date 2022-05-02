#include "stdafx.h"
#include <iostream>
#include <Rso/Excel/Excel.h>
#include "Protocol.h"


using namespace std;
using namespace rso;
using namespace excel;


int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		//wcout.imbue(std::locale("korean"));

		//CExcel Excel(L"a.xlsx");
		//STest s;
		//Excel.Extract(L"Sheet1", L"Name,Index", s);

		//for (auto& d : s.Datas)
		//	wcout << d.Index << L" " << d.Name << endl;

		wcout.imbue(std::locale("korean"));

		CExcel Excel(L"MP_Metadata.xlsm", L"userdata_preset,start_user_building", L',');
		SUserPresets UserPresets;
		Excel.Extract(L"userdata_preset", L"default_amethyst,default_topaz", UserPresets);
		for (auto& d : UserPresets.Datas)
			wcout << d.Ame << L" " << d.Topaz << endl;

		SStartUserBuildings StartUserBuildings;
		Excel.Extract(L"start_user_building", L"building_group_id,±âÈ¹_desc,building_level,building_grid_col,building_grid_row", StartUserBuildings);
		for (auto& d : StartUserBuildings.Datas)
			wcout << d.id << L" " << d.desc << L" " << (__int32)d.level << L" " << d.col << L" " << d.row << endl;
	}
	catch (const exception& Exception_)
	{
		cout << Exception_.what() << endl;
	}
	catch (...)
	{
		cout << "Exception" << endl;
	}

	return 0;
}