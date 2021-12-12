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
#include <chrono>
#include <ranges>

using namespace std;
using namespace chrono;
namespace r = std::ranges;
namespace v = std::ranges::views;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
string ElapsedMs(const steady_clock::time_point& start)
{ return S(((duration<double, milli>(high_resolution_clock::now() - start)).count())); }

struct Conn
{
	string start;
	string end;
};
istream& operator>>(istream& is, Conn& path)
{
	getline(is, path.start, '-');
	getline(is, path.end);
	return is;
}

struct Path
{
	int cur;
	set<int> visited;
	bool sv;
	bool visit(int c, bool once)
	{
		if (c < 1000)
			if (visited.contains(c))
				if (once || sv || c == 1)
					return false;
				else
					sv = true;
			else
				visited.insert(c);
		cur = c;
		return true;
	}
};
uint64_t GetPaths(const vector<Conn> input, bool once)
{
	map<string, int> mapping{ {"start", 1},{"end",2} };
	int ns = 3, nb = 1001;
	auto s2n = [&](string c) {if (mapping[c] == 0) mapping[c] = (c[0] >= 'a' && c[0] <= 'z' ? ns++ : nb++); return mapping[c]; };
	map<int, list<int>> all_conn;
	for (auto c : input)
	{
		int cs = s2n(c.start);
		int ce = s2n(c.end);

		all_conn[cs].push_back(ce);
		all_conn[ce].push_back(cs);
	}

	uint64_t res = 0;
	list<Path> paths{ { 1, {1} } };
	while (!paths.empty())
	{
		for (auto it = paths.begin(); it != paths.end();)
		{
			if (it->cur == 2)
			{
				++res;
				it = paths.erase(it);
			}
			else
			{
				auto it_conn = all_conn.find(it->cur);
				if (it_conn != all_conn.end())
				{
					for (auto itc = it_conn->second.begin(); itc != it_conn->second.end(); ++itc)
					{
						if (next(itc) == it_conn->second.end())
						{
							if (!it->visit(*itc, once))
								it = paths.erase(it);
						}
						else
						{
							Path cur(*it);
							if (!cur.visit(*itc, once))
								continue;
							paths.push_back(cur);
						}
					}
				}
			}
		}
	}
	return res;
}

void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day12.txt");
	istream_iterator<Conn> start(is), end;
	vector<Conn> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day12 Answer1: " << GetPaths(input, true); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day12 Answer2: " << GetPaths(input, false); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
}

void Test()
{
	stringstream is(R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)");
	istream_iterator<Conn> start(is), end;
	vector<Conn> input(start, end);
	cout << "Test 1: " << GetPaths(input, true) << endl;
	cout << "Test 2: " << GetPaths(input, false) << endl;
}