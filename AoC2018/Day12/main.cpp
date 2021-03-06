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
#include <time.h>
#include <iomanip>
#include <regex>
#include <functional>
#include <numeric> 
#include <cstdlib>

using namespace std;
using namespace placeholders;

string Evaluate(string state, map<string, string>& rules)
{
	string new_state(state.size(), '.');
	for (auto r : rules)
	{
		size_t pos = -1;
		while ((pos = state.find(r.first, pos + 1)) != string::npos)
			new_state[pos + 2] = '#';
	}
	return new_state;
}

int Count(string state, int shift)
{
	int res = 0;
	for (int i = 0; i < state.size(); i++)
		if (state[i] == '#')
			res += i - shift;
	return res;
}

int Solve(string initial_state, map<string, string>& rules, int generations)
{
	int shift = generations * 2;
	string state = string(shift, '.') + initial_state + string(shift, '.');
	//cout << state << endl;
	for (int i = 0; i < generations; i++)
	{
		state = Evaluate(state, rules);
		//cout << state << endl;
	}
	return Count(state, shift);
}

long long Solve2(string initial_state, map<string, string>& rules, long long generations)
{
	vector<long long> diff(200);
	long long last = Solve(initial_state, rules, 0);
	do
	{
		long long cur = Solve(initial_state, rules, (int)diff.size() + 1);
		diff.push_back(cur - last);
		last = cur;
	} while (diff.size() < 20 || !all_of(prev(diff.end(), 10), diff.end(), [&](auto e) {return e == *prev(diff.end(), 10); }));

	return (generations - diff.size()) * diff.back() + last;
}
void test()
{
	string initial_state = "#..#.#..##......###...###";
	map<string, string> rules = { {"...##","#"},{"..#..","#"},{".#...","#"},{".#.#.","#"},{".#.##","#"},{".##..","#"},{".####","#"},{"#.#.#","#"},{"#.###","#"},{"##.#.","#"},{"##.##","#"},{"###..","#"},{"###.#","#"},{"####.","#"} };
	Solve(initial_state, rules, 20);
}
int main()
{
	test();
	string initial_state = "##..##....#.#.####........##.#.#####.##..#.#..#.#...##.#####.###.##...#....##....#..###.#...#.#.#.#";
	map<string, string> _rules = { {"##.#.","."},{"##.##","."},{"#..##","."},{"#.#.#","."},{"..#..","#"},{"#.##.","."},{"##...","#"},{".#..#","."},{"#.###","."},{".....","."},{"...#.","#"},{"#..#.","#"},{"###..","#"},{".#...","#"},{"###.#","#"},{"####.","."},{".##.#","#"},{"#.#..","#"},{".###.","#"},{".#.##","."},{"#####","#"},{"....#","."},{".####","."},{".##..","#"},{"##..#","."},{"#...#","."},{"..###","#"},{"...##","."},{"#....","."},{"..##.","."},{".#.#.","#"},{"..#.#","#"} };
	map<string, string> rules;
	for (auto r : _rules) { if (r.second == "#") rules.insert(r); }

	cout << "Day12 Answer1: " << Solve(initial_state, rules, 20) << endl;
	cout << "Day12 Answer2: " << Solve2(initial_state, rules, 50000000000) << endl;

	return 0;
}
