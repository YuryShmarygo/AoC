#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
	point operator+(const point& p) const { return { x + p.x, y + p.y }; }
};
struct blizzard
{
	point pos;
	char dir;
	bool operator==(const blizzard&) const = default;
	auto operator<=>(const blizzard& point) const = default;
	bool operator<(const blizzard&) const = default;
};
set<blizzard> get_blizz(const vector<string>& input)
{
	set<blizzard> res;
	for (int y = 0; y < static_cast<int>(input.size()); ++y)
		for (int x = 0; x < static_cast<int>(input[y].size()); ++x)
			if (string("><v^").find(input[y][x]) != string::npos)
				res.insert({{x,y},input[y][x]});
	return res;
}
vector<point> all_directions{ {0,0}, {-1,0},{1,0},{0,-1},{0,1} };
map<char, point> b_moves{ {'>',{1,0}},{'<',{-1,0}},{'v',{0,1}},{'^',{0,-1}} };
blizzard b_move(const blizzard b, const point& d)
{
	blizzard res{ b.pos + b_moves[b.dir], b.dir };
	if (res.pos.x <= 0)
		res.pos.x = d.x - 1;
	if (res.pos.x >= d.x)
		res.pos.x = 1;
	if (res.pos.y <= 0)
		res.pos.y = d.y - 1;
	if (res.pos.y >= d.y)
		res.pos.y = 1;
	return res;
}
void move_blizz(set<blizzard>& blizz, const point& b_dim)
{
	set<blizzard> tmp;
	for (auto& b : blizz) tmp.insert(b_move(b, b_dim));
	swap(blizz, tmp);
}
void step(list<list<point>>& paths, set<blizzard>& blizz, const point& b_dim)
{
	point p_start = { 1,0 }, p_end{ b_dim + point{-1,0} };
	set<point> visited;
	for (auto it = paths.begin(); it != paths.end();)
	{
		auto p = *it; it = paths.erase(it);
		auto itb = blizz.lower_bound({ p.back(), '<' });
		if (itb != blizz.end() && itb->pos == p.back()) continue;
		for (auto& d : all_directions)
		{
			auto next = p.back() + d;
			if (next.x <= 0 || next.x >= b_dim.x || next.y <= 0 && next != p_start || next.y >= b_dim.y && next != p_end) continue;
			if (visited.contains(next)) continue;
			visited.insert(next);
			auto path = p;
			path.push_back(next);
			paths.push_front(path);
		}
	}
	move_blizz(blizz, b_dim);
}
int solve1(const vector<string>& input)
{
	auto blizz = get_blizz(input);
	point b_dim{ static_cast<int>(input[0].size()) - 1, static_cast<int>(input.size()) - 1 };
	point p_start = { 1,0 }, p_end{ b_dim + point{-1,0}};
	list<list<point>> paths{ {{1,0}} };
	while (!paths.empty() && (static_cast<int>(paths.front().size()) < b_dim.x+b_dim.y-2 || r::find_if(paths, [&](auto& p) {return p.back() == p_end; }) == paths.end()))
		step(paths, blizz, b_dim);
	return static_cast<int>(paths.front().size()) - 1;
}
bool reached(const list<list<point>>& paths, const point& b_dim, const point& p_end)
{
	return static_cast<int>(paths.front().size()) > b_dim.x + b_dim.y - 3
	&& r::find_if(paths, [&](auto& p) {return p.back() == p_end; }) != paths.end();
}
int solve2(const vector<string>& input)
{
	auto blizz = get_blizz(input);
	point b_dim{ static_cast<int>(input[0].size()) - 1, static_cast<int>(input.size()) - 1 };
	point p_start = { 1,0 }, p_end{ b_dim + point{-1,0} };
	list<list<point>> paths{ {{1,0}} };
	while (!paths.empty() && !reached(paths, b_dim, p_end))
		step(paths, blizz, b_dim);
	int res = static_cast<int>(paths.front().size()) - 1;
	paths = { {{p_end}} };
	while (!paths.empty() && !reached(paths, b_dim, p_start))
		step(paths, blizz, b_dim);
	res += static_cast<int>(paths.front().size()) - 1;
	paths = { {{p_start}} };
	while (!paths.empty() && !reached(paths, b_dim, p_end))
		step(paths, blizz, b_dim);
	res += static_cast<int>(paths.front().size()) - 1;
	return res;
}
int main()
{
	test(); //return 0;
	ifstream is("Day24.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << "Day24 answer1: " << solve1(input) << endl;
	cout << "Day24 answer2: " << solve2(input) << endl;
}
void test()
{
	stringstream is(R"(#.######
#>>.<^<#
#.<..<<#
#>v.><>#
#<^v^^>#
######.#)");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input) << endl;
	cout << "test2: " << solve2(input) << endl;
}
