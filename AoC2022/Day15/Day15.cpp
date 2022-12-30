#include "../common.h"

struct point
{
	int x;
	int y;
	bool operator==(const point&) const = default;
	auto operator<=>(const point& point) const = default;
	bool operator<(const point&) const = default;
};
point operator+(const point& p1, const point& p2) { return {p1.x + p2.x, p1.y + p2.y};}
int dist(const point& p1, const point& p2) { return abs(p1.x - p2.x) + abs(p1.y - p2.y); }
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
	array<point, 4> v;
};
istream& operator>>(istream& is, sensor& s)
{
	string str;
	is >> str >> str >> s.s >> str >> str >> str >> str >> str >> s.b;
	s.d = dist(s.s, s.b);
	s.v = array<point, 4>({ {s.s.x - s.d, s.s.y}, {s.s.x + s.d, s.s.y}, {s.s.x, s.s.y - s.d}, {s.s.x, s.s.y + s.d} });
	return is;
}
ostream& operator<<(ostream& os, const sensor& s)
{ return os << s.s << " " << s.b; }

int solve1(const vector<sensor>& input, int row)
{
	vector<point> shorts;
	for (auto& s : input)
	{
		int dx = s.d - abs(s.s.y - row);
		if (dx >= 0) shorts.push_back({ s.s.x - dx, s.s.x + dx });
	}
	r::sort(shorts);
	int sum = 0;
	int end_x = shorts.front().x - 1;
	for (auto& p : shorts)
	{
		sum += p.y > end_x ? p.y - max(end_x+1,p.x) + 1 : 0;
		end_x = max(p.y, end_x);
	}
	set<point> in_row; for (auto& s : input) { if (s.b.y == row) in_row.insert(s.b); }
	return sum - static_cast<int>(in_row.size());
}
struct box
{
	box(const point& p1, const point& p2, const int i = 0) : p1(p1), p2(p2), i(i), v{ p1, p2, { p1.x, p2.y }, { p2.x, p1.y } } {}

	point p1;
	point p2;
	int i;
	array<point, 4> v;
};
ostream& operator<<(ostream& os, const box& b) { return os << "[p1: " << b.p1 << ", p2: " << b.p2 << ", i: " << b.i << "]"; }
bool operator<(const box& b1, const box& b2) { return b1.i > b2.i || b1.i == b2.i && b1.p2.x + b1.p2.y - b1.p1.x - b1.p1.y > b2.p2.x + b2.p2.y - b2.p1.x - b2.p1.y; }
bool inside_box(const box& b, const point& p) { return p.x >= b.p1.x && p.x <= b.p2.x && p.y >= b.p1.y && p.y <= b.p2.y; }
bool inside_sensor(const sensor& s, const point& p) { return dist(s.s, p) <= s.d; }
bool inside_sensor(const sensor& s, const box& b) { return r::all_of(b.v, [&](auto& p) { return inside_sensor(s, p); }); }
bool intersects(const box& b, const sensor& s)
{ return r::any_of(b.v, [&](auto& p) {return inside_sensor(s, p); }) || r::any_of(s.v, [&](auto& p) {return inside_box(b, p); }); }
int count_int(const vector<sensor>& input, const box& b) { return static_cast<int>(r::count_if(input, [&](auto& s) {return intersects(b, s); })); }
array<box, 4> split(const box& b)
{
	int dx = b.p2.x - b.p1.x; dx = (dx + 1) >> 1;
	int dy = b.p2.y - b.p1.y; dy = (dy + 1) >> 1;

	return array<box, 4>({ box({ b.p1.x, b.p1.y }, { dx > 0 ? b.p1.x + dx - 1 : b.p1.x, dy > 0 ? b.p1.y + dy - 1 : b.p1.y }),
		box({ b.p1.x + dx, b.p1.y }, { b.p2.x, dy > 0 ? b.p1.y + dy - 1 : b.p1.y }),
		box({ b.p1.x, b.p1.y + dy }, { dx > 0 ? b.p1.x + dx - 1 : b.p1.x, b.p2.y }),
		box({ b.p1.x + dx, b.p1.y + dy }, { b.p2.x, b.p2.y }) });
}
array<box, 4> split(const vector<sensor>& input, const box& b)
{
	auto res = split(b);
	for (auto& sub : res)
		sub.i = count_int(input, sub);
	return res;
}
ostream& operator<<(ostream& os, const vector<box>& bb) { r::copy(bb, ostream_iterator<box>(os, ", ")); return os; }
long long solve2(const vector<sensor>& input, int max_size)
{
	priority_queue<box> queue;
	queue.push({ {0,0}, {max_size, max_size}, static_cast<int>(input.size())});
	while (!queue.empty() && queue.top().i > 0)
	{
		auto b = queue.top(); queue.pop();
//		cout << "Checking: " << b << endl;
		if (b.p1.x == b.p2.x && b.p1.y == b.p2.y) continue;
		if (r::any_of(input, [&](auto& s) { return inside_sensor(s, b); })) continue;
		auto sub = split(input, b);
//		cout << "Adding: " << sub << endl;
		for (auto& sb : sub) queue.push(sb);
	}
	return queue.empty() ? 0 : static_cast<long long>(queue.top().p1.x) * 4000000 + queue.top().p1.y;
}

int main()
{
	test(); //return 0;
	ifstream is("Day15.txt");
	istream_iterator<sensor> start(is), end;
	vector<sensor> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day15 answer1: " << solve1(input, 2000000) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day15 answer2: " << solve2(input, 4000000) << " took: " << ElapsedMs(t_start) << "ms" << endl;
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
