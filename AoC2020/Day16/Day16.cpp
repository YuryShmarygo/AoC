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

struct Range
{
	int min;
	int max;
};
struct Rule
{
	string name;
	vector<Range> range;
	set<int> matches;
};
typedef vector<Rule> Rules;
typedef vector<int> Ticket;
typedef vector<Ticket> Tickets;
struct Input
{
	Rules rules;
	Ticket ticket;
	Tickets other;
};
Ticket ReadTicket(string str)
{
	Ticket ticket;
	stringstream s(str);
	string num;
	while (getline(s, num, ','))
	{
		if (!num.empty())
			ticket.push_back(stoi(num));
	}
	return ticket;
}
Rules ReadRules(istream& is)
{
	Rules rules;
	string line;
	while (is && getline(is, line) && !line.empty())
	{
		smatch m;
		if (regex_match(line, m, regex("(.+): (\\d+)-(\\d+) or (\\d+)-(\\d+)")))
			rules.push_back(Rule{ m[1].str(), {{stoi(m[2].str()), stoi(m[3].str())},{stoi(m[4].str()), stoi(m[5].str())}} });
	}
	return rules;
}
Ticket ReadYourTicket(istream& is)
{
	string line, empty;
	getline(is, line);
	if (line != "your ticket:")
		throw exception("failed to parse your ticket");
	getline(is, line); getline(is, empty);
	return ReadTicket(line);
}
Tickets ReadOtherTickets(istream& is)
{
	Tickets tickets;
	string line;
	getline(is, line);
	if (line != "nearby tickets:")
		throw exception("failed to parse nearby tickets");
	while (is && getline(is, line) && !line.empty())
		tickets.push_back(ReadTicket(line));
	return tickets;
}
Input ReadInput(istream& is)
{
	return Input{ ReadRules(is), ReadYourTicket(is), ReadOtherTickets(is) };
}
bool is_valid(const Rule& rule, int num)
{
	return any_of(rule.range.begin(), rule.range.end(), [&](auto& range) {return num >= range.min && num <= range.max; });
}
bool is_valid(const Rules& rules, int num)
{
	return any_of(rules.begin(), rules.end(), [&](auto& rule) {return is_valid(rule, num); });
}
int invalid_field(const Rules& rules, const Ticket& ticket)
{
	auto it = find_if(ticket.begin(), ticket.end(), [&](auto& num) { return !is_valid(rules, num); });
	return it == ticket.end() ? 0 : *it;
}
int error_rate(const Rules& rules, const Tickets& tickets)
{
	return accumulate(tickets.begin(), tickets.end(), 0, [&](int sum, auto& ticket) {return sum + invalid_field(rules, ticket); });
}
bool is_valid(const Rules& rules, const Ticket& ticket)
{
	return all_of(ticket.begin(), ticket.end(), [&](auto& num) { return is_valid(rules, num); });
}
set<int> find_matching(const Rule& rule, const Ticket& ticket)
{
	set<int> res;
	for (int i = 0; i < ticket.size(); ++i)
		if (is_valid(rule, ticket[i]))
			res.insert(i);
	return res;
}
set<int> find_matching(const Rule& rule, const Tickets& tickets)
{
	set<int> res;
	for (auto& ticket : tickets)
	{
		set<int> cur = find_matching(rule, ticket);
		if (res.empty())
		{
			res = cur;
			continue;
		}
		set<int> cross;
		set_intersection(cur.begin(), cur.end(), res.begin(), res.end(), inserter(cross, cross.end()));
		res = cross;
	}
	return res;
}
ostream& operator<<(ostream& os, const set<int>& m) { copy(m.begin(), m.end(), ostream_iterator<int>(os,",")); return os; }
long long departure(const Input& input)
{
	Rules rules = input.rules;
	Tickets tickets;
	tickets.push_back(input.ticket);
	for (auto& ticket : input.other)
		if (is_valid(input.rules, ticket))
			tickets.push_back(ticket);
	for (auto& rule : rules)
		rule.matches = find_matching(rule, tickets);
	sort(rules.begin(), rules.end(), [](auto& r1, auto& r2) {return r1.matches.size() < r2.matches.size() || r1.matches.size() == r2.matches.size() && r1.name < r2.name; });
	//for (auto& rule : rules) cout << rule.name << " : " << rule.matches << endl;
	while (!all_of(rules.begin(), rules.end(), [](auto& rule) {return rule.matches.size() == 1; }))
	{
		for (int i = 0; i < rules.size() && rules[i].matches.size() == 1; ++i)
			for (int j = i + 1; j < rules.size(); ++j)
				rules[j].matches.erase(*(rules[i].matches.begin()));
	}
	return accumulate(rules.begin(), rules.end(), (long long)1, [&](auto res, auto& rule) { return rule.name.find("departure") == 0 ? res * input.ticket[*rule.matches.begin()] : res;  });
}
void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day16.txt");
	Input input = ReadInput(is);
	//cout << "Read " << input.rules.size() << " rules, " << input.other.size() << " tickets" << endl;

	cout << "Day16 Answer1: " << error_rate(input.rules, input.other) << endl;
	cout << "Day16 Answer2: " << departure(input) << endl;
}

void Test()
{
	stringstream test(R"()");
	//Schedule s = ReadInput(test);
	//cout << "Test 1: " << get_schedule(s) << endl;
}