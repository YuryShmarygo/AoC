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
{
	int x;
	int y;
};
bool operator<(const Pos& p1, const Pos& p2) { return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y; }
bool operator==(const Pos& p1, const Pos& p2) { return p1.x == p2.x && p1.y == p2.y; }
istream& operator>>(istream& is, Pos& pos)
{
	char sep;
	is >> pos.x >> sep >> pos.y;
	return is;
}
struct Man
{
	vector<Pos> dots;
	vector<Pos> folds;
};
istream& operator>>(istream& is, Man& man)
{
	string line;
	while (getline(is, line) && !line.empty())
	{
		Pos pos;
		stringstream(line) >> pos;
		man.dots.push_back(pos);
	}
	while (getline(is, line) && !line.empty())
	{
		Pos pos{ 0,0 };
		if (line.find("fold along x=") != string::npos)
			stringstream(line.substr(13)) >> pos.x;
		else
			stringstream(line.substr(13)) >> pos.y;
		man.folds.push_back(pos);
	}
	return is;
}
ostream& operator<<(ostream& os, const set<Pos>& dots)
{
	vector<string> p;
	for (auto& d : dots)
	{
		while (p.size() < d.y + 1)
			p.push_back(string());
		while (p[d.y].size() < d.x + 1)
			p[d.y].push_back(' ');
		p[d.y][d.x] = '#';
	}
	r::copy(p, ostream_iterator<string>(os, "\n"));
	return os;
}

set<Pos> Fold(const set<Pos>& dots, Pos fold)
{
	set<Pos> folded;
	for (auto it = dots.begin(); it != dots.end(); ++it)
	{
		if (fold.x > 0)
		{
			if (it->x < fold.x)
				folded.insert(*it);
			else
				folded.insert(Pos{ fold.x + fold.x - it->x, it->y });
		}
		else
		{
			if (it->y < fold.y)
				folded.insert(*it);
			else
				folded.insert(Pos{ it->x, fold.y + fold.y - it->y });
		}
	}
	return folded;
}
uint64_t CountFold(const vector<Pos>& dots, const Pos& fold)
{
	set<Pos> sdots(dots.begin(), dots.end());
	return Fold(sdots, fold).size();
}
set<Pos> Fold(const Man& input)
{
	set<Pos> sdots(input.dots.begin(), input.dots.end());
	for (auto& f : input.folds)
		sdots = Fold(sdots, f);
	return sdots;
}

void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day13.txt");
	Man input; is >> input;

	auto t_start = high_resolution_clock::now();
	cout << "Day13 Answer1: " << CountFold(input.dots, input.folds[0]); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	auto folded = Fold(input);
	cout << "Day13 Answer2: " << endl << folded << endl; cout << " took " << ElapsedMs(t_start) << "ms" << endl;
}

void Test()
{
	stringstream is(R"(6,10
0,14
9,10
0,3
10,4
4,11
6,0
6,12
4,1
0,13
10,12
3,4
3,0
8,4
1,10
2,14
8,10
9,0

fold along y=7
fold along x=5)");
	Man input; is >> input;
	cout << "Test 1: " << CountFold(input.dots, input.folds[0]) << endl;
	auto folded = Fold(input);
	cout << "Test 2: " << endl << folded << endl;
}