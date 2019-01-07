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

int Distance(const NBot& b1, const NBot& b2) { return abs(b1.x - b2.x) + abs(b1.y - b2.y) + abs(b1.z - b2.z); }
bool InRange(const NBot& b1, const NBot& b2) {return Distance(b1, b2) <= b2.r; }
bool InRange2(const NBot&b1, const NBot& b2) {return Distance(b1, b2) < b1.r + b2.r; }

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

int Solve(const vector<NBot>& bots)
{
	typedef map<int, set<int>> CMap;
	CMap zmap;
	for (int i = 0; i < bots.size(); ++i)
	{
		const NBot& b = bots[i];
		AddRange(zmap, b.z - b.r, b.z, i);
		AddRange(zmap, b.z, b.z + b.r, i);
	}
	typedef vector<CMap::iterator> IMap;
	IMap imap; imap.reserve(zmap.size());
	for (auto it = zmap.begin(); it != zmap.end(); ++it)
		imap.push_back(it);
	sort(imap.begin(), imap.end(), [](auto i1, auto i2) {return i1->second.size() > i2->second.size(); });
	double cos45 = cos(M_PI / 4);
	double sin45 = sin(M_PI / 4);
	int bestI = 0;
	vector<int> bestXYZ;
	for (auto it = imap.begin(); it != imap.end() && (*it)->second.size() >= bestI; ++it)
	{
		int curI = 0;
		typedef map<double, set<int>> DMap;
		DMap xmap, ymap;
		for (auto i : (*it)->second)
		{
			auto& b = bots[i];
			double x = cos45 * b.x - sin45 * b.y;
			double y = sin45 * b.x + cos45 * b.y;
			double r = (b.r - abs((*it)->first - b.z)) / sqrt(2);
			AddRange(xmap, x - r, x + r, i);
			AddRange(ymap, y - r, y + r, i);
		}
		typedef vector<DMap::iterator> IDMap;
		IDMap ixmap, iymap; ixmap.reserve(xmap.size()); iymap.reserve(ymap.size());
		for (auto it = xmap.begin(); it != xmap.end(); ++it)
			ixmap.push_back(it);
		for (auto it = ymap.begin(); it != ymap.end(); ++it)
			iymap.push_back(it);
		sort(ixmap.begin(), ixmap.end(), [](auto i1, auto i2) {return i1->second.size() > i2->second.size(); });
		sort(iymap.begin(), iymap.end(), [](auto i1, auto i2) {return i1->second.size() > i2->second.size(); });
		for (auto itx = ixmap.begin(); itx != ixmap.end() && (*itx)->second.size() >= bestI; ++itx)
		{
			for (auto ity = iymap.begin(); ity != iymap.end() && (*ity)->second.size() >= bestI; ++ity)
			{

			}
		}
	}
}


struct Range
{
	int x1;
	int x2;
	set<int> a;
};
bool GetNextComb(int& cur, const vector<int>& b, NBot& c)
{
	int base = *max_element(b.begin(), b.end())+1;
	for (; cur <= pow(base, 3) - 1; ++cur)
	{
		int x = cur / base / base;
		if (x > b[0]) continue;
		int y = (cur - x * base * base) / base;
		if (y > b[1]) continue;
		int z = cur - x * base * base - y * base;
		if (z > b[2]) continue;
		if (x < base-1 && y < base-1 && z < base-1) continue;
		c = { x, y, z, 0 };
		++cur;
		return true;
	}
	return false;
}

int Solve4(const vector<NBot>& bots)
{
	typedef map<int, set<int>> IMap;
	IMap imap;
	typedef vector<pair<int, set<int>>> CMap;
	CMap cmap(bots.size());
	for (int i = 0; i < bots.size(); ++i)
	{
		const NBot& b = bots[i];
		cmap[i].first = i;
		for (int j = 0; j < bots.size(); ++j)
		{
			if (InRange2(b, bots[j]))
			{
				cmap[i].second.insert(j);
				imap[i].insert(j);
			}
		}
	}
	sort(cmap.begin(), cmap.end(), [](auto& b1, auto& b2) { return b1.second.size() > b2.second.size(); });
	vector<set<int>> best(1);
	for (int i = 0; i < cmap.size() && cmap[i].second.size() >= best[0].size(); i++)
	{
		cout << "i: " << i << ", cur size: " << cmap[i].second.size() << endl;
		vector<int> local(cmap[i].second.begin(), cmap[i].second.end());
		sort(local.begin(), local.end(), [&](auto a, auto b) {return imap[a].size() < imap[b].size(); });
		for (int j = 0; j < local.size(); ++j)
		{
			if (imap[local[j]].size() < best[0].size())
				continue;
			cout << "  j: " << j << ", cur size: " << imap[local[j]].size();
			int real_inter = 0;
			set<int> inter = imap[local[j]];
			for (auto it = imap[local[j]].begin(); it != imap[local[j]].end() && inter.size() >= best[0].size(); ++it)
			{
				set<int> tmp;
				set_intersection(inter.begin(), inter.end(), imap[*it].begin(), imap[*it].end(), inserter(tmp, tmp.end()));
				swap(inter, tmp);
			}
			cout << " real intersections: " << real_inter << endl;
			if (inter.size() == best[0].size())
				best.push_back(inter);
			else if (inter.size() > best[0].size())
				best = { inter };
		}
	}
	cout << "found " << best.size() << " bests of " << best[0].size() << " size each" << endl;

	return 0;
}

int Solve3(const vector<NBot>& bots)
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
		//cout << "n: " << n << " res: " << res;
		int cur = 0;
		vector<int> baseR(3);
		for (int t = 0; t < 3; ++t)
		{
			int i = 0;
			for (; i <= n && ranges[t][i].a.size() > res; ++i)
				;
			baseR[t] = i-1;
		}
		//cout << " ranges[0][" << baseR[0] << "].a.size() = " << ranges[0][baseR[0]].a.size() << " ranges[1][" << baseR[1] << "].a.size() = " << ranges[1][baseR[1]].a.size() << " ranges[2][" << baseR[2] << "].a.size() = " << ranges[2][baseR[2]].a.size() << endl;
		NBot c;
		for (;GetNextComb(cur, baseR, c);)
		{
			//cout << " comb: " << c.x << ", " << c.y << ", " << c.z;
			auto& s1 = ranges[0][c.x];
			auto& s2 = ranges[1][c.y];
			auto& s3 = ranges[2][c.z];
			set<int> tmp;
			set_intersection(s1.a.begin(), s1.a.end(), s2.a.begin(), s2.a.end(), inserter(tmp, tmp.end()));
			if (tmp.size() <= res) continue;
			set<int> all;
			set_intersection(tmp.begin(), tmp.end(), s3.a.begin(), s3.a.end(), inserter(all, all.end()));
			// check if all really intersect
			auto count_inter = [&](int i) {return count_if(all.begin(), all.end(), [&](int j) {return InRange(bots[i], bots[j]); }); };
			auto comp_inter = [&](int i, int j) {return count_inter(i) < count_inter(j); };
			auto min_inter = min_element(all.begin(), all.end(), comp_inter);
			while (count_inter(*min_inter) != all.size())
			{
				all.erase(min_inter);
				min_inter = min_element(all.begin(), all.end(), comp_inter);
			}
			/*
			vector<NBot> inter; inter.reserve(all.size());
			transform(all.begin(), all.end(), back_inserter(inter), [&](auto i) {return bots[i]; });
			auto count_inter = [&](auto& b) {return count_if(inter.begin(), inter.end(), bind(InRange, b, _1)); };
			sort(inter.begin(), inter.end(), [&](auto& b1, auto& b2) {return count_inter(b1) > count_inter(b2); });
			*/

			if (res < all.size())
			{
				res = (int)all.size();
				best = NBot{ s1.x1, s2.x1, s3.x1, 0 };
				bestR = { s1, s2, s3 };
				bestB = all;
				for (int t = 0; t < 3; ++t)
				{
					int i = 0;
					for (; i <= n && ranges[t][i].a.size() > res; ++i)
						;
					baseR[t] = i-1;
					for (; i <= ranges[t].size() && ranges[t][i].a.size() > res; ++i)
						;
					maxN = t == 0 ? i : max(maxN, i);
				}
				cout << endl << "res: " << res << " maxN: " << maxN << endl;
			}
		}
		//cout << endl;
	}
	cout << best.x << "," << best.y << "," << best.z << endl;
	cout << "Best range x:[" << bestR[0].x1 << ", " << bestR[0].x2 << "], y:[" << bestR[1].x1 << ", " << bestR[1].x2 << "], z:[" << bestR[2].x1 << ", " << bestR[2].x2 << "]" << endl;

	cout << "In range: " << bestB.size() << ", best in range: " << count_if(bestB.begin(), bestB.end(), [&](auto i) {return InRange(NBot{ bestR[0].x1, bestR[1].x1, bestR[2].x1, 0 }, bots[i]); }) << endl;

	

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
