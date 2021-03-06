#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 

using namespace std;
using namespace placeholders;

enum EventType
{
	ShiftStart,
	FallAsleep,
	WakeUp
};

struct RawEvent
{
	string time;
	string event;
};
struct GuardSleep
{
	string name;
	time_t start;
	time_t end;
	int minute;
	int Duration() const { return (int)(difftime(end, start) / 60); }
};

int main()
{
	//read input file
	vector<RawEvent> input;
	ifstream is("input.txt");
	while (is)
	{
		string line;
		getline(is, line);
		if (line.size() == 0) break;
		input.push_back(RawEvent{ line.substr(0, 18), line.substr(19, line.length() - 18) });
	}
	cout << "Read " << input.size() << " events" << endl;
	sort(input.begin(), input.end(), [](auto l, auto r) {return l.time < r.time; });

	map<string, list<GuardSleep>> guards;
	int firstYear = 0;
	GuardSleep s;
	for (auto i : input)
	{
		std::tm tm = {0}; std::stringstream(i.time) >> std::get_time(&tm, "[%Y-%m-%d %H:%M]");
		firstYear = firstYear == 0 ? tm.tm_year : firstYear;
		tm.tm_year -= firstYear - 71;
		time_t tt = mktime(&tm);
		
		smatch g;
		if (regex_match(i.event, g, regex("Guard #(\\d+) begins shift")))
		{
			s = GuardSleep{ g[1], 0, 0, 0 };
		}
		else if (i.event == "falls asleep")
		{
			s.start = tt;
			s.minute = tm.tm_min;
		}
		else
		{
			s.end = tt;
			guards[s.name].push_back(s);
		}
	}

	auto summDuration = [](const list<GuardSleep>& l) { double sum = 0; for (auto i : l) { sum += i.Duration(); } return sum; };
	auto countMinutes = [](const list<GuardSleep>& l, const GuardSleep& e) {return count_if(l.begin(), l.end(), [&](auto i) {return i.minute <= e.minute && i.minute + i.Duration() >= e.minute; }); };
	auto compareMinutes = [&](const list<GuardSleep>& l, const GuardSleep& i, const GuardSleep& j) {return countMinutes(l, i) < countMinutes(l, j); };
	auto maxMinutes = [&](const list<GuardSleep>& l) {return max_element(l.begin(), l.end(), bind(compareMinutes, l, _1, _2)); };
	auto countMaxMinutes = [&](const list<GuardSleep>& l) {return countMinutes(l, *maxMinutes(l)); };

	// First task
	auto m = max_element(guards.begin(), guards.end(), [&](auto l, auto r) {return summDuration(l.second) < summDuration(r.second); });
	cout << "Day4 Answer1: " << stoi(m->first) * maxMinutes(m->second)->minute << endl;

	// Second task
	auto m2 = max_element(guards.begin(), guards.end(), [&](auto l, auto r) {return countMaxMinutes(l.second) < countMaxMinutes(r.second); });
	cout << "Day4 Answer2: " << stoi(m2->first) * maxMinutes(m2->second)->minute << endl;

	return 0;
}
