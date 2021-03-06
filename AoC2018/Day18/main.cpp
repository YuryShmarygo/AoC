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

using namespace std;
using namespace placeholders;

typedef vector<string> Area;

int countAdjacent(const Area& area, int x, int y, function<bool(char)> pred)
{
	int res = 0;
	for (int y1 = max(0, y - 1); y1 < min((int)area.size(), y + 2); ++y1)
		for (int x1 = max(0, x - 1); x1 < min((int)area[y].size(), x + 2); ++x1)
			if ((y1 != y || x1 != x) && pred(area[y1][x1]))
				++res;
	return res;
}
int countArea(const Area& area, function<bool(char)> pred)
{
	int res = 0;
	for (int y = 0; y < area.size(); ++y)
		for (int x = 0; x < area[y].size(); ++x)
			if (pred(area[y][x]))
				++res;
	return res;
}
auto isTree = [](char c) {return c == '|'; };
auto isLumb = [](char c) {return c == '#'; };
Area Envolve(const Area& area)
{
	Area a(area.size(), string(area[0].size(), ' '));
	for (int y = 0; y < area.size(); ++y)
	{
		for (int x = 0; x < area[y].size(); ++x)
		{
			switch (area[y][x])
			{
			case '.':
				a[y][x] = countAdjacent(area, x, y, isTree) >= 3 ? '|' : '.';
				break;
			case '|':
				a[y][x] = countAdjacent(area, x, y, isLumb) >= 3 ? '#' : '|';
				break;
			case '#':
				a[y][x] = countAdjacent(area, x, y, isLumb) >= 1
					&& countAdjacent(area, x, y, isTree) >= 1 ? '#' : '.';
				break;
			}
		}
	}
	return a;
}

int Solve1(Area a)
{
	for (int i = 0; i < 10; ++i)
		a = Envolve(a);
	return countArea(a, isTree) * countArea(a, isLumb);
}

int Solve2(Area a, long long minutes)
{
	vector<int> lumb;
	lumb.reserve(3001);
	lumb.push_back(countArea(a, isLumb));
	for (long long i = 0; i < 3000; ++i)
	{
		a = Envolve(a);
		lumb.push_back(countArea(a, isLumb) * countArea(a, isTree));
	}
	int cycle = 5;
	while (true)
	{
		bool allEq = true;
		for (int i = 1; i <= 3; i++)
			allEq = allEq && equal(prev(lumb.end(), (i + 1) * cycle), prev(lumb.end(), i * cycle), prev(lumb.end(), i *cycle));
			
		if (allEq) break;
		cycle++;
	}
	//cout << "cycle: " << cycle << " " << minutes << " % " << cycle << " = "  << minutes % cycle << endl;
	return lumb[(((int)lumb.size()-cycle-1) / cycle) * cycle + minutes % cycle];
}
int main()
{
	ifstream is("d18.txt");
	istream_iterator<string> start(is), end;
	Area input(start, end);

	cout << "Day18 Answer1: " << Solve1(input) << endl;
	cout << "Day18 Answer2: " << Solve2(input, 1000000000) <<  endl;
	return 0;
}
