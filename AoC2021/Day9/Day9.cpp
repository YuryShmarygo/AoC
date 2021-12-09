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
#include <ctime>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>
#include <queue>
#include <chrono>
#include <ranges>

using namespace std;
using namespace chrono;
namespace r = std::ranges;
namespace rv = std::ranges::views;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
string ElapsedMs(const steady_clock::time_point& start)
{ return S(((duration<double, milli>(high_resolution_clock::now() - start)).count())); }

uint64_t FindLowest(const vector<string>& input)
{
	uint64_t res = 0;
	for (int y = 0; y < input.size(); ++y)
	{
		for (int x = 0; x < input[y].size(); ++x)
		{
			if ((x == 0 || input[y][x - 1] > input[y][x])
				&& (x == input[y].size() - 1 || input[y][x + 1] > input[y][x])
				&& (y == 0 || input[y - 1][x] > input[y][x])
				&& (y == input.size() - 1 || input[y + 1][x] > input[y][x]))
			{
				res += input[y][x] - '0' + 1;
			}
		}
	}
	return res;
}
struct Point
{
	int x;
	int y;
};
bool operator<(const Point& p1, const Point& p2)
{ return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y; }
uint64_t GetBasin(const vector<string>& input, int x, int y)
{
	set<Point> visited;
	set<Point> step;
	step.insert(Point{ x,y });
	while (!step.empty())
	{
		auto it = step.begin();
		visited.insert(*it);
		if (it->x > 0 && input[it->y][it->x - 1] != '9' && !visited.contains(Point{ it->x - 1, it->y }))
			step.insert(Point{ it->x - 1, it->y });
		if (it->x < input[it->y].size() - 1 && input[it->y][it->x + 1] != '9' && !visited.contains(Point{ it->x + 1, it->y }))
			step.insert(Point{ it->x + 1, it->y });
		if (it->y > 0 && input[it->y - 1][it->x] != '9' && !visited.contains(Point{ it->x, it->y - 1 }))
			step.insert(Point{ it->x, it->y - 1 });
		if (it->y < input.size()-1 && input[it->y + 1][it->x] != '9' && !visited.contains(Point{ it->x, it->y + 1 }))
			step.insert(Point{ it->x, it->y + 1 });
		step.erase(it);
	}
	return visited.size();
}
uint64_t FindBasins(const vector<string>& input)
{
	vector<uint64_t> basins;
	for (int y = 0; y < input.size(); ++y)
	{
		for (int x = 0; x < input[y].size(); ++x)
		{
			if ((x == 0 || input[y][x - 1] > input[y][x])
				&& (x == input[y].size() - 1 || input[y][x + 1] > input[y][x])
				&& (y == 0 || input[y - 1][x] > input[y][x])
				&& (y == input.size() - 1 || input[y + 1][x] > input[y][x]))
			{
				basins.push_back(GetBasin(input, x, y));
			}
		}
	}
	sort(basins.begin(), basins.end());
	return basins[basins.size()-1] * basins[basins.size() - 2] * basins[basins.size() - 3];
}

void Test();
int main()
{
	Test();
	ifstream is("Day9.txt");
	istream_iterator<string> begin(is), end;
	vector<string> input(begin, end);

	auto start = high_resolution_clock::now();
	cout << "Day9 Answer1: " << FindLowest(input); cout << " took " << ElapsedMs(start) << "ms" << endl;
	start = high_resolution_clock::now();
	cout << "Day9 Answer2: " << FindBasins(input); cout << " took " << ElapsedMs(start) << "ms" << endl;
}

void Test()
{
	stringstream is(R"(2199943210
3987894921
9856789892
8767896789
9899965678)");

	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	//cout << "Read " << input.size() << " patterns" << endl;

	cout << "test 1: " << FindLowest(input) << endl;
	cout << "test 2: " << FindBasins(input) << endl;
}