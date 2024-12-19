#include "../common.h"

struct input_t
{
	vector<string> towels;
	vector<string> designs;
};
vector<string> read_towels(string line)
{
	vector<string> res;
	smatch m;
	while (regex_search(line, m, regex(R"(([^, ]+))")))
	{
		res.push_back(m[1].str());
		line = m.suffix();
	}
	return res;
}
input_t read_input(istream& is)
{
	input_t res;
	string line;
	getline(is, line);
	res.towels = read_towels(line);
	while (getline(is, line))
	{
		if (!line.empty())
			res.designs.push_back(line);
	}
	return res;
}
string make_reg(const vector<string>& patterns)
{
	string str = "^(?:";
	for (auto s : patterns)
	{
		if (str.size() > 4)
			str.append("|");
		str.append(s);
	}
	str.append(")+$");
	return str;
}
vector<string> get_matching(const input_t& input)
{
	vector<string> towels;
	for (auto it = input.towels.begin(); it != input.towels.end(); ++it)
	{
		vector<string> patterns(input.towels.begin(), it);
		patterns.insert(patterns.end(), next(it), input.towels.end());
		smatch m;
		if (!regex_match(*it, m, regex(make_reg(patterns))))
			towels.push_back(*it);
	}

	auto reg = regex(make_reg(towels));
	vector<string> res;
	for (auto d : input.designs)
	{
		smatch m;
		if (regex_match(d, m, reg))
			res.push_back(d);
	}
	return res;
}
long long solve1(const input_t& input)
{
	return get_matching(input).size();
}

long long count(string design, const input_t& input)
{
	map<string, long long> cache;
	set<string> patterns;
	int max_p = 0;
	for (auto t : input.towels)
	{
		patterns.insert(t);
		max_p = max(max_p, (int)t.size());
	}
	long long res = 0;
	for (int i = (int)design.size() - 1; i >= 0; --i)
	{
		string cur = design.substr(i);
		int max_s = min(max_p, (int)design.size() - i);
		for (int l = 1; l <= max_s; ++l)
		{
			string first = cur.substr(0, l);
			string second = cur.substr(l);
			if (patterns.contains(first))
			{
				if (second.size() == 0)
					cache[cur] += 1;
				else 
				{
					auto it = cache.find(second);
					if (it != cache.end())
						cache[cur] += it->second;
				}
			}
		}
	}
	return cache[design];
}
long long solve2(const input_t& input)
{
	auto designs = get_matching(input);
	long long res = 0;
	for (auto d : designs)
	{
		res += count(d, input);
	}
	return res;
}

int main()
{
	test();
	ifstream is("Day19.txt");
	input_t input = read_input(is);

	cout << "Day19 Answer1: " << solve1(input) << endl;
	cout << "Day19 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(r, wr, b, g, bwu, rb, gb, br

brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
