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
#include <utility>
#include <numeric> 
#include <cstdlib>

using namespace std;
using namespace placeholders;

struct UInstr
{
	int code;
	int a;
	int b;
	int c;
};
istream& operator>>(istream& i, UInstr& s)
{
	i >> s.code >> s.a >> s.b >> s.c;
	return i;
}
ostream& operator<<(ostream& o, const UInstr& s)
{ o.width(2); o << s.code << " " << s.a << " " << s.b << " " << s.c; return o; }
struct Sample
{
	vector<int> r0;
	UInstr i;
	vector<int> r1;
};
istream& operator>>(istream& i, Sample &s)
{
	s.r0 = s.r1 = vector<int>(4, 0);
	string line;
	while(i && line.empty())
		getline(i,line);
	sscanf_s(line.c_str(), "Before: [%d, %d, %d, %d]", &s.r0[0], &s.r0[1], &s.r0[2], &s.r0[3]);
	getline(i, line);
	sscanf_s(line.c_str(), "%d %d %d %d", &s.i.code, &s.i.a, &s.i.b, &s.i.c);
	getline(i, line);
	sscanf_s(line.c_str(), "After:  [%d, %d, %d, %d]", &s.r1[0], &s.r1[1], &s.r1[2], &s.r1[3]);

	return i;
}
vector<int> reg;
ostream& operator<<(ostream& o, const vector<int>& v)
{ for (auto e : v) {o.width(3); o << e << " "; } return o;}
typedef map<string, function<void(int, int, int)>> InstSet;
InstSet instSet =
{
	{"addr", [&](int a, int b, int c) {reg[c] = reg[a] + reg[b]; }},
	{"addi", [&](int a, int b, int c) {reg[c] = reg[a] + b; }},
	{"mulr", [&](int a, int b, int c) {reg[c] = reg[a] * reg[b]; }},
	{"muli", [&](int a, int b, int c) {reg[c] = reg[a] * b; }},
	{"banr", [&](int a, int b, int c) {reg[c] = reg[a] & reg[b]; }},
	{"bani", [&](int a, int b, int c) {reg[c] = reg[a] & b; }},
	{"borr", [&](int a, int b, int c) {reg[c] = reg[a] | reg[b]; }},
	{"bori", [&](int a, int b, int c) {reg[c] = reg[a] | b; }},
	{"setr", [&](int a, int b, int c) {reg[c] = reg[a]; }},
	{"seti", [&](int a, int b, int c) {reg[c] = a; }},
	{"gtir", [&](int a, int b, int c) {reg[c] = a > reg[b] ? 1 : 0; }},
	{"gtri", [&](int a, int b, int c) {reg[c] = reg[a] > b ? 1 : 0; }},
	{"gtrr", [&](int a, int b, int c) {reg[c] = reg[a] > reg[b] ? 1 : 0; }},
	{"eqir", [&](int a, int b, int c) {reg[c] = a == reg[b] ? 1 : 0; }},
	{"eqri", [&](int a, int b, int c) {reg[c] = reg[a] == b ? 1 : 0; }},
	{"eqrr", [&](int a, int b, int c) {reg[c] = reg[a] == reg[b] ? 1 : 0; }},
};

set<string> MatchInstr(const Sample& s)
{
	set<string> res;
	for (auto it = instSet.begin(); it != instSet.end(); ++it)
	{
		reg = s.r0;
		it->second(s.i.a, s.i.b, s.i.c);
		if (equal(reg.begin(), reg.end(), s.r1.begin()))
			res.insert(it->first);
	}
	return res;
}

map<int, string> FindMapping(vector<Sample>& input)
{
	set<string> all; transform(instSet.begin(), instSet.end(), inserter(all, all.end()), [](auto i) {return i.first; });
	vector<set<string>> m(instSet.size(), all);
	for (auto s : input)
	{
		auto match = MatchInstr(s);
		set<string> new_set;
		set_intersection(m[s.i.code].begin(), m[s.i.code].end(), match.begin(), match.end(), inserter(new_set, new_set.end()));
		m[s.i.code] = new_set;
	}
	while (m.end() != find_if(m.begin(), m.end(), [](auto s) { return s.size() > 1; }))
	{
		for (int i = 0; i < m.size(); i++)
		{
			if (m[i].size() == 1)
				for (int j = 0; j < m.size(); j++)
					if (j != i) m[j].erase(*m[i].begin());
		}
	}
	map<int, string> res;
	for (int i = 0; i < m.size(); i++)
		res[i] = *m[i].begin();
	return res;
}

int Solve(vector<Sample>& input)
{
	auto s2i = FindMapping(input);

	ifstream is("input2.txt");
	istream_iterator<UInstr> start(is), end;
	vector<UInstr> instructions(start, end);
	cout << "Read " << instructions.size() << " instructions" << endl;

	reg = { 0,0,0,0 };
	for (auto i : instructions)
	{
		instSet[s2i[i.code]](i.a, i.b, i.c);
		//cout << i << " " << s2i[i.code] << " reg: " << reg << endl;
	}

	return reg[0];
}

int main()
{
	ifstream is("input.txt");
	istream_iterator<Sample> start(is), end;
	vector<Sample> input(start, end);
	cout << "Read " << input.size() << " samples" << endl;

	cout << "Day16 Answer1: " << count_if(input.begin(), input.end(), [&](auto s) {return MatchInstr(s).size() >= 3; }) << endl;
	cout << "Day16 Answer2: " << Solve(input) << endl;
	return 0;
}