#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>
#include <unordered_map>

using namespace std;
using namespace placeholders;

struct Point
{
	int x;
	int y;
};
bool operator<(const Point& p1, const Point& p2)
{ return p1.y < p2.y || p1.y == p2.y && p1.x < p2.x; }

enum RType {rocky, wet, narrow};
ostream& operator<<(ostream& os, const RType& t)
{ os << (t == rocky ? '.' : t == wet ? '=' : '|'); return os; }
RType FromChar(char c)
{ return c == '.' ? rocky : c == '=' ? wet : narrow; }

typedef vector<vector<RType>> Cave;
ostream& operator<<(ostream& os, const Cave& cave)
{
	for (auto line : cave) {
		for (auto e : line)
			os << e;
		os << endl;
	}
	return os;

}
Cave BuildCave(int depth, const Point& target)
{
	int h = target.y + 1 + target.x * 2;
	int w = target.x + 1 + target.y * 2;

	Cave cave(h, vector<RType>(w, rocky));
	vector<vector<int>> el(h, vector<int>(w, -1));
	for (int n = 0; n < max(h, w); ++n)
	{
		for (int y = 0; y < min(n, h); ++y)
		{
			for (int x = 0; x < min(n, w); ++x)
			{
				int gi = 0;
				if (x == 0 && y == 0)
					gi = 0;
				else if (x == target.x && y == target.y)
					gi = 0;
				else if (y == 0)
					gi = x * 16807;
				else if (x == 0)
					gi = y * 48271;
				else
					gi = el[y - 1][x] * el[y][x - 1];

				el[y][x] = (gi + depth) % 20183;
				cave[y][x] = (RType)(el[y][x] % 3);
			}
		}
	}
	return cave;
}
const int cm = 999999999;
struct Cost
{
	int x;
	int y;
	vector<int> c;
	Cost(int _x = 0, int _y = 0, vector<int> _c = { cm,cm,cm }) : x(_x), y(_y), c(_c) {}
};

enum EType {torch, climb, none};
vector<EType> GetAllowed(RType r)
{
	switch (r)
	{
	case rocky: return { torch, climb };
	case wet: return { climb, none };
	case narrow: return { torch, none };
	};
}
bool IsAllowed(RType r, EType e)
{
	return r == rocky && (e == torch || e == climb)
		|| r == wet && (e == climb || e == none)
		|| r == narrow && (e == torch || e == none);
}
vector<Point> GetCross(const Point& p, int w, int h)
{
	vector<Point> c;
	if (p.y > 0)
		c.push_back(Point{ p.x, p.y - 1 });
	if (p.x > 0)
		c.push_back(Point{ p.x - 1, p.y });
	if (p.x < w - 1)
		c.push_back(Point{ p.x + 1, p.y });
	if (p.y < h - 1)
		c.push_back(Point{ p.x, p.y + 1 });
	return c;
}
int FindRoute(const Cave& cave, const Point& target)
{
	int h = (int)cave.size();
	int w = (int)cave[0].size();
	vector<vector<vector<int>>> cost(h, vector<vector<int>>(w, vector<int>(3, cm)));
	map<Point, Cost> cur{ { { 0,0 }, { 0, 0, { 0, 7, cm } } } };
	map<Point, Cost> next;
	EType tool = torch;
	while (cur.size() > 0)
	{
		for (auto pc : cur)
		{
			auto p = pc.second;
			// should we continue from this point?
			bool cont = false;
			for (int t = 0; t < 3; ++t)
			{
				if (p.c[t] < cost[p.y][p.x][t])
				{
					cont = true;
					cost[p.y][p.x][t] = p.c[t];
				}
			}
			if (cont)
			{
				vector<Cost> temp; // collect all possible costs for all directions : for each direction it is 3 x 3 (from each tool to each tool)
				for (auto& cp : GetCross(Point{ p.x, p.y }, w, h))
				{
					int x = cp.x, y = cp.y;
					vector<int> newCost(3, cm);
					bool add = false;
					for (int t = 0; t < 3; ++t)
					{
						newCost[t] = !IsAllowed(cave[y][x], (EType)(t)) ? cm : min(
							min(p.c[0] + (!IsAllowed(cave[y][x], (EType)0) ? cm : t == 0 ? 1 : 8)
								, p.c[1] + (!IsAllowed(cave[y][x], (EType)1) ? cm : t == 1 ? 1 : 8))
							, p.c[2] + (!IsAllowed(cave[y][x], (EType)2) ? cm : t == 2 ? 1 : 8));
						if (newCost[t] < cost[y][x][t])
							add = true;
					}
					if (add)
						temp.push_back({ x, y, newCost });
				}
				for (auto& n : temp)
				{
					auto& c = next[Point{ n.x, n.y }];
					c.x = n.x; c.y = n.y;
					for (int t = 0; t < 3; ++t)
						c.c[t] = min(c.c[t], n.c[t]);
				}
			}
		}
		swap(cur, next);
		next.clear();
	}
	cout << cost[target.y][target.x][0] << " " << cost[target.y][target.x][1] << " " << cost[target.y][target.x][2] << endl;
	return min(min(cost[target.y][target.x][0], cost[target.y][target.x][1] + 7), cost[target.y][target.x][2] +7);
}
void test();
int main()
{
	//test(); return 0;
	int depth = 4848;
	Point target = { 15, 700 };
	Cave cave = BuildCave(depth, target);

	//ofstream("d22.out") << cave;
	/*ifstream is(R"(C:\Projects\AoC\AoC2018\x64\Release\d22.out)");
	Cave cave;
	cave.reserve(800);
	string line;
	while (getline(is, line) && line.size() > 0)
	{
		vector<RType> level;
		level.reserve(line.size());
		transform(line.begin(), line.end(), back_inserter(level), &FromChar);
		cave.push_back(level);
	}*/
	
	int risk = 0;
	for (int y = 0; y <= target.y; ++y)
		for (int x = 0; x <= target.x; ++x)
			risk += (int)cave[y][x];

	cout << "Day22 Answer1: " << risk << endl;
	cout << "Day22 Answer2: " << FindRoute(cave, target) <<  endl;
	return 0;
}

void test()
{
	vector<string> input = {
		".=.|=.|.|=.|=|=.",
		".|=|=|||..|.=...",
		".==|....||=..|==",
		"=.|....|.==.|==.",
		"=|..==...=.|==..",
		"=||.=.=||=|=..|=",
		"|.=.===|||..=..|",
		"|..==||=.|==|===",
		".=..===..=|.|||.",
		".======|||=|=.|=",
		".===|=|===T===||",
		"=|||...|==..|=.|",
		"=.=|=.=..=.||==|",
		"||=|=...|==.=|==",
		"|=.=||===.|||===",
		"||.|==.|.|.||=||" };

	Cave cave;
	for(auto line : input)
	{
		vector<RType> level;
		transform(line.begin(), line.end(), back_inserter(level), &FromChar);
		cave.push_back(level);
	}
	cout << "Test: " << FindRoute(cave, Point{ 10,10 }) << endl;
}
