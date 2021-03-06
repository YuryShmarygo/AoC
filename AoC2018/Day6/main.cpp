#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;
using namespace placeholders;

struct Point
{
	int x;
	int y;
};
int Distance(const Point& p1, const Point& p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

int main()
{
	//read input file
	vector<Point> input;
	ifstream is("input.txt");
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() == 0)
			break;
		Point p;
		if (2 == sscanf_s(line.c_str(), "%d, %d", &p.x, &p.y))
			input.push_back(p);
	}
	cout << "Read " << input.size() << " points" << endl;

	int w = 0, h = 0;
	for (auto& p : input) { w = max(w, p.x); h = max(h, p.y); }
	vector<vector<int>> grid(h, vector<int>(w, -1));
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int minD = 99999999;
			int minP;
			for (int i = 0; i < input.size(); i++)
			{
				Point& p = input[i];
				int d = Distance(p, Point{ x, y });
				if (d < minD)
				{
					minD = d;
					minP = i;
				}
				else if (d == minD)
				{
					minP = -1;
				}
			}
			grid[y][x] = minP;
		}
	}
	map<int, int> aria;
	for (int i = 0; i < input.size(); i++) aria[i] = 0;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int pn = grid[y][x];
			if (x == 0 || y == 0)
			{
				if (pn != -1)
					aria[pn] = -1;
			}
			else if (aria[pn] != -1)
			{
				aria[pn]++;
			}
		}
	}
	int a1 = 0;
	for (auto a : aria) { a1 = max(a1, a.second); }
	cout << "Day6 Answer1: " << a1 << endl;

	int a2 = 0;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int d = 0;
			for (auto p : input) { d += Distance(p, Point{ x,y }); }
			if (d < 10000)
				a2++;
		}
	}
	cout << "Day6 Answer2: " << a2 << endl;


	return 0;
}
