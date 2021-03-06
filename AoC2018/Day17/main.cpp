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

enum RangeType
{
	Vert, Hor
};
struct Range
{
	RangeType type;
	int x1;
	int x2;
	int y1;
	int y2;
};
istream& operator>>(istream& i, Range& range)
{
	string line;
	getline(i, line);
	if (line[0] == 'y')
	{
		range.type = Hor;
		sscanf_s(line.c_str(), "y=%d, x=%d..%d", &range.y1, &range.x1, &range.x2);
		range.y2 = range.y1;
	}
	else
	{
		range.type = Vert;
		sscanf_s(line.c_str(), "x=%d, y=%d..%d", &range.x1, &range.y1, &range.y2);
		range.x2 = range.x1;
	}
	return i;
}
enum ClayType
{
	Sand,
	Clay,
	WSett,
	WFlow,
	WaterS
};
typedef vector<vector<ClayType>> Scan;
char ClayToChar(ClayType type)
{
	switch (type) {
	case Sand: return '.';
	case Clay: return '#';
	case WSett: return '~';
	case WFlow: return '|';
	};
	return 'x';
}
ostream& operator<<(ostream& o, const Scan& scan)
{
	for (auto line : scan)
	{
		for (auto i : line)
			o << ClayToChar(i);
		o << endl;
	}
	return o;
}
int FillRight(Scan& scan, int x, int y)
{
	for (int i = x + 1; i < scan[0].size(); i++)
	{
		if (scan[y][i] == WFlow)
			return i;
		if (scan[y][i] == Sand)
			scan[y][i] = WFlow;
		if (scan[y][i] == Clay || scan[y + 1][i] == Sand)
			return i;
	}
	return (int)scan[0].size();
}
int FillLeft(Scan& scan, int x, int y)
{
	for (int i = x - 1; i >= 0; i--)
	{
		if (scan[y][i] == Sand)
			scan[y][i] = WFlow;
		if (scan[y][i] == Clay || scan[y + 1][i] == Sand)
			return i;
	}
	return -1;
}
void FillWater(Scan& scan, int x, int y)
{
	if (scan[y][x] == Sand || scan[y][x] == WFlow)
	{
		for (; y < scan.size() - 1; ++y)
		{
			if ((scan[y][x] == Sand || scan[y][x] == WFlow )&& scan[y + 1][x] == Sand)
				scan[y][x] = WFlow;
			else
				break;
		}
		scan[y][x] = WFlow;

		if (y == scan.size() - 1)
			return;

		else if (scan[y + 1][x] == Clay || scan[y+1][x] == WSett)
		{
			//scan[y][x] = WFlow;
			int right = FillRight(scan, x, y);
			int left = FillLeft(scan, x, y);
			if (right < scan[0].size() && scan[y][right] == Clay &&
				left >= 0 && scan[y][left] == Clay)
			{
				for (int i = left + 1; i < right; i++)
					scan[y][i] = WSett;
				FillWater(scan, x, y - 1);
				return;
			}
			if (right < scan[0].size() && scan[y][right] == WFlow)
				FillWater(scan, right, y);
			if (left >= 0 && scan[y][left] == WFlow)
				FillWater(scan, left, y);
		}
	}
}
Scan CreateScan(vector<Range>& input, int& shift)
{
	int minX = 500, maxX = 500, minY = 0, maxY = 0;
	for (auto i : input)
	{
		if (i.x1 < minX)
			minX = i.x1;
		if (i.x2 > maxX)
			maxX = i.x2;
		if (i.y1 < minY)
			minY = i.y1;
		if (i.y2 > maxY)
			maxY = i.y2;
	}

	shift = minX - 1;
	vector<vector<ClayType>> scan(maxY + 1, vector<ClayType>(maxX - minX + 3, Sand));
	for (auto i : input)
	{
		if (i.type == Hor)
		{
			for (int x = i.x1 - shift; x <= i.x2 - shift; ++x)
				scan[i.y1][x] = Clay;
		}
		else
		{
			for (int y = i.y1; y <= i.y2; ++y)
				scan[y][i.x1 - shift] = Clay;
		}
	}
	return scan;
}
int CountWater(const Scan& scan, function<bool(ClayType)> pred = [](auto t) {return t == WSett || t == WFlow; })
{
	int water = 0;
	int y = 0;
	for (; y < scan.size(); ++y)
		if (scan[y].end() != find(scan[y].begin(), scan[y].end(), Clay))
			break;
	for (; y < scan.size(); y++)
	{
		for (int x = 0; x < scan[y].size(); x++)
			if (pred(scan[y][x]))
				water++;
	}
	return water;
}
void test();
int main()
{
	//test(); return 0;
	vector<Range> input;
	ifstream is("d17.txt");
	Range range;
	while (is >> range)
		input.push_back(range);
	cout << "Read " << input.size() << " ranges" << endl;

	int shift = 0;
	Scan scan = CreateScan(input, shift);
	FillWater(scan, 500 - shift, 0);
	ofstream("d17.out") << scan;

	cout << "Day17 Answer1: " << CountWater(scan) << endl;
	cout << "Day17 Answer2: " << CountWater(scan, [](auto t) {return t == WSett; }) <<  endl;
	return 0;
}

void test()
{
	vector<string> test = { 
"x=495, y=2..7",
"y=7, x=495..501",
"x=501, y=3..7",
"x=498, y=2..4",
"x=506, y=1..2",
"x=498, y=10..13",
"x=504, y=10..13",
"y=13, x=498..504" };

	vector<Range> input;
	for (auto s : test)
	{
		Range r;
		istringstream(s) >> r;
		input.push_back(r);
	}
	int shift = 0;
	auto scan = CreateScan(input, shift);
	FillWater(scan, 500 - shift, 0);
	cout << "Test 1: " << CountWater(scan) << endl;
}
