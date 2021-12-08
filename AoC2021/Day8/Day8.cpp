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

struct Signal
{
	vector<string> input;
	vector<string> output;
};

istream& operator>>(istream& is, Signal& sig)
{
	sig.input.clear(); sig.output.clear();
	string str;
	for (int i = 0; i < 10; ++i)
	{
		is >> str;
		sig.input.push_back(str);
	}
	is >> str;
	for (int i = 0; i < 4; ++i)
	{
		is >> str;
		sig.output.push_back(str);
	}
	return is;
}

int64_t CountDigits(const vector<Signal>& input)
{
	int64_t res = 0;
	for (auto & sig : input)
		res += count_if(sig.output.begin(), sig.output.end(), [](auto& s) { return s.size() == 2 || s.size() == 3 || s.size() == 4 || s.size() == 7; });
	return res;
}

void Exclude(pair<const string, set<int>>& comb, string code, const set<int>& cont)
{
	string inter;
	set_intersection(comb.first.begin(), comb.first.end(), code.begin(), code.end(), back_inserter(inter));
	if (inter.size() == code.size())
	{
		for (auto it = comb.second.begin(); it != comb.second.end();)
		{
			if (cont.contains(*it))
				++it;
			else
				it = comb.second.erase(it);
		}
	}
}
void Resolve(map<int, string>& known, map<string, set<int>>& comb)
{
	/*
1 | 0 3 4 7 8 9
3 | 8 9
4 | 8 9
4-1 | 5 6 8 9
6x9 = 5
8-9 | 0 2 6
	*/
	struct Rule
	{
		function<string(const map<int, string>&)> fn;
		set<int> cont;
	};
	static vector<Rule> rules{
		{[](const map<int,string>& known) {auto it = known.find(1); return it != known.end() ? it->second : string(""); }, {0, 3, 4, 7, 8, 9}},
		{[](const map<int,string>& known) {auto it = known.find(3); return it != known.end() ? it->second : string(""); }, {8, 9}},
		{[](const map<int,string>& known) {auto it = known.find(4); return it != known.end() ? it->second : string(""); }, {8, 9}},
		{[](const map<int,string>& known)
		{
			auto it4 = known.find(4); auto it1 = known.find(1);
			if (it4 == known.end() || it1 == known.end()) return string("");
			string diff41;
			set_difference(it4->second.begin(), it4->second.end(), it1->second.begin(), it1->second.end(), back_inserter(diff41));
			return diff41; 
		}, { 5, 6, 8, 9 } },
		{[](const map<int,string>& known) {
			auto it6 = known.find(6); auto it9 = known.find(9);
			if (it6 != known.end() && it9 != known.end())
			{
				string inter69;
				set_intersection(it6->second.begin(), it6->second.end(), it9->second.begin(), it9->second.end(), back_inserter(inter69));
				return inter69;
			}
			return string(""); }, { 5 } },
		{[](const map<int,string>& known) {
			auto it8 = known.find(8); auto it9 = known.find(9);
			if (it8 != known.end() && it9 != known.end())
			{
				string diff89;
				set_difference(it8->second.begin(), it8->second.end(), it9->second.begin(), it9->second.end(), back_inserter(diff89));
				return diff89;
			}
			return string(""); }, { 0, 2, 6 } }
	};
	for (auto r : rules)
	{
		auto code = r.fn(known);
		if (!code.empty())
		{
			for (auto& c : comb)
			{
				if (c.second.size() > 1)
					Exclude(c, code, r.cont);
				if (c.second.size() == 1)
					known[*c.second.begin()] = c.first;
			}
		}
	}
}
int64_t Decode(const Signal& sig)
{
	static set<int> unique_comb{ 2, 3, 4, 7 };
	static map<int, set<int>> len2num{
		{2, {1}},
		{3, {7}},
		{4, {4}},
		{5, {2,3,5}},
		{6, {0,6,9}},
		{7, {8}} };

	map<string, set<int>> comb;
	map<int, string> known;
	vector<string> all(sig.input);
	all.insert(all.end(), sig.output.begin(), sig.output.end());
	for (auto c : all)
	{
		string code = c;
		sort(code.begin(), code.end());
		if (unique_comb.contains((int)code.size()))
			known[*len2num[(int)code.size()].begin()] = code;
		comb[code] = len2num[(int)code.size()];
	}
	while (any_of(comb.begin(), comb.end(), [](auto& c) {return c.second.size() > 1; }))
		Resolve(known, comb);
	
	uint64_t res = 0;
	for (auto& r : sig.output)
	{
		res = res * 10;
		string code = r;
		sort(code.begin(), code.end());
		res += *comb[code].begin();
	}
	return res;
}
uint64_t Solve(const vector<Signal>& input)
{ return accumulate(input.begin(), input.end(), 0ULL, [](auto sum, auto s) { return sum + Decode(s); });}

void Test();
int main()
{
	Test();
	ifstream is("Day8.txt");
	istream_iterator<Signal> begin(is), end;
	vector<Signal> input(begin, end);

	auto start = high_resolution_clock::now();
	cout << "Day8 Answer1: " << CountDigits(input); cout << " took " << ElapsedMs(start) << "ms" << endl;
	start = high_resolution_clock::now();
	cout << "Day8 Answer2: " << Solve(input); cout << " took " << ElapsedMs(start) << "ms" << endl;
}

void Test()
{
	stringstream is(R"(be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce)");

	istream_iterator<Signal> start(is), end;
	vector<Signal> input(start, end);
	//cout << "Read " << input.size() << " patterns" << endl;

	cout << "test 1: " << CountDigits(input) << endl;
	stringstream test2(R"(acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf)");
	Signal sig;
	test2 >> sig;
	cout << "test 2: " << Decode(sig) << endl;
	cout << "test 3: " << Solve(input) << endl;
}