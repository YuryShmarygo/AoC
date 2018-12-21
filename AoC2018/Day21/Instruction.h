#pragma once

#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <vector>
#include <map>

struct Instr
{
	std::string code;
	int a;
	int b;
	int c;
};
std::istream& operator>>(std::istream& i, Instr& s);
std::ostream& operator<<(std::ostream& o, const Instr& s);
std::pair<int, std::vector<Instr>> ReadInstructions(const std::string& inFile);

typedef std::vector<long long> Registers;
std::ostream& operator<<(std::ostream& o, const Registers& v);

struct Processor
{
	Registers reg;
	typedef std::unordered_map<std::string, std::function<void(int, int, int)>> InstSet;
	InstSet instSet;
	Processor();
	Registers Process(int ipBound, const std::vector<Instr>& instructions, const Registers& reg = { 0,0,0,0,0,0 }
	, std::function<bool(int ip, Registers& reg, const Instr& instr)> call_back = [](int ip, Registers& reg, const Instr& instr) {return false; });
};

struct Converter
{
	static std::string Convert(int ipBound, const std::vector<Instr>& instructions);
	static void Convert(const std::string& inFile, const std::string& outFile);
};

#endif //_INSTRUCTION_H_