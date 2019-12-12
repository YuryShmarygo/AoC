#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;

struct Point
{
	Point() :x(0), y(0), z(0) {}
	Point(int _x, int _y, int _z) : x(_x), y(_y), z(_z){}
	int x;
	int y;
	int z;

	int& operator[](int i)
	{
		return i == 0 ? x : (i == 1 ? y : z);
	}

	void Move(const Point& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	int En(void)
	{
		return abs(x) + abs(y) + abs(z);
	}
};

int Sign(int n)
{
	return n > 0 ? 1 : (n < 0) ? -1 : 0;
}
Point GetVel(const Point& p1, const Point& p2)
{
	return Point(Sign(p2.x - p1.x), Sign(p2.y - p1.y), Sign(p2.z - p1.z));
}
Point RVel(const Point& v)
{
	return Point(-v.x, -v.y, -v.z);
}

struct Moon
{
	Moon() : pos(), vel() {}
	Moon(const Point& p, const Point& v) : pos(p), vel(v) {}
	Point pos;
	Point vel;
};

struct State
{
	State() : x1(0), x2(0), x3(0),x4(0),dx1(0),dx2(0),dx3(0),dx4(0){}
	State(int _x1, int _x2, int _x3, int _x4, int _dx1, int _dx2, int _dx3, int _dx4) : x1(_x1), x2(_x2), x3(_x3), x4(_x4), dx1(_dx1), dx2(_dx2), dx3(_dx3), dx4(_dx4){}
	int x1;
	int x2;
	int x3;
	int x4;
	int dx1;
	int dx2;
	int dx3;
	int dx4;

	void MoveNext()
	{
		auto ix = beginx();
		auto idx = begindx();
		for (; ix != endx(); ++ix, ++idx)
			*ix += *idx;
		ix = beginx();
		idx = begindx();
		for (; ix != endx(); ++ix, ++idx)
		{
			auto ix2 = next(ix);
			auto idx2 = next(idx);
			for (; ix2 != endx(); ++ix2, ++idx2)
			{
				int d = Sign(*ix2 - *ix);
				*idx += d;
				*idx2 -= d;
			}
		}
	}

	int* beginx() { return &x1; }
	int* endx() { return &dx1; }
	int* begindx() { return &dx1; }
	int* enddx() { return &dx4 + 1; }

	const int * begin() const
	{ return &x1; }
	const int * end() const
	{ return &dx4 + 1;}
};
bool operator<(const State& s1, const State& s2)
{
	auto ix1 = s1.begin();
	auto ix2 = s2.begin();

	for (; ix1 != s1.end() && ix2 != s2.end(); ++ix1, ++ix2)
	{
		if (*ix1 < *ix2)
			return true;
		if (*ix1 != *ix2)
			return false;
	}
	return false;

/*	return
		s1.x1 < s2.x1 ||
		s1.x1 == s2.x1 && s1.x2 < s2.x2 ||
		s1.x1 == s2.x1 && s1.x2 == s2.x2 && s1.x3 < s2.x3 ||
		s1.x1 == s2.x1 && s1.x2 == s2.x2 && s1.x3 == s2.x3 && s1.x4 < s2.x4 ||
		s1.x1 == s2.x1 && s1.x2 == s2.x2 && s1.x3 == s2.x3 && s1.x4 == s2.x4 && s1.dx1 < s2.dx1 ||
		s1.x1 == s2.x1 && s1.x2 == s2.x2 && s1.x3 == s2.x3 && s1.x4 == s2.x4 && s1.dx1 == s2.dx1 && s1.dx2 < s2.dx2 ||
		s1.x1 == s2.x1 && s1.x2 == s2.x2 && s1.x3 == s2.x3 && s1.x4 == s2.x4 && s1.dx1 == s2.dx1 && s1.dx2 == s2.dx2 && s1.dx3 < s2.dx3 ||
		s1.x1 == s2.x1 && s1.x2 == s2.x2 && s1.x3 == s2.x3 && s1.x4 == s2.x4 && s1.dx1 == s2.dx1 && s1.dx2 == s2.dx2 && s1.dx3 == s2.dx3 && s1.dx4 < s2.dx4;
	*/
}

typedef vector<Moon> Moons;
int main()
{
	Moons input = {
		{ {14, 2, 8},{0,0,0}},
		{ {7, 4, 10},{0,0,0}},
		{ {1, 17, 16},{0,0,0}},
		{ {-4, -1, 1},{0,0,0}}
	};

	Moons moons = input;
	for (int n = 0; n < 1000; n++)
	{
		for (size_t i = 0; i < moons.size(); ++i)
		{
			for (size_t j = i + 1; j < moons.size(); j++)
			{
				auto& m1 = moons[i];
				auto& m2 = moons[j];
				auto v = GetVel(m1.pos, m2.pos);
				m1.vel.Move(v);
				m2.vel.Move(RVel(v));
			}
		}
		for (auto& m : moons)
			m.pos.Move(m.vel);
	}

	int e = 0;
	for (auto& m : moons)
		e += m.pos.En() * m.vel.En();
	cout << "Day12, answer1: " << e << endl;

	vector<int> shifts(3, 0);
	vector<long long> periods(3, 0);
	for (int c = 0; c < 3; ++c)
	{
		map<State, int> states;
		State s(input[0].pos[c], input[1].pos[c], input[2].pos[c], input[3].pos[c], 0, 0, 0, 0);
		for (; states.find(s) == states.end(); s.MoveNext())
		{
			states[s] = (int)states.size();
		}

		shifts[c] = states[s];
		periods[c] = states.size() - states[s];
		cout << c << ": shift: " << shifts[c] << ", period: " << periods[c] << endl;
	}

	cout << "Day12, answer2: " << *max_element(shifts.begin(), shifts.end()) + lcm(lcm(periods[0], periods[1]), periods[2]) << endl;
	return 0;
}
