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

list<map<string, string>> ReadInput(istream& is)
{
	list<map<string, string>> res;
	map<string, string> cur;
	while (is)
	{
		string line;
		getline(is, line);
		if (line.empty() && !cur.empty())
		{
			res.push_back(cur);
			cur.clear();
		}

		smatch m;
		while (regex_search(line, m, regex("([^ ][^ ][^ ]):([^ ]+)")))
		{
			for (int i = 1; i < m.size(); i+=2)
			{
				if (i + 1 < m.size())
					cur[m[i].str()] = m[i + 1].str();
				else
					cout << "Failed to parse line " << line << endl;
			}
			line = m.suffix();
		}
	}
	return res;
}
ostream& operator<<(ostream& o, const pair<string, string>& p) { o << p.first << ':' << p.second; return o; }
ostream& operator<<(ostream& o, const map<string, string>& m)
{
	for (auto& e : m)
		o << e << ' ';
	return o;
}
ostream& operator<<(ostream& o, const list<map<string, string>>& p) 
{
	for (auto& e : p)
		o << e << endl;
	return o;
}

int count_valid(const list<map<string, string>>& input)
{
	static list<string> required = { "byr","iyr","eyr","hgt","hcl","ecl","pid" };
	return (int)count_if(input.begin(), input.end(), [&](auto& p) { return all_of(required.begin(), required.end(), [&](auto& f) {return p.find(f) != p.end(); }); });
}

int count_valid2(const list<map<string, string>>& input)
{
	static map < string, function<bool(string val)>> checks = {
		{"byr", [](string val) { int ival = stoi(val); return ival >= 1920 && ival <= 2002; } },
		{"iyr", [](string val) { int ival = stoi(val); return ival >= 2010 && ival <= 2020; } },
		{"eyr", [](string val) { int ival = stoi(val); return ival >= 2020 && ival <= 2030; } },
		{"hgt", [](string val) 
		{
			smatch m;
			if (!regex_match(val, m, regex("(\\d+)(in|cm)")))
				return false;
			int ival = stoi(m[1].str());
			return m[2] == "cm" && ival >= 150 && ival <= 193 || m[2] == "in" && ival >= 59 && ival <= 76;
		} },
		{"hcl", [](string val) { return regex_match(val, regex("#[a-f0-9]{6}")); } },
		{"ecl", [](string val) { return regex_match(val, regex("amb|blu|brn|gry|grn|hzl|oth")); } },
		{"pid", [](string val) { return regex_match(val, regex("\\d{9}")); } },
	};
	return (int)count_if(input.begin(), input.end(), [&](auto& p) { return all_of(checks.begin(), checks.end(), [&](auto& f) { auto it = p.find(f.first); return it != p.end() && f.second(it->second); }); });
}
int main()
{
	ifstream is("Day4.txt");
	auto input = ReadInput(is);
	//cout << "Read " << input.size() << " lines." << endl << input << endl;

	cout << "Day4 Answer1: " << count_valid(input) << endl;

	cout << "Day4 Answer2: " << count_valid2(input) << endl;
}
