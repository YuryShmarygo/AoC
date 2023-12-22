#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2) { return point{ p1.x + p2.x, p1.y + p2.y }; }
vector<point> all_dir{ {-1,0},{0,-1},{1,0},{0,1} };

point find_s(const vector<string>& input)
{
	for (int y = 0; y < input.size(); ++y)
		for (int x = 0; x < input[y].size(); ++x)
			if (input[y][x] == 'S')
				return { x, y };
	cout << "start not found" << endl;
	return { 0, 0 };
}
long long solve1(const vector<string>& input, int steps)
{
	point sp = find_s(input);
	set<point> cur = { sp };
	long long res = 0;
	for (; steps > 0; --steps)
	{
		set<point> next;
		for (auto& p : cur)
		{
			for (auto& d : all_dir)
			{
				point pn = p + d;
				if (pn.x >= 0 && pn.x < input[0].size() && pn.y >= 0 && pn.y < input.size() && input[pn.y][pn.x] != '#')
					next.insert(pn);
			}
		}
		swap(cur, next);
	}
	return cur.size();
}
long long solve2(const vector<string>& input, int steps)
{
	long long res = 0;
	return res;
}
int main()
{
	test();
	ifstream is("Day21.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day21 Answer1: " << solve1(input, 64) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day21 Answer2: " << solve2(input, 26501365) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(...........
.....###.#.
.###.##..#.
..#.#...#..
....#.#....
.##..S####.
.##..#...#.
.......##..
.##.#.####.
.##..##.##.
...........)";

	auto is = stringstream(test1);
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Test1: " << solve1(input, 6) << endl;
	cout << "Test2: " << solve2(input, 6) << endl;
	cout << "Test2: " << solve2(input, 10) << endl;
	cout << "Test2: " << solve2(input, 50) << endl;
	cout << "Test2: " << solve2(input, 100) << endl;
	cout << "Test2: " << solve2(input, 500) << endl;
	cout << "Test2: " << solve2(input, 1000) << endl;
	cout << "Test2: " << solve2(input, 5000) << endl;
}
