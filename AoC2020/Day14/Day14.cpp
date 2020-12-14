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

struct Instr
{
	string mask;
	int addr;
	long long val;
};

vector<Instr> ReadInput(istream& is)
{
	vector<Instr> res;
	string mask;
	while (is)
	{
		string line;
		getline(is, line);
		if (line.empty())
			continue;
		smatch m;
		if (regex_match(line, m, regex("^mask = (\\w+)$")))
			mask = m[1].str();
		else if (regex_match(line, m, regex("mem\\[(\\d+)\\] = (\\d+)")))
			res.push_back(Instr{ mask, stoi(m[1].str()), stoll(m[2].str()) });
	}
	return res;
}
long long sum(const vector<Instr>& instructions)
{
	map<int, long long> mem;
	string mask;
	long long amask, omask;
	for (auto& i : instructions)
	{
		if (mask != i.mask)
		{
			amask = omask = 0;
			for (auto& c : i.mask)
			{
				amask <<= 1;
				omask <<= 1;
				if (c == 'X')
					++amask;
				else if (c == '1')
				{
					++omask;
					++amask;
				}
			}
			mask = i.mask;
		}
		mem[i.addr] = i.val & amask | omask;
	}
	return accumulate(mem.begin(), mem.end(), (long long)0, [](auto sum, auto& val) { return sum + val.second; });
}
long long sum2(const vector<Instr>& instructions)
{
	map<long long, long long> mem;
	vector<long long> amask(26244,0), omask(26244,0);
	string mask;
	for (auto& i : instructions)
	{
		if (mask != i.mask)
		{
			amask.clear(); amask.push_back(0);
			omask.clear(); omask.push_back(0);
			for (auto& c : i.mask)
			{
				for (int j = 0; j < amask.size(); ++j)
				{
					amask[j] <<= 1;
					omask[j] <<= 1;
				}
				if (c == 'X')
				{
					int num = (int)amask.size();
					for (int j = 0; j < num; ++j)
					{
						amask.push_back(amask[j] + 1); amask.push_back(amask[j]);
						omask.push_back(omask[j]); omask.push_back(omask[j] + 1);
					}
				}
				else if (c == '1')
				{
					for (int j = 0; j < omask.size(); ++j)
						++omask[j];
				}
				else if (c == '0')
				{
					for (int j = 0; j < amask.size(); ++j)
						++amask[j];
				}
			}
			mask = i.mask;
		}
		for (int j = 0; j < amask.size(); ++j)
			mem[(long long)i.addr & amask[j] | omask[j]] = i.val;
	}
	return accumulate(mem.begin(), mem.end(), (long long)0, [](auto sum, auto& val) { return sum + val.second; });
}

void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day14.txt");
	auto s = ReadInput(is);
	//cout << "Read " << s.size() << " instructions" << endl;

	cout << "Day14 Answer1: " << sum(s) << endl;
	cout << "Day14 Answer2: " << sum2(s) << endl;
}

void Test()
{
	stringstream test(R"()");
	//Schedule s = ReadInput(test);
	//cout << "Test 1: " << get_schedule(s) << endl;
}