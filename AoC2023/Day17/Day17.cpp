#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point&) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2) { return point{ p1.x + p2.x, p1.y + p2.y }; }
typedef vector<point> point_v;
struct vec_t
{
	point pos;
	point dir;
	bool operator==(const vec_t&) const = default;
	auto operator<=>(const vec_t&) const = default;
	bool operator<(const vec_t&) const = default;
};
typedef vector<vec_t> vec_v;
map<point, vector<point>> all_turns{
	{{ 1, 0}, {{ 1, 0},{ 0, 1},{ 0,-1}}},
	{{-1, 0}, {{-1, 0},{ 0, 1},{ 0,-1}}},
	{{ 0, 1}, {{ 0, 1},{-1, 0},{ 1, 0}}},
	{{ 0,-1}, {{ 0,-1},{-1, 0},{ 1, 0}}},
};
typedef vector<vector<int>> heat_map;
heat_map to_heat_map(const vector<string>& input)
{
	heat_map res(input.size(), vector<int>(input[0].size(), 0));
	for (int y = 0; y < input.size(); ++y)
		for (int x = 0; x < input[y].size(); ++x)
			res[y][x] = input[y][x] - '0';
	return res;
}

struct step_t
{
	vec_t vec;
	int sn;
	int heat;
	bool operator==(const step_t&) const = default;
	auto operator<=>(const step_t&) const = default;
	bool operator<(const step_t&) const = default;
};
typedef vector<step_t> path_t;
bool operator<(const path_t& p1, const path_t& p2) { return !p1.empty() && p2.empty() || !p1.empty() && !p2.empty() && p1.back().heat > p2.back().heat; }
path_t turn(const path_t& path, const heat_map& hmap, int min_s, int max_s)
{
	auto& step = path.back();
	auto& vec = step.vec;
	path_t res;
	auto turns = all_turns[vec.dir];
	for (auto& t : turns)
	{
		if (vec.dir != t && step.sn < min_s)
			continue;
		point p = vec.pos + t;
		int sn = vec.dir == t ? step.sn + 1 : 1;
		if (p.x < 0 || sn == 1 && t.x < 0 && p.x < min_s-1 || p.x >= hmap[0].size() || sn ==1 && t.x > 0 && p.x > hmap[0].size() - min_s 
			|| p.y < 0 || sn == 1 && t.y < 0 && p.y < min_s-1 || p.y >= hmap.size() || sn == 1 && t.y > 0 && p.y > hmap.size() - min_s || sn > max_s)
			continue;
		res.push_back({ { p, t }, sn, step.heat + hmap[p.y][p.x]});
	}
	return res;
}
long long solve1(const vector<string>& input, int min_s = 1, int max_s = 3)
{
	heat_map hmap = to_heat_map(input);
	vector<vector<path_t>> visited(hmap.size(), vector<path_t>(hmap[0].size(), path_t()));
	priority_queue<path_t> paths;
	paths.push({ {{{0,0},{1,0}}, 1, 0} });
	paths.push({ {{{0,0},{0,1}}, 1, 0} });
	point dest_p{ (int)(input[0].size()) - 1, (int)(input.size()) - 1 };
	while (paths.top().back().vec.pos != dest_p)
	{
		auto path = paths.top(); paths.pop();
		auto& cur = path.back();
		auto dest = turn(path, hmap, min_s, max_s);
		for (auto& d : dest)
		{
			auto& visit = visited[d.vec.pos.y][d.vec.pos.x];
			auto itv = r::find_if(visit, [&](auto& p) { return p.vec == d.vec && p.sn == d.sn; });
			if (itv == visit.end() || d.heat < itv->heat)
			{
				if (itv != visit.end())
					*itv = d;
				else
					visit.push_back(d);
				path_t new_path(path);
				new_path.push_back(d);
				paths.push(new_path);
			}
		}
	}
	return paths.top().back().heat;
}
long long solve2(const vector<string>& input)
{
	return solve1(input, 4, 10);
}
int main()
{
	test();
	ifstream is("Day17.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day17 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day17 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533)";

	auto is = stringstream(test1);
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;

	is = stringstream(R"(111111111111
999999999991
999999999991
999999999991
999999999991)");
	start = istream_iterator<string>(is);
	input = vector<string>(start, end);
	cout << "Test3: " << solve2(input) << endl;
}
