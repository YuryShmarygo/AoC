#include "../common.h"

struct robot
{
	point p;
	point v;
};
istream& operator>>(istream& is, robot& r)
{
	string line;
	getline(is, line);
	smatch m;
	if (regex_match(line, m, regex(R"(p=(\d+),(\d+) v=(-?\d+),(-?\d+))")))
	{
		r.p.x = stoi(m[1].str());
		r.p.y = stoi(m[2].str());
		r.v.x = stoi(m[3].str());
		r.v.y = stoi(m[4].str());
	}
	return is;
}
ostream& operator<<(ostream& os, const point& p) { os << p.x << "," << p.y; return os; }
ostream& operator<<(ostream& os, const robot& r) { os << "p=" << r.p << " v=" << r.v; return os; }
typedef vector<robot> input_t;
input_t read_input(istream& is) 
{
	istream_iterator<robot> start(is), end;
	return input_t(start, end);
}
long long conv(long long n, int b)
{
	return n > 0 ? n % b : (b * (-n / b + 1) + n) % b;
}
int move(int n, int v, int b, int s)
{
	long long res = n + (long long)v * s;
	return (int)conv(res, b);
}
void move(robot& r, int w, int h, int s)
{
	r.p.x = move(r.p.x, r.v.x, w, s);
	r.p.y = move(r.p.y, r.v.y, h, s);
}
void move(vector<robot>& robots, int w, int h, int s)
{
	for (auto& r : robots)
		move(r, w, h, s);
}
int count_robots(const input_t& input, point p1, point p2)
{
	return (int)r::count_if(input, [&](auto& r) { return r.p.x >= p1.x && r.p.x < p2.x && r.p.y >= p1.y && r.p.y < p2.y; });
}
long long solve1(const input_t& input, int w, int h)
{
	vector<robot> robots(input);
	move(robots, w, h, 100);

	//cout << endl; r::copy(robots, ostream_iterator<robot>(cout, "\n"));

	int n1 = count_robots(robots, { 0, 0 }, { w / 2, h / 2 });
	int n2 = count_robots(robots, { w / 2 + 1, 0 }, { w, h / 2 });
	int n3 = count_robots(robots, { 0, h / 2 + 1 }, { w / 2, h });
	int n4 = count_robots(robots, { w / 2 + 1, h / 2 + 1 }, { w, h });
	return (long long)n1 * n2 * n3 * n4;
}

long long solve2(const input_t& input, int w, int h)
{
	long long num = 0;
	return num;
}

int main()
{
	test();
	ifstream is("Day14.txt");
	input_t input = read_input(is);

	cout << "Day14 Answer1: " << solve1(input, 101, 103) << endl;
	cout << "Day14 Answer2: " << solve2(input, 101, 103) << endl;
}
void test()
{
	auto test0 = R"(p=2,4 v=2,-3)";
	auto is = stringstream(test0);
	robot r; is >> r;
	for (int s = 1; s <= 5; ++s)
	{
		move(r, 11, 7, 1);
		cout << s << ": " << r << endl;
	}
	is = stringstream(test0);
	is >> r;
	move(r, 11, 7, 5);
	cout << r << endl;
	auto test1 = R"(p=0,4 v=3,-3
p=6,3 v=-1,-3
p=10,3 v=-1,2
p=2,0 v=2,-1
p=0,0 v=1,3
p=3,0 v=-2,-2
p=7,6 v=-1,-3
p=3,0 v=-1,-2
p=9,3 v=2,3
p=7,3 v=-1,2
p=2,4 v=2,-3
p=9,5 v=-3,-3)";

	is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input, 11, 7) << endl;
	cout << "Test2: " << solve2(input, 11, 7) << endl;
}
