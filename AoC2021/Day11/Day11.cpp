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

struct Pos { int x; int y; };
uint64_t CountFlashes(const vector<string>& input, int cycles)
{
	vector<string> oct(input);
	uint64_t res = 0;
	list<Pos> cur;
	for (int i = 0; i < cycles; ++i)
	{
		vector<string> flashed(oct.size(), string(oct[0].size(), '0'));
		list<Pos> toCheck;
		for (int y = 0; y < oct.size(); ++y)
			for (int x = 0; x < oct[y].size(); ++x)
				toCheck.push_back(Pos{ x,y });
		while (!toCheck.empty())
		{
			for (auto it = toCheck.begin(); it != toCheck.end(); ++it)
				if (flashed[it->y][it->x] == '0')
					++oct[it->y][it->x];
			toCheck.clear();
			for (int y = 0; y < oct.size(); ++y)
			{
				for (int x = 0; x < oct[y].size(); ++x)
				{
					if (oct[y][x] > '9')
					{
						if (flashed[y][x] == '0')
						{
							++res;
							flashed[y][x] = '1';
							oct[y][x] = '0';
							for (int i = max(y - 1, 0); i <= min(y + 1, (int)oct.size() - 1); ++i)
								for (int j = max(x - 1, 0); j <= min(x + 1, (int)oct[0].size() - 1); ++j)
									if (i != y || j != x)
										toCheck.push_back(Pos{ j,i });
						}
					}
				}
			}
		}
		//r::copy(oct, ostream_iterator<string>(cout, "\n")); cout << endl;
		if (all_of(oct.begin(), oct.end(), [](auto& s) {return all_of(s.begin(), s.end(), [](auto c) {return c == '0'; }); }))
			return i + 1;
	}
	return res;
}
void Test();
int main()
{
	Test();
	ifstream is("Day11.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day11 Answer1: " << CountFlashes(input, 100); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day11 Answer2: " << CountFlashes(input, 999999); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
}

void Test()
{
	stringstream test(R"(5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526)");
	istream_iterator<string> start(test), end;
	vector<string> input(start, end);
	vector<string> test0{ "11111","19991","19191","19991","11111" };
	cout << "Test 0: " << CountFlashes(test0, 1) << endl;
	cout << "Test 1: " << CountFlashes(input, 100) << endl;
	cout << "Test 2: " << CountFlashes(input, 999999) << endl;
}