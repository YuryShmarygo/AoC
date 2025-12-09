#include "../common.h"

typedef vector<point> input_t;
input_t read_input(istream& is) 
{
	input_t res;
	string line;
	while (getline(is, line) && !line.empty())
	{
		point p;
		stringstream s(line);
		char c;
		s >> p.x >> c >> p.y;
		res.push_back(p);
	}
	return res;
}

struct rect
{
	point p1;
	point p2;
};
rect make_rect(const point& p1, const point& p2)
{
	return { { min(p1.x, p2.x), min(p1.y, p2.y) }, { max(p1.x, p2.x), max(p1.y, p2.y) } };
}

long long area(const point& p1, const point& p2)
{
	return (abs((long long) p1.x - p2.x)+1) * (abs((long long)p1.y - p2.y)+1);
}

vector<pair<long long, rect>> areas(const input_t& input)
{
	vector<pair<long long, rect>> areas;
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		for (auto it2 = next(it); it2 != input.end(); ++it2)
		{
			auto a = area(*it, *it2);
			areas.push_back({ a, make_rect(*it, *it2) });
		}
	}
	r::sort(areas, [](const auto& p1, const auto& p2) { return p1.first > p2.first; });
	return areas;
}

long long solve1(const input_t& input)
{
	auto a = areas(input);
	return a[0].first;
}

struct segment
{
	point p1;
	point p2;
	bool contains(const point& p) const
	{
		return p1.x == p2.x && p.x == p1.x && p.y >= min(p1.y, p2.y) && p.y <= max(p1.y, p2.y)
			|| p1.y == p2.y && p.y == p1.y && p.x >= min(p1.x, p2.x) && p.x <= max(p1.x, p2.x);
	}
	bool crossed_right(const point& p) const
	{ 
		return p1.x == p2.x && p.x < p1.x && p.y >= min(p1.y, p2.y) && p.y < max(p1.y, p2.y); 
	}
};
bool cross(const segment& s1, const segment& s2)
{
	if (s1.p1.x == s1.p2.x && s2.p1.y == s2.p2.y)
	{
		return min(s2.p1.x, s2.p2.x) < s1.p1.x && s1.p1.x < max(s2.p1.x, s2.p2.x)
			&& min(s1.p1.y, s1.p2.y) <= s2.p1.y && s2.p1.y <= max(s1.p1.y, s1.p2.y)
			|| min(s2.p1.x, s2.p2.x) <= s1.p1.x && s1.p1.x <= max(s2.p1.x, s2.p2.x)
			&& min(s1.p1.y, s1.p2.y) < s2.p1.y && s2.p1.y < max(s1.p1.y, s1.p2.y);
	}
	else if (s1.p1.y == s1.p2.y && s2.p1.x == s2.p2.x)
	{
		return min(s1.p1.x, s1.p2.x) < s2.p1.x && s2.p1.x < max(s1.p1.x, s1.p2.x)
			&& min(s2.p1.y, s2.p2.y) <= s1.p1.y && s1.p1.y <= max(s2.p1.y, s2.p2.y)
			|| min(s1.p1.x, s1.p2.x) <= s2.p1.x && s2.p1.x <= max(s1.p1.x, s1.p2.x)
			&& min(s2.p1.y, s2.p2.y) < s1.p1.y && s1.p1.y < max(s2.p1.y, s2.p2.y);
	}
	return false;
}
bool inside(const point& p, const vector<segment>& path)
{
	 if(r::any_of(path, [&](const segment& s) { return s.contains(p); }))
		return true;
	int crossed = r::count_if(path, [&](const segment& s) { return s.crossed_right(p); });
	return crossed % 2 == 1;
}
vector<point> vertices(const rect& r)
{
	return { r.p1, {r.p1.x, r.p2.y}, r.p2, {r.p2.x, r.p1.y} };
}
vector<segment> edges(const rect& r)
{
	if (r.p1.x == r.p2.x || r.p1.y == r.p2.y)
		return { {min(r.p1.x, r.p2.x), min(r.p1.y, r.p2.y)}, {max(r.p1.x, r.p2.x), max(r.p1.y, r.p2.y)}};
	return { {r.p1, {r.p1.x, r.p2.y}}, {{r.p1.x, r.p2.y}, r.p2}, {r.p2, {r.p2.x, r.p1.y}}, {{r.p2.x, r.p1.y}, r.p1} };
}
bool inside(const rect& r, const vector<segment>& path)
{
	list<rect> to_check = { r };
	while (!to_check.empty())
	{
		auto cr = to_check.front();
		to_check.pop_front();
		auto v = vertices(cr);
		if (r::any_of(v, [&](const point& p) { return !inside(p, path); }))
			return false;
		auto e = edges(cr);
		if (r::any_of(edges(cr), [&](const auto& e) { return r::any_of(path, [&](const auto& p) { return cross(e, p); }); }))
		{
			point mid = { (cr.p1.x + cr.p2.x) / 2, (cr.p1.y + cr.p2.y) / 2 };
			if (mid.x == cr.p1.x && mid.y == cr.p1.y)
				continue;
			to_check.push_back({ cr.p1, mid });
			to_check.push_back({ { mid.x + 1, cr.p1.y }, { cr.p2.x, mid.y } });
			to_check.push_back({ { cr.p1.x, mid.y + 1 }, { mid.x, cr.p2.y } });
			to_check.push_back({ { mid.x + 1, mid.y + 1 }, cr.p2 });
		}
	}
	return true;
}
void check_intersections(const vector<segment>& path)
{
	for (int i = 0; i < path.size(); ++i)
	{
		for (int j = 0; j < path.size(); ++j)
		{
			if (abs(i - j) <= 1 || abs(i-j) == path.size()-1)
				continue;
			auto p1 = path[i];
			auto p2 = path[j];
			if (cross(p1,p2))
				cout << "Intersected " << i << " " <<j << " : p1 = " << p1.p1 << " - " << p1.p2 << " p2 = " << p2.p1 << " - " << p2.p2 << endl;
		}
	}
}
long long solve2(const input_t& input)
{
	vector<segment> path;
	for (int i = 0; i < input.size() - 1; ++i)
		path.push_back(segment(input[i], input[i + 1]));
	path.push_back(segment(input.back(), input.front()));
	check_intersections(path);

	auto av = areas(input);
	for (auto& a : av)
	{
		if (inside(a.second, path))
			return a.first;
	}
	return 0;
}

int main()
{
	test();
	ifstream is("Day09.txt");
	input_t input = read_input(is);

	cout << "Day9 Answer1: " << solve1(input) << endl;
	cout << "Day9 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(7,1
11,1
11,7
9,7
9,5
2,5
2,3
7,3)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
