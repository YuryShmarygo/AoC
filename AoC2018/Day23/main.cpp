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
#include <unordered_map>

using namespace std;
using namespace placeholders;

struct NBot
{
	int x;
	int y;
	int z;
	int r;
};
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

int Distance(const NBot& b1, const NBot& b2) { return abs(b1.x - b2.x) + abs(b1.y - b2.y) + abs(b1.z - b2.z); }
bool InRange(const NBot& b1, const NBot& b2) {return Distance(b1, b2) <= b2.r; }
bool InRange2(const NBot&b1, const NBot& b2) {return Distance(b1, b2) < b1.r + b2.r; }

struct Range
{
	int x1;
	int x2;
	set<int> a;
};
bool GetNextComb(int& cur, const vector<int>& b, NBot& c)
{
	int base = *max_element(b.begin(), b.end()) + 1;
	for (; cur <= pow(base, 3) - 1; ++cur)
	{
		int z = cur % base;
		if (z > b[2]) continue;
		int y = cur / base;
		if (y > b[1]) continue;
		int x = cur / base / base;
		if (x > b[0]) continue;
		if (x < base && y < base && z < base) continue;
		c = { x, y, z, 0 };
		++cur;
		return true;
	}
	return false;
}

int Solve(const vector<NBot>& bots)
{
	typedef map<int, set<int>> CMap;
	vector<CMap> xyz(3, CMap());
	for (int i = 0; i < bots.size(); ++i)
	{
		const NBot& b = bots[i];
		for (int t = 0; t < 3; ++t)
		{
			int xs = t == 0 ? b.x - b.r : t == 1 ? b.y - b.r : b.z - b.r;
			int xe = t == 0 ? b.x + b.r : t == 1 ? b.y + b.r : b.z + b.r;

			auto& x = xyz[t];
			auto is = x.lower_bound(xs);
			if (is == x.end() || is->first != xs)
				is = x.insert(is, make_pair(xs, is != x.begin() ? prev(is)->second : set<int>()));
			auto ie = x.lower_bound(xe);
			if (ie == x.end() || ie->first != xe)
				ie = x.insert(ie, make_pair(xe, prev(ie)->second));

			for (; is != ie; ++is)
				is->second.insert(i);
			ie->second.erase(i);
		}
	}
	vector<vector<Range>> ranges(3, vector<Range>());
	for (int t = 0; t < 3; ++t)
	{
		ranges[t].reserve(2001);
		for (auto it = xyz[t].begin(); it != xyz[t].end(); ++it)
		{
			auto itn = next(it);
			if (itn != xyz[t].end())
				ranges[t].push_back(Range{ it->first, itn->first, it->second });
		}
		sort(ranges[t].begin(), ranges[t].end(), [](auto& r1, auto& r2) {return r1.a.size() > r2.a.size(); });
	}

	int res = 0;
	NBot best{ 0,0,0,0 };
	vector<Range> bestR;
	set<int> bestB;
	int maxN = (int)min(min(ranges[0].size(), ranges[1].size()), ranges[2].size());

	for (int n = 0; n <= maxN; ++n)
	{
		cout << "n: " << n << " res: " << res;
		int cur = 0;
		vector<int> baseR(3);
		for (int t = 0; t < 3; ++t)
		{
			int i = 0;
			for (; i <= n && ranges[t].size() >= res; ++i)
				;
			baseR[t] = i;
		}
		cout << " baseR[0]: " << baseR[0] << " baseR[1]: " << baseR[1] << " baseR[2]: " << baseR[2] << endl;
		NBot c;
		for (;GetNextComb(cur, baseR, c);)
		{
			auto& s1 = ranges[0][c.x];
			auto& s2 = ranges[1][c.y];
			auto& s3 = ranges[2][c.z];
			set<int> tmp;
			set_intersection(s1.a.begin(), s1.a.end(), s2.a.begin(), s2.a.end(), inserter(tmp, tmp.end()));
			if (tmp.size() <= res) continue;
			set<int> all;
			set_intersection(tmp.begin(), tmp.end(), s3.a.begin(), s3.a.end(), inserter(all, all.end()));
			if (res < all.size())
			{
				res = (int)all.size();
				best = NBot{ s1.x1, s2.x1, s3.x1, 0 };
				bestR = { s1, s2, s3 };
				bestB = all;
				for (int t = 0; t < 3; ++t)
				{
					int i = 0;
					for (; i <= n && ranges[t].size() >= res; ++i)
						;
					baseR[t] = i;
				}
			}
		}
	}
	cout << best.x << "," << best.y << "," << best.z << endl;
	int bestD = abs(best.x) + abs(best.y) + abs(best.z);

	/*
	for (int x = bestR[0].x1; x <= bestR[0].x2; x++)
		for (int y = bestR[1].x1; y <= bestR[1].x2; y++)
			for (int z = bestR[2].x1; z <= bestR[2].x2; z++)
				if (abs(x) + abs(y) + abs(z) < bestD && all_of(bestB.begin(), bestB.end(), [&](auto bid) { return InRange(NBot{ x, y, z, 0 }, bots[bid]); }))
					bestD = abs(x) + abs(y) + abs(z);
					*/
	return bestD;
}

int Solve2(const vector<NBot>& bots)
{
	typedef map<int, set<int>> CMap;
	vector<CMap> xyz(3, CMap());
	for (int i = 0; i < bots.size(); ++i)
	{
		const NBot& b = bots[i];
		for (int t = 0; t < 3; ++t)
		{
			int xs = t == 0 ? b.x - b.r : t == 1 ? b.y - b.r : b.z - b.r;
			int xe = t == 0 ? b.x + b.r : t == 1 ? b.y + b.r : b.z + b.r;

			auto& x = xyz[t];
			auto is = x.lower_bound(xs);
			if (is == x.end() || is->first != xs)
				is = x.insert(is, make_pair(xs, is != x.begin() ? prev(is)->second : set<int>()));
			auto ie = x.lower_bound(xe);
			if (ie == x.end() || ie->first != xe)
				ie = x.insert(ie, make_pair(xe, prev(ie)->second));

			for (; is != ie; ++is)
				is->second.insert(i);
			ie->second.erase(i);
		}
	}
	vector<vector<Range>> ranges(3, vector<Range>());
	for (int t = 0; t < 3; ++t)
	{
		ranges[t].reserve(2001);
		for (auto it = xyz[t].begin(); it != xyz[t].end(); ++it)
		{
			auto itn = next(it);
			if (itn != xyz[t].end())
				ranges[t].push_back(Range{ it->first, itn->first, it->second });
		}
		sort(ranges[t].begin(), ranges[t].end(), [](auto& r1, auto& r2) {return r1.a.size() > r2.a.size(); });
	}

	int res = 0;
	NBot best{ 0,0,0,0 };
	vector<Range> bestR;
	set<int> bestB;
	for (int n = 0; n < max(max(ranges[0].size(), ranges[1].size()), ranges[2].size()); ++n)
	{
		if (all_of(ranges.begin(), ranges.end(), [&](auto& r) {return r[n < r.size() ? n : r.size() - 1].a.size() < res; }))
			break;
		for (int i = 0; i <= n; ++i)
		{
			for (int j = 0; j <= n; ++j)
			{
				for (int k = 0; k <= n; ++k)
				{
					auto& s1 = ranges[0][i];
					auto& s2 = ranges[1][j];
					auto& s3 = ranges[2][k];
					set<int> tmp;
					set_intersection(s1.a.begin(), s1.a.end(), s2.a.begin(), s2.a.end(), inserter(tmp, tmp.end()));
					set<int> all;
					set_intersection(tmp.begin(), tmp.end(), s3.a.begin(), s3.a.end(), inserter(all, all.end()));
					if (res < all.size())
					{
						res = (int)all.size();
						best = NBot{ s1.x1, s2.x1, s3.x1, 0 };
						bestR = { s1, s2, s3 };
						bestB = all;
					}
				}
			}
		}
	}
	//cout << best.x << "," << best.y << "," << best.z << endl;
	int bestD = 999999999;
	int n = 0;
	for (int n = 0; n <= max(max(bestR[0].x2 - bestR[0].x1, bestR[1].x2 - bestR[1].x1), bestR[2].x2 - bestR[2].x1); ++n)
	{

		int x = bestR[0].x1 + n;
		int y = bestR[1].x1 + n;
		int z = bestR[2].x1 + n;
		if (all_of(bestB.begin(), bestB.end(), [&](auto bid) { return InRange(NBot{ x, y, z, 0 }, bots[bid]); }))
			break;
	}
	for (int x = bestR[0].x1; x <= bestR[0].x2; x++)
		for (int y = bestR[1].x1; y <= bestR[1].x2; y++)
			for (int z = bestR[2].x1; z <= bestR[2].x2; z++)
				if (abs(x) + abs(y) + abs(z) < bestD && all_of(bestB.begin(), bestB.end(), [&](auto bid) { return InRange(NBot{ x, y, z, 0 }, bots[bid]); }))
					bestD = abs(x) + abs(y) + abs(z);
	return bestD;
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

	cout << "Day23 Answer2: " << Solve(bots) <<  endl;
	return 0;
}
