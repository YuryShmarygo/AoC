#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
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
#include <unordered_map>
#include <queue>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace placeholders;


struct NBot
{
	int x;
	int y;
	int z;
	int r;
	int operator[](int i) const
	{ return (&x)[i]; }
	int& operator[](int i)
	{ return (&x)[i]; }
	static NBot Empty;
};
NBot NBot::Empty = { 0,0,0,-1 };
NBot operator|(const NBot& b1, const NBot& b2)
{
	if (b1.z - b1.r < b2.z || b1.z + b1.r > b2.z)
		return NBot::Empty;
	return NBot{ b1.x, b1.y, b2.z, b1.r - abs(b2.z - b1.z) };
}
bool operator<(const NBot& b1, const NBot& b2)
{
	return b1.x < b2.x 
		|| b1.x == b2.x && b1.y < b2.y 
		|| b1.x == b2.x && b1.y == b2.y && b1.z < b2.z 
		|| b1.x == b2.x && b1.y == b2.y && b1.z == b2.z && b1.r < b2.r;
}
istream& operator>>(istream& i, NBot& b)
{
	string line;
	getline(i, line);
	sscanf_s(line.c_str(), "pos=<%d,%d,%d>, r=%d", &b.x, &b.y, &b.z, &b.r);
	return i;
}
ostream& operator<<(ostream& o, const NBot& b)
{
	o << "(" << b.x << "," << b.y << "," << b.z << ")";
	return o;
}
struct Box
{
	int x;
	int y;
	int z;
	int r;
	int n;
	int d;
	int operator[](int i) const
	{ return ((int*)&x)[i]; }
	int& operator[](int i)
	{ return ((int*)&x)[i]; }
};
bool operator<(const Box& b1, const Box& b2)
{
	return b1.n <  b2.n 
		|| b1.n == b2.n && b1.d >  b2.d
		|| b1.n == b2.n && b1.d == b2.d && b1.r > b2.r;
}
ostream& operator<<(ostream& o, const Box& b)
{
	o << b.n << ": (" << b.x << "," << b.y << "," << b.z << "," << b.r << "): " << b.d;
	return o;
}
int Distance(const NBot& b1, const NBot& b2) { return abs(b1.x - b2.x) + abs(b1.y - b2.y) + abs(b1.z - b2.z); }
bool InRange(const NBot& b1, const NBot& b2) {return Distance(b1, b2) <= b2.r; }
bool InRange2(const NBot&b1, const NBot& b2) {return Distance(b1, b2) < b1.r + b2.r; }

int RangeDist(int c, int l, int r)
{
	return c < l ? l - c : c > r ? c - r : 0;
}
int BoxDist(const array<int, 3>& c1, const array<int, 3>& c2, const array<int, 3>& p)
{
	int d = 0;
	for (int i = 0; i < 3; ++i)
		d += RangeDist(p[i], c1[i], c2[i]);
	return d;
}
Box SplitBox(const Box& box, int offset, const vector<NBot>& bots)
{
	int r = box.r / 2;
	array<int, 3> c1, c2;
	for (int i = 0; i < 3; ++i)
	{
		c1[i] = box[i] + ((offset >> i) & 1) * r;
		c2[i] = c1[i] + r - 1;
	}
	int n = 0;
	for (auto& bot : bots)
	{
		int d = 0;
		for (int i = 0; i < 3; ++i)
			d += RangeDist(bot[i], c1[i], c2[i]);
		if (d <= bot.r) n++;
	}
	int d = BoxDist(c1, c2, array<int, 3>{0, 0, 0});
	return Box{ c1[0], c1[1], c1[2], r, n, d };
}
int Solve_Divide(const vector<NBot>& bots)
{
	auto nDimLess = [](const NBot& b1, const NBot& b2, int n) { return b1[n] < b2[n]; };
	auto xRange = minmax_element(bots.begin(), bots.end(), bind(nDimLess, _1, _2, 0));
	auto yRange = minmax_element(bots.begin(), bots.end(), bind(nDimLess, _1, _2, 1));
	auto zRange = minmax_element(bots.begin(), bots.end(), bind(nDimLess, _1, _2, 2));
	int xs = 2 * max(abs(xRange.second->x), abs(xRange.first->x));
	int ys = 2 * max(abs(yRange.second->y), abs(yRange.first->y));
	int zs = 2 * max(abs(zRange.second->z), abs(zRange.first->z));

	int dist = (int)log2(max(max(xs, ys), zs)) + 1;
	//cout << dist << " " << pow(2, dist) << " " << xs << " " << ys << " " << zs << endl;
	int r = (int)pow(2, dist);
	priority_queue<Box> queue;
	queue.push(Box{ -r/2, -r/2, -r/2,r,(int)bots.size(), 3 * r / 2 });
	while (queue.top().r != 1)
	{
		auto b = queue.top();
		queue.pop();
		for (int c = 0; c < 8; c++)
		{
			queue.push(SplitBox(b, c, bots));
		}
	}
	cout << queue.top() << endl;
	return queue.top().d;
}


int main()
{
	ifstream is("d23.txt");
	istream_iterator<NBot> start(is), end;
	vector<NBot> bots(start, end);

	cout << "Read " << bots.size() << " nanobots" << endl;

	auto m = max_element(bots.begin(), bots.end(), [](auto b1, auto b2) {return b1.r < b2.r; });
	int r1 = (int)count_if(bots.begin(), bots.end(), bind(InRange, _1, *m));
	cout << "Day23 Answer1: " << r1 << endl;

	cout << "Day23 Answer2: " << Solve_Divide(bots) <<  endl;
	return 0;
}
