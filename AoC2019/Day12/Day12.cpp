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

typedef vector<Moon> Moons;
int main()
{
	Moons moons = {
		{ {14, 2, 8},{0,0,0}},
		{ {7, 4, 10},{0,0,0}},
		{ {1, 17, 16},{0,0,0}},
		{ {-4, -1, 1},{0,0,0}}
	};

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
	return 0;
}
