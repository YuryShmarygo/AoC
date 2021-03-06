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

struct Point
{
	int x;
	int y;
	int dx;
	int dy;
};

void Move(Point& p, int m)
{
	m = max(m, 1);
	p.x += p.dx * m;
	p.y += p.dy * m;
}

void Move(vector<Point>& points, int m)
{
	for (auto& p : points) Move(p, m);
}

void MinMax(vector<Point>& points, int& minX, int& maxX, int& minY, int& maxY)
{
	minX = minY = 99999; maxX = maxY = -999999;
	for (auto i : points) { minX = min(minX, i.x); maxX = max(maxX, i.x); minY = min(minY, i.y); maxY = max(maxY, i.y); }
}

int GetDistance(vector<Point>& points)
{
	int minX, maxX, minY, maxY;
	MinMax(points, minX, maxX, minY, maxY);

	return maxX - minX;
}

void Project(vector<Point>& points)
{
	cout << "============================================" << endl;

	sort(points.begin(), points.end(), [](auto p1, auto p2) {return p1.y < p2.y || p1.y == p2.y && p1.x < p2.x; });
	int minX, maxX, minY, maxY;
	MinMax(points, minX, maxX, minY, maxY);
	int curX = minX, curY = minY;
	for (auto p : points)
	{
		if (p.y != curY) 
		{ 
			cout << endl; curY = p.y; curX = minX; 
		}
		generate_n(ostream_iterator<char>(cout), p.x - curX, []() {return ' '; });
		if (p.x >= curX)
			cout << '#';
		curX = p.x + 1;
	}
}

int main()
{
	vector<Point> input;
	ifstream is("input.txt");
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() == 0)
			break;
		Point p;
		if (4 == sscanf_s(line.c_str(), "position=<%d, %d> velocity=<%d, %d>", &p.x, &p.y, &p.dx, &p.dy))
			input.push_back(p);
	}
	cout << "Read " << input.size() << " points" << endl;

	int duration = 0;
	while (true)
	{
		int d = GetDistance(input);
		cout << d << " ";
		int m = d > 100 ? d /30 : 1;
		Move(input,  m);
		duration += m;
		if (d < 100)
		{
			cout << endl;
			Project(input);
			string line;
			getline(cin, line);
			cout << "Duration: " << duration << endl;
		}
	}

	return 0;
}
