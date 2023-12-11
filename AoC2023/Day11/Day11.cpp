#include "../common.h"

vector<string> expand(const vector<string>& o)
{
	vector<string> res(o);
	for (auto it = res.begin(); it != res.end(); ++it)
	{
		if (r::all_of(*it, [](auto& c) {return c == '.'; }))
			it = res.insert(next(it), string(it->size(), '.'));
	}
	for (int x = 0; x < res[0].size(); ++x)
	{
		if (r::all_of(res, [x](auto& s) {return s[x] == '.'; }))
		{
			for (auto& s : res)
				s.insert(x, 1, '.');
			++x;
		}
	}
	return res;
}
struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
int distance(const point& p1, const point& p2) { return abs(p1.x - p2.x) + abs(p1.y - p2.y); }
set<point> build_g(const vector<string>& input)
{
	set<point> res;
	for (int y = 0; y < input.size(); ++y)
		for (int x = 0; x < input[y].size(); ++x)
			if (input[y][x] == '#')
				res.insert({ x,y });
	return res;
}
long long solve1(vector<string>& input)
{
	auto g_map = expand(input);
	auto g = build_g(g_map);
	long long res = 0;
	for (auto it = g.begin(); it != g.end(); ++it)
		for (auto it2 = next(it); it2 != g.end(); ++it2)
			res += distance(*it, *it2);
	return res;
}
struct expantion
{
	set<int> rows;
	set<int> cols;
};
long long solve2(vector<string>& input, int factor)
{
	expantion exp;
	for (int y = 0; y < input.size(); ++y)
	{
		if (r::all_of(input[y], [](auto& c) {return c == '.'; }))
			exp.rows.insert(y);
	}
	for (int x = 0; x < input[0].size(); ++x)
	{
		if (r::all_of(input, [x](auto& s) {return s[x] == '.'; }))
			exp.cols.insert(x);
	}
	auto g = build_g(input);
	long long res = 0;
	for (auto it = g.begin(); it != g.end(); ++it)
	{
		for (auto it2 = next(it); it2 != g.end(); ++it2)
		{
			for (int y = min(it->y, it2->y); y < max(it->y, it2->y); ++y)
				if (exp.rows.find(y) != exp.rows.end())
					res += factor;
				else
					++res;
			for (int x = min(it->x, it2->x); x < max(it->x, it2->x); ++x)
				if (exp.cols.find(x) != exp.cols.end())
					res += factor;
				else
					++res;
		}
	}
	return res;
}
int main()
{
	test();
	ifstream is("Day11.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day11 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day11 Answer2: " << solve2(input, 1000000) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....)";

	auto is = stringstream(test1);
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input, 10) << endl;
	cout << "Test2: " << solve2(input, 100) << endl;
}
