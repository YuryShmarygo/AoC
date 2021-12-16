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
namespace v = std::ranges::views;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
string ElapsedMs(const steady_clock::time_point& start)
{ return S(((duration<double, milli>(high_resolution_clock::now() - start)).count())); }

struct Step
{
	int x;
	int y;
	int r;
};
//bool operator<(const Pos& p1, const Pos& p2) { return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y; }
uint64_t FindPath(const vector<vector<int>>& input)
{
	vector<vector<int>> risk{ input.size(), vector<int>(input[0].size(), 0) };
	list<Step> paths{ {0,0,0} };
	while (!paths.empty())
	{
		auto it = paths.begin();
		for (int y = max(0, it->y-1); y <= min((int)input.size()-1, it->y+1); ++y)
			for (int x = max(0, it->x - 1); x <= min((int)input[y].size() - 1, it->x + 1); ++x)
			{
				if (y == it->y && x == it->x || y != it->y && x != it->x)
					continue;
				int r = it->r + input[y][x];
				if (risk[y][x] == 0 || r < risk[y][x])
				{
					risk[y][x] = r;
					paths.push_back({ x, y, r });
				}
			}
		paths.erase(it);
	}
	return risk[risk.size()-1][risk[risk.size()-1].size()-1];
}
vector<vector<int>> s2i(const vector<string>& input)
{
	vector<vector<int>> map(input.size(), vector<int>(input[0].size(), 0));
	for (int i = 0; i < input.size(); ++i)
		for (int j = 0; j < input[i].size(); ++j)
			map[i][j] = input[i][j] - '0';
	return map;
}
vector<vector<int>> s5i(const vector<string>& input)
{
	vector<vector<int>> map(input.size() * 5, vector<int>(input[0].size() * 5, 0));
	for (int i = 0; i < input.size(); ++i)
		for (int j = 0; j < input[i].size(); ++j)
			for (int y = 0; y < 5; ++y)
				for (int x = 0; x < 5; ++x)
				{
					int r = (input[i][j] - '0') + x + y;
					map[i + y * input.size()][j + x * input.size()] = r > 9 ? r % 9 : r;
				}
	return map;
}
void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day15.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day15 Answer1: " << FindPath(s2i(input)) << endl;
	cout << "Day15 Answer2: " << FindPath(s5i(input)) << endl;
}

void Test()
{
	stringstream is(R"(1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581)");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << "Test 1: " << FindPath(s2i(input)) << endl;
	cout << "Test 2: " << FindPath(s5i(input)) << endl;
}