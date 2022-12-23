#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
tuple<point, point> dimentions(const set<point>& aria)
{
	int minx = aria.begin()->x, miny = aria.begin()->y, maxx = aria.begin()->x, maxy = aria.begin()->y;
	for (auto& p : aria)
	{
		minx = min(p.x, minx);
		miny = min(p.y, miny);
		maxx = max(p.x, maxx);
		maxy = max(p.y, maxy);
	}
	return { {minx, miny}, {maxx, maxy} };
}
ostream& operator<<(ostream& os, const set<point>& a)
{
	auto [l, r] = dimentions(a);
	vector<string> res(r.y - l.y + 1, string(r.x - l.x + 1, '.'));
	for (auto& e : a)
		res[e.y - l.y][e.x - l.x] = '#';
	r::copy(res, ostream_iterator<string>(os, "\n"));
	return os;
}
vector<point> directions{ {0,-1},{0,1},{-1,0},{1,0} };
map<int, vector<point>> checks{ {0,{{-1,-1},{0,-1},{1,-1}}}, {1,{{-1,1},{0,1},{1,1}}}, {2,{{-1,-1},{-1,0},{-1,1}}}, {3,{{1,-1},{1,0},{1,1}}} };
struct elf
{
	point pos;
	point next;
};
struct game
{
	vector<elf> elfs;
	set<point> aria;
	int dir;
	game(const vector<string>& input)
	{
		for (int y = 0; y < static_cast<int>(input.size()); ++y)
		{
			for (int x = 0; x < static_cast<int>(input[y].size()); ++x)
				if (input[y][x] == '#') elfs.push_back({ {x, y},{x, y} });
		}
		for (auto& e : elfs) aria.insert(e.pos);
		dir = 0;
	}
	int get_size()
	{
		auto [l, r] = dimentions(aria);
		return (r.x - l.x + 1) * (r.y - l.y + 1);
	}
	bool step()
	{
		bool moved = false;
		map<point, bool> moves;
		for (auto& e : elfs)
		{
			if (r::all_of(checks, [&](auto& c) {return r::all_of(c.second, [&](auto& d) {return !aria.contains({ e.pos.x + d.x, e.pos.y + d.y }); }); }))
			{
				e.next = e.pos;
				moves[e.next] = true;
			}
			else
			{
				moved = true;
				for (int j = 0; j < 4; j++)
				{
					if (r::all_of(checks[(dir + j) % 4], [&](auto& d) { return !aria.contains({ e.pos.x + d.x, e.pos.y + d.y }); }))
					{
						e.next = { e.pos.x + directions[(dir + j) % 4].x, e.pos.y + directions[(dir + j) % 4].y };
						moves[e.next] = !moves.contains(e.next);
						break;
					}
				}
			}
		}
		aria.clear();
		for (auto& e : elfs)
		{
			if (moves[e.next])
				e.pos = e.next;
			else
				e.next = e.pos;
			aria.insert(e.pos);
		}
		dir = (dir + 1) % 4;
		return moved;
	}
};
int solve1(const vector<string>& input)
{
	game g(input);
	for (int i = 0; i < 10; ++i)
		g.step();
	return g.get_size() - static_cast<int>(g.aria.size());
}
int solve2(const vector<string>& input)
{
	game g(input);
	int n = 0;
	while (g.step()) ++n;
	return n+1;
}
int main()
{
	test(); //return 0;
	ifstream is("Day23.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << "Day23 answer1: " << solve1(input) << endl; //4207
	cout << "Day23 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(....#..
..###.#
#...#.#
.#...##
#.###..
##.#.##
.#..#..)");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
