#include "../common.h"

typedef map<int, vector<int>> rules_t;
typedef vector<int> upd_t;
struct input_t
{
	rules_t rules;
	vector<upd_t> updates;
};

rules_t read_rules(istream& is)
{
	rules_t rules;
	string line;
	while (getline(is, line) && !line.empty())
	{
		smatch m;
		if (regex_match(line, m, regex(R"((\d+)\|(\d+))")))
		{
			int p1 = stoi(m[1].str());
			int p2 = stoi(m[2].str());

			auto it = rules.find(p1);
			if (it == rules.end())
				rules[p1] = { p2 };
			else it->second.push_back(p2);
		}
	}
	return rules;
}
upd_t read_update(string line)
{
	upd_t upd;
	smatch m;
	while (regex_search(line, m, regex(R"(,?(\d+))")))
	{
		upd.push_back(stoi(m[1].str()));
		line = m.suffix();
	}
	return upd;
}
vector<upd_t> read_updates(istream& is)
{
	vector<upd_t> updates;
	string line;
	while (getline(is, line) && !line.empty())
		updates.push_back(read_update(line));
	
	return updates;
}
input_t read_input(istream& is) 
{ 
	input_t input;
	input.rules = read_rules(is);
	input.updates = read_updates(is);
	return input;
}

bool is_after(int p1, int p2, const rules_t& rules)
{
	auto it = rules.find(p2);
	return it == rules.end() || r::find(it->second, p1) == it->second.end();
}
bool is_correct(const upd_t& upd, const rules_t& rules)
{
	bool correct = true;
	for (auto it = upd.begin(); it != upd.end() && correct; ++it)
	{
		int p1 = *it;
		correct = correct && all_of(next(it), upd.end(), [&](int p2) { return is_after(p1, p2, rules); });
	}
	return correct;
}
long long solve1(const input_t& input)
{
	long long sum = 0;
	for (auto& upd : input.updates)
	{
		if (is_correct(upd, input.rules))
			sum += upd[upd.size() / 2];
	}
	return sum;
}
long long solve2(const input_t& input)
{
	long long sum = 0;
	for (auto upd : input.updates)
	{
		if (!is_correct(upd, input.rules))
		{
			while (!is_correct(upd, input.rules))
			{
				for (auto it = upd.begin(); it != upd.end() && next(it) != upd.end(); ++it)
				{
					int p1 = *it;
					int p2 = *next(it);
					if (!is_after(p1, p2, input.rules))
					{
						*it = p2;
						*next(it) = p1;
					}
				}
			}
			sum += upd[upd.size() / 2];
		}
	}
	return sum;
}

int main()
{
	test();
	ifstream is("Day05.txt");
	input_t input = read_input(is);

	cout << "Day5 Answer1: " << solve1(input) << endl;
	cout << "Day5 Answer2: " << solve2(input) << endl;
}
void test()
{
	auto test1 = R"(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47)";
	auto test2 = R"(MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX)";

	auto is = stringstream(test1);
	auto input = read_input(is);
	cout << "Test1: " << solve1(input) << endl;
	cout << "Test2: " << solve2(input) << endl;
}
