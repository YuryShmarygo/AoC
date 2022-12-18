#include "../common.h"

struct point
{
	point() : x(0), y(0){}
	point(int _x, int _y) : x(_x), y(_y) {}
	point(const point&) = default;
	point& operator=(const point&) = default;
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2)
{ return {p1.x + p2.x, p1.y + p2.y};}
int dist(const point& p1, const point& p2)
{ return abs(p1.x - p2.x) + abs(p1.y - p2.y); }
istream& operator>>(istream& is, point& p)
{
	char c;
	is >> c >> c >> p.x >> c >> c >> c >> p.y;
	return is;
}
ostream& operator<<(ostream& os, const point& p)
{ return os << p.x << "," << p.y; }
struct sensor
{
	point s;
	point b;
	int d;
};
istream& operator>>(istream& is, sensor& s)
{
	string str;
	is >> str >> str >> s.s >> str >> str >> str >> str >> str >> s.b;
	s.d = dist(s.s, s.b);
	return is;
}
ostream& operator<<(ostream& os, const sensor& s)
{ return os << s.s << " " << s.b; }
struct box
{
	point c;
	int d;
};
/*
int solve1(const vector<sensor>& input, int row)
{
	set<point> res;
	for (auto& s : input)
	{
		int d = dist(s.s, s.b);
		int dx = d - abs(s.s.y - row);
		for (int x = s.s.x - dx; x <= s.s.x + dx; ++x)
			res.insert(point(x, row));
	}
	return static_cast<int>(res.size()) - 1; // should actually substruct number of beacons in this row
}
*/
/*
struct sline
{
	int x1;
	int x2;
	bool operator<(const sline&) const = default;
};
struct srow
{
	vector<sline> s;
	void add_sline(const sline& l)
	{
		auto it = s.begin();
		for (; it != s.end(); ++it)
		{
			if (l.x1 < it->x1 && l.x2 < it->x1)
			{
				s.insert(it, l);
				break;
			}
			if (l.x1 < it->x1 && l.x2 >= it->x1 && l.x2 < it->x2)
			{
				it->x1 = l.x1;
				break;
			}
			if (l.x1 < it->x1 && l.x2 > )
			if (l.x1 >= it->x1 && l.x1 <= it->x2 && l.x2 <= it->x2)
			if (s[i].x1 <= l.x1 && s[i].x2 >= l.x1)
			{
				s[i].x2 = max(s[i].x2, l.x2);
				if (i < s.size() - 1 && s[i + 1].x1 <= s[i].x2)
				{
					s[i].x2 = s[i + 1].x2;
					s.erase(s.begin() + i + 1);
				}
				break;
			}
		}
	}
};
*/
/*
int solve1(const vector<sensor>& input, int row)
{
	set<int> res;
	for (auto& s : input)
	{
		int d = dist(s.s, s.b);
		int dx = d - abs(s.s.y - row);
		for (int x = s.s.x - dx; x <= s.s.x + dx; ++x)
			res.insert(x);
	}
	return static_cast<int>(res.size()) - 1; // should actually substruct number of beacons in this row
}
*/
int solve1(const vector<sensor>& input, int row)
{
	set<pair<int, bool>> joints;
	set<point> ends;
	for (auto& s : input)
	{
		if (s.b.y == row)
			ends.insert(s.b);
		int dx = s.d - abs(s.s.y - row);
		if (dx < 0)
			continue;
		int x1 = s.s.x - dx;
		int x2 = s.s.x + dx;
		auto it1 = joints.lower_bound({ x1, false });
		auto it2 = joints.lower_bound({ x2, false });
		if (it1 == joints.end() || !it1->second)
			joints.insert(it1, { x1, false });
		while (it1 != it2)
			it1 = joints.erase(it1);
		if (it2 == joints.end() || !it2->second)
			joints.insert(it2, { x2, true });
	}
	int res = 0;
	for (auto it = joints.begin(); it != joints.end();)
	{
		int x1 = it->first;
		++it;
		int x2 = it->first;
		++it;
		res += x2 - x1 + 1;
	}
	return res + static_cast<int>(ends.size());
}

/*
long long solve2(const vector<sensor>& input, int max_size)
{
	for (int y = 0; y <= max_size; ++y)
	{
		set<int> res;
		for (auto& s : input)
		{
			int d = dist(s.s, s.b);
			int dx = d - abs(s.s.y - y);
			for (int x = s.s.x - dx; x <= s.s.x + dx; ++x)
				if (x >= 0 && x <= max_size)
					res.insert(x);
		}
		if (res.size() < max_size+1)
		{
			for (int x = 0; x <= max_size; ++x)
				if (!res.contains(x))
					return (long long)x * 4000000 + y;
		}
	}
	return 0;
}
*/
struct area
{};
long long solve2(const vector<sensor>& input, int max_size)
{
	return 0;
}

int main()
{
	test();
	ifstream is("Day15.txt");
	istream_iterator<sensor> start(is), end;
	vector<sensor> input(start, end);

	cout << "Day15 answer1: " << solve1(input, 2000000) << endl;
	cout << "Day15 answer2: " << solve2(input, 4000000) << endl;
}
void test()
{
	ifstream is("Day15_test.txt");
	istream_iterator<sensor> start(is), end;
	vector<sensor> input(start, end);
	cout << input.size() << endl;
	cout << "test1: " << solve1(input, 10) << endl;
	cout << "test2: " << solve2(input, 20) << endl;
}
