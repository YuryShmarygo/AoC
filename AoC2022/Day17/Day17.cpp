#include "../common.h"

struct point
{
	point() : x(0), y(0) {}
	point(int _x, int _y) : x(_x), y(_y) {}
	point(const point&) = default;
	point& operator=(const point&) = default;
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
/*
vector<vector<point>> patterns{
	{{0,0},{1,0},{2,0},{3,0}},
	{{1,0},{0,1},{1,1},{2,1},{1,2}},
	{{2,0},{2,1},{0,2},{1,2},{2,2}},
	{{0,0},{0,1},{0,2},{0,3}},
	{{0,0},{1,0},{0,1},{1,1}}};
*/
vector<vector<point>> patterns{
	{{0,0},{1,0},{2,0},{3,0}},
	{{1,0},{0,1},{1,1},{2,1},{1,2}},
	{{0,0},{1,0},{2,0},{2,1},{2,2}},
	{{0,0},{0,1},{0,2},{0,3}},
	{{0,0},{1,0},{0,1},{1,1}} };

void move_p(set<point>& s, const point& p)
{
	for (auto& r : s)
	{
		const_cast<point&>(r).x += p.x;
		const_cast<point&>(r).y += p.y;
	}
}

struct shape
{
	point pos;
	int w;
	int h;
	set<point> s;
	void init(int hs)
	{
		pos.x = 2;
		pos.y = hs + 4;
		move_p(s, pos);
	}
	void shift(const point& dp)
	{
		pos.x += dp.x;
		pos.y += dp.y;
		move_p(s, dp);
	}
};
set<point> to_set(const vector<point>& p) { return { p.begin(), p.end() }; }
vector<shape> all_shapes{
	{{0,0},4,1,to_set(patterns[0])},
	{{0,0},3,3,to_set(patterns[1])},
	{{0,0},3,3,to_set(patterns[2])},
	{{0,0},1,4,to_set(patterns[3])},
	{{0,0},2,2,to_set(patterns[4])}};

struct cave
{
	set<point> floor;
	int h;
	cave() : floor({{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0}}), h(0) {}
};
ostream& operator<<(ostream& os, const vector<string>& v) { copy(v.rbegin(), v.rend(), ostream_iterator<string>(os, "\n")); return os; }
vector<string> to_string(const cave& c)
{
	vector<string> res(c.h + 1, string("......."));
	for (auto& p : c.floor)
		res[p.y][p.x] = '#';
	return res;
}
ostream& operator<<(ostream& os, const cave& c) { return os << to_string(c); }
vector<string> to_string(const cave& c, const shape& s)
{
	vector<string> res(max(s.pos.y + s.h, c.h+1), string("......."));
	for (auto& p : c.floor)
		res[p.y][p.x] = '#';
	for (auto& p : s.s)
		res[p.y][p.x] = '@';
	return res;
}
bool can_move(cave& c, set<point>& s)
{
	set<point> inter;
	set_intersection(c.floor.begin(), c.floor.end(), s.begin(), s.end(), inserter(inter, inter.end()));
	return inter.empty();
}
bool fall(cave& c, shape& s, char w)
{
	set<point> m = s.s;
	int dx = (w == '<' ? -1 : 1);
	if (s.pos.x + dx >= 0 && s.pos.x + s.w - 1 + dx < 7)
	{
		move_p(m, { dx, 0 });
		if (can_move(c, m))
		{
			s.s = m;
			s.pos.x += dx;
		}
	}

	m = s.s;
	move_p(m, { 0, -1 });
	if (can_move(c, m))
	{
		s.s = m;
		--s.pos.y;
		//cout << "fall wind: " << w << endl << to_string(c, s) << endl;
		return true;
	}
	c.floor.insert(s.s.begin(), s.s.end());
	int max_y = r::max_element(s.s, [](auto& p1, auto& p2) {return p1.y < p2.y; })->y;
	c.h = max(c.h, max_y);
	//cout << "fall wind: " << w << endl << to_string(c, s) << endl;
	return false;
}
int solve1(const string& wind, int steps)
{
	cave c;
	int cur_w = 0;
	for (int i = 0; i < steps; ++i)
	{
		//cout << "i: " << i << endl << c << endl;
		int cur_s = i % 5;
		shape s = all_shapes[cur_s];
		s.init(c.h);
		while (fall(c, s, wind[cur_w]))
			cur_w = (cur_w + 1) % static_cast<int>(wind.size());
		cur_w = (cur_w + 1) % static_cast<int>(wind.size());
	}
	//cout << c << endl;
	auto str = to_string(c);
	int start_s = 0;
	for (int i = 1; i < str.size(); ++i)
	{
		
	}
	return c.h;
}

int main()
{
	test(); return 0;
	ifstream is("Day17.txt");
	string w;
	is >> w;

	cout << "Day17 answer1: " << solve1(w, 2022) << endl;
	cout << "Day17 answer2: " << 1 << endl;
}
void test()
{
	stringstream is(R"(>>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>)");
	string w;
	is >> w;
	//cout << "test1: " << solve1(w,10) << endl;
	cout << "test2: " << solve1(w, 2022) << endl;
	cout << "test3: " << 1 << endl;
}
