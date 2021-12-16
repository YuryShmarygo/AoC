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

struct Mat
{
	string str;
	map<string, char> ins;
};
istream& operator>>(istream& is, Mat& m)
{
	is >> m.str;
	string str;
	while (getline(is, str))
		if (!str.empty())
			m.ins[str.substr(0, 2)] = str[6];
	return is;
}
uint64_t GetFormula_bak(Mat& input, int steps)
{
	list<char> res(input.str.begin(), input.str.end());
	map<char, uint64_t> chars;
	for (auto c : res)
		++chars[c];
	for (; steps > 0; --steps)
	{
		for (auto it = res.begin(); it != res.end(); ++it)
		{
			auto itn = next(it);
			if (itn != res.end())
			{
				string seq(it, next(itn));
				char c = input.ins[seq];
				++chars[c];
				it = res.insert(itn, c);
			}
		}
	}
	auto [min, max] = minmax_element(chars.begin(), chars.end(), [](auto& e1, auto& e2) {return e1.second < e2.second; });
	return max->second - min->second;
}
struct Key
{
	int step;
	string str;
};
bool operator<(const Key& k1, const Key& k2)
{ return k1.step < k2.step || k1.step == k2.step && k1.str < k2.str; }
uint64_t GetFormula(Mat& input, int n)
{
	return 0;
	/*
	list<string> res;
	for (auto it = input.str.begin(); next(it) != input.str.end(); ++it)
		res.push_back(string{ *it, *next(it) });
	map<char, uint64_t> chars;

	map<Key, map<char, uint64_t>> cache;

	while(res.size() > 1)
	{
		string key{ *res.begin(), *next(res.begin()) };
		if (cache[key].empty())
		{
			vector<char> tmp{ *res.begin(), *next(res.begin()) };
			tmp.reserve(n + 2);
			for (int s = 0; s < n; ++s)
				for (int i = 0; i < tmp.size() - 1; i += 2)
					tmp.insert(tmp.begin() + i + 1, input.ins[string{ tmp[i],tmp[i + 1] }]);

			tmp.resize(tmp.size() - 1);
			map<char, uint64_t> tmp_chars;
			for (auto c : tmp)
				++tmp_chars[c];
			cache[key] = tmp_chars;
		}
		for (auto& t : cache[key])
			chars[t.first] += t.second;
		res.erase(res.begin());
	}
	++chars[*res.begin()];

	auto [min, max] = minmax_element(chars.begin(), chars.end(), [](auto& e1, auto& e2) {return e1.second < e2.second; });
	return max->second - min->second;
	*/
}
uint64_t GetFormula_bak2(Mat& input, int n)
{
	vector<char> res(input.str.begin(), input.str.end());
	map<char, uint64_t> chars;
	map<char, map<char, char>> mapping;
	for (auto& i : input.ins)
		mapping[i.first[0]][i.first[1]] = i.second;
	map<char, map<char, map<char, uint64_t>>> cache;

	while (res.size() > 1)
	{
		vector<char> tmp{ *res.begin(), *next(res.begin()) };
		tmp.reserve(n + 2);
		if (cache[tmp[0]][tmp[1]].empty())
		{
			for (int s = 0; s < n; ++s)
			{
				for (int i = 0; i < tmp.size() - 1; i += 2)
				{
					tmp.insert(tmp.begin() + i + 1, mapping[tmp[i]][tmp[i + 1]]);
				}
			}
			tmp.resize(tmp.size() - 1);
			map<char, uint64_t> tmp_chars;
			for (auto c : tmp)
				++tmp_chars[c];
			cache[tmp[0]][tmp[1]] = tmp_chars;
		}
		for (auto& t : cache[tmp[0]][tmp[1]])
			chars[t.first] += t.second;
		res.erase(res.begin());
	}
	++chars[*res.begin()];

	auto [min, max] = minmax_element(chars.begin(), chars.end(), [](auto& e1, auto& e2) {return e1.second < e2.second; });
	return max->second - min->second;
}
void Test();
int main()
{
	Test();// return 0;
	ifstream is("Day14.txt");
	Mat input; is >> input;

	cout << "Day14 Answer1: " << GetFormula(input, 10) << endl;
	cout << "Day14 Answer2: " << GetFormula(input, 40) << endl;
}

void Test()
{
	stringstream is(R"(NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C)");
	Mat input; is >> input;
	GetFormula(input, 3);
	cout << "Test 1: " << GetFormula(input, 10) << endl;
	cout << "Test 2: " << GetFormula(input, 40) << endl;
}