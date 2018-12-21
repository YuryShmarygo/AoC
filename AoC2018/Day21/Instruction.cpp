#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <set>
#include "Instruction.h"

using namespace std;

#define S(s) ((std::ostringstream&)(std::ostringstream() << s)).str()

std::istream& operator>>(std::istream& i, Instr& s)
{
	i >> s.code >> s.a >> s.b >> s.c;
	return i;
}
std::ostream& operator<<(std::ostream& o, const Instr& s)
{
	o << s.code << " " << setw(2) << s.a << " " << setw(2) << s.b << " " << setw(2) << s.c;
	return o;
}
ostream& operator<<(ostream& o, const Registers& v)
{
	for (auto e : v) { o << setw(3) << e << " "; } return o;
}

Processor::Processor()
{
	instSet =
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
}
Registers Processor::Process(int ipBound, const std::vector<Instr>& instructions, const Registers& r /*= { 0, 0, 0, 0, 0, 0 }*/
	, std::function<bool(int ip, Registers& reg, const Instr& instr)> call_back /*= [](int ip, Registers& reg, Instr& instr) {return false; }*/)
{
	//ofstream o("test.out");
	reg = r;
	int ip = 0;
	while (ip >= 0 && ip < instructions.size())
	{
		reg[ipBound] = ip;
		const Instr& i = instructions[ip];
		//o << "ip="; o.width(2); o << ip << " " << reg << " " << i;
		if (call_back(ip, reg, i))
			return reg;
		instSet[i.code](i.a, i.b, i.c);
		//o << reg << endl;
		ip = (int)reg[ipBound] + 1;
	}
	return reg;
}

std::pair<int, std::vector<Instr>> ReadInstructions(const std::string& inFile)
{
	std::ifstream is(inFile);
	int ipBound = 0; std::string some;
	is >> some >> ipBound;
	std::istream_iterator<Instr> start(is), end;
	std::vector<Instr> instructions(start, end);
	return make_pair(ipBound, instructions);
}

std::string Converter::Convert(int ipBound, const std::vector<Instr>& instructions)
{
	typedef std::unordered_map<std::string, std::function<std::string(int, int, int)>> ConvSet;
	auto Lbl = [](int i) {return S("l_" << i); };
	auto IsComp = [](const std::string& s)
	{
		static std::set<std::string> comp{ "gtir","gtri","gtrr","eqir","eqri","eqrr" };
		return comp.end() != comp.find(s);
	};
	int cur = 0;
	ConvSet conv = {
		{"addr", [&](int a, int b, int c) {return ipBound == c ?
			((a == c && instructions[cur - 1].c == b || b == c && instructions[cur - 1].c == a) && IsComp(instructions[cur - 1].code) ?
													S("if (reg[" << (a == c ? b : a) << "] == 0) goto " << Lbl(cur + 1) << ";" << " else goto " << Lbl(cur + 2) << ";") :
													S("Could not generate jump from " << instructions[cur])) :
													S("reg[" << c << "] = reg[" << a << "] + reg[" << b << "];"); }},
		{"addi", [&](int a, int b, int c) {return ipBound == c ?
													(a == c ? S("goto " << Lbl(cur + b + 1) << ";") : S("Could not generate jump from " << instructions[cur])) :
													S("reg[" << c << "] = reg[" << a << "] + " << b << ";"); }},
		{"mulr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] * reg[" << b << "];"); }},
		{"muli", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] * " << b << ";"); }},
		{"banr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] & reg[" << b << "];"); }},
		{"bani", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] & " << b << ";"); }},
		{"borr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] | reg[" << b << "];"); }},
		{"bori", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] | " << b << ";"); }},
		{"setr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "];"); }},
		{"seti", [&](int a, int b, int c) {return ipBound == c ?
													S("goto " << Lbl(a + 1) << ";") :
													S("reg[" << c << "] = " << a << ";"); }},
		{"gtir", [&](int a, int b, int c) {return S("reg[" << c << "] = " << a << " > reg[" << b << "] ? 1 : 0;"); }},
		{"gtri", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] > " << b << " ? 1 : 0;"); }},
		{"gtrr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] > reg[" << b << "] ? 1 : 0;"); }},
		{"eqir", [&](int a, int b, int c) {return S("reg[" << c << "] = " << a << " == reg[" << b << "] ? 1 : 0;"); }},
		{"eqri", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] == " << b << " ? 1 : 0;"); }},
		{"eqrr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] == reg[" << b << "] ? 1 : 0;"); }},
		{"modr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] % reg[" << b << "];"); }},
		{"divr", [&](int a, int b, int c) {return S("reg[" << c << "] = reg[" << a << "] / reg[" << b << "];"); }}, };
	std::ostringstream s;
	string reg_type = "std::vector<long long>";
	s << "#include<vector>" << endl << endl
		<< "#pragma warning (disable : 4102)" << endl << endl
		<< reg_type << " Process(const " << reg_type << "& r)" << endl
		<< "{" << endl
		<< "\t" << reg_type << " reg = r;" << endl;
	for (; cur < instructions.size(); ++cur)
	{
		auto& ii = instructions[cur];
		s << left << setw(5) << Lbl(cur) + ":" << " " << left << setw(48) << conv[ii.code](ii.a, ii.b, ii.c) << " //(" << cur << ") "<< ii << std::endl;
	}
	s << "\t" << "return reg;" << endl
		<< "}";
	return s.str();
}
void Converter::Convert(const std::string& inFile, const std::string& outFile)
{
	auto input = ReadInstructions(inFile);
	//cout << "Read " << input.second.size() << " instructions" << endl;
	std::ofstream(outFile) << Convert(input.first, input.second);
}

