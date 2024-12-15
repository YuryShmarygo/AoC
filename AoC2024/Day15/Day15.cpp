#include "../common.h"

struct input_t
{
	vector<string> map;
	string moves;
};
input_t read_input(istream& is)
{
	input_t input;
	string line;
	while (getline(is, line) && !line.empty())
		input.map.push_back(line);
	while (getline(is, line) && !line.empty())
		input.moves.append(line);
	return input;
}
point find_robot(const vector<string>& map)
{
	for (int y = 0; y < map.size(); ++y)
		for (int x = 0; x < map[y].size(); ++x)
			if (map[y][x] == '@')
				return { x,y };
	return { 0,0 };
}
bool try_move(point p, point d, vector<string>& map)
{
	point n = p + d;
	if (map[n.y][n.x] == '.')
	{
		map[n.y][n.x] = 'O';
		map[p.y][p.x] = '.';
		return true;
	}
	if (map[n.y][n.x] == 'O')
	{
		if (try_move(p + d, d, map))
		{
			map[n.y][n.x] = 'O';
			map[p.y][p.x] = '.';
			return true;
		}
	}
	return false;
}
map<char, point> mp = { {'<',{-1,0}}, {'>', {1,0}}, {'^', {0,-1}}, {'v', {0,1}} };
long long solve1(const input_t& input)
{
	vector<string> map(input.map);
	auto r = find_robot(map);
	for (auto m : input.moves)
	{
		point d = mp[m];
		point n = r + d;
		if (map[n.y][n.x] == '.')
		{
			map[n.y][n.x] = '@';
			map[r.y][r.x] = '.';
			r = n;
		}
		else if (map[n.y][n.x] == 'O')
		{
			if (try_move(n, d, map))
			{
				map[n.y][n.x] = '@';
				map[r.y][r.x] = '.';
				r = n;
			}
		}
	}
	long long num = 0;

	for (int y = 1; y < map.size() - 1; ++y)
		for (int x = 1; x < map[y].size() - 1; ++x)
			if (map[y][x] == 'O')
				num += y * 100 + x;
	return num;
}
map<char, point> bc = { {'[',{1,0}}, {']', {-1,0}} };
void expand(vector<string>& map)
{
	for (int y = 0; y < map.size(); ++y)
		for (int x = 0; x < map[y].size(); x += 2)
		{
			if (map[y][x] == '#')
				map[y].insert(x, 1, '#');
			else if (map[y][x] == '.')
				map[y].insert(x, 1, '.');
			else if (map[y][x] == 'O')
			{
				map[y][x] = '[';
				map[y].insert(x+1, 1, ']');
			}
			else if (map[y][x] == '@')
				map[y].insert(x+1, 1, '.');
		}
}
void moveh(point p, point n, point n2, vector<string>& map)
{
	map[n2.y][n2.x] = map[n.y][n.x];
	map[n.y][n.x] = map[p.y][p.x];
	map[p.y][p.x] = '.';
}
void movev(vector<point> dx, point d, vector<string>& map)
{
	for (auto p : dx)
	{
		auto n = p + d;
		map[n.y][n.x] = map[p.y][p.x];
		map[p.y][p.x] = '.';
	}
}
bool try_movev(vector<point> dx, char m, vector<string>& map)
{
	point d = mp[m];
	if (r::all_of(dx, [&](auto p) { point n = p + d; return map[n.y][n.x] == '.'; }))
	{
		movev(dx, d, map);
		return true;
	}
	set<point> sx;
	for (auto p : dx)
	{
		point n = p + d;
		char mn = map[n.y][n.x];
		if (mn == '#')
			return false;
		if (mn == '[' || mn == ']')
		{
			sx.insert(n);
			sx.insert(n + bc[mn]);
		}
	}
	vector<point> nx(sx.begin(), sx.end());
	if (try_movev(nx, m, map))
	{
		movev(dx, d, map);
		return true;
	}
	return false;
}
bool try_moveh(point p, char m, vector<string>& map)
{
	point d = mp[m];
	point n = p + d;
	point n2 = n + d;
	if (map[n2.y][n2.x] == '.')
	{
		moveh(p, n, n2, map);
		return true;
	}
	else if (map[n2.y][n2.x] == '[' || map[n2.y][n2.x] == ']')
	{
		if (try_moveh(n2, m, map))
		{
			moveh(p, n, n2, map);
			return true;
		}
	}
	return false;
}
bool try_move(point p, char m, vector<string>& map)
{
	if (m == '<' || m == '>')
		return try_moveh(p, m, map);

	point db = bc[map[p.y][p.x]];
	return try_movev({ p, p + db }, m, map);
}
long long solve2(const input_t& input)
{
	vector<string> map(input.map);
	expand(map);
	auto r = find_robot(map);
	for (auto m : input.moves)
	{
		point d = mp[m];
		point n = r + d;
		if (map[n.y][n.x] == '.')
		{
			map[n.y][n.x] = '@';
			map[r.y][r.x] = '.';
			r = n;
		}
		else if (map[n.y][n.x] == '[' || map[n.y][n.x] == ']')
		{
			if (try_move(n, m, map))
			{
				map[n.y][n.x] = '@';
				map[r.y][r.x] = '.';
				r = n;
			}
		}
	}
	long long num = 0;
	for (int y = 1; y < map.size() - 1; ++y)
		for (int x = 1; x < map[y].size() - 1; ++x)
			if (map[y][x] == '[')
				num += y * 100 + x;
	return num;
}

int main()
{
	test();
	ifstream is("Day15.txt");
	input_t input = read_input(is);

	cout << "Day15 Answer1: " << solve1(input) << endl;
	cout << "Day15 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

<^^>>>vv<v>>v<<)";
	auto test2 = R"(##########
#..O..O.O#
#......O.#
#.OO..O.O#
#..O@..O.#
#O#..O...#
#O..O..O.#
#.OO.O.OO#
#....O...#
##########

<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;

	is = stringstream(test2); input = read_input(is);
	cout << "Test2: " << solve1(input) << endl;
	cout << "Test3: " << solve2(input) << endl;
}
