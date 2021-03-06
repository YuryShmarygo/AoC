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

struct Point
{
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
	int x;
	int y;
};

bool operator<(const Point& p1, const Point& p2)
{
	return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y;
}
bool operator==(const Point& p1, const Point& p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}
bool IsVisible(const Point& p1, const Point& p2, const map<Point, int>& points, const vector<string>& input)
{
	if (p1 == p2)
		return false;
	
	if (p1.x == p2.x)
	{
		for (int y = min(p1.y, p2.y) + 1; y < max(p1.y, p2.y); ++y)
			if (input[y][p1.x] == '#')
				return false;
	}
	if (p1.y == p2.y)
	{
		for (int x = min(p1.x, p2.x) + 1; x < max(p1.x, p2.x); ++x)
			if (input[p1.y][x] == '#')
				return false;
	}

	for (int y = min(p1.y, p2.y) + 1; y < max(p1.y, p2.y); ++y)
	{
		for (int x = min(p1.x, p2.x) + 1; x < max(p1.x, p2.x); ++x)
		{
			if (input[y][x] == '#' && (p1.y - y) * (p1.x - p2.x) == (p1.y - p2.y) * (p1.x - x))
				return false;
		}
	}

	return true;
}

int main()
{
	vector<string> input;
	ifstream is("Day10.txt");
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() > 0)
			input.push_back(line);
	}
	cout << "Read " << input.size() << " lines" << endl;

	map<Point, int> points;
	for (int y = 0; y < input.size(); ++y)
		for (int x = 0; x < input[y].size(); ++x)
			if (input[y][x] == '#')
				points[Point(x, y)] = 0;

	map<Point, map<Point, bool>> vis;
	for (auto& p : points)
	{
		for (auto& p2 : points)
		{
			auto v = vis[p.first];
			auto it2 = v.find(p2.first);
			if (it2 != v.end())
			{
				if (it2->second)
					p.second++;
			}
			else
			{
				bool isVis = IsVisible(p.first, p2.first, points, input);
				vis[p.first][p2.first] = isVis;
				vis[p2.first][p.first] = isVis;
				if (isVis)
					p.second++;

			}
		}
	}

	auto m = max_element(points.begin(), points.end(), [&](auto p1, auto p2) {return p1.second < p2.second; });
	cout << "Day10, Answer1: " << m->second << endl;

	map<Point, double> rad;
	vector<Point> kill;
	auto c = m->first;
	for (auto vp : vis[c])
	{
		if (vp.second)
		{
			auto p = vp.first;
			double r = 0;
			double g = sqrt(((c.x - p.x) * (c.x - p.x) + (c.y - p.y) * (c.y - p.y)));
			if (p.x >= c.x && p.y <= c.y)
				r = (p.x - c.x) / g;
			else if (p.x >= c.x && p.y >= c.y)
				r = 1 + (p.y - c.y) / g;
			else if (p.x <= c.x && p.y >= c.y)
				r = 2 + (c.x - p.x) / g;
			else
				r = 3 + (c.y - p.y) / g;
			rad[p] = r;
			kill.push_back(p);
		}
	}
	sort(kill.begin(), kill.end(), [&](auto& p1, auto& p2) {return rad[p1] < rad[p2]; });

	cout << "Day 10, answer2: " << kill[199].x * 100 + kill[199].y << endl;
	return 0;
}
