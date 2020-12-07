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

struct Bag
{
	string color;
	int num;
};
map<string, list<Bag>> ReadInput(istream& is)
{
	map<string, list<Bag>> res;
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() == 0)
			continue;
		smatch s;
		if (regex_match(line, s, regex("^(.+) bags contain (.+)$")))
		{
			string key = s[1].str();
			string val = s[2].str();
			list<Bag> bags;
			if (val != "no other bags.")
			{
				smatch g;
				while (regex_search(val, g, regex("(\\d)+ ([^,.]+) (?:bag|bags)(?:, |.)")))
				{
					bags.push_back(Bag{ g[2], stoi(g[1]) });
					val = g.suffix();
				}
			}
			res[key] = bags;
		}
		else
		{
			cout << "Failed to parse line: " << line << endl;
		}
	}
	return res;
}

void visit_inv(const map<string, list<string>>& inv, string color, set<string>& counted)
{
	if (counted.find(color) != counted.end())
		return;
	counted.insert(color);
	auto it = inv.find(color);
	if (it == inv.end())
		return;
	for (auto& c : it->second)
		visit_inv(inv, c, counted);
}
int count_inv(const map<string, list<Bag>>& input, string color)
{
	map<string, list<string>> inv;
	for (auto& i : input)
		for (auto& b : i.second)
			inv[b.color].push_back(i.first);
	//cout << "Inv: " << endl << inv;

	set<string> counted;
	visit_inv(inv, color, counted);
	return (int)counted.size() - 1;
}

int count_bags(const map<string, list<Bag>>& input, string color)
{
	auto it = input.find(color);
	if (it == input.end())
		return 1;
	return 1 + accumulate(it->second.begin(), it->second.end(), (int)0, [&](int sum, auto& bag) { return sum + bag.num * count_bags(input, bag.color); });
}
ostream& operator<<(ostream& o, const map<string, list<string>>& bags)
{
	for (auto& bag : bags)
	{
		o << bag.first << ": ";
		copy(bag.second.begin(), bag.second.end(), ostream_iterator<string>(o, ","));
		o << endl;
	}
	return o;
}
void Test();
int main()
{
	//Test(); return 0;
	ifstream is("Day7.txt");
	auto input = ReadInput(is);
	cout << "Read " << input.size() << " rules" << endl;
	//cout << input << endl;

	cout << "Day7 Answer1: " << count_inv(input, "shiny gold") << endl;

	cout << "Day7 Answer2: " << (count_bags(input, "shiny gold") -1) << endl;
}

void Test()
{
	auto test = stringstream(R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)");
	auto input = ReadInput(test);
	cout << "test 1: " << count_inv(input, "shiny gold") << endl;
	cout << "test 2: " << count_bags(input, "shiny gold")-1 << endl;

	test = stringstream(R"(shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.)");
	input = ReadInput(test);
	cout << "test 3: " << count_bags(input, "shiny gold") - 1 << endl;
}