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

vector<vector<string>> ReadInput(istream& is)
{
	vector<vector<string>> res;
	vector<string> cur;
	while (is)
	{
		string line;
		getline(is, line);
		if (line.empty() && !cur.empty())
		{
			res.push_back(cur);
			cur.clear();
		}
		if (!line.empty())
			cur.push_back(line);
	}
	return res;
}

int count(const vector<string>& g)
{
	set<char> answers;
	for (auto& s : g)
		for (auto& c : s)
			answers.insert(c);
	return (int)answers.size();
}
int count(const vector<vector<string>>& input)
{
	return accumulate(input.begin(), input.end(), (int)0, [](auto sum, auto& g) { return sum + count(g); });
}
int count_i(const vector<string>& g)
{
	set<char> res(g[0].begin(), g[0].end());
	for (auto& s : g)
	{
		set<char> cur(s.begin(), s.end());
		vector<char> r;
		set_intersection(res.begin(), res.end(), cur.begin(), cur.end(), back_inserter(r));
		res = set<char>(r.begin(), r.end());
	}
	return (int)res.size();
}
int count_i(const vector<vector<string>>& input)
{
	return accumulate(input.begin(), input.end(), (int)0, [](auto sum, auto& g) { return sum + count_i(g); });
}
void Test();
int main()
{
	//Test(); return 0;
	ifstream is("Day6.txt");
	auto input = ReadInput(is);
	//cout << "Read " << input.size() << " groups" << endl;


	cout << "Day6 Answer1: " << count(input) << endl;

	cout << "Day6 Answer2: " << count_i(input) << endl;
}

void Test()
{

}