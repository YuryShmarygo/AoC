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
#include <unordered_map>

using namespace std;
using namespace placeholders;

struct Instr
{
	string code;
	int a;
	int b;
	int c;
};
istream& operator>>(istream& i, Instr& s)
{
	i >> s.code >> s.a >> s.b >> s.c;
	return i;
}
ostream& operator<<(ostream& o, const Instr& s)
{
	o << s.code << " "; o.width(2); o << s.a << " "; o.width(2); o << s.b << " "; o.width(2); o << s.c; return o;
}

vector<int> reg;
ostream& operator<<(ostream& o, const vector<int>& v)
{	for (auto e : v) { o.width(3); o << e << " "; } return o;}
typedef unordered_map<string, function<void(int, int, int)>> InstSet;
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
	{"modr", [&](int a, int b, int c) {reg[c] = reg[a] % reg[b]; }},
	{"divr", [&](int a, int b, int c) {reg[c] = reg[a] / reg[b]; }},
};

void Process(int ipBound, const vector<Instr>& instructions)
{
	ofstream o("d19.out");
	int ip = 0;
	while (ip >= 0 && ip < instructions.size())
	{
		reg[ipBound] = ip;
		const Instr& i = instructions[ip];
		//o << "ip="; o.width(2); o << ip << " " << reg << " " << i;
		instSet[i.code](i.a, i.b, i.c);
		//o << reg << endl;
		ip = reg[ipBound] + 1;
	}
}
int main()
{
	ifstream is("d19.txt");
	int ipBound = 0; string some;
	is >> some >> ipBound;
	istream_iterator<Instr> start(is), end;
	vector<Instr> instructions(start, end);
	cout << "Read " << instructions.size() << " instructions" << endl;

	reg = { 0,0,0,0,0,0 };
	Process(ipBound, instructions);
	cout << "Day19 Answer1: " << reg[0] << endl;
	reg = { 1,0,0,0,0,0 };
	instructions[1] = Instr{ "seti", (int)instructions.size(), 0, ipBound };//exit
	Process(ipBound, instructions);
	int x = reg[5];
	// revese engineered from instructions and optimized
	int res = 0;
	for (int i = 1; i <= x; i++)
	{
		if (x % i == 0)
			res += i;
	}
	cout << "Day19 Answer2: " << res <<  endl;
	return 0;
}
