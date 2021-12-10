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
namespace rv = std::ranges::views;
#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()
#define C(s) S(s).c_str()
string ElapsedMs(const steady_clock::time_point& start)
{ return S(((duration<double, milli>(high_resolution_clock::now() - start)).count())); }

static map<char, char> closing{
	{'(', ')'},
	{'[', ']'},
	{'{', '}'},
	{'<', '>'}
};

uint64_t GetScore(const vector<string> input)
{
	static map<char, int> scores{
		{')', 3},
		{']', 57},
		{'}', 1197},
		{'>', 25137}};
	uint64_t res = 0;
	for (auto s : input)
	{
		vector<char> st; st.reserve(s.size());
		for (auto c : s)
		{
			auto it = closing.find(c);
			if (it != closing.end())
				st.push_back(it->first);
			else if (st.empty() || closing[st.back()] != c)
			{
				res += scores[c];
				break;
			}
			else
				st.pop_back();
		}
	}
	return res;
}
uint64_t GetScore2(const vector<string> input)
{
	static map<char, int> scores{
		{')', 1},
		{']', 2},
		{'}', 3},
		{'>', 4} };
	vector<uint64_t> sc;
	for (auto s : input)
	{
		bool valid = true;
		vector<char> st; st.reserve(s.size());
		for (auto c : s)
		{
			auto it = closing.find(c);
			if (it != closing.end())
				st.push_back(it->first);
			else if (st.empty() || closing[st.back()] != c)
			{
				valid = false;
				break;
			}
			else
				st.pop_back();
		}
		if (valid && st.size() > 0)
		{
			uint64_t res = 0;
			while (!st.empty())
			{
				res *= 5;
				res += scores[closing[st.back()]];
				st.pop_back();
			}
			sc.push_back(res);
		}
	}
	sort(sc.begin(), sc.end());
	return sc[sc.size() / 2];
}

void Test();
int main()
{
	Test();
	ifstream is("Day10.txt");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	auto t_start = high_resolution_clock::now();
	cout << "Day10 Answer1: " << GetScore(input); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
	t_start = high_resolution_clock::now();
	cout << "Day10 Answer2: " << GetScore2(input); cout << " took " << ElapsedMs(t_start) << "ms" << endl;
}

void Test()
{
	stringstream is(R"([({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]])");
	istream_iterator<string> start(is), end;
	vector<string> input(start, end);

	cout << "test1: " << GetScore(input) << endl;
	cout << "test2: " << GetScore2(input) << endl;
}