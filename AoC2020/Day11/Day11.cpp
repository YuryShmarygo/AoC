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

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

struct Pos
{
	int y;
	int x;
	Pos operator+(const Pos& p) const { return Pos{ y + p.y, x + p.x }; }
};
char get_pos(const vector<string>& input, const Pos& p)
{
	return p.y < 0 || p.y >= input.size() || p.x < 0 || p.x >= input[p.y].size() ? ' ' : input[p.y][p.x];
}
char get_state(const vector<string>& input, const Pos& pos, bool first = false)
{
	static vector<Pos> moves{ { -1,-1 }, { -1,0 }, { -1,1 }, { 0,-1 }, { 0,1 }, { 1,-1 }, { 1,0 }, { 1,1 } };

	if (input[pos.y][pos.x] == '.')
		return '.';
	int occupied = 0;
	for (auto& m : moves)
	{
		Pos next = pos + m;
		char c = get_pos(input, next);
		while (first && c == '.')
		{
			next = next + m;
			c = get_pos(input, next);
		}
		if (c == '#')
			++occupied;
	}
	if (input[pos.y][pos.x] == 'L' && occupied == 0)
		return '#';
	if (input[pos.y][pos.x] == '#' && (!first && occupied >= 4 || first && occupied >= 5))
		return 'L';
	return input[pos.y][pos.x];
}
vector<string> process(const vector<string>& input, bool& changed, bool first = false)
{
	changed = false;
	vector<string> res(input.size(), string(input[0].size(), ' '));
	for (int y = 0; y < input.size(); ++y)
	{
		for (int x = 0; x < input[y].size(); ++x)
		{
			res[y][x] = get_state(input, Pos{ y, x }, first);
			if (!changed && res[y][x] != input[y][x])
				changed = true;
		}
	}
	return res;
}
int find_stable(const vector<string>& input, bool first = false)
{
	int cycle = 0;
	auto cur = input;
	bool changed = true;
	while (changed)
	{
		cur = process(cur, changed, first);
		++cycle;
	}
	
	return accumulate(cur.begin(), cur.end(), (int)0, [](auto sum, auto& s) {return sum + (int)count(s.begin(), s.end(), '#'); });
}

void Test();
int main()
{
	//Test(); return 0;
	ifstream is("Day11.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day11 Answer1: " << find_stable(input) << endl;

	cout << "Day11 Answer2: " << find_stable(input, true) << endl;
}

void Test()
{
	stringstream test(R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL)");
	istream_iterator<string> start(test), end;
	vector<string> input(start, end);
	cout << "Test 1: " << find_stable(input) << endl;
	cout << "Test 2: " << find_stable(input, true) << endl;
}