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
#include <unordered_set>

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

typedef vector<long long> Registers;
Registers reg;
ostream& operator<<(ostream& o, const Registers& v)
{
	for (auto e : v) { o.width(3); o << e << " "; } return o;
}
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
	//ofstream o("d21.out");
	int ip = 0;
	while (ip >= 0 && ip < instructions.size())
	{
		reg[ipBound] = ip;
		const Instr& i = instructions[ip];
		//o << "ip="; o.width(2); o << ip << " " << reg << " " << i;
		instSet[i.code](i.a, i.b, i.c);
		//o << reg << endl;
		ip = (int)reg[ipBound] + 1;
	}
}

long long Process2(int ipBound, const vector<Instr>& instructions)
{
	int ip = 0;
	set<long long> ans;
	long long last;
	while (ip >= 0 && ip < instructions.size())
	{
		reg[ipBound] = ip;
		const Instr& i = instructions[ip];
		if (ip == 28)
		{
			if (ans.find(reg[3]) != ans.end())
				return last;
			ans.insert(reg[3]);
			last = reg[3];
		}
		instSet[i.code](i.a, i.b, i.c);
		ip = (int)reg[ipBound] + 1;
	}
}

int main()
{
	ifstream is("d21.txt");
	int ipBound = 0; string some;
	is >> some >> ipBound;
	istream_iterator<Instr> start(is), end;
	vector<Instr> instructions(start, end);
	cout << "Read " << instructions.size() << " instructions" << endl;

	reg = { 0,0,0,0,0,0 };
	instructions[28] = Instr{ "seti", (int)instructions.size(), 0, ipBound };//exit
	Process(ipBound, instructions);
	cout << "Day21 Answer1: " << reg[3] << endl;

	reg = { 0,0,0,0,0,0 };
	instructions[28] = Instr{ "eqrr", 3, 0, 5 };
	cout << "Day21 Answer2: " << Process2(ipBound, instructions) << endl;
	return 0;
}
