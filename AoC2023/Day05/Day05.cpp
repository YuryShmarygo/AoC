#include "../common.h"

struct srange
{
	long long s_start;
	long long s_end;
	long long d_start;
	long long d_end;
	bool is_in(long long n) { return n >= s_start && n <= s_end; }
	long long convert(long long n) { return d_start + n - s_start; }
};
istream& operator>>(istream& is, srange& r) 
{
	long long d, s, c;
	is >> d >> s >> c;
	r.s_start = s;
	r.s_end = s + c - 1;
	r.d_start = d;
	r.d_end = d + c - 1;
	return is;
}
ostream& operator<<(ostream& os, const srange& r) { return os << '[' << r.s_start << " - " << r.s_end << "] ==> [" << r.d_start << " - " << r.d_end << ']'; }
ostream& operator<<(ostream& os, const vector<srange>& r) { r::copy(r, ostream_iterator<srange>(os, "\n")); return os; }
auto less_s = [](const srange& a, const srange& b) { return a.s_start < b.s_start; };
struct smap
{
	string s;
	string d;
	vector<srange> r;
	long long convert(long long n)
	{
		for (auto it = r.begin(); it != r.end(); ++it)
			if (it->is_in(n))
				return it->convert(n);
		return n;
	}
};
istream& operator>>(istream& is, smap& m)
{
	m.r.clear();
	string line;
	if (!getline(is, line) || line.empty()) return is;
	smatch s;
	if (regex_match(line, s, regex(R"(([^-]+)-to-([^-]+) map:)")))
	{
		m.s = s[1].str();
		m.d = s[2].str();
	}
	while (getline(is, line) && !line.empty())
	{
		stringstream iss(line);
		srange r;
		iss >> r;
		m.r.push_back(r);
	}
	r::sort(m.r, less_s);
	return is;
}
struct almanac
{
	vector<long long> seeds;
	vector<smap> maps;
	map<string, smap*> link;
};
istream& operator>>(istream& is, almanac& a)
{
	string line;
	getline(is, line);
	stringstream iis(line.substr(6));
	istream_iterator<long long> start(iis), end;
	a.seeds = vector<long long>(start, end);
	a.maps.reserve(20);
	getline(is, line);
	while (!is.eof())
	{
		smap m;
		is >> m;
		a.maps.push_back(m);
		a.link[m.s] = &(*prev(a.maps.end()));
	}
	return is;
}

long long solve1(const almanac& a)
{
	long long res = LLONG_MAX;
	for (auto s : a.seeds)
	{
		string name = "seed";
		long long val = s;
		while (name != "location")
		{
			val = a.link.find(name)->second->convert(val);
			name = a.link.find(name)->second->d;
		}
		if (val < res)
			res = val;
	}
	return res;
}
long long solve2(const almanac& a)
{
	vector<srange> seeds;
	for (int i = 0; i < a.seeds.size() - 1; i += 2)
		seeds.push_back({ a.seeds[i],a.seeds[i] + a.seeds[i + 1]-1, 0, 0 });
	r::sort(seeds, less_s);
	string name = "seed";
	while (name != "location")
	{
		auto conv = a.link.find(name)->second;
		vector<srange> temp; temp.reserve(seeds.size() + seeds.size());
//		cout << name << endl;
//		cout << "seeds" << endl << seeds << endl;
//		cout << "conv" << endl << conv->r << endl;
		for (auto& seed : seeds)
		{
			long long cur_start = seed.s_start;
			auto it = conv->r.begin();
			while (cur_start < seed.s_end)
			{
				while (it != conv->r.end() && cur_start > it->s_end) ++it;
				if (it == conv->r.end())
				{
//					cout << "seed start after last conv, adding tail [cur_start - seed.send]: [" << cur_start << " - " << seed.s_end << "] => [" << cur_start << " - " << seed.s_end << "]" << endl;
					temp.push_back({ cur_start, seed.s_end, 0, 0 });
					break;
				}
				if (seed.s_end < it->s_start)
				{
//					cout << "seed end is before conv start, adding prefix [cur_start - seed.s_end]: [" << cur_start << " - " << seed.s_end << "] => [" << cur_start << " - " << seed.s_end << "], cur conv: " << *it << endl;
					temp.push_back({ cur_start, seed.s_end, 0, 0 });
					break;
				}
				if (it->s_start > cur_start)
				{
//					cout << "seed start before conv start, adding prefix [cur_start - it->s_start - 1]: [" << cur_start << " - " << it->s_start - 1 << "] => [" << cur_start << " - " << it->s_start - 1 << "], cur conv : " << *it << endl;
					temp.push_back({ cur_start, it->s_start - 1, 0, 0 });
					cur_start = it->s_start;
				}
				if (seed.s_end <= it->s_end)
				{
					long long start_offset = cur_start - it->s_start;
					long long end_offset = start_offset + seed.s_end - cur_start;
					temp.push_back({ it->d_start + start_offset, it->d_start + end_offset, 0,0 });
//					cout << "converting [cur_start - seed.s_end]: [" << cur_start << " - " << seed.s_end << "] => [" << it->d_start + start_offset << " - " << it->d_start + end_offset << "], cur conv : " << *it << endl;
					break;
				}
				else
				{
					long long start_offset = cur_start - it->s_start;
					temp.push_back({ it->d_start + start_offset, it->d_end, 0, 0 });
//					cout << "converting till end of conv and continue [cur_start - it->s_end]: [" << cur_start << " - " << it->s_end << "] => [" << it->d_start + start_offset << " - " << it->d_end << "], cur conv : " << *it << endl;
					cur_start = it->s_end + 1;
				}
			}
		}
		name = conv->d;
		seeds = temp;
		r::sort(seeds, less_s);
	}
//	cout << "seeds" << endl << seeds << endl;
	return seeds[0].s_start;
}
int main()
{
	test();
	ifstream is("Day05.txt");
	almanac a;
	is >> a;

	auto t_start = high_resolution_clock::now();
	cout << "Day5 Answer1: " << solve1(a) << " took: " << ElapsedMs(t_start) << endl;
	t_start = high_resolution_clock::now();
	cout << "Day5 Answer2: " << solve2(a) << " took: " << ElapsedMs(t_start) << endl;
	//183212530 too high
}

void test()
{
	auto test1 = R"(seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4)";

	auto is = stringstream(test1);
	almanac a;
	is >> a;
	cout << "Test1: " << solve1(a) << endl;
	cout << "Test2: " << solve2(a) << endl;
}
