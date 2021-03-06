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

int Solve_Find(const vector<NBot>& bots);

int main()
{
	ifstream is("d23.txt");
	istream_iterator<NBot> start(is), end;
	vector<NBot> bots(start, end);

	cout << "Read " << bots.size() << " nanobots" << endl;

	auto m = max_element(bots.begin(), bots.end(), [](auto b1, auto b2) {return b1.r < b2.r; });
	int r1 = (int)count_if(bots.begin(), bots.end(), bind(InRange, _1, *m));
	cout << "Day23 Answer1: " << r1 << endl;

	cout << "Day23 Answer2 (divide): " << Solve_Divide(bots) <<  endl;
	cout << "Day23 Answer2 (solve) : " << Solve_Find(bots) << endl;
	return 0;
}

template<typename T>
bool Eq(T t1, T t2)
{
	return t1 == t2;
}
bool Eq(double d1, double d2)
{
	return fabs(d1 - d2) < DBL_EPSILON;
}
template<typename T>
typename map<T, set<int>>::iterator FindOrCreate(map<T, set<int>>& tmap, T t)
{
	auto it = tmap.lower_bound(t);
	if (it == tmap.end() || !Eq(it->first, t))
		it = tmap.insert(it, make_pair(t, it != tmap.begin() ? prev(it)->second : set<int>()));
	return it;
}
template<typename T>
void AddRange(map<T, set<int>>& tmap, T cstart, T cend, int i)
{
	auto start = FindOrCreate(tmap, cstart);
	auto end = next(FindOrCreate(tmap, cend));
	for (; start != end; ++start)
		start->second.insert(i);
}

struct Point
{
	double x;
	double y;
};
struct Cube
{
	double x;
	double y;
	double r;
};
int GetIntersect(double x, double y, const vector<Cube>& cubes)
{
	return (int)count_if(cubes.begin(), cubes.end(), [x, y](auto& c)
	{
		return x > c.x - c.r - DBL_EPSILON && x < c.x + c.r + DBL_EPSILON
			&& y > c.y - c.r - DBL_EPSILON && y < c.y + c.r + DBL_EPSILON;
	});
}
int GetIntersect(const NBot& b, const vector<NBot>& bots)
{
	return (int)count_if(bots.begin(), bots.end(), [&](auto& c) { return InRange(b, c); });
}
int Solve_Find(const vector<NBot>& bots)
{
	const double sqrt2 = M_SQRT2;
	const double cos45 = cos(M_PI_4);
	int bestI = 0;
	vector<NBot> best;
	for (auto& bz : bots)
	{
		double z = bz.z - bz.r;
		for (int iz = 0; iz < 3; iz++, z += bz.r)
		{
			int countz = (int)count_if(bots.begin(), bots.end(), [z](auto& b) {return z >= b.z - b.r - DBL_EPSILON && z <= b.z + b.r + DBL_EPSILON; });
			if (countz < bestI)
				continue;
			cout << "z: " << fixed << setprecision(2) << z << " bestI: " << bestI << " potentially crossing: " << countz << endl;
			vector<Cube> cubes; cubes.reserve(bots.size());
			for (auto& by : bots)
			{
				if (z < by.z - by.r - DBL_EPSILON || z > by.z + by.r + DBL_EPSILON)
					continue;

				double x = cos45 * (by.x - by.y);
				double y = cos45 * (by.x + by.y);
				double r2 = M_SQRT1_2 * (by.r - fabs(by.z - z));
				cubes.push_back(Cube{ x, y, r2 });
			}
			for (auto& c : cubes)
			{
				for (int sx = -1; sx < 2; sx += 2)
				{
					for (int sy = -1; sy < 2; sy += 2)
					{
						double x = c.x + sx * c.r;
						double y = c.y + sy * c.r;
						int inter = GetIntersect(x, y, cubes);
						if (inter > bestI)
							best.clear();
						if (inter >= bestI)
						{
							bestI = inter;
							best.push_back(NBot{ (int)(cos45 * (x + y)), (int)(cos45 * (y - x)), (int)z, 0 });
						}
					}
				}
			}
		}
	}
	cout << bestI << " : " << best.size() << endl;
	sort(best.begin(), best.end(), [](auto& b1, auto& b2) {return Distance(b1, NBot{ 0,0,0,0 }) < Distance(b2, NBot{ 0,0,0,0 }); });
	NBot res = best.front();
	for (auto& p : best)
	{
		NBot start{ 0,0,0,0 };
		NBot end = p;
		vector<NBot> filtered; filtered.reserve(bots.size());
		copy_if(bots.begin(), bots.end(), back_inserter(filtered), [&](auto& bot) {return InRange(p, bot); });
		while (Distance(start, end) > 1)
		{
			for (int i = 0; i < 3; i++)
			{
				NBot cur = end;
				for (int j = 0; j < 3; ++j)
				{
					cur[j] = (end[j] + start[j]) / 2;
					if (all_of(filtered.begin(), filtered.end(), [&](auto& bot) {return InRange(cur, bot); }))
					{
						end[j] = cur[j];
					}
					else
					{
						start[j] = cur[j];
						cur[j] = end[j];
					}
				}
			}
		}
		cout << p << " => " << end << " Distance: " << Distance(end, NBot{ 0,0,0,0 }) << " best distance: " << Distance(res, NBot{ 0,0,0,0 }) << endl;
		if (Distance(end, NBot{ 0,0,0,0 }) < Distance(res, NBot{ 0,0,0,0 }))
			res = end;
	}
	return Distance(res, NBot{ 0,0,0,0 });
}
