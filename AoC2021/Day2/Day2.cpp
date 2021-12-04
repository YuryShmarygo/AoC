#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <regex>
#include <functional>

using namespace std;

enum EDir { FORWARD, UP, DOWN };
struct Cmd
{
	EDir dir;
	long n;
};

istream& operator>>(istream& i, Cmd& p)
{
	static map<string, EDir> s2d{ {"up", UP}, {"down", DOWN}, {"forward", FORWARD} };
	string dir;
	i >> dir >> p.n;
	p.dir = s2d[dir];
	return i;
}

struct Pos
{
	long l;
	long d;
};

Pos Move(const vector<Cmd>& input)
{
	static map<EDir, function<void(Pos& pos, int n)>> moves{
		{FORWARD,	[](Pos& pos, int n) { pos.l += n; }},
		{UP,		[](Pos& pos, int n) { pos.d -= n; }},
		{DOWN,		[](Pos& pos, int n) { pos.d += n; }},
	};
	Pos pos{ 0, 0 };
	for (auto it = input.begin(); it != input.end(); ++it)
		moves[it->dir](pos, it->n);
	return pos;
}

struct Aim
{
	long l;
	long d;
	long a;
};

Aim Move2(const vector<Cmd>& input)
{
	static map < EDir, function<void(Aim& aim, int n)>> moves{
		{FORWARD,	[](Aim& aim, int n) { aim.l += n; aim.d += aim.a * n; }},
		{UP,		[](Aim& aim, int n) { aim.a -= n; }},
		{DOWN,		[](Aim& aim, int n) { aim.a += n; }},
	};
	Aim aim{ 0,0,0 };
	for (auto it = input.begin(); it != input.end(); ++it)
		moves[it->dir](aim, it->n);
	return aim;
}

int main()
{
	ifstream is("Day2.txt");
	istream_iterator<Cmd> start(is), end;
	vector<Cmd> input(start, end);
	//cout << "Read " << input.size() << " commands" << endl;

	auto pos = Move(input);
	cout << "Day2 answer1: " << pos.l * pos.d << endl;
	auto aim = Move2(input);
	cout << "Day2 answer2: " << aim.l * aim.d << endl;
}
