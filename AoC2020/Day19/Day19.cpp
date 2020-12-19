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
#include <ctime>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>
#include <queue>

using namespace std;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()

typedef vector<string> Rules;
ostream& operator<<(ostream& os, const Rules& rules) { for (auto& r : rules) cout << r << endl; return os; }

typedef string Rule;
struct Input
{
	Rules rules;
	vector<string> mess;
};

Input read_input(istream& is)
{
	Input input;
	string line;
	map<int, string> rules;
	while (is && getline(is, line) && !line.empty())
	{
		smatch m;
		if (regex_match(line, m, regex("(\\d+): (.+)$")))
		{
			int id = stoi(m[1]);
			string rule = m[2];
			smatch p;
			while (regex_search(rule, p, regex("(\\d+)|(\\|)|(a)|(b)")))
			{
				if (p[1].matched)
					rules[id].append(S("(?:" << p[1] << ")"));
				for (int i = 2; i < p.size(); ++i)
					if (p[i].matched)
						rules[id].append(p[i]);
				rule = p.suffix();
			}
		}
	}
	input.rules.resize(rules.rbegin()->first + 1);
	for (auto& r : rules)
		input.rules[r.first] = r.second;

	while (is && getline(is, line) && !line.empty())
		input.mess.push_back(line);
	return input;
}

void process_rule(const Rules& rules, Rule& rule)
{
	smatch m;
	while (regex_search(rule, m, regex("(\\d+)")))
	{
		auto r = rules[stoi(m[1])];
		rule = S(m.prefix() << r << m.suffix());
	}
}

void process_rules(Rules& rules)
{
	bool changed = false;
	for (auto it = rules.rbegin(); it != rules.rend(); ++it)
		process_rule(rules, *it);
}

int count_matching(const Input& input)
{
	Rule rule = input.rules[0];
	return (int)count_if(input.mess.begin(), input.mess.end(), [&](auto& m) { smatch s; return regex_match(m, s, regex(rule)); });
}

void Test();
int main()
{
	Test(); //return 0;
	ifstream is("Day19.txt");
	Input input = read_input(is);

	Input task1 = input;
	process_rules(task1.rules);
	cout << "Day19 Answer1: " << count_matching(task1) << endl;
	Input task2 = input;
	task2.rules[8] = Rule{ "(?:42)+" };
	task2.rules[11] = Rule{ "(?:42)(?:31)|(?:42)(?:42)(?:31)(?:31)|(?:42)(?:42)(?:42)(?:31)(?:31)(?:31)|(?:42)(?:42)(?:42)(?:42)(?:31)(?:31)(?:31)(?:31)|(?:42)(?:42)(?:42)(?:42)(?:42)(?:31)(?:31)(?:31)(?:31)(?:31)" };
	process_rules(task2.rules);
	cout << "Day19 Answer2: " << count_matching(task2) << endl;
}

void Test()
{
	stringstream test(R"(0: 1 2
1: "a"
2: 1 3 | 3 1
3: "b")");
	auto input = read_input(test);
	process_rules(input.rules);
	cout << "test1:" << endl << input.rules;

	test = stringstream(R"(0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b")");
	input = read_input(test);
	process_rules(input.rules);
	cout << "test2: " << endl << input.rules;

	string str = R"(42: 9 14 | 10 1
9: 14 27 | 1 26
10: 23 14 | 28 1
1: "a"
11: 42 31
5: 1 14 | 15 1
19: 14 1 | 14 14
12: 24 14 | 19 1
16: 15 1 | 14 14
31: 14 17 | 1 13
6: 14 14 | 1 14
2: 1 24 | 14 4
0: 8 11
13: 14 3 | 1 12
15: 1 | 14
17: 14 2 | 1 7
23: 25 1 | 22 14
28: 16 1
4: 1 1
20: 14 14 | 1 15
3: 5 14 | 16 1
27: 1 6 | 14 18
14: "b"
21: 14 1 | 1 14
25: 1 1 | 1 14
22: 14 14
8: 42
26: 14 22 | 1 20
18: 15 15
7: 14 5 | 1 21
24: 14 1

abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa
bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
aaaaabbaabaaaaababaa
aaaabbaaaabbaaa
aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
babaaabbbaaabaababbaabababaaab
aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba)";

	test = stringstream(str);
	input = read_input(test);
	process_rules(input.rules);
	cout << "test3: " << count_matching(input) << endl;

	test = stringstream(str);
	input = read_input(test);
	input.rules[8] = Rule{ "(?:42)+"};
	input.rules[11] = Rule{ "(?:42)(?:31)|(?:42)(?:42)(?:31)(?:31)|(?:42)(?:42)(?:42)(?:31)(?:31)(?:31)|(?:42)(?:42)(?:42)(?:42)(?:31)(?:31)(?:31)(?:31)|(?:42)(?:42)(?:42)(?:42)(?:42)(?:31)(?:31)(?:31)(?:31)(?:31)" };
	process_rules(input.rules);
	cout << "test4: " << count_matching(input) << endl;
}
