#include "../common.h"

struct condition
{
	char field;
	char op;
	int val;
	bool is_empty;
	string dest;
};
ostream& operator<<(ostream& os, const condition& c) { os << "[" << (c.is_empty ? "any" : S(c.field << c.op << c.val)) << "] -> " << c.dest; return os; }
struct rule_t
{
	string name;
	vector<condition> cond;
};
ostream& operator<<(ostream& os, const rule_t& r) { os << r.name << ": ["; r::copy(r.cond, ostream_iterator<condition>(os, ",")); return os; }
rule_t read_rule(string line)
{
	rule_t rule;
	smatch m;
	if (regex_match(line, m, regex(R"((\w+)\{(.+)\})")))
	{
		rule.name = m[1].str();
		string r_line = m[2].str();
		smatch r;
		while (regex_search(r_line, r, regex(R"(,?(x|m|s|a)(>|<)(\d+):(\w+))")))
		{
			condition c;
			c.is_empty = false;
			c.field = r[1].str()[0];
			c.op = r[2].str()[0];
			c.val = stoi(r[3].str());
			c.dest = r[4];
			rule.cond.push_back(c);
			r_line = r.suffix();
		}
		if (regex_match(r_line, r, regex(R"(,?(\w+))")))
		{
			condition c;
			c.is_empty = true;
			c.dest = r[1].str();
			rule.cond.push_back(c);
		}
	}
	return rule;
}
struct part_t
{
	int x;
	int m;
	int a;
	int s;
	int get_field(char c) const
	{
		switch (c)
		{
		case 'x': return x;
		case 'm': return m;
		case 'a': return a;
		case 's': return s;
		default: return x;
		};
	}
	bool check_val(char op, int val1, int val2) const
	{
		switch (op)
		{
		case '>': return val1 > val2;
		case '<': return val1 < val2;
		default: return false;
		}
	}
	bool check_cond(const condition& cond) const
	{
		return cond.is_empty || check_val(cond.op, get_field(cond.field), cond.val);
	}
	int sum() const { return x + m + a + s; }
};
ostream& operator<<(ostream& os, const part_t& p) { os << "[x=" << p.x << ",m=" << p.m << ",a=" << p.a << ",s=" << p.s << "]"; return os; }
part_t read_part(string line)
{
	part_t p;
	smatch m;
	if (regex_match(line, m, regex(R"(\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\})")))
	{
		p.x = stoi(m[1].str());
		p.m = stoi(m[2].str());
		p.a = stoi(m[3].str());
		p.s = stoi(m[4].str());
	}
	return p;
}
struct puzzle_t
{
	vector<rule_t> rules;
	vector<part_t> parts;
	const rule_t find_rule(string name) const { return *r::find_if(rules, [name](auto& r) { return r.name == name; }); }
};
istream& operator>>(istream& is, puzzle_t& p)
{
	p.rules.clear(); p.parts.clear();
	string line;
	while (getline(is, line) && !line.empty())
		p.rules.push_back(read_rule(line));
	while (getline(is, line) && !line.empty())
		p.parts.push_back(read_part(line));
	return is;
}
string process(const part_t& part, const puzzle_t& puzzle)
{
	//cout << part << " : in";
	string rule_name = "in";
	while (rule_name != "R" && rule_name != "A")
	{ 
		auto& rule = puzzle.find_rule(rule_name);
		for (auto& cond : rule.cond)
		{
			if (part.check_cond(cond))
			{
				rule_name = cond.dest;
				break;
			}
		}
		//cout << " -> " << rule_name;
	}
	//cout << endl;
	return rule_name;
}
long long solve1(puzzle_t& input)
{
	long long res = 0;
	for (auto& part : input.parts)
	{
		string dest = process(part, input);
		if (dest == "A")
			res += part.sum();
	}
	return res;
}
struct interval_t
{
	int start;
	int end;
};
typedef vector<interval_t> interval_v;
long long value(const interval_v& r) { return accumulate(r.begin(), r.end(), 0LL, [](auto sum, auto& i) { return sum + i.end - i.start + 1; }); }
void invert(interval_v& r)
{
	interval_v res;
	if (r.empty())
	{
		r.push_back({ 1,4000 });
		return;
	}
	if (r[0].start > 1)
		res.push_back({ 1, r[0].start - 1 });
	for (int i = 1; i < r.size() - 1; ++i)
		res.push_back({ r[i].end + 1, r[i + 1].start - 1 });
	if (r.back().end < 4000)
		res.push_back({ r.back().end + 1, 4000 });
	swap(r, res);
}
void add(interval_v& r, const interval_t& n)
{
	if (r.empty())
	{
		r.push_back(n);
		return;
	}
	int start = n.start;
	int end = n.end;
	auto it = r.begin();
	while (start != end)
	{
		while (it != r.end() && n.start > it->end) ++it; // advanced to closest interval
		if (it == r.end()) // after last interval
		{
			r.push_back(n);
			return;
		}
		if (n.start < it->start) // start between intervals
		{
			if (n.end < it->start) // insert new interval
			{
				r.insert(it, n);
				return;
			}
			if (n.end >= it->start) // extend start
				it->start = n.start;
			start = min(it->end, end);
		}
	}
}
void add(interval_v& r, const interval_v& n)
{
	for (auto& i : n)
		add(r, i);
}

typedef map<char, interval_v> restriction_t;

long long solve2(puzzle_t& input)
{
	unordered_map<string, long long> cache;
	long long res = 0;
	for (auto& rule : input.rules)
	{
		long long cur = 0;
		for (auto& cond : rule.cond)
		{
			if (cond.dest == "A")
			{
			}
		}
	}
	return res;
}
int main()
{
	test();
	ifstream is("Day19.txt");
	puzzle_t input; is >> input;

	auto t_start = high_resolution_clock::now();
	cout << "Day19 Answer1: " << solve1(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day19 Answer2: " << solve2(input) << " took: " << ElapsedMs(t_start) << "ms" << endl;
}

void test()
{
	auto test1 = R"(px{a<2006:qkq,m>2090:A,rfg}
pv{a>1716:R,A}
lnx{m>1548:A,A}
rfg{s<537:gd,x>2440:R,A}
qs{s>3448:A,lnx}
qkq{x<1416:A,crn}
crn{x>2662:A,R}
in{s<1351:px,qqz}
qqz{s>2770:qs,m<1801:hdj,R}
gd{a>3333:R,R}
hdj{m>838:A,pv}

{x=787,m=2655,a=1222,s=2876}
{x=1679,m=44,a=2067,s=496}
{x=2036,m=264,a=79,s=2244}
{x=2461,m=1339,a=466,s=291}
{x=2127,m=1623,a=2188,s=1013})";

	auto is = stringstream(test1);
	puzzle_t input; is >> input;

	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
