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
	bool operator<(const Pos& p) const { return x < p.x || x == p.x && y < p.y; }
	Pos operator+(const Pos& p) const { return Pos{ x + p.x, y + p.y }; }
};
map<string, Pos> moves = { {"e", {2, 0}}, {"se", {1, -1} }, {"sw", {-1, -1}}, {"w", {-2, 0}}, {"nw", {-1, 1}}, {"ne", {1, 1}} };
vector<Pos> all_moves = { {2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1}, {1, 1} };
set<Pos> build(const vector<string>& input)
{
	set<Pos> flip;
	for (auto& s : input)
	{
		Pos pos{ 0,0 };
		for (int i = 0; i < s.size(); i++)
		{
			if ((s[i] == 's' || s[i] == 'n') && i < s.size() - 1 && (s[i + 1] == 'e' || s[i + 1] == 'w'))
			{
				pos = pos + moves[s.substr(i, 2)];
				++i;
			}
			else
				pos = pos + moves[s.substr(i, 1)];
		}
		if (flip.end() != flip.find(pos))
			flip.erase(pos);
		else
			flip.insert(pos);
	}
	return flip;
}
int task1(const vector<string>& input)
{
	return (int)build(input).size();
}
int count_blacks(const set<Pos>& flip, const Pos& f)
{
	return (int)count_if(all_moves.begin(), all_moves.end(), [&](auto& m) {return flip.end() != flip.find(f + m); });
}
int task2(const vector<string>& input)
{
	auto flip = build(input);
	for (int i = 0; i < 100; ++i)
	{
		set<Pos> tmp;
		set<Pos> whites;
		for (auto& f : flip)
		{
			int blacks = count_blacks(flip, f);
			if (blacks == 1 || blacks == 2)
				tmp.insert(f);
			for (auto& m : all_moves)
			{
				Pos p = f + m;
				if (flip.end() == flip.find(p) && count_blacks(flip, p) == 2)
					whites.insert(p);
			}
		}
		flip.clear();
		set_union(tmp.begin(), tmp.end(), whites.begin(), whites.end(), inserter(flip, flip.end()));
	}
	return (int)flip.size();
}



void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day24.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day24 Answer1: " << task1(input) << endl;
	cout << "Day24 Answer2: " << task2(input) << endl;
}

void Test()
{
	stringstream test(R"(sesenwnenenewseeswwswswwnenewsewsw
neeenesenwnwwswnenewnwwsewnenwseswesw
seswneswswsenwwnwse
nwnwneseeswswnenewneswwnewseswneseene
swweswneswnenwsewnwneneseenw
eesenwseswswnenwswnwnwsewwnwsene
sewnenenenesenwsewnenwwwse
wenwwweseeeweswwwnwwe
wsweesenenewnwwnwsenewsenwwsesesenwne
neeswseenwwswnwswswnw
nenwswwsewswnenenewsenwsenwnesesenew
enewnwewneswsewnwswenweswnenwsenwsw
sweneswneswneneenwnewenewwneswswnese
swwesenesewenwneswnwwneseswwne
enesenwswwswneneswsenwnewswseenwsese
wnwnesenesenenwwnenwsewesewsesesew
nenewswnwewswnenesenwnesewesw
eneswnwswnwsenenwnwnwwseeswneewsenese
neswnwewnwnwseenwseesewsenwsweewe
wseweeenwnesenwwwswnew)");
	istream_iterator<string> start(test), end;
	vector<string> input(start, end);
	cout << "Test 1: " << task1(input) << endl;
	cout << "Test 2: " << task2(input) << endl;
}