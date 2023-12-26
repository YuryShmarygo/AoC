#include "../common.h"

template<typename T>
struct point_t
{
	T x;
	T y;
	T z;
	bool operator==(const point_t&) const = default;
	auto operator<=>(const point_t& point) const = default;
	bool operator<(const point_t&) const = default;
};
template<typename T> point_t<T> operator+(const point_t<T>& p1, const point_t<T>& p2) { return point_t{ p1.x + p2.x, p1.y + p2.y, p1.z + p2.z }; }
template<typename T> point_t<T>& operator+=(point_t<T>& p1, const point_t<T>& p2) { p1.x += p2.x; p1.y += p2.y; p1.z += p2.z; return p1; }
template<typename T> point_t<T> operator-(const point_t<T>& p1, const point_t<T>& p2) { return point_t{ p1.x - p2.x, p1.y - p2.y, p1.z - p2.z }; }
template<typename T> point_t<T>& operator-=(point_t<T>& p1, const point_t<T>& p2) { p1.x -= p2.x; p1.y -= p2.y; p1.z -= p2.z; return p1; }

typedef point_t<double> point;

struct hail
{
	point p;
	point v;
};
hail read_hail(string line)
{
	hail h;
	smatch m;
	if (regex_match(line, m, regex(R"((\d+),\s+(\d+),\s+(\d+)\s+@\s+(-?\d+),\s+(-?\d+),\s+(-?\d+))")))
	{
		h.p.x = stoll(m[1].str());
		h.p.y = stoll(m[2].str());
		h.p.z = stoll(m[3].str());
		h.v.x = stoll(m[4].str());
		h.v.y = stoll(m[5].str());
		h.v.z = stoll(m[6].str());
	}
	return h;
}
typedef vector<hail> hail_v;
istream& operator>>(istream& is, hail_v& hails)
{
	hails.clear();
	string line;
	while (getline(is, line) && !line.empty())
		hails.push_back(read_hail(line));
	return is;
}
void denominate(hail_v& input, double& minx, double& maxx)
{
	point minp = { LLONG_MAX, LLONG_MAX, LLONG_MAX };
	for (auto& h : input)
	{
		minp.x = min(minp.x, h.p.x);
		minp.y = min(minp.y, h.p.y);
		minp.z = min(minp.z, h.p.z);
	}
	minx -= minp.x;
	maxx -= minp.x;
	minp.y = minp.z = minp.x;
	for (auto& h : input)
		h.p -= minp;
}
bool cross(const hail& h1, const hail& h2)
{
	return h2.p.x > h1.p.x && h1.v.x > h2.v.x || h2.p.x < h1.p.x && h1.v.x < h2.v.x;
}
hail to_segment(const hail& h, double minp, double maxp)
{
	hail s{ {0,0,0},{0,0,0} };
	if (h.p.y < minp && h.v.y < 0 || h.p.y > maxp && h.v.y > 0 || h.p.x < minp && h.v.x < 0 || h.p.x > maxp && h.v.x > 0) return s;
	double tminx = (minp - h.p.x) / h.v.x;
	double tmaxx = (maxp - h.p.x) / h.v.x;
	double tminy = (minp - h.p.y) / h.v.y;
	double tmaxy = (maxp - h.p.y) / h.v.y;
	if (h.p.x < minp && (h.v.y > 0 && tmaxy < tminx || h.v.y < 0 && tminy < tminx) ||
		h.p.x > maxp && (h.v.y > 0 && tmaxy < tmaxx || h.v.y < 0 && tminy < tmaxx) ||
		h.p.x > minp && h.p.x < maxp && h.p.y < minp && (h.v.x < 0 && tminx < tminy || h.v.x > 0 && tmaxx < tminy) ||
		h.p.x > minp && h.p.x < maxp && h.p.y > maxp && (h.v.x < 0 && tminx < tmaxy || h.v.x > 0 && tmaxx < tmaxy)) return s;
	double t1 = 0, t2 = 0;
	if (h.p.x < minp)
	{
		t1 = h.p.y < minp ? max(tminx, tminy) : h.p.y > maxp ? max(tminx, tmaxy) : tminx;
		t2 = h.p.y < minp ? min(tmaxx, tmaxy) : h.p.y > maxp ? min(tmaxx, tminy) : tmaxx;
	}
	else if (h.p.x > maxp)
	{
		t1 = h.p.y < minp ? max(tmaxx, tminy) : h.p.y > maxp ? max(tmaxx, tmaxy) : tmaxx;
		t2 = h.p.y < minp ? min(tminx, tmaxy) : h.p.y > maxp ? min(tminx, tminy) : tminx;
	}
	else
	{
		t1 = h.p.y < minp ? tminy : h.p.y > maxp ? tmaxy : 0;
		t2 = h.v.y < 0 ? h.v.x < 0 ? min(tminx, tmaxy) : min(tmaxx, tmaxy) : h.v.x < 0 ? min(tminx, tmaxy) : min(tmaxx, tmaxy);
	}
	s.p.x = h.p.x + h.v.x * t1;
	s.p.y = h.p.y + h.v.y * t1;
	s.v.x = h.p.x + h.v.x * t2;
	s.v.y = h.p.y + h.v.y * t2;
	return s;
}
hail_v to_segment(const hail_v& input, double minp, double maxp)
{
	hail_v res;
	for (auto& h : input) res.push_back(to_segment(h, minp, maxp));
	return res;
}
bool ccw(const point& a, const point& b, const point& c)
{ return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x); }
bool intersect(const hail& h1, const hail& h2)
{ return ccw(h1.p, h2.p, h2.v) != ccw(h1.v, h2.p, h2.v) && ccw(h1.p, h1.v, h2.p) != ccw(h1.p, h1.v, h2.v); }
long long solve1(hail_v input, double minp, double maxp)
{
	//denominate(input, minp, maxp);
	hail_v segments = to_segment(input, minp, maxp);
	long long res = 0;
	for (auto s1 = segments.begin(); s1 != segments.end(); ++s1)
		for (auto s2 = next(s1); s2 != segments.end(); ++s2)
			if (intersect(*s1, *s2))
				++res;

	return res;
}
long long solve2(const hail_v& input)
{
	long long res = 0;
	return res;
}
int main()
{
	test();
	ifstream is("Day24.txt");
	hail_v input; is >> input;

	auto t_start = high_resolution_clock::now();
	cout << "Day24 Answer1: " << solve1(input, 200000000000000, 400000000000000) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day24 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(19, 13, 30 @ -2,  1, -2
18, 19, 22 @ -1, -1, -2
20, 25, 34 @ -2, -2, -4
12, 31, 28 @ -1, -2, -1
20, 19, 15 @  1, -5, -3)";

	auto is = stringstream(test1);
	hail_v input; is >> input;

	cout << "Test1: " << solve1(input, 7, 27) << endl;
	cout << "Test2: " << solve2(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
