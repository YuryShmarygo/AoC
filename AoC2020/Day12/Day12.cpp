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
	int x;
	int y;
	char dir;
};
struct Dir
{
	char dir;
	int num;
};
istream& operator>>(istream& is, Dir& dir) { is >> dir.dir >> dir.num; return is; }

static map<char, map<char, char>> tmoves = {
	{'R', {{'E', 'S'}, {'S','W'}, {'W','N'}, {'N','E'}}},
	{'L', {{'E', 'N'}, {'N','W'}, {'W','S'}, {'S','E'}}}
};
char turn(char d, const Dir& dir)
{
	for (int i = 0; i < dir.num / 90; ++i)
		d = tmoves[dir.dir][d];
	return d;
}
static map<char, function<Pos(const Pos& pos, const Dir& dir)>> fmoves = {
	{'N', [&](const Pos& pos, const Dir& dir) {return Pos{pos.x, pos.y - dir.num, pos.dir}; }},
	{'W', [&](const Pos& pos, const Dir& dir) {return Pos{pos.x - dir.num, pos.y, pos.dir}; }},
	{'S', [&](const Pos& pos, const Dir& dir) {return Pos{pos.x, pos.y + dir.num, pos.dir}; }},
	{'E', [&](const Pos& pos, const Dir& dir) {return Pos{pos.x + dir.num, pos.y, pos.dir}; }}
};
static map<char, function<Pos(const Pos& pos, const Dir& dir)>> smoves = {
	{'L', [&](const Pos& pos, const Dir& dir) {return Pos{pos.x, pos.y, turn(pos.dir, dir)}; }},
	{'R', [&](const Pos& pos, const Dir& dir) {return Pos{pos.x, pos.y, turn(pos.dir, dir)}; }},
	{'F', [&](const Pos& pos, const Dir& dir) {return fmoves[pos.dir](pos, dir); }}
};
static map<char, function<Pos(const Pos& pos, const Dir& dir)>> wmoves = {
	{'L', [&](const Pos& pos, const Dir& dir) {return Pos{pos.y, -pos.x, pos.dir}; }},
	{'R', [&](const Pos& pos, const Dir& dir) {return Pos{-pos.y, pos.x, pos.dir}; }}
};

int find_distance(const vector<Dir>& input)
{
	auto moves = fmoves;
	moves.insert(smoves.begin(), smoves.end());

	Pos pos{ 0,0,'E' };

	for (auto& m : input)
	{
		//cout << pos.dir << '[' << pos.x << ',' << pos.y << "] - " << m.dir << m.num << endl;
		pos = moves[m.dir](pos, m);
	}
	return abs(pos.x) + abs(pos.y);
}

int find_distance_w(const vector<Dir>& input)
{
	Pos wp{ 10, -1, 'N' };
	Pos pos{ 0,0,'E' };

	for (auto& m : input)
	{
		//cout << '[' << pos.x << ',' << pos.y << "] - [" << wp.x << "," << wp.y << "] - " << m.dir << m.num << endl;
		if (m.dir == 'F')
		{
			pos.x += wp.x * m.num;
			pos.y += wp.y * m.num;
		}
		else if (m.dir == 'L' || m.dir == 'R')
		{
			for (int i = 0; i < m.num / 90; ++i)
				wp = wmoves[m.dir](wp, m);
		}
		else
		{
			wp = fmoves[m.dir](wp, m);
		}
	}
	return abs(pos.x) + abs(pos.y);
}

void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day12.txt");
	istream_iterator<Dir> start(is), end;
	vector<Dir> input(start, end);

	cout << "Day12 Answer1: " << find_distance(input) << endl;

	cout << "Day12 Answer2: " << find_distance_w(input) << endl;
}

void Test()
{
	stringstream test(R"(F10
N3
F7
R90
F11)");
	istream_iterator<Dir> start(test), end;
	vector<Dir> input(start, end);
	cout << "Test 1: " << find_distance(input) << endl;
	cout << "Test 2: " << find_distance_w(input) << endl;
}