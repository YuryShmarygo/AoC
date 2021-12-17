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

struct Pos
{ int x; int y; };
struct Area
{ Pos p1; Pos p2; };
int Check(const Area& target, Pos d)
{
	int max = 0;
	Pos p{ 0,0 };
	for (int t = 0; ; ++t)
	{
		p = Pos{ p.x + d.x, p.y + d.y };
		d.x = d.x > 0 ? d.x - 1 : 0;
		d.y -= 1;
		if (p.x > target.p2.x || d.x == 0 && p.x < target.p1.x)
			return -1000;
		if (p.y <= target.p1.y && p.y >= target.p2.y && p.x >= target.p1.x && p.x <= target.p2.x)
			return max;
		if (p.y < target.p2.y)
			return -1000;
		if (p.y > max)
			max = p.y;
	}
}
tuple<int, int> Solve(const Area& target)
{
	int maxy = 0;
	int count = 0;
	for (int y = target.p2.y; y <= 1000; ++y)
	{
		for (int x = 0; x <= target.p2.x; ++x)
		{
			int s = Check(target, Pos{ x,y });
			if (s > maxy)
				maxy = s;
			if (s > -1000)
				++count;
		}
	}
	return { maxy, count };
}
void Test();
int main()
{
	Test();// return 0;
	Area input{ {265, -58}, {287, -103} };

	auto t_start = high_resolution_clock::now();
	auto [maxy, count] = Solve(input);
	cout << "Day17 Answer1: " << maxy; cout << " took " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day17 Answer2: " << count; cout << " took " << ElapsedMs(t_start) << "ms" << endl;
}

void Test()
{
	Area input{ {20, -5}, {30, -10} };
	auto [maxy, count] = Solve(input);
	cout << "Test1: " << maxy << endl;
	cout << "Test1: " << count << endl;
}