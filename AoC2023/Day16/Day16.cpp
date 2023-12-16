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
struct beam
{
	point p;
	point d;
	bool operator==(const beam&) const = default;
	auto operator<=>(const beam&) const = default;
	bool operator<(const beam&) const = default;
};
map<pair<point, char>, vector<point>> split =
{
	{{{ 1, 0}, '.'}, {{ 1, 0}}},
	{{{ 0, 1}, '.'}, {{ 0, 1}}},
	{{{-1, 0}, '.'}, {{-1, 0}}},
	{{{ 0,-1}, '.'}, {{ 0,-1}}},
	{{{ 1, 0}, '/'}, {{ 0,-1}}},
	{{{-1, 0}, '/'}, {{ 0, 1}}},
	{{{ 0, 1}, '/'}, {{-1, 0}}},
	{{{ 0,-1}, '/'}, {{ 1, 0}}},
	{{{ 1, 0}, '\\'}, {{ 0, 1}}},
	{{{-1, 0}, '\\'}, {{ 0,-1}}},
	{{{ 0, 1}, '\\'}, {{ 1, 0}}},
	{{{ 0,-1}, '\\'}, {{-1, 0}}},
	{{{ 1, 0}, '|'}, {{ 0,-1}, { 0, 1}}},
	{{{-1, 0}, '|'}, {{ 0,-1}, { 0, 1}}},
	{{{ 0, 1}, '|'}, {{ 0, 1}}},
	{{{ 0,-1}, '|'}, {{ 0,-1}}},
	{{{ 1, 0}, '-'}, {{ 1, 0}}},
	{{{-1, 0}, '-'}, {{-1, 0}}},
	{{{ 0, 1}, '-'}, {{ 1, 0}, {-1, 0}}},
	{{{ 0,-1}, '-'}, {{ 1, 0}, {-1, 0}}}
};

long long trace(const vector<string>& input, const beam& start)
{
	list<beam> beams;
	beams.push_back(start);
	set<beam> visited;
	set<point> energized;
	while (!beams.empty())
	{
		auto b = beams.front(); beams.erase(beams.begin());
		while (!visited.contains(b) && b.p.y >= 0 && b.p.y < input.size() && b.p.x >= 0 && b.p.x < input[0].size())
		{
			visited.insert(b);
			energized.insert(b.p);
			auto& dir = split[{b.d, input[b.p.y][b.p.x]}];
			if (dir.size() > 1)
				beams.push_back({ b.p + dir[1], dir[1] });
			b.p = b.p + dir[0];
			b.d = dir[0];
		}
	}
	return energized.size();
}
long long solve1(const vector<string>& input)
{
	return trace(input, { {0,0}, {1, 0} });
}
long long solve2(vector<string>& input)
{
	long long res = 0;
	for (int x = 0; x < input[0].size(); ++x)
	{
		res = max(res, trace(input, { { x, 0 }, { 0, 1 } }));
		res = max(res, trace(input, { { x, (int)input.size() - 1}, {0, -1}}));
	}
	for (int y = 0; y < input.size(); ++y)
	{
		res = max(res, trace(input, { { 0, y }, { 1, 0 } }));
		res = max(res, trace(input, { { (int)input[0].size() - 1, y}, {-1, 0} }));
	}
	return res;
}
int main()
{
	test();
	ifstream is("Day16.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day16 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day16 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....)";

	auto is = stringstream(test1);
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
