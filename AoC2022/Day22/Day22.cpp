#include "../common.h"

struct board
{
	vector<string> map;
	string moves;
};
istream& operator>>(istream& is, vector<string>& map)
{
	map.clear();
	string line;
	while (getline(is, line) && !line.empty())
		map.push_back(line);
	return is;
}
istream& operator>>(istream& is, board& b)
{
	is >> b.map >> b.moves;
	auto max_l = r::max_element(b.map, [](auto& l1, auto& l2) {return l1.size() < l2.size(); })->size();
	for (auto& l : b.map)
		l.resize(max_l, ' ');
	return is;
}
ostream& operator<<(ostream& os, const vector<string>& m) { r::copy(m, ostream_iterator<string>(os, "\n")); return os; }
struct move_t
{
	move_t():c(' '), n(0){}
	char c;
	int n;
};
vector<move_t> parse(string moves)
{
	vector<move_t> res;
	smatch m;
	while(regex_match(moves, m, regex(R"(^(\d+|[RL]).*)")))
	{
		move_t mv;
		if (m[1].str()[0] >= '0' && m[1].str()[0] <= '9')
		{
			mv.c = ' ';
			mv.n = stoi(m[1]);
		}
		else
		{
			mv.c = m[1].str()[0];
			mv.n = 0;
		}
		res.push_back(mv);
		moves = moves.substr(m[1].str().size());
	}
	return res;
}
map<char, char> r_turns{ {'R','D'}, {'D','L'}, {'L','U'}, {'U','R'}};
map<char, char> l_turns{ {'R','U'}, {'U','L'}, {'L','D'}, {'D','R'} };
map<char, int> facing{ {'R',0}, {'D',1}, {'L',2}, {'U',3} };
long long solve1(const board& b)
{
	int x = static_cast<int>(r::find_if(b.map[0], [](auto c) {return c == '.'; }) - b.map[0].begin());
	int y = 0;
	char dir = 'R';
	auto moves = parse(b.moves);
	vector<string> move_map(b.map);
	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		auto& m = *it;
		if (m.c == ' ')
		{
			move_map[y][x] = dir;
			if (dir == 'R')
			{
				for (int i = 0; i < m.n; ++i)
				{
					if (x == (int)b.map[y].size() -1 || b.map[y][x + 1] == ' ')
					{
						int n = 0;
						for (; n < static_cast<int>(b.map[y].size()) && b.map[y][n] == ' '; ++n) {}
						if (b.map[y][n] == '.')
							x = n;
						else
							break;
					}
					else if (b.map[y][x + 1] == '.')
						++x;
					else if (b.map[y][x + 1] == '#')
						break;
				}
			}
			else if (dir == 'L')
			{
				for (int i = 0; i < m.n; ++i)
				{
					if (x == 0 || b.map[y][x-1] == ' ')
					{
						int n = static_cast<int>(b.map[y].size()) - 1;
						for (; n >0  && b.map[y][n] == ' '; --n) {}
						if (b.map[y][n] == '.')
							x = n;
						else
							break;
					}
					else if (b.map[y][x - 1] == '.')
						--x;
					else if (b.map[y][x - 1] == '#')
						break;
				}
			}
			else if (dir == 'D')
			{
				for (int i = 0; i < m.n; ++i)
				{
					if (y == static_cast<int>(b.map.size()) - 1 || b.map[y+1][x] == ' ')
					{
						int n = 0;
						for (; n < static_cast<int>(b.map.size()) && b.map[n][x] == ' '; ++n) {}
						if (b.map[n][x] == '.')
							y = n;
						else
							break;
					}
					else if (b.map[y + 1][x] == '.')
						++y;
					else if (b.map[y + 1][x] == '#')
						break;
				}
			}
			else if (dir == 'U')
			{
				for (int i = 0; i < m.n; ++i)
				{
					if (y == 0 || b.map[y-1][x] == ' ')
					{
						int n = static_cast<int>(b.map.size()) - 1;
						for (; n > 0 && b.map[n][x] == ' '; --n) {}
						if (b.map[n][x] == '.')
							y = n;
						else
							break;
					}
					else if (b.map[y - 1][x] == '.')
						--y;
					else if (b.map[y - 1][x] == '#')
						break;
				}
			}
			move_map[y][x] = dir;
//			cout << "turn: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl; cout << move_map << endl;
		}
		else if (m.c == 'R')
		{
			dir = r_turns[dir];
		}
		else if (m.c == 'L')
		{
			dir = l_turns[dir];
		}
//		cout << "x :" << x << " y: " << y << " dir: " << dir << endl;
	}
	return 1000 * (y+1) + 4 * (x+1) + facing[dir];
}
int cube_n(int x, int y) { 	return 3 * (y / 50) + x / 50; }

void fold(int& x, int& y, char& d)
{
	if (cube_n(x,y) == 1)
	{
		if (d == 'R') { ++x; }
		else if (d == 'D') { ++y; }
		else if (d == 'L') { x = 0; y = 149 - y; d = 'R'; }
		else if (d == 'U') { y = 100 + x; x = 0; d = 'R'; }
	}
	else if (cube_n(x,y) == 2)
	{
		if (d == 'L') { --x; }
		else if (d == 'R') { x = 99;  y = 149 - y; d = 'L'; }
		else if (d == 'U') { x = x - 100; y = 199; d = 'U'; }
		else if (d == 'D') { y = x - 50; x = 99; d = 'L'; }
	}
	else if (cube_n(x,y) == 4)
	{
		if (d == 'U') { --y; }
		else if (d == 'D') { ++y; }
		else if (d == 'R') { x = y + 50; y = 49; d = 'U'; }
		else if (d == 'L') { x = y - 50; y = 100; d = 'D'; }
	}
	else if (cube_n(x,y) == 7)
	{
		if (d == 'U') { --y; }
		else if (d == 'L') { --x; }
		else if (d == 'R') { y = 149 - y; x = 149; d = 'L'; }
		else if (d == 'D') { y = x + 100; x = 49; d = 'L'; }
	}
	else if (cube_n(x,y) == 6)
	{
		if (d == 'R') { ++x; }
		else if (d == 'D') { ++y; }
		else if (d == 'U') { y = x + 50; x = 50; d = 'R'; }
		else if (d == 'L') { y = 149 - y; x = 50; d = 'R'; }
	}
	else if (cube_n(x,y) == 9)
	{
		if (d == 'U') { --y; }
		else if (d == 'R') { x = y - 100; y = 149; d = 'U'; }
		else if (d == 'L') { x = y - 100; y = 0; d = 'D'; }
		else if (d == 'D') { x = x + 100; y = 0; d = 'D'; }
	}
}
bool fold(int& x, int& y, char& d, const vector<string>& map)
{
	int tx = x;
	int ty = y;
	char td = d;
	fold(tx, ty, td);
	if (map[ty][tx] == '#') return false;
	x = tx;
	y = ty;
	d = td;
	return true;

}
long long solve2(const board& b)
{
	int x = static_cast<int>(r::find_if(b.map[0], [](auto c) {return c == '.'; }) - b.map[0].begin());
	int y = 0;
	char dir = 'R';
	auto moves = parse(b.moves);
	
	vector<string> move_map(b.map);
	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		auto& m = *it;
		if (m.c == ' ')
		{
			move_map[y][x] = dir;
			for (int i = 0; i < m.n; ++i)
			{
				if (dir == 'R')
				{
					if (x == (int)b.map[y].size() - 1 || cube_n(x, y) != cube_n(x + 1, y))
					{
						cout << "turn: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl; cout << move_map << endl;
						if (!fold(x, y, dir, b.map)) break;
						cout << "after fold: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl;
					}
					else if (b.map[y][x + 1] == '.')
						++x;
					else if (b.map[y][x + 1] == '#')
						break;
				}
				else if (dir == 'L')
				{
					if (x == 0 || cube_n(x, y) != cube_n(x - 1, y))
					{
						cout << "turn: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl; cout << move_map << endl;
						if (!fold(x, y, dir, b.map)) break;
						cout << "after fold: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl;
					}
					else if (b.map[y][x - 1] == '.')
						--x;
					else if (b.map[y][x - 1] == '#')
						break;
				}
				else if (dir == 'D')
				{
					if (y == static_cast<int>(b.map.size()) - 1 || cube_n(x, y) != cube_n(x, y+1))
					{
						cout << "turn: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl; cout << move_map << endl;
						if (!fold(x, y, dir, b.map)) break;
						cout << "after fold: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl;
					}
					else if (b.map[y + 1][x] == '.')
						++y;
					else if (b.map[y + 1][x] == '#')
						break;
				}
				else if (dir == 'U')
				{
					if (y == 0  || cube_n(x, y) != cube_n(x, y - 1))
					{
						cout << "turn: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl; cout << move_map << endl;
						if (!fold(x, y, dir, b.map)) break;
						cout << "after fold: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl;
					}
					else if (b.map[y - 1][x] == '.')
						--y;
					else if (b.map[y - 1][x] == '#')
						break;
				}
				move_map[y][x] = dir;
			}
//				cout << "turn: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl; cout << move_map << endl;
		}
		else if (m.c == 'R')
		{
			dir = r_turns[dir];
		}
		else if (m.c == 'L')
		{
			dir = l_turns[dir];
		}
//		cout << "x :" << x << " y: " << y << " dir: " << dir << endl;
//		cout << "turn: " << (it != moves.begin() ? prev(it)->c : ' ') << " dir: " << dir << " moves: " << m.n << " x :" << x << " y: " << y << endl; cout << move_map << endl;
	}
	return 1000 * (y + 1) + 4 * (x + 1) + facing[dir];
}
int main()
{
	test(); //return 0;
	ifstream is("Day22.txt");
	board b; is >> b;
	cout << "Day22 answer1: " << solve1(b) << endl;
	cout << "Day22 answer2: " << solve2(b) << endl;
}
void test()
{
	stringstream is(R"(        ...#
        .#..
        #...
        ....
...#.......#
........#...
..#....#....
..........#.
        ...#....
        .....#..
        .#......
        ......#.

10R5L5R10L4R5L5)");
	board b; is >> b;
	cout << b.map.size() << " " << b.moves.size() << endl;
	cout << "test1: " << solve1(b) << endl;
	//cout << "test2: " << solve2(b) << endl;
}
