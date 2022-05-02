#include "stdafx.h"

#define Test_PathFind 1

#if Test_Graph
#include <Rso/PathFind/Graph.h>

using namespace rso;
using namespace pathfind;

void Print(const CGraph::TPath& Path_)
{
	for (auto& i : Path_)
		cout << "[" << i.X << ", " << i.Y << "]\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	CGraph g;

	auto it = g.AddNode(SPoint(0.0, 0.0));
	auto it1 = g.AddNode(SPoint(1.0, 1.0));
	auto it2 = g.AddNode(SPoint(2.0, 0.0));
	auto it3 = g.AddNode(SPoint(1.0, -1.0));
	auto it4 = g.AddNode(SPoint(-1.0, 0.0));
	auto it5 = g.AddNode(SPoint(3.0, 0.0));

	g.AddLink(&(*it), &(*it3));
	g.AddLink(&(*it3), &(*it2));
	g.AddLink(&(*it), &(*it1));
	g.AddLink(&(*it1), &(*it2));
	g.AddLink(&(*it), &(*it4));
	g.AddLink(&(*it4), &(*it5));

	auto path = g.Find(&(*it5), &(*it));

	Print(path);

	return 0;
}

#elif Test_PathFind
#include <Rso/PathFind/PathFind.h>

using namespace rso;
using namespace pathfind;

using TPoint = CPathFind::SPoint;

const size_t g_Width = 100;
const size_t g_Height = 100;

char m[g_Height][g_Width + 1];

void Print(const CPathFind::TPath& Path_)
{
	for (size_t h = 0; h < g_Height; ++h)
	{
		for (size_t w = 0; w < g_Width; ++w)
			m[h][w] = '0';

		m[h][g_Width] = '\0';
	}

	for (auto& i : Path_)
		m[i.Y][i.X] = '*';

	for (size_t h = g_Height; h > 0; --h)
		cout << m[h - 1] << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CPathFind p(g_Width, g_Height);

	auto& path = p.Find(TPoint(0, 0), TPoint(g_Width - 1, g_Height - 1));

	Print(path);

	return 0;
}

#endif