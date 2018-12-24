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

int Distance(const NBot& b1, const NBot& b2, int scale = 1) { return abs(b1.x / scale - b2.x / scale) + abs(b1.y / scale - b2.y / scale) + abs(b1.z / scale - b2.z / scale); }
bool InRange(const NBot& b1, const NBot& b2, int scale = 1) {return Distance(b1, b2, scale) <= b2.r / scale; }
bool InRange2(const NBot&b1, const NBot& b2) {return Distance(b1, b2) < b1.r + b2.r; }

int Solve(const vector<NBot>& bots)
{
	auto mx = minmax_element(bots.begin(), bots.end(), [](auto& b1, auto& b2) {return b1.x < b2.x; });
	auto my = minmax_element(bots.begin(), bots.end(), [](auto& b1, auto& b2) {return b1.y < b2.y; });
	auto mz = minmax_element(bots.begin(), bots.end(), [](auto& b1, auto& b2) {return b1.z < b2.z; });

	int md = min(min(mx.second->x - mx.first->x, my.second->y - my.first->y), mz.second->z - mz.first->z);
	int scale = 1;
	for (; scale < md; scale *= 2);
	do
	{
		int maxInt = 2;
		NBot best;
		int bestD = md;
		for (int z = mz.first->z; z <= mz.second->z; z += scale)
			for (int y = my.first->y; y <= my.second->y; y += scale)
				for (int x = mx.first->x; x <= mx.second->x; x += scale)
				{ }

	} while (true);
	NBot start{ 0,0,0,0 };
	vector<NBot> res;
	res.reserve(1000);
	int maxIn = 2;
	for (; scale >= 1; scale /= 10)
	{
		res.clear();
		for (int z = mz.first->z / scale; z <= mz.second->z / scale; ++z)
		{
			for (int y = my.first->y / scale; y <= my.second->y / scale; ++y)
			{
				for (int x = mx.first->x / scale; x <= mx.second->x / scale; ++x)
				{
					int intr = 0;
					for (auto& b : bots)
					{
						if (InRange(NBot{ x * scale, y * scale, z * scale, 0 }, b, scale))
							intr++;
					}
				}
			}
		}
	}
	return 0;
}

int Solve4(const vector<NBot>& bots)
{
	vector<pair<NBot, vector<NBot>>> ranges; ranges.reserve(bots.size());
	for (auto b : bots)
	{
		ranges.push_back(make_pair(b, vector<NBot>()));
		vector<NBot>& in = ranges.back().second;
		copy_if(bots.begin(), bots.end(), inserter(in, in.begin()), bind(InRange2, _1, b));
		sort(in.begin(), in.end());
	}
	sort(ranges.begin(), ranges.end(), [](auto r1, auto r2) {return r1.second.size() < r2.second.size(); });
	ofstream os("d23.out");
	for (auto r : ranges)
	{
		os << r.first << " " << r.second.size() << endl;
	}
	return (int)ranges[0].second.size();
}
int Solve3(const vector<NBot>& bots)
{
	auto mx = minmax_element(bots.begin(), bots.end(), [](auto& b1, auto& b2) {return b1.x < b2.x; });
	auto my = minmax_element(bots.begin(), bots.end(), [](auto& b1, auto& b2) {return b1.y < b2.y; });
	auto mz = minmax_element(bots.begin(), bots.end(), [](auto& b1, auto& b2) {return b1.z < b2.z; });

	int md = max(max(mx.second->x - mx.first->x, my.second->y - my.first->y), mz.second->z - mz.first->z);
	int scale = 1;
	for (; md > 100; md /= 10) scale *= 10;
	NBot start{ 0,0,0,0 };
	vector<NBot> res;
	res.reserve(1000);
	int maxIn = 2;
	for (; scale >= 1; scale /= 10)
	{
		res.clear();
		for (int z = mz.first->z / scale; z <= mz.second->z / scale; ++z)
		{
			for (int y = my.first->y / scale; y <= my.second->y / scale; ++y)
			{
				for (int x = mx.first->x / scale; x <= mx.second->x / scale; ++x)
				{
					int intr = 0;
					for (auto& b : bots)
					{
						if (InRange(NBot{ x * scale, y * scale, z * scale, 0 }, b, scale))
							intr++;
					}
				}
			}
		}
	}
	return 0;
}

int Solve2(const vector<NBot>& bots)
{
	map<NBot, vector<NBot>> ranges;
	for (auto b : bots)
	{
		vector<NBot>& in = ranges[b];
		copy_if(bots.begin(), bots.end(), inserter(in, in.begin()), bind(InRange2, _1, b));
		sort(in.begin(), in.end());
	}
	int maxI = 2;
	vector<vector<NBot>> maxR;
	int  progress = 0;
	for (auto r : ranges)
	{
		if (r.second.size() < maxI)
			continue;

		for (int cur = (int)r.second.size(); cur >= maxI; --cur)
		{
			vector<int> bitMask(cur, 1);
			bitMask.resize(r.second.size(), 0);
			do
			{
				vector<NBot> intr = r.second;
				for (int i = 0; i < bitMask.size(); ++i)
				{
					if (bitMask[i] == 1)
					{
						vector<NBot> temp;
						vector<NBot>& r2 = ranges[r.second[i]];
						set_intersection(intr.begin(), intr.end(), r2.begin(), r2.end(), back_inserter(temp));
						if (temp.size() > maxI)
							swap(intr, temp);
					}
					if (intr.size() < maxI)
						break;
				}
				if (intr.size() > maxI)
				{
					maxI = (int)intr.size();
					maxR.clear();
				}
				if (intr.size() == maxI)
					maxR.push_back(intr);
			} while (prev_permutation(bitMask.begin(), bitMask.end()));
		}
		++progress;
		cout << "Processed " << progress << " bots, max intersections: " << maxI << ", same number of intersections sets: " << maxR.size() << endl;
	}

	cout << "Max intersections: " << maxI << " same number of intersections bots: " << maxR.size() << endl;
	return 0;
}
int main()
{
	ifstream is("d23.txt");
	istream_iterator<NBot> start(is), end;
	vector<NBot> bots(start, end);

	cout << "Read " << bots.size() << " nanobots" << endl;

	auto m = max_element(bots.begin(), bots.end(), [](auto b1, auto b2) {return b1.r < b2.r; });
	int r1 = (int)count_if(bots.begin(), bots.end(), bind(InRange, _1, *m, 1));
	cout << "Day23 Answer1: " << r1 << endl;

	cout << "Day23 Answer2: " << Solve(bots) <<  endl;
	return 0;
}
