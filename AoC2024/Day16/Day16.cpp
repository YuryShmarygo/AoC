#include "../common.h"

typedef vector<string> input_t;
input_t read_input(istream& is)
{
	return read_strings(is);
}
point find_char(const vector<string>& map, char c)
{
	for (int y = 0; y < map.size(); ++y)
		for (int x = 0; x < map[y].size(); ++x)
			if (map[y][x] == c)
				return { x,y };
	return { 0,0 };
}
struct vis_t
{
	point p;
	point d;
	bool operator==(const vis_t&) const = default;
	auto operator<=>(const vis_t& point) const = default;
	bool operator<(const vis_t&) const = default;
};
map<point, vector<point>> moves = { {{0,-1},{{0,-1},{ 1,0 },{-1,0}}}, {{0,1},{{0,1},{ 1,0 },{-1,0}}}, {{1,0},{{1,0},{ 0,-1 },{0,1}}},{{-1,0},{{-1,0}, { 0,-1 },{0,1}}} };
vector<vis_t> get_moves(const vis_t& cur, const input_t& input)
{
	vector<vis_t> res;
	for (auto& d : moves[cur.d])
	{
		point n = cur.p + d;
		if (input[n.y][n.x] != '#')
			res.push_back({ n,d });
	}
	return res;
}
struct path_t
{
	vector<vis_t> p;
	long long s;
};
vector<path_t> get_paths_reverse(const input_t& input)
{
	point start = find_char(input, 'S');
	point end = find_char(input, 'E');
	map<vis_t, long long> visited;
	map<vis_t, vector<path_t>> best_paths;
	visited[{start, { -1,0 }}] = 0;
	visited[{start, { 0,-1 }}] = 1000;

	list<path_t> paths;
	paths.push_back(path_t({ vector<vis_t>(1, {start, {1,0}}),0 }));
	paths.push_back(path_t({ vector<vis_t>(1, {start, {0,-1}}),1000 }));
	vector<path_t> res;
	while (!paths.empty())
	{
		for (auto it = paths.begin(); it != paths.end();)
		{
			auto path = *it;
			it = paths.erase(it);
			if (path.p.back().p == end)
				res.push_back(path);
			else
			{
				auto& cur = path.p.back();
				for (auto& move : get_moves(cur, input))
				{
					long long score = path.s + (cur.d == move.d ? 1 : 1001);
					auto itv = visited.find(move);
					if (itv == visited.end() || score <= itv->second)
					{
						auto path_new(path);
						path_new.p.push_back(move);
						path_new.s = score;
						paths.push_back(path_new);
						visited[move] = score;
					}
				}
			}
		}
	}
	return res;
}
vector<path_t> get_paths(const input_t& input)
{
	point start = find_char(input, 'S');
	point end = find_char(input, 'E');
	map<vis_t, long long> visited;
	visited[{start, { -1,0 }}] = 0;
	visited[{start, { 0,-1 }}] = 1000;

	list<path_t> paths;
	paths.push_back(path_t({ vector<vis_t>(1, {start, {1,0}}),0 }));
	paths.push_back(path_t({ vector<vis_t>(1, {start, {0,-1}}),1000 }));
	vector<path_t> res;
	while (!paths.empty())
	{
		for (auto it = paths.begin(); it != paths.end();)
		{
			auto path = *it;
			it = paths.erase(it);
			if (path.p.back().p == end)
				res.push_back(path);
			else
			{
				auto& cur = path.p.back();
				for (auto& move : get_moves(cur, input))
				{
					long long score = path.s + (cur.d == move.d ? 1 : 1001);
					auto itv = visited.find(move);
					if (itv == visited.end() || score <= itv->second)
					{
						auto path_new(path);
						path_new.p.push_back(move);
						path_new.s = score;
						paths.push_back(path_new);
						visited[move] = score;
					}
				}
			}
		}
	}
	return res;
}
long long solve1(const input_t& input)
{
	auto paths = get_paths(input);
	r::sort(paths, [](auto& p1, auto& p2) { return p1.s < p2.s; });
	return paths.front().s;
}
long long solve2(const input_t& input)
{
	auto paths = get_paths(input);
	r::sort(paths, [](auto& p1, auto& p2) { return p1.s < p2.s; });
	auto cost = paths.front().s;
	set<point> res;
	for (auto& p : paths)
	{
		if (p.s == cost)
		{
			for (auto& v : p.p)
				res.insert(v.p);
		}
	}
	return res.size();
}

int main()
{
	test();
	ifstream is("Day16.txt");
	input_t input = read_input(is);

	cout << "Day16 Answer1: " << solve1(input) << endl;
	cout << "Day16 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(###############
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############)";
	auto test2 = R"(#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;

	is = stringstream(test2); input = read_input(is);
	cout << "Test2: " << solve1(input) << endl;
	cout << "Test3: " << solve2(input) << endl;
}
