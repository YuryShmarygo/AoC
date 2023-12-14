#include <execution>
#include "../common.h"

struct arrangement
{
	string s;
	vector<int> d;
};
ostream& operator<<(ostream& os, const arrangement& a) { os << a.s << " "; r::copy(a.d, ostream_iterator<int>(os, ",")); return os; }
istream& operator>>(istream& is, arrangement& a)
{
	a.s.clear();
	a.d.clear();
	string line;
	if (!getline(is, line) || line.empty()) return is;
	smatch m;
	if (regex_match(line, m, regex(R"(([^ ]+) (.*))")))
	{
		a.s = m[1].str();
		string numbers = m[2].str();
		smatch n;
		while (regex_search(numbers, n, regex(R"((\d+),?)")))
		{
			a.d.push_back(stoi(n[1].str()));
			numbers = n.suffix();
		}
	}
	return is;
}
struct comb
{
	//string::const_iterator its;
	//vector<int>::const_iterator itd;
	int s_pos;
	int d_pos;

	bool operator==(const comb&) const = default;
	auto operator<=>(const comb& point) const = default;
	bool operator<(const comb&) const = default;

	struct HashFunction
	{
		size_t operator()(const comb& c) const
		{
			return (c.d_pos << 6) + c.s_pos;
		}
	};
};
//bool operator<(const comb& c1, const comb& c2) { return distance(c1.its, c2.its) > 0 || distance(c1.its, c2.its) == 0 && distance(c1.itd, c2.itd) > 0; }
//bool operator==(const comb& c1, const comb& c2) { return distance(c1.its, c2.its) == 0 && distance(c1.itd, c2.itd) == 0; }
tuple<bool, string::const_iterator> match(string::const_iterator its, int n, const arrangement& a)
{
	for (; its != a.s.end() && n > 0 && (*its == '#' || *its == '?'); ++its, --n);
	return { n == 0 && (its == a.s.end() || *its == '.' || *its == '?'), its == a.s.end() ? its : its + 1 };
}
long long find_comb(const arrangement& a)
{
	long long res = 0;
	//list<comb> c; c.push_back({ a.s.begin(), a.d.begin() });
	//vector<comb> c; c.reserve(1000000); c.push_back({ a.s.begin(), a.d.begin() });
	vector<comb> c; c.reserve(1000000); c.push_back({ 0, 0 });
	unordered_map<comb, long long, comb::HashFunction> cache;
	while (!c.empty())
	{
		auto cur = c.back();
		c.erase(prev(c.end()));
		//if (cur.itd == a.d.end())
		if (cur.d_pos == a.d.size())
		{
			//if (find(cur.its, a.s.end(), '#') == a.s.end())
			if (find(a.s.begin() + cur.s_pos, a.s.end(), '#') == a.s.end())
				++res;
			continue;
		}
		//auto its = find_if(cur.its, a.s.end(), [](auto& c) {return c == '#' || c == '?'; });
		auto its = find_if(a.s.begin() + cur.s_pos, a.s.end(), [](auto& c) {return c == '#' || c == '?'; });
		if (its == a.s.end()) continue;
		if (*its == '?')
			c.push_back({ (int)(its - a.s.begin()) + 1, cur.d_pos });//c.push_back({ its + 1, cur.itd });
		//auto [matched, itn] = match(its, *cur.itd, a);
		auto [matched, itn] = match(its, a.d[cur.d_pos], a);
		if (matched)
			c.push_back({ int(itn - a.s.begin()), cur.d_pos + 1});//c.push_back({ itn, cur.itd + 1 });
	}
	//cout << a << " " << res << endl;
	return res;
}
long long find_comb_wcache(const arrangement& a)
{
	long long res = 0;
	//list<comb> c; c.push_back({ a.s.begin(), a.d.begin() });
	//vector<comb> c; c.reserve(1000000); c.push_back({ a.s.begin(), a.d.begin() });
	unordered_map<comb, long long, comb::HashFunction> cache;
	for (int d_pos = a.d.size() - 1; d_pos >= 0; --d_pos)
	{
		for (int s_pos = a.s.size() - 1; s_pos >= 0; --s_pos)
		{
			long long res = 0;
			vector<comb> c; c.reserve(1000000); c.push_back({ s_pos, d_pos });
			while (!c.empty())
			{
				auto cur = c.back();
				c.erase(prev(c.end()));
				auto itc = cache.find(cur);
				if (itc != cache.end())
				{
					res += itc->second;
					continue;
				}
				if (cur.d_pos == a.d.size())
				{
					if (find(a.s.begin() + cur.s_pos, a.s.end(), '#') == a.s.end())
					{
						++res;
						cache[cur] = 1;
					}
					else
						cache[cur] = 0;
					continue;
				}
				auto its = find_if(a.s.begin() + cur.s_pos, a.s.end(), [](auto& c) {return c == '#' || c == '?'; });
				if (its == a.s.end()) continue;
				auto [matched, itn] = match(its, a.d[cur.d_pos], a);
				if (matched)
					c.push_back({ int(itn - a.s.begin()), cur.d_pos + 1 });//c.push_back({ itn, cur.itd + 1 });
				if (*its == '?')
					c.push_back({ (int)(its - a.s.begin()) + 1, cur.d_pos });//c.push_back({ its + 1, cur.itd });
			}
			cache[{s_pos, d_pos}] = res;
		}
	}
	return cache[{0, 0}];
}

long long solve1(const vector<arrangement>& input)
{
	long long res = 0;
	int i = 0;
	//*
	for (auto& a : input)
	{
		auto t_start = high_resolution_clock::now();
		long long cur = find_comb(a);
		res += cur;
		cout << i << ": " << cur << " took: " << ElapsedMs(t_start)  << "    " << a << endl;
		cur = find_comb_wcache(a);
		cout << i << ": with cache " << cur << " took: " << ElapsedMs(t_start) << "    " << a << endl;
		++i;
	}
	/*/
	for_each(execution::par, input.begin(), input.end(), [&](auto& a) 
		{
			auto t_start = high_resolution_clock::now();
			long long cur = find_comb(a);
			res += cur;
			cout << cur << " took: " << ElapsedMs(t_start) << "    " << a << endl;
		});
		//*/
	return res;
}
vector<arrangement> unfold(const vector<arrangement>& input)
{
	vector<arrangement> res; res.reserve(input.size());
	for (auto& a : input)
	{
		res.push_back(a);
		for (int i = 0; i < 4; ++i)
		{
			res.back().s += "?" + a.s;
			res.back().d.insert(res.back().d.end(), a.d.begin(), a.d.end());
		}
	}
	return res;
}
long long solve2(const vector<arrangement>& input)
{
	return solve1(unfold(input));
}
int main()
{
	test(); return 0;
	ifstream is("Day12.txt");
	istream_iterator<arrangement> start(is), end;
	vector<arrangement> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day12 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day12 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	string test1 = R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1)";

	auto is = stringstream(test1);
	istream_iterator<arrangement> start(is), end;
	vector<arrangement> input(start, end);

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;

	string test2 = R"(?###???????? 3,2,1
?.?????#???#? 1,1,2,2
??????????.?.?. 1,1)";
	is = stringstream(test2);
	start = istream_iterator<arrangement>(is);
	input = vector<arrangement>(start, end);
	cout << "Test3: " << solve2(input) << endl;
}
