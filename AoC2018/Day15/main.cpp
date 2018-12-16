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
		s.width(2); s << i << " ";
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
Units::iterator FindAdjacent(const Unit& u, Units& units, Board& board)
{
	vector<Units::iterator> res;
	for (auto it = units.begin(); it != units.end(); ++it)
	{
		if (it->type != u.type && abs(it->x - u.x) + abs(it->y - u.y) == 1)
			res.push_back(it);
	}
	stable_sort(res.begin(), res.end(), [](auto r1, auto r2) {return r1->hp < r2->hp || r1->hp == r2->hp && r1->y < r2->y || r1->hp == r2->hp && r1->y == r2->y && r1->x < r2->x; });
	return res.empty() ? units.end() : *(res.begin());
}
struct PointPath
{
	int x;
	int y;
	int distance;
};
typedef vector<PointPath> Path;
typedef vector<vector<int>> PathBoard;
ostream& operator<<(ostream& o, const PathBoard& board)
{
	for (int i = 0; i < board.size(); i++)
	{
		o.width(2); o << i << " ";
		auto line = board[i];
		for (auto d : line)
			if (d == -1) cout << " "; else if (d == -2) cout << "#"; else cout << d;
		cout << endl;
	}
	return o;
}
Path GetAdjacent(int ux, int uy, const PathBoard& board)
{
	Path res;
	vector<Point> moves = { {ux, uy - 1}, {ux - 1,uy}, {ux + 1,uy}, {ux,uy+1} };
	for (auto m : moves)
		if (board[m.y][m.x] >= 0)
			res.push_back(PointPath{ m.x,m.y,board[m.y][m.x] });

	stable_sort(res.begin(), res.end(), [](auto p1, auto p2) {return p1.distance < p2.distance; });
	return res;
}
vector<Point> GetCross(int x, int y, const PathBoard& paths)
{
	static vector<Point> cross = {{ 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 }};
	vector<Point> moves;
	for (auto d : cross)
		if (paths[y + d.y][x + d.x] == -1)
			moves.push_back(Point{ x + d.x, y + d.y });
	return moves;
}
// Fills not visited adjacent positions. returns true if reaches opponent
bool FillCross(const Unit& u, UnitType ut, const Board& board, PathBoard& paths, int distance, vector<Point>& nextMoves)
{
	auto moves = GetCross(u.x, u.y, paths);
	for (auto m : moves)
	{
		if (board[m.y][m.x].type == Wall || board[m.y][m.x].type == ut)
			paths[m.y][m.x] = -2;
		else if (board[m.y][m.x].type == Cavern)
		{
			paths[m.y][m.x] = distance+1;
			nextMoves.push_back(m);
		}
		else // Opponent
			return true;
	}
	return false;
}
// Build path to the nearest opponent
Path BuildPaths(const Unit& u, const Board& board, PathBoard& paths)
{
	vector<Point> nextMoves(1, Point{ u.x, u.y });
	int distance = 0;
	UnitType ut = u.type;
	paths[u.y][u.x] = distance;
	vector<Point> moves;
	bool found = false;
	while (!nextMoves.empty() && !found)
	{
		moves = nextMoves;
		nextMoves.clear();
		for (auto m : moves)
			if (FillCross(board[m.y][m.x], ut, board, paths, distance, nextMoves))
				return Path(1, PointPath{ m.x,m.y,distance });
		distance++;
//		cout << paths << endl;
//		cin.get();
	}
	return Path();
}
// Finds closest path
PointPath NextPath(const PointPath& p, const PathBoard& paths)
{
	return GetAdjacent(p.x, p.y, paths).front();
}
// Returns position where to move or empty if should stay
Path NextMove(const Unit& u, const Board& board)
{
	vector<vector<int>> paths((int)board.size(), vector<int>((int)board[0].size(), -1));
	paths[u.y][u.x] = 0;

	Path path = BuildPaths(u, board, paths);
	if (path.empty())
		return path; // no path to opponent => stay
	// Go back by fastest route
	PointPath p = path.front();
	while (p.distance != 1)
		p = NextPath(p, paths);
	return Path(1, p);
}
void Attack(Units::iterator it, Units& units, Board& board, int ap)
{
	it->hp -= ap;
	if (it->hp <= 0)
	{
		board[it->y][it->x].type = Cavern;
		units.erase(it);
	}
}
void Turn(Units::iterator itu, Units& units, Board& board)
{
	//cout << "Turn " << *itu;
	auto it = FindAdjacent(*itu, units, board);
	if (it != units.end())
	{
		//cout << " : Attack " << *it << endl;
		Attack(it, units, board, itu->ap);
		return;
	}
	auto p = NextMove(*itu, board);
	if (!p.empty())
	{
		board[itu->y][itu->x].type = Cavern;
		board[p.front().y][p.front().x].type = itu->type;
		itu->y = p.front().y;
		itu->x = p.front().x;
		//cout << " : Move => " << *itu << endl;
	}
	//else { cout << " : no moves" << endl; }
	it = FindAdjacent(*itu, units, board);
	if (it != units.end())
	{
		//cout << " : Attack " << *it << endl;
		Attack(it, units, board, itu->ap);
		return;
	}
	//cout << board << endl;
	//cin.get();
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
	while (Round(board, units, full))
		rounds++;
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
	int ap = 3, apmin = 3, apmax = 0;
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
	cout << "Read " << input.size() << " lines" << endl;

	Board board = CreateBoard(input);
	//cout << board;

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