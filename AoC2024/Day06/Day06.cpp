#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is) 
{ 
	return read_strings(is);
}

point find_guard(const input_t& input)
{
	for (int y = 0; y < input.size(); ++y)
		for (int x = 0; x < input[y].size(); ++x)
			if (input[y][x] == '^')
				return { x, y };
	return { 0, 0 };
}
bool move(input_t& m)
{
	point g = find_guard(m);
	point d = { 0, -1 };
	m[g.y][g.x] = '.';
	map<point, point> turns = { { {0,-1}, {1,0}},{{1,0}, {0,1}},{{0,1},{-1,0}},{{-1,0},{0,-1}} };
	map<point, char> move_code = { { {0,-1}, 1 },{{1,0},2},{{0,1},4},{{-1,0},8} };
	while (true)
	{
		do
		{
			m[g.y][g.x] = m[g.y][g.x] == '.' ? move_code[d] : m[g.y][g.x] | move_code[d];
			g.y += d.y;
			g.x += d.x;
		} while (g.y >= 0 && g.y < m.size() && g.x >= 0 && g.x < m[g.y].size() && m[g.y][g.x] != '#' && (m[g.y][g.x] == '.' || (m[g.y][g.x] & move_code[d]) == 0));
		if (g.y < 0 || g.y >= m.size() || g.x < 0 || g.x >= m[g.y].size())
			return false;
		if (m[g.y][g.x] != '#' && ((m[g.y][g.x] & move_code[d]) > 0))
			return true;
		g.y -= d.y;
		g.x -= d.x;
		d = turns[d];
		if (((m[g.y][g.x] & move_code[d]) > 0))
			return true;
	}
	return true;
}
long long solve1(const input_t& input)
{
	input_t m = input;
	move(m);
	long long sum = 0;
	for (int y = 0; y < m.size(); ++y)
		for (int x = 0; x < m[y].size(); ++x)
			sum += (m[y][x] == '#' || m[y][x] == '.') ? 0 : 1;
	return sum;
}
long long solve2(const input_t& input)
{
	point g = find_guard(input);
	input_t m = input;
	move(m);
	long long sum = 0;
	for (int y = 0; y < m.size(); ++y)
	{
		for (int x = 0; x < m[y].size(); ++x)
		{
			if (m[y][x] != '#' && m[y][x] != '.' && (y != g.y || x != g.x))
			{
				input_t test = input;
				test[y][x] = '#';
				sum += move(test) ? 1 : 0;
			}
		}
	}
	return sum;
}

int main()
{
	test();
	ifstream is("Day06.txt");
	input_t input = read_input(is);

	cout << "Day6 Answer1: " << solve1(input) << endl;
	cout << "Day6 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
