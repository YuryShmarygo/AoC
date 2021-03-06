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
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()

bool trace_path = false;
bool trace_turn = false;
bool trace_round = false;

enum UnitType {Wall, Cavern, Goblin, Elf};
UnitType ToType(char c){ return c == '#' ? Wall : c == '.' ? Cavern : c == 'G' ? Goblin : Elf;}
char ToChar(UnitType t) { return t == Wall ? '#' : t == Cavern ? '.' : t == Goblin ? 'G' : 'E'; }
bool IsGE(UnitType t) { return t == Goblin || t == Elf; }
struct Unit
{
	UnitType type;
	int x;
	int y;
	int hp;
	int ap;
};
ostream& operator<<(ostream& o, const Unit& u)
{ o << ToChar(u.type) << "[" << u.hp <<"]" << "(" << u.x << "," << u.y << ")"; return o; }

typedef vector<vector<Unit>> Board;
typedef list<Unit> Units;
ostream& operator<< (ostream& s, const Board& board)
{
	for (auto i = 0; i < board.size(); i++)
	{
		//s.width(2); s << i << " ";
		for (auto u : board[i]) s << ToChar(u.type);
		s << endl;
	}
	return s;
}

struct Point
{
	int x;
	int y;
};
void Attack(const Unit& u, Units& units, Board& board)
{
	vector<Units::iterator> res;
	for (auto it = units.begin(); it != units.end(); ++it)
	{
		if (it->type != u.type && abs(it->x - u.x) + abs(it->y - u.y) == 1)
			res.push_back(it);
	}
	auto target = min_element(res.begin(), res.end(), [](auto r1, auto r2) {return r1->hp < r2->hp || r1->hp == r2->hp && r1->y < r2->y || r1->hp == r2->hp && r1->y == r2->y && r1->x < r2->x; });
	if (target != res.end())
	{
		(*target)->hp -= u.ap;
		if ((*target)->hp <= 0)
		{
			board[(*target)->y][(*target)->x].type = Cavern;
			units.erase((*target));
		}
	}
}
typedef vector<Point> Path;
typedef vector<Path> Paths;
void PrintPaths(const Paths& paths, const Board& board)
{
	vector<string> str(board.size(), string(board[0].size(), '#'));
	for (int y = 0; y < board.size(); ++y)
		for (int x = 0; x < board[y].size(); ++x)
			str[y][x] = ToChar(board[y][x].type);
	for (auto& p : paths)
		for (int i = 0; i < p.size(); ++i)
			str[p[i].y][p[i].x] = S(i % 10)[0];
	for (int i = 0; i < str.size(); ++i)
		cout << setw(2) << i << ":" << str[i] << endl;
}
bool IsOpposite(UnitType t1, UnitType t2)
{
	return t1 == Goblin && t2 == Elf || t1 == Elf && t2 == Goblin;
}
bool IsAdjacent(const Point& p, UnitType type, const Board& board)
{
	return IsOpposite(board[p.y - 1][p.x].type, type) || IsOpposite(board[p.y][p.x - 1].type, type)
		|| IsOpposite(board[p.y][p.x + 1].type, type) || IsOpposite(board[p.y + 1][p.x].type, type);
}
vector<Point> GetMoves(const Point& p, const vector<vector<int>>& visited)
{
	static vector<Point> cross = { { 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 } };
	vector<Point> moves;
	for (auto d : cross)
		if (visited[p.y + d.y][p.x + d.x] == 0)
			moves.push_back(Point{ p.x + d.x, p.y + d.y });
	return moves;
}
void Move(Unit& u, Board& board)
{
	if (IsAdjacent(Point{ u.x, u.y }, u.type, board))
		return;
	vector<vector<int>> visited((int)board.size(), vector<int>((int)board[0].size(), 0));
	Paths paths_(1, Path(1, Point{ u.x, u.y }));
	paths_.reserve(board.size() * board[0].size());
	Paths nextPaths_; nextPaths_.reserve(paths_.size());
	Paths *paths = &paths_;
	Paths *nextPaths = &nextPaths_;
	Paths res; res.reserve(paths_.size());
	while (res.empty() && !paths->empty())
	{
		for (auto it = paths->begin(); it != paths->end(); ++it)
		{
			auto& cur = it->back();
			auto moves = GetMoves(cur, visited);
			for (auto& m : moves)
			{
				auto& mu = board[m.y][m.x];
				if (mu.type == Cavern)
				{
					auto newPath = *it;
					newPath.push_back(m);
					nextPaths->push_back(newPath);
				}
				else if (IsOpposite(mu.type, u.type))
				{
					auto newPath = *it;
					newPath.erase(newPath.begin());
					res.push_back(newPath);
				}
				visited[m.y][m.x] = 1;
			}
		}
		if (trace_path) PrintPaths(*nextPaths, board);
		paths->clear();
		swap(paths, nextPaths);
	};
	if (!res.empty())
	{
		if (trace_path) { cout << "target paths:" << endl; PrintPaths(res, board); }
		auto it = min_element(res.begin(), res.end(), [&](auto& p1, auto& p2)
		{
			auto& b1 = p1.back();
			auto& b2 = p2.back();
			auto& f1 = p1.front();
			auto& f2 = p2.front();
			return b1.y < b2.y || b1.y == b2.y && b1.x < b2.x
				|| b1.y == b2.y && b1.x == b2.x && f1.y < f2.y
				|| b1.y == b2.y && b1.x == b2.x && f1.y == f2.y && f1.x < f2.x;
		});
		auto& f = it->front();
		board[u.y][u.x].type = Cavern;
		board[f.y][f.x].type = u.type;
		u.y = f.y;
		u.x = f.x;
	}
}

void Turn(Units::iterator itu, Units& units, Board& board)
{
	Move(*itu, board);
	Attack(*itu, units, board);
}
bool Round(Board& board, Units& units, bool& full)
{
	units.sort([](auto u1, auto u2) { return u1.y < u2.y || u1.y == u2.y && u1.x < u2.x; });
	for (auto it = units.begin(); it != units.end(); ++it)
	{
		Turn(it, units, board);
		if (units.end() == adjacent_find(units.begin(), units.end(), [](auto u1, auto u2) {return u1.type != u2.type; }))
		{
			full = next(it) == units.end();
			return false;
		}
	}
	return true;
}

int CountHP(const Units& units)
{
	int hp = 0;
	for (auto unit : units) hp += unit.hp;
	return hp;
}

Units CreateUnits(const Board& board)
{
	Units units;
	for (auto line : board)	for (auto unit : line)
		if (IsGE(unit.type))
			units.push_back(unit);
	return units;
}
int Combat2(Board board, Units& units)
{
	int rounds = 0;
	bool full = false;
	if (trace_turn || trace_round) cout << board << endl;
	while (Round(board, units, full))
	{ 
		if (trace_turn || trace_round) cout << board << endl;
		rounds++;
		//trace_path = rounds == 13;
	}
	rounds += full ? 1 : 0;
	return rounds;
}
int Combat(Board board)
{
	Units units = CreateUnits(board);
	int rounds = Combat2(board, units);
	//cout << board << endl;
	//cout << "Rounds: " << rounds << " HP: " << CountHP(units) << endl;
	return rounds * CountHP(units);
}

int CountElfs(const Units& units) {return (int)count_if(units.begin(), units.end(), [](auto u) {return u.type == Elf; });}
int FindHP(Board input)
{
	Units units = CreateUnits(input);
	int i_elfs = CountElfs(units);
	int ap = 3, apmin = 1, apmax = 0;
	while(true)
	{
		Board board = input;
		Units units = CreateUnits(board);
		for (auto& u : units)
			if (u.type == Elf)
				u.ap = ap;
		int rounds = Combat2(board, units);
		int elfs = CountElfs(units);
		//cout << "AP: " << ap << ", rounds: " << rounds << ", efls killed: " << i_elfs - elfs << endl;
		if (elfs == i_elfs && ap == apmin+1)
			return rounds * CountHP(units);
		if (elfs == i_elfs)
			apmax = ap;
		else
			apmin = ap;

		if (elfs == 0)
			ap += ((CountHP(units) + i_elfs - 1) / i_elfs + rounds -1) / rounds;
		else
			ap = apmax == 0 ? ap + 200 / rounds : (apmax + apmin + 1) / 2;
	};
}

Board CreateBoard(const vector<string>& input)
{
	Board board(input.size(), vector<Unit>(input.front().size(), Unit()));
	for (int y = 0; y < input.size(); y++)
		for (int x = 0; x < input[y].size(); x++)
			board[y][x] = Unit{ ToType(input[y][x]), x, y, 200, 3 };
	return board;
}

void RunTests();

int main()
{
	//RunTests(); return 0;
	ifstream is("input.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	//cout << "Read " << input.size() << " lines" << endl;

	Board board = CreateBoard(input);
	if (trace_turn) cout << board << endl;

	cout << "Day15 Answer1: " << Combat(board) << endl;
	cout << "Day15 Answer2: " << FindHP(board) << endl;
}

vector<vector<string>> tests = 
{
	{"#######",
	 "#.G...#",
	 "#...EG#",
	 "#.#.#G#",
	 "#..G#E#",
	 "#.....#",
	 "#######" },
	{"#######",
	 "#G..#E#",
	 "#E#E.E#",
	 "#G.##.#",
	 "#...#E#",
	 "#...E.#",
	 "#######"},
{"#######",
"#E..EG#",
"#.#G.E#",
"#E.##E#",
"#G..#.#",
"#..E#.#",
"#######"},
{"#######",
"#E.G#.#",
"#.#G..#",
"#G.#.G#",
"#G..#.#",
"#...E.#",
"#######"},
{"#######",
"#.E...#",
"#.#..G#",
"#.###.#",
"#E#G#G#",
"#...#G#",
"#######"},
{"#########",
"#G......#",
"#.E.#...#",
"#..##..G#",
"#...##..#",
"#...#...#",
"#.G...G.#",
"#.....G.#",
"#########"},
{
"##########",
"###.G#.E##",
"###....###",
"#E..######",
"##########"},
};

void RunTests()
{
	for (int i = 0; i < tests.size(); i++)
	{
		cout << "Test" << i << endl;
		cout << "Test" << i << ": " << Combat(CreateBoard(tests[i])) << endl;
		cout << "Test" << i << " winnig HP " << FindHP(CreateBoard(tests[i])) << endl;
	}
}