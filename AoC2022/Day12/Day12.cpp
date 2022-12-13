#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};

list<point> find_all(const vector<string>& map, char e)
{
	list<point> res;
	for (size_t y = 0; y < map.size(); ++y)
		for (size_t x = 0; x < map[y].size(); ++x)
			if (map[y][x] == e)
				res.push_back({ (int)x, (int)y });
	return res;
}
point find_e(const vector<string>& map, char e)
{ return find_all(map, e).front(); }
void add_route(const vector<string>& map, vector<vector<list<point>>>& shortest, list<list<point>>& next, list<point> path, const point& p, char h)
{
	if (map[p.y][p.x] <= h + 1)
	{
		path.push_back(p);
		if (shortest[p.y][p.x].empty() || shortest[p.y][p.x].size() > path.size())
		{
			shortest[p.y][p.x] = path;
			next.push_back(path);
		}
	}
}
int find_route(const vector<string>& map, const list<point>& start, const point& end)
{
	vector<vector<list<point>>> shortest(map.size(), vector<list<point>>(map[0].size(), list<point>()));
	list<list<point>> next;
	for (auto& p : start)
	{
		shortest[p.y][p.x] = { p };
		next.push_back({ p });
	}
	while (!next.empty())
	{
		auto cur = next;
		next.clear();
		for (auto p : cur)
		{
			auto& pos = p.back();
			char h = map[pos.y][pos.x];
			for (int y = max(0, pos.y - 1); y <= min(pos.y + 1, (int)map.size() - 1); ++y)
				add_route(map, shortest, next, p, { pos.x, y }, h);
			for (int x = max(0, pos.x - 1); x <= min(pos.x + 1, (int)map[pos.y].size() - 1); ++x)
				add_route(map, shortest, next, p, { x, pos.y }, h);
		}
	}
	return static_cast<int>(shortest[end.y][end.x].size()) - 1;
}
int solve1(vector<string> map)
{
	point start = find_e(map, 'S'); map[start.y][start.x] = 'a';
	point end = find_e(map, 'E'); map[end.y][end.x] = 'z';
	return find_route(map, {start}, end);
}
int solve2(vector<string> map)
{
	point pstart = find_e(map, 'S'); map[pstart.y][pstart.x] = 'a';
	point end = find_e(map, 'E'); map[end.y][end.x] = 'z';
	auto start = find_all(map, 'a');
	return find_route(map, { start }, end);
}

int main()
{
	test();
	ifstream is("Day12.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Day12 answer1: " << solve1(input) << endl;
	cout << "Day12 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi)");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
