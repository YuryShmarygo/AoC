#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is) 
{
	return read_strings(is);
}
typedef map<char, vector<point>> fmap_t;
fmap_t build_fmap(const input_t& input)
{
	fmap_t res;
	int h = static_cast<int>(input.size());
	int w = static_cast<int>(input[0].size());
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			if (input[y][x] != '.')
				res[input[y][x]].push_back({ x, y });
		}
	}
	return res;
}
bool check_and_add(point pa, int w, int h, vector<point>& res)
{
	if (pa.x >= 0 && pa.x < w && pa.y >= 0 && pa.y < h)
	{
		res.push_back(pa);
		return true;
	}
	return false;
}
vector<point> get_a(point p1, point p2, int w, int h)
{
	vector<point> res;
	check_and_add(p2 + p2 - p1, w, h, res);
	check_and_add(p1 + p1 - p2, w, h, res);
	return res;
}
long long solve(const input_t& input, function<vector<point>(point, point, int, int)> f)
{
	int h = static_cast<int>(input.size());
	int w = static_cast<int>(input[0].size());
	fmap_t fmap = build_fmap(input);
	vector<string> amap(input.size(), string(input[0].size(), '.'));
	for (auto fit = fmap.begin(); fit != fmap.end(); ++fit)
	{
		for (auto it = fit->second.begin(); it != fit->second.end(); ++it)
		{
			for (auto it2 = next(it); it2 != fit->second.end(); ++it2)
			{
				for (auto p : f(*it, *it2, w, h))
					amap[p.y][p.x] = '#';
			}
		}
	}
	long long sum = 0;
	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x)
			sum += amap[y][x] == '#' ? 1 : 0;
	return sum;
}
long long solve1(const input_t& input)
{
	return solve(input, get_a);
}
vector<point> get_all(point p1, point p2, int w, int h)
{
	vector<point> res;
	point d = p2 - p1;
	point cur = p1 + d;
	while (check_and_add(cur, w, h, res))
		cur = cur + d;
	cur = p2 - d;
	while (check_and_add(cur, w, h, res))
		cur = cur - d;
	return res;
}
long long solve2(const input_t& input)
{
	return solve(input, get_all);
}

int main()
{
	test();
	ifstream is("Day08.txt");
	input_t input = read_input(is);

	cout << "Day8 Answer1: " << solve1(input) << endl;
	cout << "Day8 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;

	auto test2 = R"(T.........
...T......
.T........
..........
..........
..........
..........
..........
..........
..........)";
	is = stringstream(test2);
	input = read_input(is);
	cout << "Test3: " << solve2(input) << endl;
}
